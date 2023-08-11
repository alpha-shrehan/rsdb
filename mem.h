#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define RS_Malloc(X) rs_malloc((X))
#define RS_Realloc(X, Y) rs_realloc((X), (Y))
#define RS_Free(X) rs_free((X))

#ifndef RS_API
#define RS_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    RS_API void *rs_malloc(size_t);
    RS_API void *rs_realloc(void *, size_t);
    RS_API void rs_free(void *);

#ifdef __cplusplus
}
#endif
