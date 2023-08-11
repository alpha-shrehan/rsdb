#pragma once

#include "header.h"
#include "textblob.h"

enum RSVFilePermEnum
{
    FILE_PERM_READ = 1,
    FILE_PREM_WRITE = 2,
    FILE_PERM_APPEND = 4,
    FILE_PERM_DELETE = 8
};

struct _rs_vfile_stru
{
    tblob_t *fcont;
    char *fname;

    struct
    {
        int perms;
    } meta;
};

typedef struct _rs_vfile_stru vfile_t;