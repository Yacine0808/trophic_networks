/// PRISE EN COMPTE EXCLUSION

#include "header.h"


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

Graphe * exclusions(Graphe* g){

    g = lire_graphe("../exclusions.txt", 0); // Orientation 0

    ColorisationNaive(g);
    /// afficher le graphe
    graphe_afficher(g);

    return g;
}