//
// Created by erelr on 02/12/2023.
//
#include "header.h"

/* affichage des successeurs du sommet num*/
void afficher_successeurs2(Graphe* graphe, int num)
{
    printf(" sommet %d , usine %d:\n", graphe->pSommet[num]->valeur, graphe->pSommet[num]->couleur);
}

///Cette fonction remplace CreerArete car elle ne fonctionnais pas bien.
void CreerLiaison2(Graphe* graphe, int s1, int s2){
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
Graphe* CreerGraphe2(int ordre, int taille, int orientation, FILE * ifs, char * nomFichier){
    Graphe * Newgraphe = (Graphe*)malloc(sizeof(Graphe));

    //on réouvre le fichier
    ifs = fopen(nomFichier,"r");

    ordre = taille_fichier("../exclusions.txt");
    taille = taille_fichier("../exclusions.txt");
    orientation = 1;


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
Graphe * lire_graphe2(char * nomFichier)
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

    graphe = CreerGraphe2(ordre, taille, orientation, ifs , nomFichier); // créer le graphe d'ordre sommets avec les bons numéros

    /// on réouvre le fichier car il a totalement était lus puis fermé dans la fonction CreerGraphe
    ifs = fopen(nomFichier,"r");

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    ordre = taille_fichier("../operations.txt");
    taille = taille_fichier(nomFichier);
    orientation = 0;

    // créer les arêtes du graphe
    for (int i=0; i<taille; ++i) {
        fscanf(ifs, "%d %d", &s1, &s2);
        CreerLiaison2(graphe, s2, s1);

    }

    fclose(ifs);
    return graphe;
}


/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher2(Graphe* graphe)
{


    printf("usine des points :\n");

    for (int i = 0; i < graphe->ordre; i++)
    {
        afficher_successeurs2(graphe, i);
        printf("\n");
    }

}

// Fonction pour obtenir l'indice du sommet dans le tableau pSommet de Graphe
int obtenirIndiceSommet2(Graphe *graphe, int valeurSommet){
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->valeur == valeurSommet) {
            return i;
        }
    }
}

// DFS
void dfs(Graphe *graphe, int sommetIndex) {
    int *pile = (int *)malloc(graphe->ordre * sizeof(int));
    int sommet;
    int sommetVoisin;
    int sommetCount = 0;

    // Initialisation du tableau pour suivre les sommets visités
    int *visite = (int *)malloc(graphe->ordre * sizeof(int));
    for (int i = 0; i < graphe->ordre; ++i) {
        visite[i] = 0;
    }

    pile[sommetCount++] = sommetIndex;

    while (sommetCount > 0) {
        // Dépile le sommet actuel
        sommet = pile[--sommetCount];

        // Si le sommet n'a pas encore été visité
        if (!visite[sommet]) {
            visite[sommet] = 1;
            pArc arc = graphe->pSommet[sommet]->arc;

            graphe->pSommet[sommet]->couleur++;



            // Parcours les voisins du sommet actuel
            while (arc != NULL) {
                sommetVoisin = obtenirIndiceSommet2(graphe, arc->sommet);
                if (sommetVoisin != -1 && !visite[sommetVoisin]) {
                    // Empile le voisin non visité
                    pile[sommetCount++] = sommetVoisin;
                }
                arc = arc->arc_suivant;
            }
        }
    }

    // Libère la mémoire allouée
    free(pile);
    free(visite);
}

// Fonction pour trier les indices des sommets par couleur décroissante
int* listeIndicesParCouleur(Graphe *graphe) {

    int* indices = (int*)malloc(graphe->ordre * sizeof(int));
    for (int i = 0; i < graphe->ordre; i++) {
        indices[i] = i;
    }

    // Tri des indices en fonction de la couleur décroissante
    for (int i = 0; i < graphe->ordre - 1; i++) {
        for (int j = i + 1; j < graphe->ordre; j++) {
            if (graphe->pSommet[indices[i]]->couleur < graphe->pSommet[indices[j]]->couleur) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    return indices;
}

Graphe * exclusions_precedence(Graphe* g_exclusions){
    Graphe *g_precedences;
    int* indicesParCouleur;
    int couleur = 0;
    int existe = 0;
    int existe2 = 0;
    int m_couleur = 0;
    int indexpred;

    g_precedences = lire_graphe2("../precedences.txt");
    /// afficher le graphe

    for (int i = 0; i < g_precedences->ordre; ++i) {
        g_precedences->pSommet[i]->couleur = 0;
    }

    // Exécute la recherche en profondeur (DFS) sur le graphe de précédence
    for (int i = 0; i < g_precedences->ordre; ++i) {
            dfs(g_precedences, i);
    }

    indicesParCouleur = listeIndicesParCouleur(g_precedences);

    for (int i = 0; i < g_precedences->ordre; i++) {
        existe = 0;
        existe2 = 0;
        m_couleur = 0;


        if (i == 0){
            g_precedences->pSommet[indicesParCouleur[i]]->couleur = couleur;
        } else{
            for (int j = 0; j < g_exclusions->ordre; ++j) {
                if(g_precedences->pSommet[indicesParCouleur[i]]->valeur == g_exclusions->pSommet[j]->valeur){
                    existe++;
                }
            }
            if(!existe){
                g_precedences->pSommet[indicesParCouleur[i]]->couleur = g_precedences->pSommet[indicesParCouleur[indexpred]]->couleur;
            } else{
                for (int j = 0; j < g_exclusions->ordre; ++j) {
                    if(g_precedences->pSommet[indicesParCouleur[indexpred]]->valeur == g_exclusions->pSommet[j]->valeur){
                        existe2++;
                    }
                }
                if(!existe2){
                    g_precedences->pSommet[indicesParCouleur[i]]->couleur = g_precedences->pSommet[indicesParCouleur[indexpred]]->couleur;
                } else{
                    for (int j = 0; j < g_exclusions->ordre; ++j) {
                        for (int k = 0; k < g_exclusions->ordre; ++k) {
                            if (g_precedences->pSommet[indicesParCouleur[i - 1]]->valeur ==
                                g_exclusions->pSommet[j]->valeur &&
                                g_precedences->pSommet[indicesParCouleur[i]]->valeur ==
                                g_exclusions->pSommet[k]->valeur) {
                                if (g_exclusions->pSommet[j]->couleur == g_exclusions->pSommet[k]->couleur) {
                                    m_couleur = 1;
                                }
                            }
                        }
                    }
                    if (m_couleur){
                        g_precedences->pSommet[indicesParCouleur[i]]->couleur = g_precedences->pSommet[indicesParCouleur[indexpred]]->couleur;
                    } else{
                        couleur++;
                        g_precedences->pSommet[indicesParCouleur[i]]->couleur = couleur;
                    }
                }
            }
        }
        indexpred = i;
    }
    printf("\n");

    graphe_afficher2(g_precedences);

    free(indicesParCouleur);

    return g_precedences;
}