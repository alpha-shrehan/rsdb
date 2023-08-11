#pragma once

#include "header.h"

/**
 * @brief Create Tokens from Unique Characters
 *
 * This struct represents a set of tokens based on unique characters extracted from a given string. 
 * The unique characters are collected into the `key` string, 
 * and `toks` stores the indices of these characters within the `key`.

 * Example:
 * Input String: "The big brown fox jumped over the lazy dog."
 * Resulting `key`: "The bigrownfxjumpdvlazy"
 * Note: Spaces are also treated as tokens.
 * Resulting `toks`: {0, 1, 2, 3, 4, 5, 6, 3, ...}
 */

struct _rsdb_text_blob_stru
{
    char *key;
    int *toks;
    size_t size;
};

typedef struct _rsdb_text_blob_stru tblob_t;

#ifdef __cplusplus
extern "C"
{
#endif

    RS_API tblob_t *rs_tblob_new(char *, int *, size_t);
    RS_API tblob_t *rs_tblob_new_fromRaw(char *);
    RS_API char *rs_tblob_decode(tblob_t *);

#ifdef __cplusplus
}
#endif
