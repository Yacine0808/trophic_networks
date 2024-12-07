#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================= */
/* ============ TROPHIC NETWORK ============ */
/* ===== ING2 : Yacine - Zayna - Louis ===== */
/* ========================================= */

// Liaison de predation
typedef struct Dependency {
    int predator;
    int prey;
    struct Dependency* next;
    int consumption;
    int predatorToprey; //1 if predator to prey link or 0 for prey to predator
} Dependency;

typedef struct Dependency* pDependency;

// Liste des Especes
typedef struct Species {
    pDependency dependency;
    int id;
    char name[50];
    double capacity;
    double number;
    float rate;
    int trophic_level;
    int color;
} Species;

typedef struct Species* pSpecies;

// Graphe Trophique
typedef struct Graphe {
    int ordre;
    pSpecies* pSpecies;
} Graphe;

#define MAX_SPECIES 100

/* =============================================== */
/* ========== MODULE 1 : CREER GRAPHE ============ */
/* =============================================== */

// Fonction pour obtenir l'indice du sommet dans le tableau pSommet de Graphe
int obtenirIndiceSommet(Graphe* graphe, int id) {
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSpecies[i]->id == id) {
            return i;
        }
    }
    return -1;
}

// Creation des liens entre especes
void CreerLiaison(Graphe* graphe, int s1, int s2, int consumption) {

    int i_s1 = obtenirIndiceSommet(graphe, s1);
    int i_s2 = obtenirIndiceSommet(graphe, s2);

    if (i_s1 == -1 || i_s2 == -1) {
        fprintf(stderr, "Erreur: Espece %d ou %d non trouvee\n", s1, s2);
        return;
    }
    // Creation du liens predateur > proie
    pDependency newDependency = (pDependency)malloc(sizeof(Dependency));
    newDependency->predator = s1;
    newDependency->prey = s2;
    newDependency->consumption = consumption;
    newDependency->predatorToprey = 1;
    newDependency->next = graphe->pSpecies[i_s1]->dependency;
    graphe->pSpecies[i_s1]->dependency = newDependency;
    graphe->pSpecies[i_s1]->trophic_level++;

    // Creation du liens inverse proie > predateur
    pDependency newDependency_inverse = (pDependency)malloc(sizeof(Dependency));
    newDependency_inverse->prey = s2;
    newDependency_inverse->predator = s1;
    newDependency_inverse->consumption = consumption;
    newDependency_inverse->next = graphe->pSpecies[i_s2]->dependency;
    newDependency_inverse->predatorToprey = 0;
    graphe->pSpecies[i_s2]->dependency = newDependency_inverse;

    /*printf("\nLiaison cree : Predateur %d (%s) - Proie %d (%s) - predatorTo prey ? %d",
           graphe->pSpecies[i_s1]->dependency->predator, graphe->pSpecies[i_s1]->name, graphe->pSpecies[i_s1]->dependency->prey,
           graphe->pSpecies[i_s2]->name,  graphe->pSpecies[i_s1]->dependency->predatorToprey);

    printf("\nLiaison cree : Proie %d (%s) - Predateur %d (%s) - predatorTo prey ? %d",
           graphe->pSpecies[i_s2]->dependency->prey, graphe->pSpecies[i_s2]->name, graphe->pSpecies[i_s2]->dependency->predator,
           graphe->pSpecies[i_s1]->name, graphe->pSpecies[i_s2]->dependency->predatorToprey);*/
}

