#ifndef __SWIOT_SDK_H__
#define __SWIOT_SDK_H__

/************************************************************
 *                      SDK 常量定义
 * 
 ***********************************************************/
/**
 * 注册句柄类型
 * REG_TYPE_CACHE               缓存句柄
 * REG_TYPE_DEVICE              设备句柄
 **/
typedef enum
{
    REG_TYPE_CACHE,
    REG_TYPE_DEVICE,
    REG_TYPE_EVENT
} reg_type_t;

/**
 * @brief 处理回复类型
 * SWIOT_SDK_SET                获取设备属性
 * SWIOT_SDK_GET                设置设备属性
 * SWIOT_SDK_SERVICE            设备服务，exstr中为service_id
 * SWIOT_SDK_DISABLE            禁用设备
 * SWIOT_SDK_ENABLE             启动设备
 * SWIOT_SDK_REMOVE             删除设备
 * SWIOT_SDK_UPGRADE            升级设备
*/
typedef enum
{
    SWIOT_SDK_SET = 0,
    SWIOT_SDK_GET,
    SWIOT_SDK_SERVICE,
    SWIOT_SDK_DISABLE,
    SWIOT_SDK_ENABLE,
    SWIOT_SDK_REMOVE,
    SWIOT_SDK_UPGRADE
} swiot_service_type_t;

typedef enum
{
    SWIOT_MESSAGE_SUCCESS = 200,
    SWIOT_MESSAGE_FAIL = 400
} swiot_message_code_t;

/************************************************************
 *                      SDK 对外接口
 * 
 ***********************************************************/
/**
 * 构建 SDK 句柄
 * @param   version             设备版本号
 * @param   hardware_version    硬件版本号
 * @param   manufacturer        生产厂家
 * @param   mac                 硬件地址
 * ret                  构建的 SDK 句柄
 **/
int SWIOT_SDK_Construct(const char *product_key,
			                  const char *sn,
 //                       const char *product_secret,
                        const char *mac,
                        const char *device_version,
                        const char *hardware_version,
                        const char *manufacturer);


/**
* 开启自动入网模式
* @param register_url           入网url
* @param register_id            平台提供的认证id
* @param product_secret         设备产品号
*/
int SWIOT_SDK_AutomMode_Open(const char* register_url,
                             const char* register_id,
                             const char* product_secret);

/**
 * 向 SDK 注册句柄
 * @param(in)   register_type           注册句柄类型
 * @param(in)   register_handle         注册句柄
 * ret                                  SWIOT_SUCCESS:成功
 *                                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_Register(reg_type_t register_type,
                       void *register_handle);

/**
 * 取消向 SDK 注册句柄
 * @param(in)   register_type           取消注册句柄类型
 * @param(in)   register_handle         取消注册句柄
 * ret                                  SWIOT_SUCCESS:成功
 *                                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_Unregister(reg_type_t register_type);

/**
 * 发送事件
 * @param(in)   event_name              事件名称
 * @param(in)   event_param             事件具体参数 json 键值对,形如{"key":1, "key1":"2"}
 * ret                                  SWIOT_SUCCESS:成功
 *                                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_PostEvent(char *event_name,
                        void *event_param);

/**
 * 属性上报
 * @param(in)   property_param          属性具体参数 json 键值对,形如{"key":1, "key1":"2"}
 * ret                                  SWIOT_SUCCESS:成功
 *                                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_PostProperty(char *property_param);

/**
* 升级进度上报
* @param        progress                进度上报
* @return                               SWIOT_SUCCESS :成功
*                                       SWIOT_EGENERIC:失败
*/
int SWIOT_SDK_PostUpgress(int progress);

/**
* 消息回复
* @param       message_type             消息类型
* @param       message_id               消息序号
* @param       code                     消息码
* @param       retstr                   回复的消息,json格式
* @param       exstr                    扩展字段,当swiot_message_type_e为SWIOT_SDK_COM时为服务id号
* @return                               SWIOT_SUCCESS :成功
*                                       SWIOT_EGENERIC:失败
*/
int SWIOT_SDK_Handle_Response(swiot_service_type_t message_type,
                              int message_id,
                              swiot_message_code_t code,
                              const char *retstr,
                              const char *exstr);

/**
* sdk重启
* @return                               SWIOT_SUCCESS :成功
*                                       SWIOT_EGENERIC:失败
*/
int SWIOT_SDK_Reboot();

/**
 * SDK 开始工作
 * @param(in)  inst     SDK 句柄
 * ret                  SWIOT_SUCCESS:成功
 *                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_Start();

/**
 * SDK 停止工作
 * @param(in)  inst     SDK 句柄
 * ret                  SWIOT_SUCCESS:成功
 *                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_Stop();

/**
 * 释放 SDK 句柄
 * @param(in)  inst     SDK 句柄
 * ret                  SWIOT_SUCCESS:成功
 *                      SWIOT_EGENERIC:失败
 **/
