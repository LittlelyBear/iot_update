#include "swdownload.h"

#define MAX_VALUE 150
int swDownloadJudge(const char * upgrade_version)
{
	char *cur_version = NULL;
	cur_version = getParmValue(VERSION_PATH, "ro.build.version.release");
	printf("get build.prop version is %s\n", cur_version);
	
	if(!strcmp(upgrade_version, cur_version))
	{
		printf("version is same, no need to update!\n");
		free(cur_version);
		return -1;
	}
	else{
		printf("cur_version = %s\n", upgrade_version);
		free(cur_version);
		return 0;
	}
}

char* getParmValue(char *file_name, char *parm_name)
{
	int m_fd = open(file_name, O_RDONLY);
	if(m_fd == -1)
	{
		perror("open file error:");
		return NULL;
	}
	
	char parmName[MAX_VALUE] = {0};
	char parmValue[MAX_VALUE] = {0};
	char *ret_value = NULL;
	long tmp = 0;
	int i=0, j=0, nread=0;
	
	while(1)
	{
		nread = read(m_fd, &parmName[i], 1);
		
		if(nread == 0)
		{
			printf("read over, can't find your arg\n");
			return NULL;
		}
		++tmp;
		if(parmName[i] == '='|| parmName[i] == '\n')
		{
			parmName[i] = '\0';
			
			if(!strcmp(parmName, parm_name))
			{				
				break;
			}
			i = 0;
			memset(parmName, 0, sizeof(parmName));
			continue;
		}
	
		i++;
	}
	
	lseek(m_fd, tmp, SEEK_SET);
	
	while(1)
	{
		read(m_fd, &parmValue[j] ,1);
		
		if(parmValue[j] == '\n')
		{
			parmValue[j] = '\0'; 

			break;
		}
		j++;
	}
	
	printf("%s = %s\n", parmName, parmValue);
	ret_value = calloc(1, strlen(parmValue)+1);
	if(NULL == ret_value)
	{
		printf("calloc fail!\n");
		return NULL;
	}
	strncpy(ret_value, parmValue, strlen(parmValue)+1);
	
	return ret_value;
}

int strIcmp(const char* p1, const char* p2)
{
    while (*p1)
    {
        char c1 = *p1;
        char c2 = *p2;
        if (c1 >= 'A' && c1 <= 'Z')
        {
            c1 += 'a' - 'A';
        }
        if (c2 >= 'A' && c2 <= 'Z')
        {
            c2 += 'a' - 'A';
        }
        if (c1 != c2)
        {
            return c1 - c2;
        }
        p1++;
        p2++;
    }
    return *p1 - *p2;
}

int swMd5sum(const char* download_path, const char* md5)
{
	char cmd_md5[MAX_VALUE] = {0};
	char upgrade_path[MAX_VALUE] = {0};
	char tmp[MAX_VALUE] = {0};
	char read_md5[MAX_VALUE] = {0};
	
	printf("start vertify md5!\n");
    
	sprintf(cmd_md5,"md5sum %s > %s",download_path, MD5_TXT_PATH);
	
	printf("md5sum = %s\n", cmd_md5);
	
	system(cmd_md5);
	
	int md5_fd = open(MD5_TXT_PATH, O_RDONLY);
	if(-1 == md5_fd)
	{
		printf("open /data/sunniwell/md5.txt fail!\n");
		return -1;
	}
	
	swGetInfo(md5_fd, read_md5);
	printf("read md5.txt md5 is %s\n", read_md5);
	
	if(strIcmp(md5, read_md5))
	{
		printf("md5sum diff, please download agin!\n");
		close(md5_fd);
		return -1;
	}
	else{
		printf("md5sum is the same!\n");
		close(md5_fd);
		return 0;
	}	
}

int swGetInfo(int fd, char *string)
{
	int i = 0;
	while(1)
	{
		int nread = read(fd, &string[i], 1);
		if(0 == nread ||(string[i] == 32))
		{
			string[i] = '\0';
			break;
		}	
		i++;
	}
	
	return 0;
}
