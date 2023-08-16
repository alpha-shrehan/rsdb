#include "vmgr.h"

RS_API vfile_t *
rs_vmgr_vfile_new(void)
{
    vfile_t *f = RS_Malloc(sizeof(vfile_t));
    f->fcont = NULL;
    f->fname = NULL;
    f->meta.perms = FILE_PERM_READ;

    return f;
}

RS_API vfile_t *
rs_vmgr_vfile_newWithData(
    char *fname, 
    char *fcont,
    int perms)
{
    vfile_t *f = rs_vmgr_vfile_new();
    f->fcont = rs_tblob_new_fromRaw(fcont);
    f->fname = rs_tblob_new_fromRaw(fname);
    f->meta.perms = perms;
    
    return f;
}

RS_API int 
rs_vmgr_vfile_dump(vfile_t *vf, FILE *f)
{
    if (!f)
        return RSERR_NOFILE;
    
    if (!(vf->meta.perms & FILE_PERM_WRITE))
        return RSERR_FILE_INVALIDPERMS;
    
    rs_tblob_dump(vf->fname, f);
    rs_tblob_dump(vf->fcont, f);
    fwrite(&vf->meta.perms, sizeof(int), 1, f);

    return RSERR_OK;
}

RS_API vfile_t *
rs_vmgr_vfile_read(FILE *f)
{
    vfile_t *v = rs_vmgr_vfile_new();
    v->fname = rs_tblob_read(f);
    v->fcont = rs_tblob_read(f);
    fread(&v->meta.perms, sizeof(int), 1, f);

    return v;
}

RS_API void 
rs_vmgr_vfile_dbgprint(vfile_t *vf)
{
    char *dc_vfname = rs_tblob_decode(vf->fname);
    char *dc_vfcont = rs_tblob_decode(vf->fcont);

    printf("[fname] %s, perms: %d\n[fcont]\n%s", dc_vfname, vf->meta.perms, dc_vfcont);

    RS_Free(dc_vfname);
    RS_Free(dc_vfcont);
}