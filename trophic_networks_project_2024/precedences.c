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
    for (int i=0; i<g->ordre-1; i++) {
        fscanf(ifs, "%d %f", &sommet, &temps);
        indice = obtenirIndiceSommet1(g, sommet);
        g->pSommet[indice]->temps = temps;
    }
    fclose(ifs);
}

Graphe * precedences(Graphe* g){

    g = lire_graphe1("../precedences.txt", 1); // Graphe oriente
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
    ws->operations = (int *)malloc(g->ordre * sizeof(int));
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

    // A debugger : Erreur a partir de 18 24 28 + doublons 8 8 + il faut passer a WS2
    for (int l=0; l < g->ordre; l++) {
        for (k=0; k <= indice_connexe; k++) {
            j = obtenirIndiceSommet1(g, liste[k][l]);
            if ((liste[k][l] != -1) && (ws[ws_num].temps + g->pSommet[j]->temps < temps_cycle)){
                ws[ws_num].temps = ws[ws_num].temps + g->pSommet[j]->temps;
                ws[ws_num].operations[ops_num] = liste[k][l];
                printf(" \n WS%d %d %.2f", ws[ws_num].num, ws[ws_num].operations[ops_num], ws[ws_num].temps);
                j = obtenirIndiceSommet1(g, liste[k][l]);
                ops_num++;
            }
            if (ws[ws_num].temps > temps_cycle){
                //ws_num = 1;
                //ws[ws_num].num = ws_num+1;
                //ws[ws_num].temps = 0;
                //ops_num = 0;
                //printf(" \n WS %d %d %.2f", ws[ws_num].num, ws[ws_num].operations[ops_num], ws[ws_num].temps);
            }
        }
    }
}