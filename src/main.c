#include <stdlib.h>
#include <stdio.h>

#include "node.c"

/**
 * Imprime un noeud de manière récursive.
 *
 * @param node n     noeud à imprimer
 * @param int  ident identation
 */
void node_print(node *n, int ident)
{
    int c;

    for (c = 0; c < ident; c++)
        printf("\t");

    printf("\"%s\": \"%s\"\n", n->term, n->definition);

    for (c = 0; c < ident; c++)
        printf("\t");

    printf("|\t\\\n");

    if (n->left)
        node_print(n->left, ident + 1);

    if (n->right)
        node_print(n->right, ident + 1);
}

int main(void)
{
    node *root = node_new("root", "root");
    size_t line_size = 1024;
    char *line = malloc(line_size);

    printf("Noter que la longueur maximale d'une expression est de 1024 caractères.\n");
    
    do
    {
        printf(">>> ");
        getline(&line, &line_size, stdin);

        // strip the newline character
        line[strlen(line) - 1] = '\0';

        if (line[0] == '?')
        {
            // il s'agit d'une recherche de terme
            char *d = node_definition(root, line);

            if (d == NULL)
            {
                printf("Le terme %s est inconnu.\n", line);
            }
            else
            {
                printf("%s est défini par %s.", line, d);
            }
        }
        else if (strchr(line, '='))
        {
            char *term = strtok(line, "=");

            printf("%s prendra pour définition les termes: ", term);

            // il s'agit d'une assignation de terme
            // opération sur les arguments
            node *n = node_search(root, term);

            // si le noeud est manquant dans l'arbre, on l'insère
            if (n == NULL)
                n = node_new(term, line);

            char *token;
            
            // on parse les arguments séparés par des signes +
            while (token = strtok(NULL, "+"))
            {
                printf("%s, ", token);

                // insertion d'un nouveau noeud si la déf. est manquante
                if (node_search(root, token) == NULL)
                    node_insert(root, node_new(token, token));
        
                n->definition = realloc(n->definition, sizeof(char) * (strlen(n->definition) + 1 + strlen(token) + 1));

                if (n->definition == NULL) 
                {
                    printf("Plus de mémoire pour insérer la définition du noeud %s.\n", n->term);
                    return EXIT_FAILURE;
                }

                // ajout à la définition du noeud
                strcat(n->definition, "+");
                strcat(n->definition, token);
            }

            // l'arbre ne possède pas déjà le noeud
            if (node_search(root, n->term) == NULL)
                node_insert(root, n);

            // l'utilisateur a vidée la définition et par conséquent supprimé le
            // noeud
            if (strcmp("", n->definition) == 0)
                node_delete(root, n->term);

            printf("\n");
        }
        else if (strcmp("q", line) == 0)
        {
            break;
        }
        else if (strcmp("p", line) == 0)
        {
            node_print(root, 0);    
        }
        else
        {
            printf("%s n'est pas reconnu.\n", line);    
        }
    } 
    while (1);

    node_free(root);
    free(line);

    return EXIT_SUCCESS;
}
