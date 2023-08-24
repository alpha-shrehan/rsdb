#include "vmgr.h"

void _trie_dump(trie_t *, FILE *);
trie_t *_trie_read(FILE *);

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

RS_API vdir_t *
rs_vmgr_vdir_new(char *name)
{
    vdir_t *t = RS_Malloc(sizeof(vdir_t));
    t->name = rs_tblob_new_fromRaw(name);
    t->nodes = rs_trie_new();
    t->node_list = NULL;
    t->nl_count = 0;

    return t;
}

RS_API vdnode_t *
rs_vmgr_vdnode_new(int ty, void *val)
{
    vdnode_t *t = RS_Malloc(sizeof(vdnode_t));
    t->type = ty;

    if (t->type == RS_VMGR_VDNODE_TYPE_FILE)
        t->v.vf = (vfile_t *)val;
    else if (t->type == RS_VMGR_VDNODE_TYPE_DIR)
        t->v.dr = (vdir_t *)val;

    return t;
}

RS_API void
rs_vmgr_vdir_addfile(vdir_t *t, vfile_t *f)
{
    char *dd_name = rs_tblob_decode(f->fname);

    rs_trie_add(
        t->nodes,
        dd_name,
        rs_vmgr_vdnode_new(RS_VMGR_VDNODE_TYPE_FILE, f));

    t->node_list = RS_Realloc(
        t->node_list,
        (t->nl_count + 1) * sizeof(char *));

    t->node_list[t->nl_count++] = dd_name;
}

RS_API void
rs_vmgr_vdir_adddir(vdir_t *t, vdir_t *d)
{
    char *dd_name = rs_tblob_decode(d->name);

    rs_trie_add(
        t->nodes,
        dd_name,
        rs_vmgr_vdnode_new(RS_VMGR_VDNODE_TYPE_DIR, d));

    t->node_list = RS_Realloc(
        t->node_list,
        (t->nl_count + 1) * sizeof(char *));

    t->node_list[t->nl_count++] = dd_name;
}

RS_API int
rs_vmgr_vdir_dump(vdir_t *v, FILE *f)
{
    if (!f)
        return RSERR_NOFILE;

    rs_tblob_dump(v->name, f);
    _trie_dump(v->nodes, f);
}

void _trie_dump(trie_t *t, FILE *f)
{
    fwrite(&t->child_count, sizeof(size_t), 1, f);

    for (size_t i = 0; i < t->child_count; i++)
        _trie_dump(t->children[i], f);

    fwrite(&t->kc, sizeof(size_t), 1, f);
    fwrite(t->key, sizeof(char), t->kc, f);
    fwrite(&t->marked, sizeof(bool), 1, f);

    if (t->marked)
    {
        vdnode_t *nd = (vdnode_t *)t->val;
        fwrite(&nd->type, sizeof(int), 1, f);

        if (nd->type == RS_VMGR_VDNODE_TYPE_FILE)
            rs_vmgr_vfile_dump(nd->v.vf, f);

        else if (nd->type == RS_VMGR_VDNODE_TYPE_DIR)
            rs_vmgr_vdir_dump(nd->v.dr, f);
    }
}

trie_t *_trie_read(FILE *f)
{
    trie_t *t = rs_trie_new();
    fread(&t->child_count, sizeof(size_t), 1, f);

    t->children = RS_Malloc(t->child_count * sizeof(trie_t *));

    for (size_t i = 0; i < t->child_count; i++)
        t->children[i] = _trie_read(f);

    fread(&t->kc, sizeof(size_t), 1, f);

    t->key = RS_Malloc(t->kc * sizeof(char));

    fread(t->key, sizeof(char), t->kc, f);
    fread(&t->marked, sizeof(bool), 1, f);

    if (t->marked)
    {
        int ndtype = -1;
        fread(&ndtype, sizeof(int), 1, f);
        vdnode_t *vn = rs_vmgr_vdnode_new(ndtype, NULL);

        switch (ndtype)
        {
        case RS_VMGR_VDNODE_TYPE_FILE:
        {
            vn->v.vf = rs_vmgr_vfile_read(f);
        }
        break;
        case RS_VMGR_VDNODE_TYPE_DIR:
        {
            vn->v.dr = rs_vmgr_vdir_read(f);
        }
        break;

        default:
            break;
        }

        t->val = (void *)vn;
    }

    return t;
}

