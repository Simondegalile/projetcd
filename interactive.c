#include "bigbinary.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 2048

void affiche_menu() {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║  PROJET BIGBINARY - MODE INTERACTIF                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\nChoisissez une opération:\n");
    printf("  1. Addition (A + B)\n");
    printf("  2. Soustraction (A - B, avec A >= B)\n");
    printf("  3. Multiplication Égyptienne (A × B)\n");
    printf("  4. PGCD (PGCD(A, B))\n");
    printf("  5. Modulo (A mod B)\n");
    printf("  6. Exponentiation Modulaire (A^exp mod n)\n");
    printf("  7. RSA Chiffrement (M^e mod n)\n");
    printf("  8. RSA Déchiffrement (C^d mod n)\n");
    printf("  9. Comparer (A < B, A == B)\n");
    printf("  10. Division Euclidienne (A ÷ B → quotient et reste)\n");
    printf("  0. Quitter\n");
    printf("\nVotre choix: ");
}

void lire_binaire(char *buffer, int taille, const char *message) {
    printf("%s", message);
    if (fgets(buffer, taille, stdin) != NULL) {
        // Supprimer le retour à la ligne
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

unsigned long long lire_exposant() {
    char buffer[100];
    printf("Entrez l'exposant (entier < 64 bits): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        unsigned long long exp;
        if (sscanf(buffer, "%llu", &exp) == 1) {
            return exp;
        }
    }
    return 0;
}

void test_addition() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== ADDITION ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    BigBinary resultat = Addition(A, B);
    printf("\nA + B = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&resultat);
}

void test_soustraction() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== SOUSTRACTION ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    if (Inferieur(A, B)) {
        printf("\nERREUR: A < B. La soustraction nécessite A >= B.\n");
    } else {
        BigBinary resultat = Soustraction(A, B);
        printf("\nA - B = ");
        afficheBigBinary(resultat);
        libereBigBinary(&resultat);
    }

    libereBigBinary(&A);
    libereBigBinary(&B);
}

void test_multiplication() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== MULTIPLICATION ÉGYPTIENNE ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    BigBinary resultat = MultiplicationEgyptienne(A, B);
    printf("\nA × B = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&resultat);
}

void test_pgcd() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== PGCD (Algorithme Binaire d'Euclide) ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    BigBinary resultat = PGCD(A, B);
    printf("\nPGCD(A, B) = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&resultat);
}

void test_modulo() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== MODULO (Division-Free) ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire, B != 0): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    if (estNul(B)) {
        printf("\nERREUR: B ne peut pas être 0!\n");
    } else {
        printf("\nA = ");
        afficheBigBinary(A);
        printf("B = ");
        afficheBigBinary(B);

        BigBinary resultat = Modulo(A, B);
        printf("\nA mod B = ");
        afficheBigBinary(resultat);

        libereBigBinary(&resultat);
    }

    libereBigBinary(&A);
    libereBigBinary(&B);
}

void test_exponentiation() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];
    unsigned long long exp;

    printf("\n=== EXPONENTIATION MODULAIRE ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    exp = lire_exposant();
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez n (modulo, binaire): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary n = creerBigBinaryDepuisChaine(buffer2);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("exposant = %llu\n", exp);
    printf("n = ");
    afficheBigBinary(n);

    BigBinary resultat = ExpModInt(A, (unsigned int)exp, n);
    printf("\nA^exposant mod n = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

void test_rsa_chiffrement() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE], buffer3[BUFFER_SIZE];

    printf("\n=== RSA CHIFFREMENT ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez M (message, binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez e (exposant public, binaire): ");
    lire_binaire(buffer3, BUFFER_SIZE, "Entrez n (modulus, binaire): ");

    BigBinary M = creerBigBinaryDepuisChaine(buffer1);
    BigBinary e = creerBigBinaryDepuisChaine(buffer2);
    BigBinary n = creerBigBinaryDepuisChaine(buffer3);

    printf("\nM = ");
    afficheBigBinary(M);
    printf("e = ");
    afficheBigBinary(e);
    printf("n = ");
    afficheBigBinary(n);

    BigBinary resultat = RSA_encrypt(M, e, n);
    printf("\nC = M^e mod n = ");
    afficheBigBinary(resultat);

    libereBigBinary(&M);
    libereBigBinary(&e);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

void test_rsa_dechiffrement() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE], buffer3[BUFFER_SIZE];

    printf("\n=== RSA DÉCHIFFREMENT ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez C (message chiffré, binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez d (exposant privé, binaire): ");
    lire_binaire(buffer3, BUFFER_SIZE, "Entrez n (modulus, binaire): ");

    BigBinary C = creerBigBinaryDepuisChaine(buffer1);
    BigBinary d = creerBigBinaryDepuisChaine(buffer2);
    BigBinary n = creerBigBinaryDepuisChaine(buffer3);

    printf("\nC = ");
    afficheBigBinary(C);
    printf("d = ");
    afficheBigBinary(d);
    printf("n = ");
    afficheBigBinary(n);

    BigBinary resultat = RSA_decrypt(C, d, n);
    printf("\nM = C^d mod n = ");
    afficheBigBinary(resultat);

    libereBigBinary(&C);
    libereBigBinary(&d);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

