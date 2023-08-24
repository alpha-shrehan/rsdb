#include "trie.h"

int _trie_key_exists(trie_t *, char);
int _add_key(trie_t *, char);

char *trie_key_repr(trie_t *t)
{
    char *s = RS_Malloc((t->kc + 1) * sizeof(char));

    for (size_t i = 0; i < t->kc; i++)
        s[i] = t->key[i];

    s[t->kc] = '\0';
    return s;
}

RS_API trie_t *
rs_trie_new(void)
{
    trie_t *t = RS_Malloc(sizeof(trie_t));
    t->kc = 0;
    t->key = NULL;
    t->marked = false;
    t->val = NULL;
    t->children = NULL;
    t->child_count = 0;

    return t;
}

RS_API void
rs_trie_add(trie_t *t, char *key, void *val)
{
    assert(!strstr(key, (char *)(char[]){'$', '\0'}) && "[trie] cannot have `$' in key names.");

    if (*key == '\0')
    {
        t->marked = true;
        t->val = val;
        return;
    }

    int i = -1;

    if ((i = _trie_key_exists(t, key[0])) != -1)
    {
        rs_trie_add(t->children[i], key + 1, val);
    }
    else
    {
        int ki = _add_key(t, *key);
        rs_trie_add(t->children[ki], key + 1, val);
    }
}

RS_API void *
rs_trie_get(trie_t *t, char *k)
{
    // printf("%s %s %c %d\n", trie_key_repr(t), k, *k, _trie_key_exists(t, *k));
    if (!k || *k == '\0')
    {
        if (t->marked)
            return t->val;
        return NULL;
    }

    int i = -1;
    if ((i = _trie_key_exists(t, *k)) != -1)
        return rs_trie_get(t->children[i], k + 1);
    return NULL;
}

RS_API void
rs_trie_remove(trie_t *t, char *k)
{
    int i = -1;

    if ((i = _trie_key_exists(t, *k)) != -1)
    {
        t->key[i] = '$'; /* ignore top key and free child node because reallocation is burdensome */
        rs_trie_free(t->children[i]);
    }
}

RS_API void
rs_trie_free(trie_t *t)
{
    RS_Free(t->key);
    RS_Free(t->children);
    RS_Free(t);
}

int _trie_key_exists(trie_t *t, char c)
{
    if (!t->kc)
        return -1;

    int i = 0, j = t->kc - 1;

    while (i <= j)
    {
        int mid = (i + j) / 2;

        if (t->key[mid] > c)
            j = mid - 1;
        else if (t->key[mid] < c)
            i = mid + 1;
        else if (t->key[mid] == c)
            return mid;
    }

    /* for (size_t i = 0; i < t->kc; i++)
    {
        if (t->key[i] == c)
            return i;
    } */

    return -1;
}

int _add_key(trie_t *t, char c)
{
    t->key = RS_Realloc(t->key, (t->kc + 1) * sizeof(char));
    t->children = RS_Realloc(t->children, (t->child_count + 1) * sizeof(trie_t *));
    t->children[t->child_count] = NULL; /* this will be overwritten so don't use rs_trie_new */

    int idx = t->kc;

    for (int i = 0; i < t->kc; i++)
    {
        if (c <= t->key[i])
        {
            idx = i;
            break;
        }
    }

    for (int i = t->kc; i > idx; i--)
    {
        t->key[i] = t->key[i - 1];
        t->children[i] = t->children[i - 1];
    }

    t->key[idx] = c;
    t->children[idx] = rs_trie_new();
    
    t->kc++;
    t->child_count++;

    return idx;
}

char **trie_getkeys(trie_t *t, size_t *sz)
{
    char **r = NULL;
    size_t rc = 0;

    for (size_t i = 0; i < t->kc; i++)
    {
        if (t->key[i] == '$')
            continue;

        if (t->children[i]->child_count)
        {
            size_t tsz = 0;
            char **tr = trie_getkeys(t->children[i], &tsz);

            r = RS_Realloc(r, (rc + tsz) * sizeof(char *));
            for (size_t j = 0; j < tsz; j++)
            {
                r[rc + j] = RS_Malloc((strlen(tr[j]) + 2) * sizeof(char));
                sprintf(r[rc + j], "%c%s", t->key[i], tr[j]);
            }

            rc += tsz;
        }
        else
        {
            r = RS_Realloc(r, (rc + 1) * sizeof(char *));
            r[rc] = RS_Malloc(2 * sizeof(char));
            *r[rc] = t->key[i];
            r[rc][1] = '\0';
            rc++;
        }
    }

    if (sz != NULL)
        *sz = rc;

    return r;
}

RS_API void
rs_trie_dgbprint(trie_t *t)
{
    size_t s = 0;
    char **r = trie_getkeys(t, &s);

    printf("%d\n", s);

    for (size_t i = 0; i < s; i++)
    {
        printf("%s\n", r[i]);
    }
}