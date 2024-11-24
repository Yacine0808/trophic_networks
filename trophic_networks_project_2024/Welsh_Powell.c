//Coloration d'un graphe en utilisant l'algo de Welsh et Powell

#include <stdio.h>
#include <stdlib.h>

// Nombre maximal de sommets dans le graphe
#define MAX_VERTICES 100

// Structure pour représenter un graphe
typedef struct {
    int numVertices;
    int **adjMatrix;
} Graph;

// Fonction pour créer un graphe avec un nombre spécifié de sommets
Graph *createGraph(int numVertices) {
    int i, j;
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;

    // Allocation de la matrice d'adjacence
    graph->adjMatrix = (int **)malloc(numVertices * sizeof(int *));
    for (i = 0; i < numVertices; i++) {
        graph->adjMatrix[i] = (int *)malloc(numVertices * sizeof(int));
        for (j = 0; j < numVertices; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }

    return graph;
}

// Fonction pour ajouter une arête entre deux sommets du graphe
void addEdge(Graph *graph, int start, int end) {
    graph->adjMatrix[start][end] = 1;
    graph->adjMatrix[end][start] = 1;
}

// Fonction pour colorier le graphe en utilisant l'algorithme de Welsh-Powell
void colorGraph(Graph *graph) {
    int numVertices = graph->numVertices;
    int *color = (int *)malloc(numVertices * sizeof(int));

    // Initialiser toutes les couleurs à -1 (non attribuées)
    for (int i = 0; i < numVertices; i++) {
        color[i] = -1;
    }

    // La couleur actuelle
    int currentColor = 0;

    // Colorier le premier sommet avec la couleur 0
    color[0] = currentColor;

    // Colorier les autres sommets
    for (int i = 1; i < numVertices; i++) {
        int j;
        for (j = 0; j < numVertices; j++) {
            // Si le sommet n'est pas adjacent à i et n'a pas déjà une couleur attribuée
            if (graph->adjMatrix[i][j] == 0 && color[j] == -1) {
                color[j] = currentColor;
            }
        }
        // Passer à la prochaine couleur
        currentColor++;
    }

    // Afficher les couleurs attribuées à chaque sommet
    printf("Coloration du graphe :\n");
    for (int i = 0; i < numVertices; i++) {
        printf("Sommet %d : Couleur %d\n", i, color[i]);
    }

    free(color);
}

// Fonction principale
int main() {
    // Créer un graphe avec 5 sommets
    Graph *graph = createGraph(5);

    // Ajouter des arêtes au graphe
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    // Colorier le graphe
    colorGraph(graph);

    return 0;
}
#include "header.h"

/* affichage des successeurs du sommet num*/
void afficher_successeurs(Graphe* graphe, int num)
{
    printf(" sommet %d , couleur %d:\n", graphe->pSommet[num]->valeur, graphe->pSommet[num]->couleur);

    pArc arc = graphe->pSommet[num]->arc;

    while(arc!=NULL)
    {
        printf("%d ",arc->sommet);
        arc = arc->arc_suivant;
    }

}

///Cette fonction remplace CreerArete car elle ne fonctionnais pas bien.
void CreerLiaison(Graphe* graphe, int s1, int s2){
    int i_s1 = -1;
    int i_s2 = -1;

    //On trouve le sommet qui a pour valeur S1 puis S2
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->valeur == s1) {
            i_s1 = i;
        }

        if (graphe->pSommet[i]->valeur == s2) {
            i_s2 = i;
        }

        // Si les deux sommets sont trouvés, on sort de la boucle
        if (i_s1 != -1 && i_s2 != -1) {
            break;
        }
    }

    // Création de la liaison
    pArc nouvelle_liaison = (pArc)malloc(sizeof(struct Arc));
    nouvelle_liaison->sommet = s2;
    nouvelle_liaison->arc_suivant = graphe->pSommet[i_s1]->arc;
    graphe->pSommet[i_s1]->arc = nouvelle_liaison;

    // Si le graphe n'est pas orienté, on crée également la liaison dans l'autre sens
    if (!graphe->orientation) {
        pArc nouvelle_liaison_inverse = (pArc)malloc(sizeof(struct Arc));
        nouvelle_liaison_inverse->sommet = s1;
        nouvelle_liaison_inverse->arc_suivant = graphe->pSommet[i_s2]->arc;
        graphe->pSommet[i_s2]->arc = nouvelle_liaison_inverse;
    }
}

