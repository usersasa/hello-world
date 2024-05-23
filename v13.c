#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SALLES 10
#define MAX_PLACE_NAME 50
#define MAX_ARTIST_NAME 50

typedef struct {
    char nom[MAX_PLACE_NAME];
    int nombre_rangees_A;
    int nombre_rangees_B;
    int nombre_rangees_total;
    int *sieges_par_rangee;
    int fosse;
    int concert_attribue;
    char artiste[MAX_ARTIST_NAME];
    float prix_categorie_concert[3];
    int fin_concert;
} SalleConcert;

void viderTampon() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lireChaine(char *chaine, int longueur) {
    fgets(chaine, longueur, stdin);
    chaine[strcspn(chaine, "\n")] = '\0';
}

void sauvegarderSalles(SalleConcert salles[], int nombre_salles) {
    FILE *file = fopen("salles.txt", "w");
    for (int i = 0; i < nombre_salles; i++) {
        fprintf(file, "%s %d %d %d %d %d %d\n", salles[i].nom, salles[i].nombre_rangees_A, salles[i].nombre_rangees_B, salles[i].nombre_rangees_total, salles[i].fosse, salles[i].concert_attribue, salles[i].fin_concert);
        for (int j = 0; j < salles[i].nombre_rangees_total; j++) {
            fprintf(file, "%d ", salles[i].sieges_par_rangee[j]);
        }
        fprintf(file, "\n");
        fprintf(file, "%s %.2f %.2f %.2f\n", salles[i].artiste, salles[i].prix_categorie_concert[0], salles[i].prix_categorie_concert[1], salles[i].prix_categorie_concert[2]);
    }
    fclose(file);
}

void chargerSalles(SalleConcert salles[], int *nombre_salles) {
    FILE *file = fopen("salles.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %d %d %d %d %d %d\n", salles[*nombre_salles].nom, &salles[*nombre_salles].nombre_rangees_A, &salles[*nombre_salles].nombre_rangees_B, &salles[*nombre_salles].nombre_rangees_total, &salles[*nombre_salles].fosse, &salles[*nombre_salles].concert_attribue, &salles[*nombre_salles].fin_concert) != EOF) {
        salles[*nombre_salles].sieges_par_rangee = (int *)malloc(salles[*nombre_salles].nombre_rangees_total * sizeof(int));
        for (int j = 0; j < salles[*nombre_salles].nombre_rangees_total; j++) {
            fscanf(file, "%d", &salles[*nombre_salles].sieges_par_rangee[j]);
        }
        fscanf(file, "\n");
        fscanf(file, "%s %f %f %f\n", salles[*nombre_salles].artiste, &salles[*nombre_salles].prix_categorie_concert[0], &salles[*nombre_salles].prix_categorie_concert[1], &salles[*nombre_salles].prix_categorie_concert[2]);
        (*nombre_salles)++;
    }
    fclose(file);
}

