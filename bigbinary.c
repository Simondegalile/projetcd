#include "bigbinary.h"
#include <string.h>

// Crée un BigBinary vide de taille donnée
BigBinary createBigBinary(int size) {
    BigBinary bb;
    bb.Tdigits = malloc(sizeof(int) * size);
    bb.Taille = size;
    bb.Signe = 0;  // Par défaut nul
    return bb;
}

// Initialise un BigBinary à zéro
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    nb.Tdigits = malloc(sizeof(int) * taille);
    for (int i = 0; i < taille; ++i) {
        nb.Tdigits[i] = 0;
    }
    return nb;
}

// Crée un BigBinary depuis une chaîne binaire (ex: "1010101")
BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    BigBinary nb;
    int n = strlen(chaine);
    nb.Taille = 0;

    // Comptons uniquement les caractères valides ('0' ou '1')
    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Taille++;
        }
    }

    nb.Tdigits = malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;
    int index = 0;
    int tousZeros = 1;

    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Tdigits[index] = chaine[i] - '0';
            if (nb.Tdigits[index] == 1) tousZeros = 0;
            index++;
        }
    }

    if (tousZeros) nb.Signe = 0;

    return nb;
}

// Affiche un BigBinary
void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }
    for (int i = 0; i < nb.Taille; ++i) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

// Libère la mémoire d'un BigBinary
void libereBigBinary(BigBinary *nb) {
    free(nb->Tdigits);
    nb->Tdigits = NULL;
    nb->Taille = 0;
    nb->Signe = 0;
}

// Retourne le nombre de digits (bits) d'un BigBinary
int getNombreDigits(BigBinary nb) {
    return nb.Taille;
}

// Teste l'égalité de deux BigBinary
bool Egal(BigBinary A, BigBinary B) {
    if (A.Signe != B.Signe) return false;
    if (A.Taille != B.Taille) return false;

    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] != B.Tdigits[i]) return false;
    }
    return true;
}

// Teste si A < B (en valeur absolue pour simplifier)
bool Inferieur(BigBinary A, BigBinary B) {
    // Comparaison en valeur absolue
    if (A.Taille < B.Taille) return true;
    if (A.Taille > B.Taille) return false;

    // Même taille, on compare bit par bit (de gauche à droite, MSB first)
    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }

    return false;  // Égaux
}

// Addition de deux BigBinary (algorithme naïf)
// Note: Assume A et B positifs ou nuls pour la Phase 1
BigBinary BigBinary_Addition(BigBinary A, BigBinary B) {
    // Détermine la taille maximale + 1 pour la retenue
    int maxTaille = (A.Taille > B.Taille) ? A.Taille : B.Taille;
    BigBinary resultat = createBigBinary(maxTaille + 1);

    int retenue = 0;
    int i = 0;

    // Parcours de droite à gauche (bits de poids faible à gauche)
    while (i < A.Taille || i < B.Taille || retenue) {
        int bitA = (i < A.Taille) ? A.Tdigits[A.Taille - 1 - i] : 0;
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;

        int somme = bitA + bitB + retenue;
        resultat.Tdigits[maxTaille - i] = somme % 2;
        retenue = somme / 2;
        i++;
    }

    // Ajuster la taille et le signe
    if (retenue > 0) {
        resultat.Tdigits[0] = retenue;
        resultat.Taille = maxTaille + 1;
    } else {
        // Décaler pour supprimer le zéro de tête
        for (int j = 0; j < maxTaille; ++j) {
            resultat.Tdigits[j] = resultat.Tdigits[j + 1];
        }
        resultat.Taille = maxTaille;
    }

    // Déterminer le signe
    int estZero = 1;
    for (int j = 0; j < resultat.Taille; ++j) {
        if (resultat.Tdigits[j] != 0) {
            estZero = 0;
            break;
        }
    }
    resultat.Signe = estZero ? 0 : +1;

    return resultat;
}

// Soustraction de deux BigBinary (A - B, avec A >= B)
BigBinary BigBinary_Soustraction(BigBinary A, BigBinary B) {
    // Assume A >= B pour simplifier (Phase 1)
    if (Inferieur(A, B)) {
        printf("Erreur: A < B dans la soustraction (non géré en Phase 1)\n");
        return initBigBinary(1, 0);
    }

    BigBinary resultat = createBigBinary(A.Taille);
    int emprunt = 0;

    // Parcours de droite à gauche
    for (int i = 0; i < A.Taille; ++i) {
        int bitA = A.Tdigits[A.Taille - 1 - i];
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;

        int diff = bitA - bitB - emprunt;

        if (diff < 0) {
            diff += 2;
            emprunt = 1;
        } else {
            emprunt = 0;
        }

        resultat.Tdigits[A.Taille - 1 - i] = diff;
    }

    // Supprimer les zéros de tête
    int nouveauDebut = 0;
    while (nouveauDebut < A.Taille - 1 && resultat.Tdigits[nouveauDebut] == 0) {
        nouveauDebut++;
    }

    int nouvelleTaille = A.Taille - nouveauDebut;
    BigBinary final = createBigBinary(nouvelleTaille);
    for (int i = 0; i < nouvelleTaille; ++i) {
        final.Tdigits[i] = resultat.Tdigits[nouveauDebut + i];
    }
    final.Taille = nouvelleTaille;

    // Déterminer le signe
    int estZero = 1;
    for (int j = 0; j < final.Taille; ++j) {
        if (final.Tdigits[j] != 0) {
            estZero = 0;
            break;
        }
    }
    final.Signe = estZero ? 0 : +1;

    libereBigBinary(&resultat);

    return final;
}
