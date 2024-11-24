
//Seulment le main de kruskal et prim !! voir les autres fichiers 

int main() {
    //On declare les fonctions du sous programme

    int compareKruskal(const void *a, const void *b);
    int compareAretePrim(const void *a, const void *b);
    Graphe* chargerGrapheDepuisFichier(const char *nomFichier);
    int ChercherEnsemble(EnsembleDisjoint *ensembles, int index);
    void Assembler(EnsembleDisjoint *ensembles, int a, int b);
    void kruskal(Graphe *graphe, char nomsSommets[][20]);
    void Prim(Graphe *graphe);


    //Nom de fichier que l'utilisateur ecrit
    char fichier[50];
    printf("Entrer le nom du fichier contenant le graphe : ");
    gets(fichier);
    Graphe *graphe = chargerGrapheDepuisFichier(fichier);

    if (graphe == NULL) {
        return 1;
    }

    char nomsSommets[graphe->numSommets][20];

    for (int i = 0; i < graphe->numSommets; i++) {
        snprintf(nomsSommets[i], sizeof(nomsSommets[i]), "%d", i);
    }
    // On cree un menu avec les differents algos
    int choix = 0;
    while (choix != 3) {    // le programme tourne en boucle
        printf("Choisissez l'algorithme pour ChercherEnsemble l'arbre couvrant de poids minimal\n");
        printf("1. [Algo de Kruskal]\n");
        printf("2. [Algo de Prim]\n");
        printf("3. [Quitter]\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                qsort(graphe->aretes, graphe->numAretes, sizeof(AretePrim), compareAretePrim);
                kruskal(graphe, nomsSommets);
                break;
            case 2:
                Prim(graphe);
                break;
            case 3:
                printf("Fin du programme\n");
                break;
            default:
                printf("Choix non valide\n");
                break;
        }
    }
    // Libération de la mémoire
    free(graphe->aretes);
    free(graphe);
    return 0;
}