// Capture des donnees et creation du graphe
Graphe* createGraph(char fichierEspeces[50], char fichierPredation[50]) {
    Graphe* newGraph = (Graphe*)malloc(sizeof(Graphe));
    newGraph->pSpecies = (pSpecies*)malloc(MAX_SPECIES * sizeof(pSpecies));
    for (int i = 0; i < MAX_SPECIES; ++i) {
        newGraph->pSpecies[i] = (pSpecies)malloc(sizeof(Species));
    }

    int numSpecies = 0;
    FILE* speciesFile = fopen(fichierEspeces, "r");
    if (!speciesFile) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", fichierEspeces);
        free(newGraph->pSpecies);
        free(newGraph);
        return NULL;
    }

    // Lire les especes
    while (fscanf(speciesFile, "%d %s", &newGraph->pSpecies[numSpecies]->id, &newGraph->pSpecies[numSpecies]->name) == 2) {
        newGraph->pSpecies[numSpecies]->dependency = NULL;
        newGraph->pSpecies[numSpecies]->color = 0;
        newGraph->pSpecies[numSpecies]->trophic_level= 1;
        newGraph->pSpecies[numSpecies]->number = -1;
        newGraph->pSpecies[numSpecies]->rate = -1;
        newGraph->pSpecies[numSpecies]->capacity = -1;
        numSpecies++;
    }
    fclose(speciesFile);

    newGraph->ordre = numSpecies;

    FILE* predationsFile = fopen(fichierPredation, "r");
    if (!predationsFile) {
        fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", fichierPredation);
        free(newGraph->pSpecies);
        free(newGraph);
        return NULL;
    }

    int id_predator, id_prey, consumption;
    while (fscanf(predationsFile, "%d %d %d", &id_predator, &id_prey, &consumption) == 3) {

        CreerLiaison(newGraph, id_predator, id_prey, consumption);
    }

    fclose(predationsFile);

    return newGraph;
}

/* =============================================================== */
/* ========== MODULE 2 : PARCOURIR ET AFFICHER GRAPHE ============ */
/* =============================================================== */

void afficherGraphe(Graphe* graphe) {
    printf("\n\nListe des espèces :\n");
    for (int i = 0; i < graphe->ordre; i++) {
        printf("%d: %s\n", graphe->pSpecies[i]->id, graphe->pSpecies[i]->name);
    }

    printf("\nGraphe des prédations :\n");
    for (int i = 0; i < graphe->ordre; i++) {
        pDependency dependency = graphe->pSpecies[i]->dependency;
        printf("\nEspece : %d (%s)\n", graphe->pSpecies[i]->id,graphe->pSpecies[i]->name);
        while (dependency) {
            if (dependency->predatorToprey == 1){
                printf("Predateur %d-(%s) consomme %d-(%s) au taux de %d par an, %d\n",
                       dependency->predator,
                       graphe->pSpecies[obtenirIndiceSommet(graphe, dependency->predator)]->name,
                       dependency->prey,
                       graphe->pSpecies[obtenirIndiceSommet(graphe, dependency->prey)]->name,
                       dependency->consumption, dependency->predatorToprey);
            } else if(dependency->predatorToprey == 0) {
                printf("Proie %d-(%s) est consommee par %d-(%s) au taux de %d par an, %d\n",
                       dependency->prey,
                       graphe->pSpecies[obtenirIndiceSommet(graphe, dependency->prey)]->name,
                       dependency->predator,
                       graphe->pSpecies[obtenirIndiceSommet(graphe, dependency->predator)]->name,
                       dependency->consumption, dependency->predatorToprey);
            }
            dependency = dependency->next;
        }
    }
}

// BFS
void BFS_descendants(Graphe* g, int sommet_initial, int couleur) {
    int* liste = (int*)malloc(g->ordre * sizeof(int));
    int debut = 0, fin = 0;
    liste[fin++] = sommet_initial;
    while (debut < fin) {
        int current = liste[debut++];
        int currentIndex = obtenirIndiceSommet(g, current);
        if (g->pSpecies[currentIndex]->color == couleur) {
            continue;
        }
        g->pSpecies[currentIndex]->color = couleur;
        pDependency arc = g->pSpecies[currentIndex]->dependency;
        printf("\nPredateur %d-%s : ", g->pSpecies[currentIndex]->id, g->pSpecies[currentIndex]->name);
        while (arc != NULL) {
            int nextIndex = obtenirIndiceSommet(g, arc->prey);
            if (g->pSpecies[nextIndex]->color == 0) {
                liste[fin++] = arc->prey;
                printf("%d-%s / %d par an - ", arc->prey, g->pSpecies[nextIndex]->name, arc->consumption);
            }
            arc = arc->next;
        }
    }
    free(liste);
}