void test_comparaison() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== COMPARAISON ===\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    printf("\nRésultats:\n");
    printf("A == B ? %s\n", Egal(A, B) ? "OUI" : "NON");
    printf("A < B ? %s\n", Inferieur(A, B) ? "OUI" : "NON");
    printf("A > B ? %s\n", Superieur(A, B) ? "OUI" : "NON");

    libereBigBinary(&A);
    libereBigBinary(&B);
}

void test_division_euclidienne() {
    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE];

    printf("\n=== DIVISION EUCLIDIENNE ===\n");
    printf("Calcule quotient et reste tels que A = quotient × B + reste\n\n");
    lire_binaire(buffer1, BUFFER_SIZE, "Entrez A (binaire): ");
    lire_binaire(buffer2, BUFFER_SIZE, "Entrez B (binaire, B != 0): ");

    BigBinary A = creerBigBinaryDepuisChaine(buffer1);
    BigBinary B = creerBigBinaryDepuisChaine(buffer2);

    if (estNul(B)) {
        printf("\nERREUR: Division par zéro!\n");
    } else {
        printf("\nA = ");
        afficheBigBinary(A);
        printf("B = ");
        afficheBigBinary(B);

        DivisionResult res = DivisionEuclidienne(A, B);

        printf("\nRésultat:\n");
        printf("Quotient = ");
        afficheBigBinary(res.quotient);
        printf("Reste    = ");
        afficheBigBinary(res.reste);

        // Vérification: A = quotient * B + reste
        printf("\nVérification (quotient × B + reste):\n");
        BigBinary produit = MultiplicationEgyptienne(res.quotient, B);
        BigBinary verification = Addition(produit, res.reste);
        printf("quotient × B + reste = ");
        afficheBigBinary(verification);
        printf("A original           = ");
        afficheBigBinary(A);
        printf("Égalité: %s\n", Egal(A, verification) ? "OUI ✓" : "NON ✗");

        libereBigBinary(&produit);
        libereBigBinary(&verification);
        libereDivisionResult(&res);
    }

    libereBigBinary(&A);
    libereBigBinary(&B);
}

void tester_avec_exemples() {
    printf("\n=== TESTS PRÉDÉFINIS ===\n");
    printf("\n1. Addition: 1010 + 110 = 10000 (10 + 6 = 16)\n");
    BigBinary a1 = creerBigBinaryDepuisChaine("1010");
    BigBinary b1 = creerBigBinaryDepuisChaine("110");
    BigBinary r1 = Addition(a1, b1);
    printf("Résultat: ");
    afficheBigBinary(r1);
    libereBigBinary(&a1); libereBigBinary(&b1); libereBigBinary(&r1);

    printf("\n2. PGCD: PGCD(110011, 111001) = 11 (51, 57) = 3\n");
    BigBinary a2 = creerBigBinaryDepuisChaine("110011");
    BigBinary b2 = creerBigBinaryDepuisChaine("111001");
    BigBinary r2 = PGCD(a2, b2);
    printf("Résultat: ");
    afficheBigBinary(r2);
    libereBigBinary(&a2); libereBigBinary(&b2); libereBigBinary(&r2);

    printf("\n3. Modulo: 10111 mod 101 = 11 (23 mod 5 = 3)\n");
    BigBinary a3 = creerBigBinaryDepuisChaine("10111");
    BigBinary b3 = creerBigBinaryDepuisChaine("101");
    BigBinary r3 = Modulo(a3, b3);
    printf("Résultat: ");
    afficheBigBinary(r3);
    libereBigBinary(&a3); libereBigBinary(&b3); libereBigBinary(&r3);

    printf("\n4. Multiplication: 1101 × 1011 = 10001111 (13 × 11 = 143)\n");
    BigBinary a4 = creerBigBinaryDepuisChaine("1101");
    BigBinary b4 = creerBigBinaryDepuisChaine("1011");
    BigBinary r4 = MultiplicationEgyptienne(a4, b4);
    printf("Résultat: ");
    afficheBigBinary(r4);
    libereBigBinary(&a4); libereBigBinary(&b4); libereBigBinary(&r4);
}

int main() {
    int choix;
    char buffer[10];

    // Pour s'assurer que stdin est propre
    setbuf(stdin, NULL);

    do {
        affiche_menu();

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &choix) != 1) {
                choix = -1;
            }
        } else {
            choix = 0;
        }

        switch (choix) {
            case 1:
                test_addition();
                break;
            case 2:
                test_soustraction();
                break;
            case 3:
                test_multiplication();
                break;
            case 4:
                test_pgcd();
                break;
            case 5:
                test_modulo();
                break;
            case 6:
                test_exponentiation();
                break;
            case 7:
                test_rsa_chiffrement();
                break;
            case 8:
                test_rsa_dechiffrement();
                break;
            case 9:
                test_comparaison();
                break;
            case 10:
                test_division_euclidienne();
                break;
            case 11:
                tester_avec_exemples();
                break;
            case 0:
                printf("\nAu revoir !\n");
                break;
            default:
                printf("\nChoix invalide. Veuillez réessayer.\n");
                break;
        }

        if (choix != 0) {
            printf("\nAppuyez sur Entrée pour continuer...");
            while (getchar() != '\n');
            getchar();
        }

    } while (choix != 0);

    return 0;
}