void creerSalle(SalleConcert salles[], int *nombre_salles) {
    if (*nombre_salles >= MAX_SALLES) {
        printf("Nombre maximal de salles atteint.\n");
        return;
    }

    SalleConcert nouvelle_salle;
    printf("\nCréation d'une nouvelle salle de concert :\n");
    printf("Nom de la salle de concert : ");
    lireChaine(nouvelle_salle.nom, MAX_PLACE_NAME);
    
    do {
        printf("Nombre de rangées pour la catégorie A : ");
        scanf("%d", &nouvelle_salle.nombre_rangees_A);
        if (nouvelle_salle.nombre_rangees_A < 0) {
            printf("Donner un nombre de rangées correct svp ! ");
        }
    } while (nouvelle_salle.nombre_rangees_A < 0);
    
    do {
        printf("Nombre de rangées pour la catégorie B : ");
        scanf("%d", &nouvelle_salle.nombre_rangees_B);
        if (nouvelle_salle.nombre_rangees_B < 0) {
            printf("Donner un nombre de rangées correct svp : ");
        }
    } while (nouvelle_salle.nombre_rangees_B < 0);
    
    do {
        printf("Combien de rangées au total : ");
        scanf("%d", &nouvelle_salle.nombre_rangees_total);
        if (nouvelle_salle.nombre_rangees_total < nouvelle_salle.nombre_rangees_A + nouvelle_salle.nombre_rangees_B) {
            printf("Le nombre total de rangées doit être supérieur ou égal à la somme des rangées de catégorie A et B.\n");
        }
    } while (nouvelle_salle.nombre_rangees_total < nouvelle_salle.nombre_rangees_A + nouvelle_salle.nombre_rangees_B);
    viderTampon();

    nouvelle_salle.sieges_par_rangee = (int *)malloc(nouvelle_salle.nombre_rangees_total * sizeof(int));

    int personnaliser;
    printf("Voulez-vous personnaliser le nombre de sièges par rangée ? (1 pour oui, 0 pour non) : ");
    scanf("%d", &personnaliser);
    viderTampon();

    if (personnaliser == 1) {
        for (int i = 0; i < nouvelle_salle.nombre_rangees_total; i++) {
            printf("Nombre de sièges pour la rangée %d : ", i + 1);
            scanf("%d", &nouvelle_salle.sieges_par_rangee[i]);
        }
    } else {
        int sieges_par_rangee;
        printf("Nombre de sièges par rangée : ");
        scanf("%d", &sieges_par_rangee);
        viderTampon();
        for (int i = 0; i < nouvelle_salle.nombre_rangees_total; i++) {
            nouvelle_salle.sieges_par_rangee[i] = sieges_par_rangee;
        }
    }

    printf("La salle a-t-elle une fosse ? (1 pour oui, 0 pour non) : ");
    scanf("%d", &nouvelle_salle.fosse);
    viderTampon();

    if (nouvelle_salle.fosse == 1) {
        for (int i = 0; i < nouvelle_salle.nombre_rangees_A; i++) {
            nouvelle_salle.sieges_par_rangee[i] *= 2;
        }
    }

    nouvelle_salle.concert_attribue = 0;

    salles[*nombre_salles] = nouvelle_salle;
    (*nombre_salles)++;

    sauvegarderSalles(salles, *nombre_salles);
}

void attribuerConcert(SalleConcert salles[], int nombre_salles) {
    if (nombre_salles == 0) {
        printf("Aucune salle de concert disponible.\n");
        return;
    }

    char nom_salle[MAX_PLACE_NAME];
    printf("\nAttribution d'un concert :\n");
    printf("Nom de la salle de concert : ");
    lireChaine(nom_salle, MAX_PLACE_NAME);

    int index_salle = -1;
    for (int i = 0; i < nombre_salles; i++) {
        if (strcmp(nom_salle, salles[i].nom) == 0) {
            index_salle = i;
            break;
        }
    }

    if (index_salle == -1) {
        printf("La salle de concert spécifiée n'existe pas.\n");
        return;
    }

    printf("Nom de l'artiste : ");
    lireChaine(salles[index_salle].artiste, MAX_ARTIST_NAME);
    printf("Prix catégorie A : ");
    scanf("%f", &salles[index_salle].prix_categorie_concert[0]);
    printf("Prix catégorie B : ");
    scanf("%f", &salles[index_salle].prix_categorie_concert[1]);
    printf("Prix catégorie C : ");
    scanf("%f", &salles[index_salle].prix_categorie_concert[2]);
    printf("Heure de fin du concert (format 24h) : ");
    scanf("%d", &salles[index_salle].fin_concert);
    viderTampon();

    salles[index_salle].concert_attribue = 1;

    sauvegarderSalles(salles, nombre_salles);

    printf("Concert attribué à la salle %s avec succès.\n", salles[index_salle].nom);
}

void modifierSalle(SalleConcert salles[], int nombre_salles) {
    char nom_salle[MAX_PLACE_NAME];
    printf("\nModification de la salle de concert :\n");
    printf("Nom de la salle de concert : ");
    lireChaine(nom_salle, MAX_PLACE_NAME);

    int index_salle = -1;
    for (int i = 0; i < nombre_salles; i++) {
        if (strcmp(nom_salle, salles[i].nom) == 0) {
            index_salle = i;
            break;
        }
    }

    if (index_salle == -1) {
        printf("La salle de concert spécifiée n'existe pas.\n");
        return;
    }

    do {
        printf("Nombre de rangées pour la catégorie A : ");
        scanf("%d", &salles[index_salle].nombre_rangees_A);
        if (salles[index_salle].nombre_rangees_A < 0) {
            printf("Donner un nombre de rangées correct svp ! ");
        }
    } while (salles[index_salle].nombre_rangees_A < 0);
    
    do {
        printf("Nombre de rangées pour la catégorie B : ");
        scanf("%d", &salles[index_salle].nombre_rangees_B);
        if (salles[index_salle].nombre_rangees_B < 0) {
            printf("Donner un nombre de rangées correct svp : ");
        }
    } while (salles[index_salle].nombre_rangees_B < 0);
    
    do {
        printf("Combien de rangées au total : ");
        scanf("%d", &salles[index_salle].nombre_rangees_total);
        if (salles[index_salle].nombre_rangees_total < salles[index_salle].nombre_rangees_A + salles[index_salle].nombre_rangees_B) {
            printf("Le nombre total de rangées doit être supérieur ou égal à la somme des rangées de catégorie A et B.\n");
        }
    } while (salles[index_salle].nombre_rangees_total < salles[index_salle].nombre_rangees_A + salles[index_salle].nombre_rangees_B);
    viderTampon();

    printf("La salle a-t-elle une fosse ? (1 pour oui, 0 pour non) : ");
    scanf("%d", &salles[index_salle].fosse);
    viderTampon();

    if (salles[index_salle].fosse == 1) {
        for (int i = 0; i < salles[index_salle].nombre_rangees_A; i++) {
            salles[index_salle].sieges_par_rangee[i] *= 2;
        }
    }

    sauvegarderSalles(salles, nombre_salles);

    printf("Configuration de la salle %s modifiée avec succès.\n", salles[index_salle].nom);
}