RS_API vdir_t *
rs_vmgr_vdir_read(FILE *f)
{
    if (!f)
        return NULL;

    vdir_t *vn = rs_vmgr_vdir_new(NULL);
    vn->name = rs_tblob_read(f);
}

RS_API vfile_info_t *
rs_vmgr_vfile_info_new(
    char *_FName, size_t _FSize,
    bool _R, bool _W, bool _D)
{
    vfile_info_t *t = RS_Malloc(sizeof(vfile_info_t));
    t->fname = _FName;
    t->fsize = _FSize;
    t->perms.r = _R;
    t->perms.d = _D;
    t->perms.w = _W;

    return t;
}

RS_API vfile_info_t *
rs_vmgr_vfile_info_fetch(vfile_t *vf)
{
    vfile_info_t *t = rs_vmgr_vfile_info_new(
        rs_tblob_decode(vf->fname),
        (strlen(rs_tblob_decode(vf->fcont)) + 1) * sizeof(char),
        vf->meta.perms & FILE_PERM_READ, 
        vf->meta.perms & FILE_PERM_WRITE, 
        vf->meta.perms & FILE_PERM_DELETE);

    return t;
}

RS_API vdir_info_t *
rs_vmgr_vdir_info_new(
    char *_Name, size_t _Dir_Size,
    char **_Files, size_t _FCount,
    char **_Dirs, size_t _Dir_Count)
{
    vdir_info_t *vt = RS_Malloc(sizeof(vdir_info_t));
    vt->name_ = _Name;
    vt->dir_size = _Dir_Size;
    vt->files_ = _Files;
    vt->fcount = _FCount;
    vt->dirs_ = _Dirs;
    vt->dir_count = _Dir_Count;

    return vt;
}

RS_API vdir_info_t *
rs_vmgr_vdir_info_fetch(vdir_t *vd)
{
    vdir_info_t *f = rs_vmgr_vdir_info_new(
        rs_tblob_decode(vd->name),
        0, NULL, 0, NULL, 0);

    size_t dir_size = 0;

    for (size_t i = 0; i < vd->nl_count; i++)
    {
        vdnode_t *vng = (vdnode_t *) rs_trie_get(vd->nodes, vd->node_list[i]);
        assert(!!vng);

        switch (vng->type)
        {
        case RS_VMGR_VDNODE_TYPE_FILE:
        {
            f->files_ = RS_Realloc(f->files_, (f->fcount + 1) * sizeof(char *));
            f->files_[f->fcount++] = rs_tblob_decode(vng->v.vf->fname);

            dir_size += strlen(rs_tblob_decode(vng->v.vf->fcont)) + 1;
        }
        break;
        case RS_VMGR_VDNODE_TYPE_DIR:
        {
            f->dirs_ = RS_Realloc(f->dirs_, (f->dir_count + 1) * sizeof(char *));
            f->dirs_[f->dir_count++] = rs_tblob_decode(vng->v.dr->name);

            vdir_info_t *di = rs_vmgr_vdir_info_fetch(vng->v.dr);
            dir_size += di->dir_size;

            rs_vmgr_vdir_info_free(di);
        }
        break;

        default:
            break;
        }
    }

    f->dir_size = dir_size;
    return f;
}

RS_API void 
rs_vmgr_vdir_info_free(vdir_info_t *t)
{
    for (size_t i = 0; i < t->dir_count; i++)
    {
        RS_Free(t->dirs_[i]);
    }
    RS_Free(t->dirs_);

    for (size_t i = 0; i < t->fcount; i++)
    {
        RS_Free(t->files_[i]);
    }
    RS_Free(t->files_);
    RS_Free(t->name_);
}