int SWIOT_SDK_Destory();

/************************************************************
 *                      SDK 缓存接口/结构定义
 * 
 ***********************************************************/
/**
 * 读缓存接口
 * @param(in)   read_buffer                 读取缓存数据到该内存
 * @param(in)   read_bytes                  想要读取的长度
 * @param(in)   src_offset                  读取缓存位置
 * ret                                      实际读取数据长度
 *                                          SWIOT_EGENERIC   读取失败
 **/
typedef int (*SWIOT_SDK_Cache_Read)(void *read_buffer,
                                    int read_bytes,
                                    int src_offset);

/**
 * 写缓存接口
 * @param(in,out)   write_buffer            写该内存数据到缓存
 * @param(in)       write_bytes             想要写入的长度
 * @param(in)       dst_offset              写入缓存位置
 * ret                                      实际写入数据长度
 *                                          SWIOT_EGENERIC   读取失败
 **/
typedef int (*SWIOT_SDK_Cache_Write)(void *write_buffer,
                                     int write_bytes,
                                     int dst_offset);

/**
 * 缓存句柄
 * cache_read                   读缓存接口
 * cache_write                  写缓存接口
 **/
typedef struct
{
    SWIOT_SDK_Cache_Read cache_read;
    SWIOT_SDK_Cache_Write cache_write;
} cache_handle_t;

typedef enum{
  EVENT_CONNECT_ERROR,  //连接失败
  EVENT_CONNECT_SUCSS,  //连接成功
}swiot_event_e;

typedef int (*SWIOT_SDK_Event)(swiot_event_e event);

/************************************************************
 *                      SDK 设备接口/结构定义
 * 
 ***********************************************************/
/**
 * 获取属性接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 * @param(in)   property_name           参数名 ["property_name1", "property_name2",...]
 **/
typedef int (*SWIOT_SDK_Device_Get)(swiot_service_type_t type,
                                    int message_id,
                                    char *product_key,
                                    char *device_name,
                                    char *property_name);
/**
 * 设置属性接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 * @param(in)   property_name           参数名 {"property_name1":property_value1, "property_name2":property_value2,...}
 **/
typedef int (*SWIOT_SDK_Device_Set)(swiot_service_type_t type,
                                    int message_id,
                                    char *product_key,
                                    char *device_name,
                                    char *property_name);
/**
 * 调用服务接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 * @param(in)   service_name            服务名
 * @param(in)   service_param           服务参数，肯能为null
 **/
typedef int (*SWIOT_SDK_Device_Service)(swiot_service_type_t type,
                                        int message_id,
                                        char *product_key,
                                        char *device_name,
                                        char *service_name,
                                        char *service_param);
/**
 * 启用设备接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 **/
typedef int (*SWIOT_SDK_Device_Enable)(swiot_service_type_t type,
                                       int message_id,
                                       char *product_key,
                                       char *device_name);
/**
 * 禁用设备接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 **/
typedef int (*SWIOT_SDK_Device_Disable)(swiot_service_type_t type,
                                        int message_id,
                                        char *product_key,
                                        char *device_name);
/**
 * 删除子设备接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 **/
typedef int (*SWIOT_SDK_Device_Remove)(swiot_service_type_t type,
                                       int message_id,
                                       char *product_key,
                                       char *device_name);
/**
 * 升级设备接口
 * @param(in)   type                    消息类型，需要回传到response接口
 * @param(in)   message_id              消息序号
 * @param(in)   product_key             产品ID
 * @param(in)   device_name             设备名
 * @param(in)   upgrade_url             升级文件下载地址
 * @param(in)   md5                     升级文件MD5
 * @param(in)   curversion              升级文件版本
 **/
typedef int (*SWIOT_SDK_Device_Upgrade)(swiot_service_type_t type,
                                        int message_id,
                                        char *product_key,
                                        char *device_name,
                                        const char *upgrade_url,
                                        const char *md5,
                                        const char *curversion);

/**
 * 设备句柄
 * get                      读缓存接口
 * set                      写缓存接口
 * service                  服务接口
 * enable                   启用接口
 * disable                  禁用接口
 * remove                   删除子设备接口
 * upgrade                  升级接口
 **/
typedef struct
{
    SWIOT_SDK_Device_Get get;
    SWIOT_SDK_Device_Set set;
    SWIOT_SDK_Device_Service service;
    SWIOT_SDK_Device_Enable enable;
    SWIOT_SDK_Device_Disable disable;
    SWIOT_SDK_Device_Remove remove;
    SWIOT_SDK_Device_Upgrade upgrade;
} device_handle_t;

#endif
