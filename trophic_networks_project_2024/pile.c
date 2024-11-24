#include "header.h"
Pile* creerPile(int capacite) {
    Pile* pile = (Pile*)malloc(sizeof(Pile));
    pile->capacite = capacite;
    pile->top = -1;
    pile->items = (int*)malloc(sizeof(int) * capacite);
    return pile;
}

bool estPileVide(Pile* pile) {
    return pile->top == -1;
}

void empiler(Pile* pile, int item) {
    pile->items[++pile->top] = item;
}

int depiler(Pile* pile) {
    if (!estPileVide(pile)) {
        return pile->items[pile->top--];
    }
    return -1;
}
int sommetEnHaut(Pile* pile) {
    if (!estPileVide(pile)) {
        return pile->items[pile->top];
    }
    return -1;
}
void afficherPile(Pile* pile) {
    for (int i = 0; i <= pile->top; i++) {
        printf("%d ", pile->items[i] + 1);
    }
    printf("\n");
}

/*
ajt file
struct File* creerFile() {
    struct File* f = (struct File*)malloc(sizeof(struct File));
    f->debut = f->fin = NULL;
    return f;
}

// Fonction pour ajouter un élément à la file
void enfiler(struct File* f, int valeur) {
    struct NoeudFile* temp = (struct NoeudFile*)malloc(sizeof(struct NoeudFile));
    temp->donnee = valeur;
    temp->suivant = NULL;

    if (f->fin == NULL) {
        f->debut = f->fin = temp;
        return;
    }

    f->fin->suivant = temp;
    f->fin = temp;
}

// Fonction pour retirer un élément de la file
int defiler(struct File* f) {
    if (f->debut == NULL) {
        return -1;
    }

    struct NoeudFile* temp = f->debut;
    int donnee = temp->donnee;
    f->debut = f->debut->suivant;

    if (f->debut == NULL) {
        f->fin = NULL;
    }

    free(temp);
    return donnee;
}

// Fonction pour vérifier si la file est vide
bool estVide(struct File* f){
    return (f->debut == NULL);
}
*/
