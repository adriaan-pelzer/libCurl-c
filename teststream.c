#include "curl.h"
#include "addr.h"

int streamCB (void *uCtx, char **memory, size_t *size) {
    int rc = -1;
    size_t *count = (size_t *) uCtx;
    size_t _count = *count;
    char *_memory = (char *) *memory;
    size_t _size = (size_t) *size;

    printf("%s\n", _memory);
    if (_count++ >= 9) goto over;

    printf("%d", (int) _count);

    rc = 0;
over:
    *count = _count;
    F(_memory);
    _memory = NULL;
    *memory = _memory;
    _size = 0;
    *size = _size;
    return rc;
}

int returnCB (void *uCtx, const char *memory, const size_t size) {
    return 0;
}

int main (void) {
    int rc = EXIT_FAILURE;
    size_t count1 = 0, count2 = 0;
    
    DONTS("connect POST", curl_connect, 0, "http://54.246.125.55/api/teststream", CONN_POST, "hallo=bye&yes=no", (void *) &count1, returnCB, streamCB);
    DONTS("connect GET", curl_connect, 0, "http://54.246.125.55/api/teststream?hallo=bye&yes=no", CONN_GET, NULL, (void *) &count2, returnCB, streamCB);

    rc = EXIT_SUCCESS;
    goto over;
over:
    return rc;
}
