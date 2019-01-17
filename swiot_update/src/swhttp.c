#include "swhttp.h"
#include "swdownload.h"
// #define DEBUG 1

// int begin ;

void remote_info(struct hostent *he)
{
	assert(he);

	printf("主机的官方名称：%s\n", he->h_name);

	int i;
	for(i=0; he->h_aliases[i]!=NULL; i++)
	{
		printf("别名[%d]：%s\n", i+1, he->h_aliases[i]);
	}

	printf("IP地址长度：%d\n", he->h_length);

	for(i=0; he->h_addr_list[i]!=NULL; i++)
	{
		printf("IP地址[%d]：%s\n", i+1,
			inet_ntoa(*((struct in_addr **)he->h_addr_list)[i]));
	}
}


void http_request(char*buf, int size, char *host, char *filepath, unsigned long long begin)
{
	assert(buf);
	//assert(phone_code);

	bzero(buf, size);
	snprintf(buf, size, "GET /%s HTTP/1.1\r\n"
			    "Range: bytes=%lld-\r\n"
			    "Host:%s\r\n\r\n"
			   , filepath, begin, host);
}

bool analyze_web(const char *url, char **host, int *port,char **file)
{	
	int i =0;
	char *p = NULL;
	char m_port[100] = {0};
	char *tmp = calloc(1, 100);
	
	strncpy(tmp, url, strlen(url));
	
	if(!strncmp(tmp+(strlen(tmp)-1), "/", 1))
		return false;

	if(strstr(tmp, "http://"))
	{
		tmp = strstr(tmp, "//");
		tmp = tmp+2;
	}
	else if(strstr(tmp, "https://"))
	{
		tmp = strstr(tmp, "//");
		tmp = tmp+2;
	}
	
	p = strstr(tmp, ":");
	if(p != NULL)
	{	
		while(*++p != '/')
			m_port[i++] = *p;
	
		m_port[i] = '\0';
		
		*port = atoi(m_port);
	}	
	else
		p = strstr(tmp, "/");
	
	if(p != NULL)
	{
		p = p + 1;
		if(!strcmp(p, ""))
			return false;
		else
			*file = p;
	}
	else
		return false;

	*host = strtok(tmp, "/");
	*host = strtok(tmp, ":");
	return true;
}

void *routine(void *arg)
{
	int i;
	while(1)
	{
		printf("Connecting");
				
		fflush(stdout);
		
		for(i=0;i<5;i++)
		{	
			sleep(1);
		
			printf(".");
			
			fflush(stdout);
		}
		printf("\33[2K\r");
	}
	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL);
	//立即响应
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	pthread_exit(NULL);
}

int downloading(char *host, unsigned long long tol, unsigned long long size)
{
	int i, j;
	char sql[256];
	char load[256];

	bzero(sql, 256);
	bzero(load, 256);

	sprintf(load, "--------------------");
	
	int len  = ((double)tol/(double)size)*strlen(load);
	for(i=0; i<len;i++)
	{
		load[i] = '#';
	}
	sprintf(sql, "%s", load);
	if(len == strlen(load))
	{
		printf("\33[2K\r");
		printf("(from:%s):Bytes received:[%.1fK/%.1fK] downloaded!\n", host, (float)tol/1024, (float)size/1024);
		return 0;
	}
	printf("from:%s:Bytes received[%.1fK/%.1fK] [%s] downloading\r", host, (float)tol/1024, (float)size/1024, sql);
	
	fflush(stdout);
	
	//printf("%c[2K", 27);/*有个都可达到清掉一行的效果*/
	
	printf("\33[2K\r");
}

