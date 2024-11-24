#include "header.h"



int main() {
    //// 2.1 - 1 Graphe d'exclusions

    Graphe * g_exclusions;

    printf("\n2.1.1 REPARTITION DES OPERATIONS EN PRENANT UNIQUEMENT EN COMPTE LES REGLES D'EXCLUSION \n");
    g_exclusions = exclusions(g_exclusions);

    //// 2.1 - 2 Graphe de precedences


    Graphe * g_precedences;

    // Creer le graphe de precedences
    // Renseigner le temps de chaque operations

    printf("\n\n2.1.2 REPARTITION DES OPERATIONS EN PRENANT UNIQUEMENT EN COMPTE LES REGLES DE PRECEDENCE ET DE TEMPS \n");
    g_precedences = yacine(g_precedences);

    printf("\n\n2.2.1 REPARTITION DES OPERATIONS EN PRENANT UNIQUEMENT EN COMPTE LES RESTRICTIONS D4EXCLUSIONS ET DE PRECEDENCE \n");
    Graphe * g_exclusions_precedence;
    g_exclusions_precedence = exclusions_precedence(g_exclusions);



    //on supprime la mémoire allouée dynamiquement
    for (int i = 0; i < g_exclusions->ordre; ++i) {
        free(g_exclusions->pSommet[i]->arc);
        free(g_exclusions->pSommet[i]);
    }
    free(g_exclusions->pSommet);
    free(g_exclusions);




    return 0;
}