// créer le graphe
///Le code a été changé de façon à régler le problème des numéro qui n'existe pas
///ou ce qui existe mais qui ont une valeur supérieur à celle de l'ordre du graphe
Graphe* CreerGraphe(int ordre, int taille, int orientation, FILE * ifs, char * nomFichier){
    Graphe * Newgraphe = (Graphe*)malloc(sizeof(Graphe));

    //on réouvre le fichier
    ifs = fopen(nomFichier,"r");

    ordre = taille_fichier("../exclusions.txt");
    taille = taille_fichier("../exclusions.txt");
    orientation = 0;


    Newgraphe->orientation = orientation;
    Newgraphe->ordre = ordre;
    Newgraphe->taille = taille;

    Newgraphe->pSommet = (pSommet)malloc(ordre * sizeof(struct Sommet));

    int s1, s2;
    int j = 0;
    int existe1, existe2;

    //On lit le fichier pour ensuite créer le sommet si il n'existe pas déjà
    for (int i = 0; i < taille; ++i){

        fscanf(ifs,"%d %d",&s1,&s2);;

        if(j == 0){
            Newgraphe->pSommet[j] = (pSommet)malloc(sizeof(struct Sommet));

            Newgraphe->pSommet[j]->valeur = s1;
            Newgraphe->pSommet[j]->arc = NULL;
            j++;

            Newgraphe->pSommet[j] = (pSommet)malloc(sizeof(struct Sommet));

            Newgraphe->pSommet[j]->valeur = s2;
            Newgraphe->pSommet[j]->arc = NULL;
            j++;
        } else{
            existe1 = 0;
            existe2 = 0;
            //on verifie si le sommet existe déja
            for (int k = 0; k < j; ++k) {
                if (Newgraphe->pSommet[k]->valeur == s1){
                    existe1 = 1;
                }
                if (Newgraphe->pSommet[k]->valeur == s2){
                    existe2 = 1;
                }
            }
            //on créer les sommets si ils n'existent pas
            if (!existe1){
                Newgraphe->pSommet[j] = (pSommet)malloc(sizeof(struct Sommet));

                Newgraphe->pSommet[j]->valeur = s1;
                Newgraphe->pSommet[j]->arc=NULL;
                j++;
            }
            if (!existe2){
                Newgraphe->pSommet[j] = (pSommet)malloc(sizeof(struct Sommet));

                Newgraphe->pSommet[j]->valeur = s2;
                Newgraphe->pSommet[j]->arc=NULL;
                j++;
            }
        }
    }
    Newgraphe->ordre = j;

    fclose(ifs);
    return Newgraphe;
}


/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
Graphe * lire_graphe(char * nomFichier)
{
    Graphe* graphe;
    FILE * ifs = fopen(nomFichier,"r");
    int taille, orientation, ordre, s1, s2;

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }


    fclose(ifs);

    graphe = CreerGraphe(ordre, taille, orientation, ifs , nomFichier); // créer le graphe d'ordre sommets avec les bons numéros

    /// on réouvre le fichier car il a totalement était lus puis fermé dans la fonction CreerGraphe
    ifs = fopen(nomFichier,"r");

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    ordre = taille_fichier("../exclusions.txt");
    taille = taille_fichier("../exclusions.txt");
    orientation = 0;

    // créer les arêtes du graphe
    for (int i=0; i<taille; ++i) {
        fscanf(ifs, "%d %d", &s1, &s2);
        CreerLiaison(graphe, s1, s2);

    }

    fclose(ifs);
    return graphe;
}


/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(Graphe* graphe)
{
    printf("graphe\n");

    if(graphe->orientation)
        printf("oriente\n");
    else
        printf("non oriente\n");

    printf("ordre = %d\n",graphe->ordre);

    printf("listes d'exclusions :\n");

    for (int i = 0; i < graphe->ordre; i++)
    {
        afficher_successeurs(graphe, i);
        printf("\n");
    }

}


// Fonction pour obtenir l'indice du sommet dans le tableau pSommet de Graphe
int obtenirIndiceSommet(Graphe *graphe, int valeurSommet){
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->valeur == valeurSommet) {
            return i;
        }
    }
}

// Fonction pour vérifier si la couleur peut être utilisée pour un sommet donné
int estCouleurValide(Graphe *graphe, int sommetIndex, int couleur){
    pArc arc = graphe->pSommet[sommetIndex]->arc;
    while (arc != NULL) {
        int indiceVoisin = obtenirIndiceSommet(graphe, arc->sommet);
        if (indiceVoisin != -1 && graphe->pSommet[indiceVoisin]->couleur == couleur) {
            return 0;
        }
        arc = arc->arc_suivant;
    }
    return 1;
}

//  coloration de graphe avec naïve
void ColorisationNaive(Graphe *graphe){

    // Initialisation des couleurs à -1
    for (int i = 0; i < graphe->ordre; i++) {
        graphe->pSommet[i]->couleur = -1;
    }

    // Colorer chaque sommet
    for (int i = 0; i < graphe->ordre; i++) {
        int couleur;

        for (couleur = 0; couleur < graphe->ordre; couleur++) {
            if (estCouleurValide(graphe, i, couleur)) {
                break;
            }
        }
        graphe->pSommet[i]->couleur = couleur;
         }
}


Graphe * exclusions(Graphe* g) {

    g = lire_graphe("../exclusions.txt");

    ColorisationNaive(g);
    /// afficher le graphe
    graphe_afficher(g);
}


/*

1 6
6 27
6 4
6 9
6 7
6 2
6 3
6 11
6 15
6 8
5 8
27 31
31 32
9 17
7 17
17 20
17 18
21 22
20 22
22 24
22 30
24 31
24 25
24 33
25 26
26 28
18 34
34 35
15 16
16 13
16 12
12 10
10 34
*/
