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

    // vfile_t *vf = rs_vmgr_vfile_newWithData("test.txt", "Hello, World!", FILE_PERM_READ);
    // rs_vmgr_vfile_dump(vf, op);

    vfile_t *nf = rs_vmgr_vfile_read(op);
    rs_vmgr_vfile_dbgprint(nf);

    fclose(op);
}

int main(int argc, char const *argv[])
{
    TEST(2);

    return printf("Program Ended.\n") && 0;
}
