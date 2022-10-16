#include <stdio.h>

#define true 1
#define false 0
#define bool int

typedef struct node
{
    char key;
    struct node *firstSon;
    struct node *nextBrother;
} NODE;

typedef NODE *PONT;

PONT createNewNode(char key)
{
    PONT new = (PONT)malloc(sizeof(NODE));
    new->firstSon = NULL;
    new->nextBrother = NULL;
    new->key = key;
    return (new);
}

PONT inicializaTree(char key)
{
    return (createNewNode(key));
}

bool insere(PONT root, char newKey, char fatherKey)
{
    PONT father = searchKey(fatherKey, root);
    if (!father)
        return false;
    PONT son = createNewNode(newKey);
    PONT p = father->firstSon;
    if (!p)
        father->firstSon = son;
    else
    {
        while (p->nextBrother)
            p = p->nextBrother;
        p->nextBrother = son;
    }
    return (true);
}

void showTree(PONT root)
{
    if (root == NULL)
        return;
    printf("%c(", root->key);
    PONT p = root->firstSon;
    while (p)
    {
        showTree(p);
        p = p->nextBrother;
    }
    printf(")");
}

PONT searchKey(char key, PONT root)
{
    if (root == NULL)
        return NULL;
    if (root->key == key)
        return root;
    PONT p = root->firstSon;
    while (p)
    {
        PONT resp = searchKey(key, p);
        if (resp)
            return resp;
        p = p->nextBrother;
    }
    return (NULL);
}