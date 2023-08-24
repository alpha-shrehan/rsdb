#include <rsdb.h>

#define TEST(X) test##X()

void test1()
{
    tblob_t *t = rs_tblob_new_fromRaw("the quick brown fox jumped over the lazy dog.");
    FILE *op = fopen("../../tests/test.bin", "rb");

    tblob_t *rt = rs_tblob_read(op);
    printf("%s\n%s\n", rt->key, rs_tblob_decode(rt));

    fclose(op);

    // printf("%s\n%s\n", t->key, rs_tblob_decode(t));
}

void test2()
{
    FILE *op = fopen("../../tests/test.bin", "rb");

    // vfile_t *vf = rs_vmgr_vfile_newWithData("test.txt", "Hello, World!", FILE_PERM_READ | FILE_PERM_WRITE);
    // rs_vmgr_vfile_dump(vf, op);

    vfile_t *nf = rs_vmgr_vfile_read(op);
    rs_vmgr_vfile_dbgprint(nf);

    fclose(op);
}

void test3()
{
    trie_t *t = rs_trie_new();

    rs_trie_add(t, "hello", "Hey");
    rs_trie_add(t, "name", "Shrehan");

    // char *s = RS_Malloc((t->kc + 1) * sizeof(char));
    // strncpy(s, t->key, t->kc);
    // s[t->kc] = '\0';
    // printf("%s %d\n", s, t->kc);

    rs_trie_dgbprint(t);
}

void test4()
{
    vdir_t *vd = rs_vmgr_vdir_new("testdir");

    rs_vmgr_vdir_addfile(
        vd,
        rs_vmgr_vfile_newWithData(
            "hello.txt", "Hello, World!",
            FILE_PERM_READ | FILE_PERM_WRITE));

    rs_vmgr_vdir_addfile(
        vd,
        rs_vmgr_vfile_newWithData(
            "os.conf", "~ OS Config file",
            FILE_PERM_READ));
    
    vdir_t *vn = rs_vmgr_vdir_new("nest");
    rs_vmgr_vdir_adddir(vd, vn);

    // vdnode_t *node = (vdnode_t *) rs_trie_get(vd->nodes, "nest");
    // printf("[%d]\n", node == NULL);

    vdir_info_t *vi = rs_vmgr_vdir_info_fetch(vd);
    printf("%s\n%d bytes\n", vi->name_, vi->dir_size);

    for (size_t i = 0; i < vi->fcount; i++)
    {
        printf("%s\n", vi->files_[i]);
    }

    for (size_t i = 0; i < vi->dir_count; i++)
    {
        printf("%s\n", vi->dirs_[i]);
    }
}

int main(int argc, char const *argv[])
{
    TEST(4);

    return printf("Program Ended.\n") && 0;
}