void afficherEtatSalles(SalleConcert salles[], int nombre_salles) {
    if (nombre_salles == 0) {
        printf("Aucune salle de concert disponible.\n");
        return;
    }

    printf("\nSalles de concerts :\n");
    for (int i = 0; i < nombre_salles; i++) {
        printf("Nom : %s\n", salles[i].nom);
        printf("Nombre de rangées : %d\n", salles[i].nombre_rangees_total);
        if (salles[i].fosse) {
            printf("La salle a une fosse. Les festivaliers seront debout.\n");
        } else {
            printf("La salle n'a pas de fosse.\n");
        }
        if (salles[i].concert_attribue) {
            printf("Concert attribué : Oui\n");
            printf("Artiste : %s\n", salles[i].artiste);
            printf("Heure de fin du concert : %d\n", salles[i].fin_concert);
            printf("Prix catégorie A : %.2f euros\n", salles[i].prix_categorie_concert[0]);
            printf("Prix catégorie B : %.2f euros\n", salles[i].prix_categorie_concert[1]);
            printf("Prix catégorie C : %.2f euros\n", salles[i].prix_categorie_concert[2]);
        } else {
            printf("Concert attribué : Non\n");
        }

        printf("\n🪑 Plan de la salle :\n\n   🎵 SCÈNE 🎵\n");
        printf("______________________________\n");

        for (int j = 0; j < salles[i].nombre_rangees_total; j++) {
            printf("R%-2d ", j + 1);
            for (int k = 0; k < salles[i].sieges_par_rangee[j]; k++) {
                if (salles[i].fosse && j < salles[i].nombre_rangees_A) {
                    printf("F  ");
                } else if (j < salles[i].nombre_rangees_A) {
                    printf("A  ");
                } else if (j < salles[i].nombre_rangees_A + salles[i].nombre_rangees_B) {
                    printf("B  ");
                } else {
                    printf("C  ");
                }
            }
            printf("\n");
        }

        printf("\nLégende :\n");
        printf("A - Catégorie A\n");
        printf("B - Catégorie B\n");
        printf("C - Catégorie C\n");
        printf("F - Fosse\n");
        printf("X - Siège réservé\n");
        printf("\n");
    }
}

