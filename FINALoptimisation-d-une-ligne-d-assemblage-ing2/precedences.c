#include "header.h"

float temps_cycle() {
    FILE *ifs = fopen("../temps_cycle.txt", "r");
    float temps;
    if (!ifs) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }
    //Recuperer les donnees de temps cycle
    fscanf(ifs, "%f", &temps);
    return temps;
    fclose(ifs);
}

void renseigner_temps(Graphe * g) {
    FILE *ifs = fopen("../operations.txt", "r");
    int indice, sommet;
    float temps;

    if (!ifs) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }
    //Recuperer les donnees de temps par sommet
    for (int i=0; i<g->ordre; i++) {
        fscanf(ifs, "%d %f", &sommet, &temps);
        indice = obtenirIndiceSommet(g, sommet);
        g->pSommet[indice]->temps = temps;
    }
    fclose(ifs);
}

Graphe * precedences(Graphe* g){

    g = lire_graphe("../precedences.txt", 1); // Graphe oriente
    renseigner_temps(g);
    //ColorisationNaive(g);
    /// afficher le graphe
    //graphe_afficher(g);
    return g;
}

WorkStation * ws_precedences(Graphe* g, float temps_cycle){
    int **liste = (int **)malloc(g->ordre * sizeof(int *));
    for (int i = 0; i < g->ordre; i++) {
        liste[i] = (int *)malloc(g->ordre * sizeof(int));
    }
    WorkStation * ws = (WorkStation *)malloc (g->ordre * sizeof(WorkStation));   // Liste des station de travail
    for (int i = 0; i < g->ordre; i++) {
        ws[i].operations = (int *)malloc(g->ordre * sizeof(int));
    }

    int i,j,k,ws_num,ops_num;
    int indice_connexe = 0;
    float temps_c = 0;

    //connexes_afficher(g,'B');

    liste[0] = BFS(g, 1,1);

    // On cherche les listes connexes suivantes dans les cellules ou connexe est reste a 0 ie non visitées
    for (int l=0; l < g->ordre; l++) {
        if (g->pSommet[l]->couleur==0) {
            indice_connexe++;
            liste[indice_connexe] = BFS(g, g->pSommet[l]->valeur,indice_connexe);
        }
    }

    ws_num = 0;
    ws[ws_num].num = ws_num;
    ws[ws_num].temps = 0;
    ops_num = 0;

    for (int l=0; l < g->ordre; l++) {
        for (k=0; k <= indice_connexe; k++) {
            if (liste[k][l] != -1) {
                j = obtenirIndiceSommet(g, liste[k][l]);
                if (ws[ws_num].temps + g->pSommet[j]->temps <= temps_cycle) {
                    if (ws[ws_num].operations[ops_num-1] != liste[k][l]){
                        ws[ws_num].temps = ws[ws_num].temps + g->pSommet[j]->temps;
                        ws[ws_num].operations[ops_num] = liste[k][l];
                        printf(" \n WS%d %d %.2f", ws[ws_num].num, ws[ws_num].operations[ops_num], ws[ws_num].temps);
                        //printf(" \n VERIF WS%d %d %d %.2f", ws_num, ops_num, ws[ws_num].operations[ops_num], ws[ws_num].temps);
                        ops_num++;
                    }
                } else {
                    ws_num++;
                    printf("\n");
                    ws[ws_num].num = ws_num;
                    ws[ws_num].temps = 0;
                    ops_num = 0;
                    // Dernier membre en depassement devient premier membre pour WS suivante
                    ws[ws_num].operations[ops_num] = liste[k][l];
                    ws[ws_num].temps = g->pSommet[j]->temps;
                    printf(" \n WS%d %d %.2f", ws[ws_num].num, ws[ws_num].operations[ops_num], ws[ws_num].temps);
                    ops_num++;
                }
            }
        }
    }
    ws->nb_ws = ws_num;
    return ws;
}