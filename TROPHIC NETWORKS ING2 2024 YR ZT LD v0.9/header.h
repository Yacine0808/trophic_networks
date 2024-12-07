#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dependency struct
typedef struct Dependency {
    int predator;               // ID of the predator species
    struct Dependency* next;    // Pointer to the next dependency
    int consumption;            // Annual consumption
} Dependency;

typedef struct Dependency* pDependency;

// Species struct
typedef struct Species {
    pDependency dependency;     // Pointer to the dependencies
    int id;                     // ID of the species
    char name[50];              // Name of the species
    double number;              // Population size
    double capacity;            // Capacity of the environment
    float rate;                 // Growth rate
    int trophic_level;          // Trophic level
    int couleur;                // Color (used for visualization, if any)
} Species;

typedef struct Species* pSpecies;

// Graph struct
typedef struct Graphe {
    int taille;                 // Size of the graph (number of edges)
    int ordre;                  // Order of the graph (number of vertices)
    pSpecies* pSpecies;         // Array of species pointers
} Graphe;

#endif // HEADER_H
