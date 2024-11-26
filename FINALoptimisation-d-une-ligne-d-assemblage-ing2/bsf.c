#ifndef TP1_HEADER_H
#define TP1_HEADER_H
#include "stdlib.h"
#include "string.h"
#include <stdio.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//BSF permettant le parcours de la ligne d'assemblage en profondeur EX arbre poids minimal
typedef struct Sommet {
    char *nom;
    int numero;
} t_Sommet;

typedef struct Graphe {
    int **matrice_adj;
    int ordre;
    t_Sommet *tab;
} t_Graphe;

t_Sommet *allocation_sommet(char* nom, int numero)
{
    t_Sommet *nouveau_sommet = (t_Sommet *) malloc(sizeof(t_Sommet));
    if (nouveau_sommet == NULL) {
        printf("Problème d'alloc");
        exit(EXIT_FAILURE);
    }
    nouveau_sommet->nom = (char *) malloc(strlen(nom) + 1);
    if (nouveau_sommet->nom == NULL) {
        printf("Problème d'alloc");
        exit(EXIT_FAILURE);
    }

    nouveau_sommet->numero = numero;
    strcpy(nouveau_sommet->nom, nom);
    return nouveau_sommet;
}

void initialiserGraphe(int ordre, t_Graphe *pt)
{
    pt->ordre = ordre;
    pt->matrice_adj = (int **) malloc(ordre * sizeof(int *));
    if (pt->matrice_adj == NULL) {
        printf("Problème d'alloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ordre; i++) {
        pt->matrice_adj[i] = (int *) malloc(ordre * sizeof(int));
        if (pt->matrice_adj[i] == NULL) {
            printf("Problème d'alloc");
            exit(EXIT_FAILURE);
        }
    }
    pt->tab = (t_Sommet *) malloc(ordre * sizeof(t_Sommet));
    if (pt->tab == NULL) {
        printf("Problème d'alloc");
        exit(EXIT_FAILURE);
    }
}

void chargeFichiers(char* fichier_influences, t_Graphe *pt)
{
    FILE *pt1 = fopen(fichier_influences, "r");
    if (pt1 == NULL) {
        printf("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    int ORD;
    fscanf(pt1, "%d", &ORD);
    initialiserGraphe(ORD, pt);

    for (int i = 0; i < ORD; ++i) {
        for (int j = 0; j < ORD; ++j) {
            int numero;
            fscanf(pt1, "%d", &numero);  // Lire un entier (0 ou 1) directement
            pt->matrice_adj[i][j] = numero;
        }
    }

    for (int i = 0; i < ORD; ++i) {
        char nom[100];
        fscanf(pt1, "%s", nom);
        pt->tab[i] = *allocation_sommet(nom, i);
    }

    fclose(pt1);
}

void libererGraphe(t_Graphe *pt)
{
    for (int i = 0; i < pt->ordre; i++) {
        free(pt->matrice_adj[i]);
    }
    free(pt->matrice_adj);

    for (int i = 0; i < pt->ordre; i++) {
        free(pt->tab[i].nom);
    }
    free(pt->tab);
}

void afficherJeux(t_Graphe *pt) {
    int compteur = 0;

    for (int i = 0; i < pt->ordre; ++i) {
        printf("%s : influence : ", pt->tab[i].nom);

        for (int j = 0; j < pt->ordre; ++j) {
            if (pt->matrice_adj[i][j] == 1) {
                compteur = compteur + 1;
                printf("%s\n", pt->tab[j].nom);
            }
        }
        if (compteur == 0) {
            printf("personne\n");
        }
        compteur = 0;
    }
}




#endif //TP1_HEADER_H

int main_BFS() {
    char nom[60];
    t_Graphe* pt;
    pt = (t_Graphe*) malloc(sizeof (t_Graphe));
    if(pt == NULL){
        printf("problème alloc");
        exit(EXIT_FAILURE);
    }
    printf("Sasit un nom de fichier\n");
    scanf("%s", nom);
    chargeFichiers(nom, pt);
    afficherJeux(pt);
    return 0;
}

// ajoutez le DSF en cas de besoin (parcours en largeur) 
