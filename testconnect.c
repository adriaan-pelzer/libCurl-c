#include "curl.h"
#include "addr.h"

static int streamCB (void *uCtx, char **memory, size_t *size) {
    return 0;
}

static int returnCB (void *uCtx, const char *memory, const size_t size) {
    char *u_ctx = (char *) uCtx;
    printf("User Context: %s\n\n", u_ctx);
    printf("%s\n", memory);
    return 0;
}

int main (void) {
    int rc = EXIT_FAILURE;
    char *uCtx = NULL;
   
    DOA("allocate memory for user context", strdup, uCtx, NULL, "This user context has to be passed throughout, unchanged by the library");

    DONT("connect POST", curl_connect, 0, "http://54.246.125.55/api/test", CONN_POST, "hallo=bye&yes=no", (void *) uCtx, returnCB, streamCB);
    DONT("connect GET", curl_connect, 0, "http://54.246.125.55/api/test?hallo=bye&yes=no", CONN_GET, NULL, (void *) uCtx, returnCB, streamCB);

    rc = EXIT_SUCCESS;
over:
    F(uCtx);
    return rc;
}
