#include "addr.h"
#include "curl.h"

int ProgressCallback(void *data, double dltotal, double dlnow, double ultotal, double ulnow) {
    size_t rc = -1;
    struct MemoryStruct *mem = (struct MemoryStruct *) data;

    if (mem->progressCB) {
        DONT("call progress callback function", mem->progressCB, 0, mem->uCtx, dltotal, dlnow, ultotal, ulnow);
    }

    rc = 0;
over:
    return rc;
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) {
    size_t rc = -1;
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) data;

    DOA("reallocate memory for new data", realloc, mem->memory, NULL, mem->memory, mem->size + realsize + 1);
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    if (mem->streamCB) {
        DONT("call stream callback function", mem->streamCB, 0, mem->uCtx, (char **) &mem->memory, (size_t *) &mem->size);
    }

    rc = realsize;
over:
    return rc;
}

int curl_init_if_not_init(struct MemoryStruct *mem) {
    int rc = -1;

    if (mem->glob_init != 1) { DONT("init curl", curl_global_init, CURLE_OK, CURL_GLOBAL_ALL); mem->glob_init = 1; }
    if (mem->curl_handle == NULL) DOA("get curl handle", curl_easy_init, mem->curl_handle, NULL);

    rc = 0;
over:
    return rc;
}

#define SETOPT(X, Y) ENT(curl_easy_setopt, CURLE_OK, "Cannot set curl opt " #X, mem->curl_handle, X, Y)
int curl_setopts(struct MemoryStruct *mem) {
    int rc = -1;

    SETOPT(CURLOPT_URL, mem->url);
    SETOPT(CURLOPT_SSL_VERIFYHOST, 0);
    SETOPT(CURLOPT_SSL_VERIFYPEER, 1);
    SETOPT(CURLOPT_CAINFO, "/etc/cacert.pem");
    SETOPT(CURLOPT_FAILONERROR, 1);
    SETOPT(CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    SETOPT(CURLOPT_WRITEDATA, (void *) mem);
    SETOPT(CURLOPT_NOPROGRESS, 0);
    SETOPT(CURLOPT_PROGRESSFUNCTION, ProgressCallback);
    SETOPT(CURLOPT_PROGRESSDATA, (void *) mem);
    SETOPT(CURLOPT_USERAGENT, "curl/7.26.0");
    SETOPT(CURLOPT_NOSIGNAL, 1);
    SETOPT(CURLOPT_FOLLOWLOCATION, 1);
    SETOPT(CURLOPT_MAXREDIRS, 10);

    if (mem->ctype == CONN_POST) {
        SETOPT(CURLOPT_POST, 1);
        SETOPT(CURLOPT_POSTFIELDS, (void *) mem->postargs);
        SETOPT(CURLOPT_POSTFIELDSIZE, strlen(mem->postargs));
    } else {
        SETOPT(CURLOPT_HTTPGET, 1);
    }

    rc = 0;
over:
    return rc;
}

void freeMemoryStruct(struct MemoryStruct *mem) {
    if (mem) {
        F(mem->memory);
        FF(mem->curl_handle, curl_easy_cleanup);
        if (mem->glob_init) { curl_global_cleanup(); mem->glob_init = 0; }
        F(mem);
    }
    return;
}

struct MemoryStruct *createMemoryStruct() {
    struct MemoryStruct *mem = NULL, *_mem = NULL;

    DOA("allocate memory for main object", malloc, _mem, NULL, sizeof(struct MemoryStruct));
    memset(_mem, 0, sizeof(struct MemoryStruct));
    DOA("allocate scratch memory inside main object", malloc, _mem->memory, NULL, 1);
    memset(_mem->memory, 0, 1);
    _mem->size = 0;

    mem = _mem;
over:
    FFF(_mem, freeMemoryStruct, _mem && (mem == NULL));
    return mem;
}

int setConnectionParms(struct MemoryStruct *mem, const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *), int (*pCB)(void *, double, double, double, double)) {
    int rc = -1;

    if (url == NULL) { syslog(P_ERR, "Cannot connect to empty URL"); goto over; }
    if ((ctype != CONN_GET) && (ctype != CONN_POST)) { syslog(P_ERR, "Connection type has to be one of CONN_GET or CONN_POST"); goto over; }

    mem->ctype = ctype;
    mem->postargs = (char *) postargs;
    mem->url = (char *) url;
    mem->streamCB = sCB;
    mem->returnCB = rCB;
    mem->progressCB = pCB;
    mem->uCtx = uCtx;

    rc = 0;
over:
    return rc;
}