void reserverSiege(SalleConcert salles[], int nombre_salles) {
    if (nombre_salles == 0) {
        printf("Aucune salle de concert disponible.\n");
        return;
    }

    // Afficher les salles avec un concert attribué
    printf("\nSalles avec un concert attribué :\n");
    for (int i = 0; i < nombre_salles; i++) {
        if (salles[i].concert_attribue) {
            printf("%d - Concert de %s dans la salle %s\n", i + 1, salles[i].artiste, salles[i].nom);
        }
    }

    // Sélectionner la salle
    int choix_salle;
    printf("Choisissez une salle (par numéro) : ");
    scanf("%d", &choix_salle);
    viderTampon();

    if (choix_salle < 1 || choix_salle > nombre_salles || !salles[choix_salle - 1].concert_attribue) {
        printf("Choix de salle invalide.\n");
        return;
    }

    int index_salle = choix_salle - 1;

    // Afficher le plan de la salle choisie
    afficherPlanSalle(salles[index_salle]);

    int nombre_reservations;
    printf("Nombre de sièges à réserver : ");
    scanf("%d", &nombre_reservations);
    viderTampon();

    int total_prix = 0;
    for (int i = 0; i < nombre_reservations; i++) {
        int rang, siege;
        printf("Rangée (1 à %d) : ", salles[index_salle].nombre_rangees_total);
        scanf("%d", &rang);
        viderTampon();
        printf("Siège : ");
        scanf("%d", &siege);
        viderTampon();

        if (rang < 1 || rang > salles[index_salle].nombre_rangees_total || siege < 1 || siege > salles[index_salle].sieges_par_rangee[rang - 1]) {
            printf("Emplacement invalide. Réessayez.\n");
            i--;
            continue;
        }

        // Vérifiez et réservez le siège
        // ...

        if (salles[index_salle].fosse && rang <= salles[index_salle].nombre_rangees_A) {
            total_prix += salles[index_salle].prix_categorie_concert[0];
        } else if (rang <= salles[index_salle].nombre_rangees_A) {
            total_prix += salles[index_salle].prix_categorie_concert[0];
        } else if (rang <= salles[index_salle].nombre_rangees_A + salles[index_salle].nombre_rangees_B) {
            total_prix += salles[index_salle].prix_categorie_concert[1];
        } else {
            total_prix += salles[index_salle].prix_categorie_concert[2];
        }
    }

    printf("Total à payer : %d euros\n", total_prix);
}

void afficherPlanSalle(SalleConcert salle) {
    printf("\n🪑 Plan de la salle :\n\n   🎵 SCÈNE 🎵\n");
    printf("______________________________\n");

    for (int j = 0; j < salle.nombre_rangees_total; j++) {
        printf("R%-2d ", j + 1);
        for (int k = 0; k < salle.sieges_par_rangee[j]; k++) {
            if (salle.fosse && j < salle.nombre_rangees_A) {
                printf("F  ");
            } else if (j < salle.nombre_rangees_A) {
                printf("A  ");
            } else if (j < salle.nombre_rangees_A + salle.nombre_rangees_B) {
                printf("B  ");
            } else {
                printf("C  ");
            }
        }
        printf("\n");
    }

    printf("\nLégende :\n");
    printf("A - Catégorie A\n");
    printf("B - Catégorie B\n");
    printf("C - Catégorie C\n");
    printf("F - Fosse\n");
    printf("X - Siège réservé\n");
    printf("\n");
}

void modeFestivalier(SalleConcert salles[], int nombre_salles) {
    int choix;
    do {
        printf("\nMenu du Festivalier 🎉:\n");
        printf("1 - Réserver des sièges\n");
        printf("2 - Quitter\n");
        printf("Choix (choisissez entre 1 et 2) : ");
        scanf("%d", &choix);
        viderTampon();

        switch (choix) {
            case 1:
                reserverSiege(salles, nombre_salles);
                break;
            case 2:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 2);
}

void modeManager(SalleConcert salles[], int *nombre_salles) {
    int choix;
    do {
        printf("\nMenu du Manager 🤓:\n");
        printf("1 - Créer une nouvelle salle de concert\n");
        printf("2 - Attribuer un concert à une salle existante\n");
        printf("3 - Afficher l'état des salles de concerts\n");
        printf("4 - Modifier une salle déjà existante\n");
        printf("5 - Quitter\n");
        printf("Choix (choisissez entre 1 et 5) : ");
        scanf("%d", &choix);
        viderTampon();

        switch (choix) {
            case 1:
                creerSalle(salles, nombre_salles);
                break;
            case 2:
                attribuerConcert(salles, *nombre_salles);
                break;
            case 3:
                afficherEtatSalles(salles, *nombre_salles);
                break;
            case 4:
                modifierSalle(salles, *nombre_salles);
                break;
            case 5:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);
}

int main() {
    SalleConcert salles[MAX_SALLES];
    int nombre_salles = 0;

    chargerSalles(salles, &nombre_salles);

    do {
        printf("Êtes-vous un manager (saisir M/m) ou un festivalier (saisir F/f) ? (Q/q pour quitter)\n");
        char mode = getchar();
        viderTampon();

        if (mode == 'M' || mode == 'm') {
            printf("Mode manager activé.\n");
            modeManager(salles, &nombre_salles);
        } else if (mode == 'F' || mode == 'f') {
            printf("Mode festivalier activé.\n");
            modeFestivalier(salles, nombre_salles);
        } else if (mode == 'Q' || mode == 'q') {
            printf("Au revoir !\n");
            break;
        } else {
            printf("Mode invalide.\n");
        }
    } while (1);

    sauvegarderSalles(salles, nombre_salles);

    return 0;
}