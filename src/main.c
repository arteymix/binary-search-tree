#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    if (n->left)
    {
        node_print(n->left, ident + 1);

        for (c = 0; c < ident; c++)
            printf("\t");

        printf("\t/\n");
    }

    for (c = 0; c < ident; c++)
        printf("\t");

    // alignement pour les trucs identés
    if (ident > 0)
        printf(" ");

    printf("« %s » : « %s »\n", n->term, n->definition);

    if (n->right)
    {
        for (c = 0; c < ident; c++)
            printf("\t");

        printf("\t\\\n");

        node_print(n->right, ident + 1);
    }
}

int main(void)
{
    node *root = node_new("root", "root");
    size_t line_size = 1024 * sizeof(char);
    char *line = malloc(line_size);

    begin:
    do
    {
        printf("? ");
        getline(&line, &line_size, stdin);

        if (line == NULL)
        {
            printf("mémoire épuisée\n");
            continue;
        }

        // strip the newline character
        line[strlen(line) - 1] = '\0';

        if (strchr(line, '='))
        {
            int compound = strchr(line, '+');
            char *term = strtok(line, "=");

            // il s'agit d'une assignation de terme
            // opération sur les arguments
            node *n = node_search(root, term);

            // si le noeud est manquant dans l'arbre, on l'insère
            if (n == NULL)
                n = node_new(term, "");

            // on vide la définition actuelle
            n->definition[0] = '\0';

            char *token;

            // on parse les arguments séparés par des signes +
            while (token = strtok(NULL, "+"))
            {
                if (compound && node_search(root, token) == NULL)
                {
                    printf("terme inconnu\n");
                    goto begin;
                }

                n->definition = realloc(n->definition, sizeof(char) * (strlen(n->definition) + 1 + strlen(token) + 1));

                if (n->definition == NULL)
                {
                    // plus de mémoire pour allouer la définition
                    printf("mémoire épuisée\n");

                    return EXIT_FAILURE;
                }

                // ajout à la définition du noeud
                if (strlen(n->definition) > 0)
                    strcat(n->definition, "+");

                strcat(n->definition, token);
            }

            if (!node_search(root, n->term))
                node_insert(root, n);

            /* l'utilisateur a vidé la définition, il faut donc supprimer le
             * noeud et libérer l'espace mémoire.
             */
            if (strcmp("", n->definition) == 0)
            {
                node *d = node_delete(root, n->term);

                // la racine ne peut pas être supprimée
                if (d == NULL)
                    continue;

                node_free(d);
            }
        }
        else // récupération d'un terme
        {
            node *n = node_search(root, line);

            if (n == NULL)
            {
                printf("terme inconnu\n");
            }
            else
            {
                // il s'agit d'une recherche de terme
                char *d = node_definition(root, n);

                if (d == NULL)
                {
                    printf("mémoire épuisée\n");
                }
                else
                {
                    printf("%s\n", d);
                    free(d);
                }
            }
        }
    }
    while (1);

    node_free(root);
    free(line);

    return EXIT_SUCCESS;
}
