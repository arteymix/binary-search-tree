#include <stdlib.h>
#include <stdio.h>

#include "node.c"

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

        char *operand = strtok(line, "=");
        
        if (operand == NULL)
        {
            // il s'agit d'une recherche de terme
            node *n = node_search(root, line);
            
            if (n == NULL)
            {
                printf("terme inconnu\n");
            }
            else
            {
                char* def_token = strtok(n->definition, "+");
            }
        }
        else
        {
            printf("%s prendra pour définition les termes: ", operand);

            // il s'agit d'une assignation de terme
            // opération sur les arguments
            node *n = node_search(root, operand);

            if (n == NULL)
                n = node_new(operand, "");

            char *token = NULL;
            
            while (token = strtok(NULL, "+"))
            {
                printf("%s, ", token);

                // insertion d'un nouveau noeud si la déf. est manquante
                if (node_search(root, token) == NULL)
                    node_insert(root, node_new(token, token));

                // ajout à la définition du noeud
                strcat(n->definition, "+");
                strcat(n->definition, token);
            }

            // l'arbre ne possède pas déjà le noeud
            if (node_search(root, n->term) == NULL)
                node_insert(root, n);

            // l'utilisateur a vidée la définition et par conséquent supprimé le
            // noeud
            if (strcmp(n->definition, "") == 0)
                node_delete(root, n->term);

            printf("\n");
        }
    } 
    while (1);

    return EXIT_SUCCESS;
}
