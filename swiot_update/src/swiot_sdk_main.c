#include "swiot_sdk.h"
#include "swdownload.h"
#include "swhttp.h"
#include "recovery.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#define UPGRADE_TIP_WAV_PATH "/system/upgrade.wav"

int SDK_Device_Get(swiot_service_type_t type,int message_id,char *product_key, char *device_name, char *property_name)
{
SWIOT_SDK_Handle_Response(type,message_id,200,"{}",NULL);
    return 0;
}
int SDK_Device_Set(swiot_service_type_t type,int message_id,char *product_key, char *device_name, char *property_name)
{
SWIOT_SDK_Handle_Response(type,message_id,200,"{}",NULL);
    return 0;
}
int SDK_Device_Service(swiot_service_type_t type,int message_id,char *product_key, char *device_name, char *service_name, char *service_param)
{
	printf("service_name:%s\n",service_name);
SWIOT_SDK_Handle_Response(type,message_id,200,"{}",service_name);
    return 0;
}

int SDK_Device_Enable(swiot_service_type_t type,int message_id,char *product_key, char *device_name)
{
SWIOT_SDK_Handle_Response(type,message_id,200,"{}",NULL);
    return 0;    
}

int SDK_Device_Disable(swiot_service_type_t type,int message_id,char *product_key, char *device_name)
{
SWIOT_SDK_Handle_Response(type,message_id,200,"{}",NULL);
    return 0;
}

int SDK_Device_Remove(swiot_service_type_t type,int message_id,char *product_key, char *device_name)
{
    SWIOT_SDK_Handle_Response(type,message_id,200,"{}",NULL);
    return 0;
}
int SDK_Device_Upgrade(swiot_service_type_t type,int message_id,char *product_key,char* device_name,const char *upgrade_url,const char* md5,const char* curversion)
{
	char cmd_value[100] = {0};
	char changed_name[100] = {0};
	char upgrade_filename[100] = {0};
	
    printf("SDK_Device_Upgrade , product_key=%s\n",product_key);
    printf("SDK_Device_Upgrade , device_name=%s\n",device_name);
    printf("SDK_Device_Upgrade , upgrade_url=%s\n",upgrade_url);
	printf("SDK_Device_Upgrade , md5=%s\n",md5);
	printf("SDK_Device_Upgrade , curversion=%s\n",curversion);
    SWIOT_SDK_Handle_Response(type,message_id,200,"{}",NULL);
	
	if(!swDownloadJudge(curversion))
	{
		printf("1  swDownloadJudge\n");
		//download(upgrade_url);
		for(int i=0; i<3; i++)
		{	
			memset(upgrade_filename, 0, sizeof(upgrade_filename));
			memset(cmd_value, 0, sizeof(cmd_value));
			memset(changed_name, 0, sizeof(changed_name));
			
			if(http_request_main(upgrade_url, upgrade_filename, i) == 0)
			{
				printf("2  http_request_main\n");
				printf("%s\n", upgrade_filename);
				if(!swMd5sum(upgrade_filename, md5))
				{
					printf("3  swMd5sum\n");
					
					memset(cmd_value, 0, sizeof(cmd_value));
					sprintf(changed_name, "%s/update-%s.img", DATA_ROOT_PATH, curversion);
					sprintf(cmd_value,"mv %s %s", upgrade_filename, changed_name);
					printf("4  change name is %s\n", cmd_value);

					if(!system(cmd_value))
					{
						memset(cmd_value, 0, sizeof(cmd_value));
						sprintf(cmd_value, "paplay %s", UPGRADE_TIP_WAV_PATH);
						system(cmd_value);
						sleep(3);
						
						recovery_new_img(changed_name);
						printf("5  recovery_new_img\n");
					}
					else{
						memset(cmd_value, 0, sizeof(cmd_value));
						sprintf(cmd_value, "rm %s", upgrade_filename);
						system(cmd_value);
						
						printf("md5sum verify fail,please download agin!\n");
					}
				}
				else
				{
					memset(cmd_value, 0, sizeof(cmd_value));
					sprintf(cmd_value, "rm %s", upgrade_filename);
					system(cmd_value);
					printf("md5sum %s diff\n", upgrade_filename);
				}
			}
			else
			{
				memset(cmd_value, 0, sizeof(cmd_value));
				sprintf(cmd_value, "rm %s", upgrade_filename);
				system(cmd_value);
				printf("http request fail,please download again!\n");
			}
		}	
	}
	
    return 0;
}

