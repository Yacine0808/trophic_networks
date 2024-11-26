//
// Created by farhan on 15/10/2023.
//Uniquement le header !!!

#ifndef TP5_HEADER_H
#define TP5_HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Pour INT_MAX

// Structure pour représenter une arete du graphe (Prim)
typedef struct AretePrim {
    int sommet1;
    int sommet2;
    int poids;
} AretePrim;

// Structure pour représenter le graphe avec une liste d'aretes (Prim)
typedef struct Graphe {
    int numSommets;
    int numAretes;
    AretePrim *aretes;
} Graphe;

// Définition du type d'arete pour Kruskal
typedef struct {
    int sommet1; // Numero de sommet source
    int sommet2; // Numero de sommet de destination
    int poids;
} AreteKruskal;

// Structure pour l'ensemble disjoint
typedef struct {
    int sommet; // Numero de sommet
    int parent; // Parent dans l'ensemble disjoint
} EnsembleDisjoint;


#endif //TP5_HEADER_H
