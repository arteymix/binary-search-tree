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
 * Alloue un nouveau noeud.
 */
node *node_new(char *term, char* definition)
{
    node *n = malloc(sizeof(node));
    
    if (n == NULL)
        printf("");
    
    n->term = term;
    n->definition = definition;
    
    return n;
}

/**
 * Insère un noeud contenant un terme et une définition à un noeud donné.
 */
void node_insert(node *p, node *n) 
{
    if (strcmp(p->term, n->term) > 0)
    {
        if (p->left == NULL)
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
        if (p->right == NULL)
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
    else // < 0
    {
        return node_search(p->right, t);
    }

    return NULL;
}

/**
 * Supprime un noeud dans l'arbre.
 */
node *node_delete(node *p, char *term)
{
    node *n, *left, *right = NULL;

    if (strcmp(p->term, term))
    {
        // problématique, car on ne connait pas le parent de p
        return;
    }

    if (strcmp(p->left->term, term) == 0)
    {
        n = p->left;   
        p->left = NULL;
    }

    if (strcmp(p->right->term, term) == 0)
    {
        n = p->right;
        p->right = NULL;
    }

    // le noeud n'est pas immédiatement à gauche ou à droite, on continue la
    // recherche
    if (n == NULL)
    {
        if (strcmp(p->term, term) < 0)
            node_delete(p->left, term);

        if (strcmp(p->term, term) > 0)
            node_delete(p->right, term);

        return;
    }

    // réinsère les noeuds dans l'adbre
    node_insert(p, n->left);
    node_insert(p, n->right);

    // Libère n
    free(n);
}

/**
 * Construit la définition d'un noeud
 */
char *node_definition(node *p, node *n) 
{
    char *definition = strtok(n->definition, "+");
    char *token;
    node *s;
    char *sous_definition;
    
    // On parse tous les sous-termes séparés par l'addition
    while (token = strtok(NULL, "+"))
    {
        s = node_search(p, token);
       
        if (s == NULL)
        {
            // définition inconnue d'un sous-terme    
        }
        else
        {
            // l'espace pour la définition courante, 
            definition = realloc(definition, strlen(definition) + 1 + strlen(token));
            
            if (definition == NULL)
                exit("impossible de générer une définition pour ");

            // calcul de la définition du sous-terme
            sous_definition = node_definition(s);

            strcat(definition, s->definition);
       }
    }
    
    free(s);
    
    return definition;
}

void main()
{
    node *root = malloc(sizeof(node));
    size_t line_size = 1024;
    char *line = malloc(line_size);
    
    do
    {
        printf(">>> ");
        getline(&line, &line_size, stdin);

        char *operand = strtok(line, "=");
        
        if (operand == NULL)
        {
            // il s'agit d'une recherche de terme
            node *n = node_search(root, line);
            
            if (n == NULL)
            {
                printf("terme inconnu");
            }
            else
            {
                char* def_token = strtok(n->definition, "+");
                
            }
        }
        else
        {
            char *token = NULL;
            
            // il s'agit d'une assignation de terme
            // opération sur les arguments
            while (token = strtok(NULL, "+"))
            {
                printf(token);
            }
        }
    } 
    while (1);
}
