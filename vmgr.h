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
    tblob_t *fname;

    struct
    {
        int perms;
    } meta;
};

typedef struct _rs_vfile_stru vfile_t;

#ifdef __cplusplus
extern "C"
{
#endif

    RS_API vfile_t *rs_vmgr_vfile_new(void);
    RS_API vfile_t *rs_vmgr_vfile_newWithData(char *_Name, char *_Cont, int _Perms);
    RS_API int rs_vmgr_vfile_dump(vfile_t *, FILE *);
    RS_API vfile_t *rs_vmgr_vfile_read(FILE *);
    RS_API void rs_vmgr_vfile_dbgprint(vfile_t *);

#ifdef __cplusplus
}
#endif
