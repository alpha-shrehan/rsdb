#include "textblob.h"

int _key_exists(char *, char);
int _key_append(char **, char);
void _tok_append(tblob_t *, int);

RS_API tblob_t *
rs_tblob_new(char *k, int *v, size_t s)
{
    tblob_t *t = RS_Malloc(sizeof(tblob_t));
    t->key = k;
    t->toks = v;
    t->size = s;

    return t;
}

RS_API tblob_t *
rs_tblob_new_fromRaw(char *raw)
{
    tblob_t *res = rs_tblob_new(NULL, NULL, 0);
    res->key = RS_Malloc(sizeof(char));
    *res->key = '\0';

    size_t d = strlen(raw);

    for (size_t i = 0; i < d; i++)
    {
        char c = raw[i];

        if (_key_exists(res->key, c) == -1)
            _key_append(&res->key, c);
    }

    for (size_t i = 0; i < d; i++)
        _tok_append(res, _key_exists(res->key, raw[i]));

    return res;
}

RS_API char *
rs_tblob_decode(tblob_t *t)
{
    char *res = RS_Malloc((t->size + 1) * sizeof(char));

    for (size_t i = 0; i < t->size; i++)
        res[i] = t->key[t->toks[i]];

    res[t->size] = '\0';
    return res;
}

int _key_exists(char *s, char k)
{
    int i = 0, j = strlen(s) - 1;
    int mid;

    while (i <= j)
    {
        mid = (i + j) / 2;

        if (s[mid] == k)
            return mid;
        else if (s[mid] < k)
            i = mid + 1;
        else if (s[mid] > k)
            j = mid - 1;
    }

    return -1;
}

int _key_append(char **key, char c)
{
    size_t d = strlen(*key);
    *key = RS_Realloc(*key, (d + 2) * sizeof(char));
    bool op_done = false;
    char pres;
    int idx = -1;

    for (size_t i = 0; i < d + 2; i++)
    {
        if (op_done)
        {
            char p = (*key)[i];
            (*key)[i] = pres;
            pres = p;
        }

        if (
            ((*key)[i] == '\0' ||
             (*key)[i] > c) &&
            !op_done)
        {
            pres = (*key)[i];
            (*key)[i] = c;
            op_done = true;
            idx = i;
        }
    }

    return idx;
}

void _tok_append(tblob_t *t, int val)
{
    t->toks = RS_Realloc(t->toks, (t->size + 1) * sizeof(int));
    t->toks[t->size++] = val;
}

RS_API int
rs_tblob_dump(tblob_t *t, FILE *f)
{
    if (!f)
        return RSERR_NOFILE;

    int p_len = strlen(t->key) + 1;

    fwrite(&p_len, sizeof(int), 1, f);
    fwrite(t->key, sizeof(char), p_len, f);
    fwrite(&t->size, sizeof(size_t), 1, f);
    fwrite(t->toks, sizeof(int), t->size, f);

    return RSERR_OK;
}

RS_API tblob_t *
rs_tblob_read(FILE *f)
{
    if (!f)
        return NULL;

    tblob_t *tn = rs_tblob_new(NULL, NULL, 0);
    size_t plen = 0;
    char *key = NULL;
    size_t size = 0;
    int *toks = NULL;

    fread(&plen, sizeof(size_t), 1, f);
    key = RS_Malloc(plen * sizeof(char));

    fread(key, sizeof(char), plen, f);

    fread(&size, sizeof(size_t), 1, f);
    toks = RS_Malloc(size * sizeof(int));

    fread(toks, sizeof(int), size, f);

    tn->key = key;
    tn->size = size;
    tn->toks = toks;

    return tn;
}