#pragma once

#include "header.h"
#include "textblob.h"
#include "trie.h"

enum RSVFilePermEnum
{
    FILE_PERM_READ = 1,
    FILE_PREM_WRITE = 2,
    FILE_PERM_WRITE = 4,
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

struct _rs_vfile_info_stru
{
    char *fname;
    size_t fsize;
    
    struct
    {
        bool r; /* read */
        bool w; /* write */
        bool d; /* delete */
    } perms;
};

typedef struct _rs_vfile_info_stru vfile_info_t;

enum
{
    RS_VMGR_VDNODE_TYPE_FILE,
    RS_VMGR_VDNODE_TYPE_DIR,
};

struct _rs_vdir_stru;
struct _rs_vdir_cont_s
{
    int type;

    union
    {
        vfile_t *vf;
        struct _rs_vdir_stru *dr;
    } v;
};

typedef struct _rs_vdir_cont_s vdnode_t;
struct _rs_vdir_stru
{
    tblob_t *name;
    trie_t *nodes; /* void* is vdnode_t* */
    char **node_list;
    size_t nl_count;
};

typedef struct _rs_vdir_stru vdir_t;

struct _rs_vdir_info_stru
{
    char *name_;
    size_t dir_size;
    char **files_;
    size_t fcount;
    char **dirs_;
    size_t dir_count;
};

typedef struct _rs_vdir_info_stru vdir_info_t;

#ifdef __cplusplus
extern "C"
{
#endif

    RS_API vfile_t *rs_vmgr_vfile_new(void);
    RS_API vfile_t *rs_vmgr_vfile_newWithData(char *_Name, char *_Cont, int _Perms);
    RS_API int rs_vmgr_vfile_dump(vfile_t *, FILE *);
    RS_API vfile_t *rs_vmgr_vfile_read(FILE *);
    RS_API void rs_vmgr_vfile_dbgprint(vfile_t *);

    RS_API vdir_t *rs_vmgr_vdir_new(char *);
    RS_API vdnode_t *rs_vmgr_vdnode_new(int, void *);
    RS_API void rs_vmgr_vdir_addfile(vdir_t *, vfile_t *);
    RS_API void rs_vmgr_vdir_adddir(vdir_t *, vdir_t *);
    RS_API int rs_vmgr_vdir_dump(vdir_t *, FILE *);
    RS_API vdir_t *rs_vmgr_vdir_read(FILE *);

    RS_API vfile_info_t *rs_vmgr_vfile_info_new(
        char *_FName, size_t _FSize, 
        bool _R, bool _W, bool _D);

    RS_API vfile_info_t *rs_vmgr_vfile_info_fetch(vfile_t *);

    RS_API vdir_info_t *rs_vmgr_vdir_info_new(
        char *_Name, size_t _Dir_Size,
        char **_Files, size_t _FCount,
        char **_Dirs, size_t _Dir_Count);
    
    RS_API vdir_info_t *rs_vmgr_vdir_info_fetch(vdir_t *);
    RS_API void rs_vmgr_vdir_info_free(vdir_info_t *);

#ifdef __cplusplus
}
#endif
