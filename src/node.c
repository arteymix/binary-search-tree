#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char *term;
    char *definition;
    struct node *left, *right;
}
node;

/**
 * Alloue et initialise nouveau noeud.
 *
 * @param char[] term       terme du noeud
 * @param char[] definition définition du noeud
 *
 * Retourne NULL si le noeud ne peut pas être alloué.
 */
node *node_new(char *term, char* definition)
{
    node *n = malloc(sizeof(node));

    if (n == NULL)
        return NULL;

    n->term = malloc(sizeof(char) * (strlen(term) + 1));
    n->definition = malloc(sizeof(char) * (strlen(definition) + 1));

    strcpy(n->term, term);
    strcpy(n->definition, definition);

    n->left = NULL;
    n->right = NULL;

    return n;
}

/**
 * Libère un noeud et ses enfants récurisvement.
 *
 * Cette fonction libère tout ce que node_new alloue, ce qui inclut le terme et
 * la définition de chaque noeud.
 *
 * @param node noeud à libérer
 */
void node_free(node *n)
{
    if (n->left)
        node_free(n->left);

    if (n->right)
        node_free(n->right);

    free(n->term);
    free(n->definition);
    free(n);
}

/**
 * Compte la taille d'un noeud.
 *
 * @param node noeud à compter
 */
unsigned int node_size(node *n)
{
    int size = 1;

    if (n->left)
        size += node_size(n->left);

    if (n->right)
        size += node_size(n->right);

    return size;
}

/**
 * Compte la profondeur d'un noeud
 *
 * @param node noeud à déterminer la profondeur
 *
 * @return unsigned int la profondeur du noeud
 */
unsigned int node_depth(node *n)
{
    unsigned int left_depth, right_depth = 0;

    if (n->left)
        left_depth = node_depth(n->left);

    if (n->right)
        right_depth = node_depth(n->right);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

/**
 * Insère un noeud contenant un terme et une définition à un noeud donné.
 */
void node_insert(node *p, node *n)
{
    if (strcmp(n->term, p->term) < 0)
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
    else // == ou > 0
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
    int cmp = strcmp(t, p->term);

    if (cmp == 0)
    {
        return p;
    }
    else if (cmp < 0)
    {
        if (p->left == NULL)
            return NULL;

        return node_search(p->left, t);
    }
    else // == ou > 0
    {
        if (p->right == NULL)
            return NULL;

        return node_search(p->right, t);
    }

    return NULL;
}

/**
 * Trouve un noeud et l'extrait de l'arbre.
 *
 * La racine de l'arbre ne peut pas être extraite.
 *
 * @param p    noeud parent
 * @param term terme du noeud à supprimer
 *
 * @return le noeud supprimé ou NULL si il n'est pas trouvé ou ne peut pas être
 *         supprimé.
 */
node *node_delete(node *p, char *term)
{
    node *n, *left, *right = NULL;

    /*
     * si un noeud parent est supprimé, on ne peut pas réinsérer les enfants,
     * alors il ne faut pas permettre ce cas.
     */
    if (strcmp(p->term, term) == 0)
    {
        return NULL;
    }

    // noeud à gauche
    if (p->left && strcmp(term, p->left->term) == 0)
    {
        n = p->left;
        p->left = NULL;
    }

    // noeud à droite
    if (p->right && strcmp(term, p->right->term) == 0)
    {
        n = p->right;
        p->right = NULL;
    }

    // le noeud n'est pas immédiatement à gauche ou à droite, on continue la
    // recherche
    if (n == NULL)
    {
        int cmp = strcmp(term, p->term);

        if (cmp < 0)
        {
            if (p->left == NULL)
                return NULL;

            return node_delete(p->left, term);
        }
        else // == ou >, mais l'égalité a déjà été vérifiée
        {
            if (p->right == NULL)
                return NULL;

            return node_delete(p->right, term);
        }
    }

    // réinsère les noeuds orphelins dans l'arbre
	if (n->left)
		node_insert(p, n->left);

	if (n->right)
		node_insert(p, n->right);

    // on perd les références vers les noeuds
    n->left = NULL;
    n->right = NULL;

    return n;
}

/**
 * Construit la définition d'un noeud à partir d'un certain noeud.
 *
 * @param node p noeud de recherche pour les sous-terme.
 * @param node n noeud dont on veut construire la définition.
 *
 * @return la définition du noeud ou NULL si il n'y a plus d'espace pour allouer
 *         la définition.
 */
char *node_definition(node *p, node *n)
{
    char *definition = malloc(sizeof(char) * (strlen(n->definition) + 1));

    // mémoire insuffisante pour allouer la définition
    if (definition == NULL)
        return NULL;

    char *n_definition = malloc(sizeof(char) * (strlen(n->definition) + 1));

    // mémoire insuffisante pour copier la définition du noeud
    if (n_definition == NULL)
    {
        free(definition);
        return NULL;
    }

    // on fait une copie de la définition du noeud
    strcpy(n_definition, n->definition);

    // on tokenize la définition pour trouver les définitions des sous-termes
    char *saveptr;
    char *token = strtok_r(n_definition, "+", &saveptr);

    do
    {
        // on trouve le noeud du sous-terme dans l'arbre
        node *s = node_search(p, token);

        // définition inconnue du sous-terme, on le saute
        if (s == NULL)
        {
            definition = realloc(definition, sizeof(char) * (strlen(definition) + strlen(token) + 1));

            // plus de place pour réallouer la définition
            if (definition == NULL)
                return NULL;

            strcat(definition, token);
        }
        else
        {
            char *s_definition = node_definition(p, s);

            if (s_definition == NULL)
                return NULL;

            /* réallocation d'espace pour stocker la définition courante, un espace,
             * la définition du sous-noeud et une caractère de fin de ligne \0.
             */
            definition = realloc(definition, sizeof(char) * (strlen(definition) + strlen(s_definition) + 1));

            // plus de place pour allouer la définition
            if (definition == NULL)
            {
                free(s_definition);
                return NULL;
            }

            // on concaténe le tout
            strcat(definition, s_definition);

            free(s_definition);
        }
    }
    while (token = strtok_r(NULL, "+", &saveptr));

    return definition;
}
