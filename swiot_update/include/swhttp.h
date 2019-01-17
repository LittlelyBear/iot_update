#ifndef __SW_HTTP_H
#define __SW_HTTP_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <pthread.h>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "swdownload.h"

void remote_info(struct hostent *he);
int http_request_main(const char *upgrade_url, char *upgrade_filename, int flag);
int downloading(char *host, unsigned long long tol, unsigned long long size);
bool analyze_web(const char *argv, char **host, int *port,char **file);
void http_request(char*buf, int size, char *host, char *filepath, unsigned long long begin);

#endif