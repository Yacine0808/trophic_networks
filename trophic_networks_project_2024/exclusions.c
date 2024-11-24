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
    printf("usine des points :\n");

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
Graphe * exclusions(Graphe* g){
    g = lire_graphe("../exclusions.txt");
    ColorisationNaive(g);
    /// afficher le graphe
    graphe_afficher(g);
    return g;
}//