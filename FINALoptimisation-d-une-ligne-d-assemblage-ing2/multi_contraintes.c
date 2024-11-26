#include "header.h"

// Test si un noeud est a exclure et mettre dans la WorkStation suivante
// NON FINI

int isNotExclusive(WorkStation * ws, Graphe* g, Graphe* e) {

}

WorkStation * ws_multi_contraintes(WorkStation * ws, Graphe* g, Graphe* e, float temps_c) {


    // On recupere ws et on cree wse qui repartit les operations en prenant en compte les contraintes d'exclusion

    int **liste = (int **) malloc(g->ordre * sizeof(int *));
    for (int i = 0; i < g->ordre; i++) {
        liste[i] = (int *) malloc(g->ordre * sizeof(int));
    }
    int j;

    for (int k = 0; k <= ws->nb_ws; k++) {
        for (int i = 0; i < g->ordre; i++) {
            liste[k][i] = -1;
            if (ws[k].operations[i] >0) {
                j = obtenirIndiceSommet(g, ws[k].operations[i]);
                printf(" \n WS%d %d %.2f", ws[k].num, ws[k].operations[i],g->pSommet[j]->temps);
                // inclure ici un test si pas exclusion ajouter a la workstation k
                liste[k][i] = ws[k].operations[i]; // Je mets dans la liste a la workstation k l'operation i
            }
        }
        printf("\n");
    }
    return ws;
}