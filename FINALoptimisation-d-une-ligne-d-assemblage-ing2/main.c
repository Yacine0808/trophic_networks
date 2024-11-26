#include "header.h"


// Fonction pour obtenir l'indice du sommet dans le tableau pSommet de Graphe
int obtenirIndiceSommet(Graphe *graphe, int valeurSommet){
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->valeur == valeurSommet) {
            return i;
        }
    }
}

/* affichage des successeurs du sommet num*/
void afficher_successeurs(Graphe* graphe, int num)
{
    printf("\nWS%d %d ", graphe->pSommet[num]->couleur, graphe->pSommet[num]->valeur);

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
// creation de liaison
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
Graphe * lire_graphe(char * nomFichier, int orientation){

    Graphe* graphe;
    FILE * ifs = fopen(nomFichier,"r");
    int taille, ordre, s1, s2;

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    fclose(ifs);

    ordre = taille_fichier("../operations.txt");
    taille = taille_fichier(nomFichier);

    graphe = CreerGraphe(ordre, taille, orientation, ifs , nomFichier); // créer le graphe d'ordre sommets avec les bons numéros

    /// on réouvre le fichier car il a totalement était lus puis fermé dans la fonction CreerGraphe
    ifs = fopen(nomFichier,"r");

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

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
    //printf("graphe\n");

    //if(graphe->orientation)
    //    printf("oriente\n");
    //else
    //    printf("non oriente\n");

    //printf("ordre = %d\n",graphe->ordre);

    //printf("successeurs :\n");

    for (int i = 0; i < graphe->ordre; i++)
    {
        afficher_successeurs(graphe, i);
        //printf("\n");
    }
}


/////////////////////////////////////////
/// Algorithme BFS
//////////////////////////////////////////
int * BFS(Graphe* g, int sommet_initial, int couleur){
    int num = 0;
    int* liste;
    int regarde = 0;
    int j;
    int vue;
    pArc arc;

    liste = (int*)malloc(g->ordre * sizeof(int));

    for (int i = 0; i < g->ordre; ++i){
        liste[i] = -1;
    }

    // Trouver quel sommet a pour valeur le sommet initial
    num = obtenirIndiceSommet(g, sommet_initial);

    liste[regarde] = g->pSommet[num]->valeur;

    while (liste[regarde] != -1){
        arc = g->pSommet[num]->arc;

        while (arc != NULL) {
            j = 0;
            vue = 0;

            // Parcourir la liste pour vérifier si le sommet lié y est déjà
            while (liste[j] != -1){
                if (liste[j] == arc->sommet){
                    vue++;
                    break;
                }
                j++;
            }

            // Si le sommet n'est pas dans la liste on le rajoute.
            if (!vue){
                liste[j] = arc->sommet;
            }
            arc = arc->arc_suivant;
        }

        regarde++;

        // Trouver le sommet suivant dans la liste si il y en a un
        if (liste[regarde] != -1){
            num = 0;
            while (g->pSommet[num]->valeur != liste[regarde]){
                num++;
            }
        }
    }

    // Afficher la liste
    printf("Ensemble connexe : ");
    for (int i = 0; i < g->ordre; ++i){
        if (liste[i] != -1){
            printf("%d ", liste[i]);
            num = 0;
            while ((g->pSommet[num]->valeur != liste[i]) && (num <= g->ordre)){
                num++;
            }
            g->pSommet[num]->couleur = couleur;
        }
    }
    printf("\n");

    return liste;
}

void connexes_afficher(Graphe* graphe, char BFSorDFS) {

    int indice_connexe = 0;
    int s0 = 0;
    int *liste;
    // Initialiser le champ connexe
    for (int i=0; i<graphe->ordre; i++) {
        graphe->pSommet[i]->couleur = 0;
    }
    indice_connexe = 1;
    // On cherche la 1ere liste connexe
    if (BFSorDFS == 'B') {
        printf("Connexe 1 : ");
        BFS(graphe, 1,1);

        // On cherche les listes connexes suivantes dans les cellules ou connexe est reste a 0 ie non visitées
        for (int i=0; i<graphe->ordre; i++) {
            if (graphe->pSommet[i]->couleur==0) {
                indice_connexe++;
                printf("Connexe %d : ", indice_connexe);
                BFS(graphe, graphe->pSommet[i]->valeur,indice_connexe);
            }
        }
    } else {
        printf("DFS Connexe 1 : ");
        //DFS_NEW(graphe, s0,1);
    }
}

int main() {
    //// 2.1 - 1 Graphe d'exclusions

    Graphe * g_exclusions;

    printf("\n2.1.1 REPARTITION DES OPERATIONS EN PRENANT UNIQUEMENT EN COMPTE LES REGLES D'ECLUSION \n");
    g_exclusions = exclusions(g_exclusions);

    //// 2.1 - 2 Graphe de precedences

    WorkStation * ws;
    WorkStation * wse;
    Graphe * g_precedences;
    float temps_c;

    // Creer le graphe de precedences
    // Renseigner le temps de chaque operations

    printf("\n\n2.1.2 REPARTITION DES OPERATIONS EN PRENANT UNIQUEMENT EN COMPTE LES REGLES DE PRECEDENCE ET DE TEMPS \n");
    g_precedences = precedences(g_precedences);

    // Lire le temps de cycle
    temps_c = temps_cycle();
    printf("\nTEMPS D'UN CYCLE : %.2f\n", temps_c);

    // Creer une Graphe Usine avec la liste des operations par Sommet
    ws = ws_precedences(g_precedences, temps_c);

    //// 2.2 - 2 Graphe prenant en compte precedences et exclusions

    printf("\n\n2.2 REPARTITION DES OPERATIONS EN PRENANT EN COMPTE LES REGLES DE PRECEDENCE ET D'EXCLUSION\n");

    // Creer une Graphe Usine avec la liste des operations par Sommet

    Graphe * g_exclusions_precedence;
    g_exclusions_precedence = exclusions_precendences(g_exclusions);

    //// 2.2 - 2 Graphe prenant en compte precedences et exclusions

    printf("\n\n2.2 REPARTITION DES OPERATIONS EN PRENANT EN COMPTE LES REGLES DE PRECEDENCES D'EXCLUSION ET DE TEMPS\n");

    ws = ws_multi_contraintes(ws, g_precedences, g_exclusions, temps_c);

    //on suprime la mémoire allouée dynamiquement
    for (int i = 0; i < g_exclusions->ordre; ++i) {
        free(g_exclusions->pSommet[i]->arc);
        free(g_exclusions->pSommet[i]);
    }
    free(g_exclusions->pSommet);
    free(g_exclusions);
    for (int i = 0; i < g_precedences->ordre; ++i) {
        free(g_precedences->pSommet[i]->arc);
        free(g_precedences->pSommet[i]);
    }
    free(g_precedences->pSommet);
    free(g_precedences);
    return 0;
}
