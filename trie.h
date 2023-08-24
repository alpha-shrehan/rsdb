#pragma once

#include "header.h"

struct _trie_node_s
{
    char *key;
    size_t kc;

    bool marked;
    void *val;

    struct _trie_node_s **children;
    size_t child_count;
};

typedef struct _trie_node_s trie_t;

#ifdef __cplusplus
extern "C"
{
#endif

    RS_API trie_t *rs_trie_new(void);
    RS_API void rs_trie_add(trie_t *, char *, void *);
    RS_API void rs_trie_remove(trie_t *, char *);
    RS_API void *rs_trie_get(trie_t *, char *);
    RS_API void rs_trie_free(trie_t *);
    RS_API void rs_trie_dgbprint(trie_t *);

#ifdef __cplusplus
}
#endif
