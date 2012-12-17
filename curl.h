#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <curl.h>
#include <ctype.h>
#include <syslog.h>
#include <string.h>

#ifndef _CURL_H_
#define _CURL_H_

typedef enum connectionType {
    CONN_GET,
    CONN_POST
} connectionType;

struct MemoryStruct {
    CURL *curl_handle;
    char glob_init;
    connectionType ctype;
    char *url;
    char *postargs;
    int (*streamCB)(void *uCtx, char **, size_t *);
    int (*returnCB)(void *uCtx, const char *, const size_t);
    void *uCtx;
    char *memory;
    size_t size;
};

struct MemoryStruct *createMemoryStruct();
int setConnectionParms(struct MemoryStruct *mem, const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *));
void freeMemoryStruct(struct MemoryStruct *mem);
int curl_connect(const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *));
char *curl_connect_return_url(const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *));

#endif
