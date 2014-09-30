#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct node
{
    char *term;
    char *definition;
    struct node *left, *right;
} node;

/**
 * Insère un noeud contenant un terme et une définition à un noeud donné.
 */
void node_insert(node *p, node *n) 
{
    if (strcmp(p->term, n->term))
    {
        if ( ! p->left)
        {
            p->left = n;    
        }
        else
        {
            // insertion à gauche    
            node_insert(p->left, n);
        }
    } 
    else // == ou < 0
    {
        if ( p->right == NULL)
        {
            p->right = n;    
        }
        else
        {
            // insertion à droite   
            node_insert(p->right, n);
        }
    }
}

/**
 * Retourne le noeud ayant pour term t, ou NULL si aucun noeud n'est trouvé.
 */
node *node_search(node *p, char *t)
{
    int cmp = strcmp(p->term, t);

    if (cmp == 0)
    {
        return p;
    }
    else if (cmp > 0)
    {
        return node_search(p->left, t);    
    }
    else
    {
        return node_search(p->right, t);
    }

    return NULL;
}

void main()
{
    node *root = malloc(sizeof(node));
    size_t line_size = 1024;
    char *line = malloc(m);
    do
    {
        getline(&line, &m, stdin);

        printf("%s", line);
    } 
    while (1);
}