int http_request_main(const char *upgrade_url, char *upgrade_filename, int flag)
{
	int port = 0;
	char *host = calloc(1, 100);
	char *FilePath = calloc(1, 100);
	
	if(!analyze_web(upgrade_url, &host, &port, &FilePath))
	{
		printf("web address is wrong!\n");
		return -1;
	}
	printf("host = %s\n",host);
	printf("port = %d\n",port);
	printf("filepath = %s\n", FilePath);
	
	struct hostent *he = gethostbyname(host);
	if(he == NULL)
	{
		errno = h_errno;
		perror("gethostbyname() failed");
		return -1;
	}

#ifdef DEBUG
	remote_info(he);
#endif
	pthread_t tid;
	pthread_create(&tid, NULL, routine, NULL);

	struct in_addr **addr_list = (struct in_addr **)(he->h_addr_list);

	int fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr, len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port   = htons(port);
	srvaddr.sin_addr   = *addr_list[0];

	if(connect(fd, (struct sockaddr *)&srvaddr, len) == -1)
	{
		//线程取消
		pthread_cancel(tid);
		//等待子线程退出
		pthread_join(tid, NULL);
		close(fd);
		perror("connect() failed");
		return -1;
	}

	char *file_name = FilePath;
	char *tmp = FilePath;
	while(1)
	{
		tmp = strstr(tmp, "/");
		if(tmp == NULL)
		{
			break;
		}	
		tmp = tmp + 1;
		file_name = tmp;
	}
	char cmd_value[100] = {0};
	char pack_absolute_path[100] = {0};
	sprintf(pack_absolute_path, "%s/%s", DATA_ROOT_PATH, file_name);
	printf("pack_absolute_path is %s\n", pack_absolute_path);
	
	memset(upgrade_filename, 0, sizeof(upgrade_filename));
	strncpy(upgrade_filename, pack_absolute_path, strlen(pack_absolute_path));
		
	int file_fd;
	unsigned long long begin_num = 0;
	if(!access(pack_absolute_path, F_OK))
	{
		if(flag)
		{
			memset(cmd_value, 0, sizeof(cmd_value));
			sprintf(cmd_value, "rm %s", pack_absolute_path);
			system(cmd_value);
			
			file_fd = open(pack_absolute_path, O_RDWR | O_CREAT | O_TRUNC, 0777);
		}
		else
		{
			struct stat fileinfo;
			stat(pack_absolute_path, &fileinfo);

			begin_num = fileinfo.st_size;
			
			file_fd = open(pack_absolute_path, O_RDWR);
		}
	}
	else
	{
		file_fd = open(pack_absolute_path, O_RDWR | O_CREAT | O_TRUNC, 0777);
	}
	if(file_fd == -1)
	{
		perror("open() failed");
		return -1;
	}

	char *sndbuf = calloc(1, 1000);
	http_request(sndbuf, 1000, host, FilePath, begin_num);   

#ifdef DEBUG
	printf("Request:\n");
	printf("+++++++++++++++++++++++++++++++\n");
	printf("%s", sndbuf);
	printf("+++++++++++++++++++++++++++++++\n\n");
#endif

	int n = send(fd, sndbuf, strlen(sndbuf), 0);
	if(n == -1)
	{
		//线程取消
		pthread_cancel(tid);
		//等待子线程退出
		pthread_join(tid, NULL);
		close(fd);
		perror("send() failed");
		return -1;
	}

	free(sndbuf);

#ifdef DEBUG
	printf("[%d] bytes have been sent.\n", n);
#endif

	//线程取消
	pthread_cancel(tid);
	//等待子线程退出
	pthread_join(tid, NULL);
	printf("\r");

	char *Response_head = calloc(1, 1024);
	n = 0;
	while(1)
	{
		int nread = read(fd, Response_head+n, 1);
		if(nread == 0)
			break;

		if(nread == -1)
		{
			perror("recv failed");
			return -1;
		}
		n++;

		if(strstr(Response_head, "\r\n\r\n"))
			break;
	}

	
#ifdef DEBUG
	printf("Response HTTP head:\n");
	printf("*******************************\n");
	printf("%s", Response_head);
	printf("\n*******************************\n");
#endif
	
	char *range = strstr(Response_head, "Content-Range: ");

	char *size = strstr(range, "/");
	size = size + 1;

	//本次要下载的大小:length(bytes)
	char *length = strstr(Response_head, "Content-Length:") +
		       strlen("Content-Length: ");
	if(atoi(length) == 0)
	{
		printf("%s is already download.\n", FilePath);
		return 0;
	}

	unsigned long long total_Bytes = begin_num;
	// 读取下载的文件内容
	char *recvbuf  = calloc(1, 1024);
	while(1)
	{
		if(!downloading(host, total_Bytes, (unsigned long long)atoi(size)))
			break;

		if(total_Bytes >= atoi(size))
			break;

		n = recv(fd, recvbuf, 1024, 0);

		if(n == 0)
			break;

		if(n == -1)
		{
			perror("recv failed");
			return -1;
		}

		write(file_fd, recvbuf, n);

		total_Bytes += n;
	}

#ifdef DEBUG
	printf("*******************************\n");
	printf("[%ld] bytes have been received.\n\n", total_Bytes);
#endif
	
	free(Response_head);
	free(recvbuf);
	close(file_fd);
	
	return 0;
}

