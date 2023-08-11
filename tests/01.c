#include <rsdb.h>

#define TEST(X) test##X()

void test1()
{
    tblob_t *t = rs_tblob_new_fromRaw("the quick brown fox jumped over the lazy dog.");
    
    printf("%s\n", rs_tblob_decode(t));
}

int main(int argc, char const *argv[])
{
    TEST(1);

    return printf("Program Ended.\n") && 0;
}