/* ====================================================================== */
/* ========== MODULE 3 : SIMULATIONS ET ANALYSE DE DONNEES  ============ */
/* ====================================================================== */

//  Recherche premier et dernier maillons de la chaine alimentaire, especes n'ayant qu'un seul type de proie

void rechercheEspece(Graphe* graphe){

    int num_link_pred = 0, num_link_prey = 0;

    // Recherche des producteurs primaires
    printf("\nProducteurs primaires : ");
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSpecies[i]->trophic_level == 1) {
            printf("%d-(%s) ", graphe->pSpecies[i]->id, graphe->pSpecies[i]->name);
        }
    }

    // Recherche des grands predateurs
    printf("\nGrand predateur : ");
    for (int i = 0; i < graphe->ordre; i++) {
        num_link_pred = 0;
        pDependency dependency = graphe->pSpecies[i]->dependency;
        while (dependency != NULL) {
            if (!dependency->predatorToprey) num_link_pred++;
            dependency = dependency->next;
        }
        if (num_link_pred == 0) {
            printf("%d-(%s) ", graphe->pSpecies[i]->id, graphe->pSpecies[i]->name);
        }
    }

    // Recherche des predateurs qui n'ont qu'un tyupe de proie
    printf("\nEspece a une seule source d'alimentation : ");
    for (int i = 0; i < graphe->ordre; i++) {
        num_link_prey = 0;
        pDependency dependency = graphe->pSpecies[i]->dependency;
        while (dependency != NULL) {
            if (dependency->predatorToprey) num_link_prey++;
            dependency = dependency->next;
        }
        if (num_link_prey == 1) {
            printf("%d-(%s) ", graphe->pSpecies[i]->id, graphe->pSpecies[i]->name);
        }
    }
    printf("\n");
}

// Parcours du graphe via DFS pour extraire les proies
void dfs_prey(Graphe* graphe, int predator, int visited[]) {

    int id_predator = obtenirIndiceSommet(graphe,predator);
    if (visited[id_predator]) return;
    visited[id_predator] = 1;
    printf("%d-(%s) > ", graphe->pSpecies[id_predator]->id, graphe->pSpecies[id_predator]->name);

    pDependency dependency = graphe->pSpecies[id_predator]->dependency;
    while (dependency != NULL) {
        dfs_prey(graphe, dependency->prey, visited);
        dependency = dependency->next;
    }
}

// Parcours du graphe via DFS pour extraire les predateurs
void dfs_predator(Graphe* graphe, int prey, int visited[]) {

    int id_prey = obtenirIndiceSommet(graphe,prey);
    if (visited[id_prey]) return;
    visited[id_prey] = 1;
    printf("%d-(%s) > ", graphe->pSpecies[id_prey]->id, graphe->pSpecies[id_prey]->name);

    pDependency dependency = graphe->pSpecies[id_prey]->dependency;
    while (dependency != NULL) {
        dfs_predator(graphe, dependency->predator, visited);
        dependency = dependency->next;
    }
}
void afficherDependances(Graphe* graphe, int espece) {

    int id_espece = obtenirIndiceSommet(graphe, espece);
    int visited[MAX_SPECIES] = {0};

    printf("\nEspece %s ", graphe->pSpecies[id_espece]->name);
    printf("\nNiveau trophique %d ", graphe->pSpecies[id_espece]->trophic_level);
    printf("\nChaine descendante : ");

    dfs_prey(graphe, espece, visited);
    memset(visited, 0, sizeof(visited)); // remet visiter[] a 0

    printf("\nDependances ascendentes : ");
    dfs_predator(graphe, espece, visited);
}

