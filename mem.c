#include "mem.h"

RS_API void *
rs_malloc(size_t size)
{
    void *p = malloc(size);
    assert(!!p);

    return p;
}

RS_API void *
rs_realloc(void *old_ptr, size_t size)
{
    if (!size)
    {
        rs_free(old_ptr);
        return NULL;
    }

    void *n = realloc(old_ptr, size);
    return n;
}

RS_API void
rs_free(void *ptr)
{
    if (!ptr)
        return;
    free(ptr);
}