int SDK_Device_Event(swiot_event_e event)
{

   if (EVENT_CONNECT_SUCSS == event)
    {   
           printf("connect success\n");
           SWIOT_SDK_PostEvent("sendevent","{}");
           SWIOT_SDK_PostProperty("{\"volume\":4,\"playStatus\":4,\"url\":\"http://test.com\"}");
    }else {
           printf("connect error\n");
    }   
    return 0;
}

FILE* fp;
int Read_Cache(char* buf,int buf_size,int read_size)
{
	int ret = 0;
	ret = fread(buf,1,buf_size,fp);
	return ret;	
}
int Write_Cache(char* buf,int buf_size,int write_size)
{
	int ret = 0;
    fseek(fp,write_size,SEEK_SET);
	ret = fwrite(buf,1,buf_size,fp);
	fflush(fp);
	return ret;
}

int get_dev_mac(char *cur_mac){
    //获取MAC的功能厂商自己实现，本函数仅供参考
	char device[16]="wlan0"; //teh0是网卡设备名  
	unsigned char macaddr[ETH_ALEN]; //ETH_ALEN（6）是MAC地址长度 
	//AF_INET = 1;  
	int i,s;  
	s = socket(AF_INET,SOCK_DGRAM,0); //建立套接口  
	struct ifreq req;  
	int err;
    char * ret;  
	
	ret = strcpy(req.ifr_name,device); //将设备名作为输入参数传入  
	err=ioctl(s,SIOCGIFHWADDR,&req); //执行取MAC地址操作  
	close(s);  
	if( err != -1 )
		memcpy(macaddr,req.ifr_hwaddr.sa_data,ETH_ALEN); //取输出的MAC地址  
		
		  
	sprintf(cur_mac, "%02x:%02x:%02x:%02x:%02x:%02x", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	return 0;
}

int main()
{
    device_handle_t handle = {SDK_Device_Get,
                              SDK_Device_Set,
                              SDK_Device_Service,
                              SDK_Device_Enable,
                              SDK_Device_Disable,
                              SDK_Device_Remove,
                              SDK_Device_Upgrade};
    cache_handle_t cache={Read_Cache,
			  Write_Cache
		};
	char cur_version[100] = {0};
	char cur_sn[100]  = {0};
	char update_img_path[100] = {0};
	char cmd_value[100] = {0};	
	char cur_mac[100] = {0};
	char manufacturer[100] = {0};
	char register_id[50] = {0};
	char product_secret[50] = {0};
	char product_key[50] = {0};
	char hardware_version[10] = {0};
	
	//if(!access("/data/sunniwell/config", F_OK))
	//{
	//	sprintf(cmd_value, "rm %s", "/data/sunniwell/config");
	//	printf("[main.c]%s\n", cmd_value);
	//	system(cmd_value);
	//	fp = fopen("/data/sunniwell/config","a+");
	//}
	//else
		fp = fopen("/data/sunniwell/config","a+");
   
	
	memset(cmd_value, 0, sizeof(cmd_value));
	
	property_get("ro.boot.serialno", cur_sn, "0");
	printf("sn = %s\n", cur_sn);
	
	property_get("ro.build.version.release", cur_version, "0");
	printf("version = %s\n", cur_version);
	
	property_get("ro.product.manufacturer", manufacturer, "0");
	printf("manufacturer = %s\n", manufacturer);
	property_get("ro.swiot.register.id", register_id, "0");
	printf("register_id = %s\n", register_id);	
	property_get("ro.swiot.product.secret", product_secret, "0");
	printf("product_secret = %s\n", product_secret);

	property_get("ro.build.hardware.version", hardware_version, "0");
	printf("hardware_version = %s\n", hardware_version);
	get_dev_mac(cur_mac);
	printf("mac = %s\n", cur_mac);
	
	sprintf(update_img_path, "%s/update-%s.img", DATA_ROOT_PATH, cur_version);
	if(!access(update_img_path, F_OK))
	{
		sprintf(cmd_value, "rm %s", update_img_path);
		printf("[main.c]%s\n", cmd_value);
		system(cmd_value);
	}
	
//N7N8DesO
    SWIOT_SDK_Construct("N7N8DesO", cur_sn, cur_mac ,cur_version, hardware_version, manufacturer);
	SWIOT_SDK_AutomMode_Open("http://120.24.69.144:8980/open-api/device/auth/register",register_id, product_secret);
    SWIOT_SDK_Register(REG_TYPE_DEVICE,&handle);
    SWIOT_SDK_Register(REG_TYPE_CACHE,&cache);
    SWIOT_SDK_Register(REG_TYPE_EVENT,SDK_Device_Event);
    SWIOT_SDK_Start();

    while(1)
	    sleep(1);
    
}