void connexes_afficher(Graphe* graphe) {
    for (int i = 0; i < graphe->ordre; i++) {
        graphe->pSpecies[i]->color = 0;
    }

    int indice_connexe = 1;
    BFS_descendants(graphe, 7, 1); // a debugger
    /*for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSpecies[i]->color == 0) {
            BFS(graphe, graphe->pSpecies[i]->id, indice_connexe);
            for (int j = 0; j < graphe->ordre; j++) {
                if (graphe->pSpecies[j]->color == indice_connexe) {
                    printf("%d (%s) ", graphe->pSpecies[j]->id, graphe->pSpecies[j]->name);
                }
            }
        }
    }*/
    printf("\n");
}

Graphe* intialisationSimulation(Graphe* graphe, char fichierSimulation[50]) {

        // Lecture des donnees de simulation
        // // Id_espece  nombre  K_portance  taux_croissance

        int numSpecies = 0;
        FILE* simulFile = fopen(fichierSimulation, "r");
        if (!simulFile) {
            fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier %s\n", simulFile);
            free(graphe->pSpecies);
            free(graphe);
            return NULL;
        }

        // Lire les especes
        while (fscanf(simulFile,"%d %d %d %.2f",
                      &graphe->pSpecies[numSpecies]->id,
                      &graphe->pSpecies[numSpecies]->number,
                      &graphe->pSpecies[numSpecies]->capacity,
                      &graphe->pSpecies[numSpecies]->rate) == 4) {

            numSpecies++;
        }
        fclose(simulFile);
        printf("\nInitialisation des donnees de simulation : \n");
        for (int i = 0; i < graphe->ordre; i++) {
            printf("%d-(%s) \nnombre : %d \ncapacite : %d \ntaux de croissance : %.2f\n",
                   graphe->pSpecies[i]->id, graphe->pSpecies[i]->name,
                   graphe->pSpecies[i]->number,
                   graphe->pSpecies[i]->capacity,
                   graphe->pSpecies[i]->rate );
        }
        return graphe;
}

int main() {
    char fichierEspeces[50] = "especes.txt";
    char fichierPredations[50] = "predations.txt";
    char fichierSimulation[50] = "simulation_ini.txt";

    int id_espece = -1;

    /* ======== En commentaire pendant la phase de tests ======== */
    //printf("Entrez le nom du fichier des especes à lire : ");
    //scanf("%49s", fichierEspeces);
    //printf("Entrez le nom du fichier de predations à lire : ");
    //scanf("%49s", fichierPredations);
    /* ========================================================== */

    Graphe* trophicGraph = createGraph(fichierEspeces, fichierPredations);

    if (!trophicGraph) {
        fprintf(stderr, "Echec dans la creation de graphe trophique\n");
        return EXIT_FAILURE;
    }

    afficherGraphe(trophicGraph);
    rechercheEspece(trophicGraph);

    // Affichage des dependences trophiques
    // A debuguer pour le niveau trophique

    while (id_espece !=0) {
        printf("\nEntrez le numero de l'espece (0 pour sortir) : ");
        scanf("%d", &id_espece);
        if (id_espece <= trophicGraph->ordre && id_espece !=0) {
            afficherDependances(trophicGraph, id_espece);
            printf("\n");
        } else id_espece = 0;
    }

    intialisationSimulation(trophicGraph, fichierSimulation);

    for (int i = 0; i < trophicGraph->ordre; i++) {
        pDependency dependency = trophicGraph->pSpecies[i]->dependency;
        while (dependency) {
            pDependency temp = dependency;
            dependency = dependency->next;
            free(temp);
        }
        free(trophicGraph->pSpecies[i]);
    }
    free(trophicGraph->pSpecies);
    free(trophicGraph);

    return EXIT_SUCCESS;
}
