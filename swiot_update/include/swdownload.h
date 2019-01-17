#ifndef __DOWNLOAD_H
#define __DOWNLOAD_H

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

#define DATA_ROOT_PATH "/data/sunniwell"
#define MD5_TXT_PATH "/data/sunniwell/md5.txt"
#define VERSION_PATH "/system/build.prop"
#define UPGRADE_PATH "/data/sunniwell/"

/*

*/
extern int swDownloadJudge(const char *upgrade_version);
extern char* getParmValue(char *file_name, char *parm_name);
extern int strIcmp(const char* p1, const char* p2);
extern int swMd5sum(const char* download_path, const char* md5);
extern int swGetInfo(int fd, char *string);


#endif