int curl_connect(const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *)) {
    int rc = -1;
    CURLcode cr = CURLE_FAILED_INIT;
    long int http_code = 0;
    struct MemoryStruct *mem = NULL;

    DOA("create new curl context", createMemoryStruct, mem, NULL);
    DONT("set connection parameters", setConnectionParms, 0, mem, url, ctype, postargs, uCtx, rCB, sCB, NULL);
    DONT("init curl", curl_init_if_not_init, 0, mem);
    DONT("set curl options", curl_setopts, 0, mem);

    if ((cr = curl_easy_perform(mem->curl_handle)) != CURLE_OK) {
        syslog(P_ERR, "Cannot perform curl transaction: %s", curl_easy_strerror(cr));
        goto over;
    }

    DONT("extract HTTP code from curl handle", curl_easy_getinfo, CURLE_OK, mem->curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    syslog (P_DBG, "HTTP code returned: %ld", http_code);

    if (http_code == 200) {
        if (mem->returnCB) {
            DONT("call return callback function", mem->returnCB, 0, mem->uCtx, (const char *) mem->memory, (const size_t) mem->size);
        }
    } else {
        syslog(P_ERR, "URL '%s' returned HTTP code %ld", mem->url, http_code);
        goto over;
    }

    rc = 0;
over:
    if ((rc == -1) && (cr != CURLE_OK)) {
        rc = (int) cr;
    }
    FF(mem, freeMemoryStruct);
    return rc;
}

int curl_connect_progress(const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *), int (*pCB)(void *, double, double, double, double)) {
    int rc = -1;
    CURLcode cr = CURLE_FAILED_INIT;
    long int http_code = 0;
    struct MemoryStruct *mem = NULL;

    DOA("create new curl context", createMemoryStruct, mem, NULL);
    DONT("set connection parameters", setConnectionParms, 0, mem, url, ctype, postargs, uCtx, rCB, sCB, pCB);
    DONT("init curl", curl_init_if_not_init, 0, mem);
    DONT("set curl options", curl_setopts, 0, mem);

    if ((cr = curl_easy_perform(mem->curl_handle)) != CURLE_OK) {
        syslog(P_ERR, "Cannot perform curl transaction: %s", curl_easy_strerror(cr));
        goto over;
    }

    DONT("extract HTTP code from curl handle", curl_easy_getinfo, CURLE_OK, mem->curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    syslog (P_DBG, "HTTP code returned: %ld", http_code);

    if (http_code == 200) {
        if (mem->returnCB) {
            DONT("call return callback function", mem->returnCB, 0, mem->uCtx, (const char *) mem->memory, (const size_t) mem->size);
        }
    } else {
        syslog(P_ERR, "URL '%s' returned HTTP code %ld", mem->url, http_code);
        goto over;
    }

    rc = 0;
over:
    if ((rc == -1) && (cr != CURLE_OK)) {
        rc = (int) cr;
    }
    FF(mem, freeMemoryStruct);
    return rc;
}

char *curl_connect_return_url(const char *url, connectionType ctype, const char *postargs, void *uCtx, int (*rCB)(void *, const char *, const size_t), int (*sCB)(void *, char **, size_t *)) {
    char *rc = NULL, *_rc = NULL;
    CURLcode cr = CURLE_FAILED_INIT;
    long int http_code = 0;
    char *http_url = NULL;
    struct MemoryStruct *mem = NULL;

    DOA("create new curl context", createMemoryStruct, mem, NULL);
    DONT("set connection parameters", setConnectionParms, 0, mem, url, ctype, postargs, uCtx, rCB, sCB);
    DONT("init curl", curl_init_if_not_init, 0, mem);
    DONT("set curl options", curl_setopts, 0, mem);

    if ((cr = curl_easy_perform(mem->curl_handle)) != CURLE_OK) {
        syslog(P_ERR, "Cannot perform curl transaction: %s", curl_easy_strerror(cr));
        goto over;
    }

    DONT("extract HTTP code from curl handle", curl_easy_getinfo, CURLE_OK, mem->curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    syslog (P_DBG, "HTTP code returned: %ld", http_code);

    if (http_code == 200) {
        DONT("extract effective URL from curl handle", curl_easy_getinfo, CURLE_OK, mem->curl_handle, CURLINFO_EFFECTIVE_URL, &http_url);
        DOA("allcate memory for effective URL", strdup, _rc, NULL, http_url);

        if (mem->returnCB) {
            DONT("call return callback function", mem->returnCB, 0, mem->uCtx, (const char *) mem->memory, (const size_t) mem->size);
        }
    } else {
        syslog(P_ERR, "URL '%s' returned HTTP code %ld", mem->url, http_code);
        goto over;
    }

    rc = _rc;
over:
    FFF(_rc, free, _rc && (rc == NULL));
    FF(mem, freeMemoryStruct);
    return rc;
}
