/**
 * \file interactive.c
 * \brief Mode interactif pour tester les fonctions BigBinary
 *
 * Ce fichier contient le menu interactif qui permet de tester
 * toutes les fonctionnalités de la bibliothèque BigBinary.
 *
 * L'utilisateur peut choisir entre:
 * - Saisie binaire (format "1010")
 * - Saisie décimale (format "10")
 *
 * Le menu est organisé par phases du projet ESIEA.
 */

#include "bigbinary.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

// Taille du buffer pour lire les entrées utilisateur
#define BUFFER_SIZE 2048

/**
 * \brief Affiche le menu principal du mode interactif
 *
 * Affiche toutes les opérations disponibles organisées par phase:
 * - Phase 1: Addition, Soustraction, Comparaison
 * - Phase 2: Multiplication, PGCD, Modulo, Exponentiation, Division
 * - Phase 3: RSA (Bonus)
 */
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

/**
 * \brief Lit une chaîne binaire au clavier
 * \param buffer Buffer où stocker la chaîne lue
 * \param taille Taille maximale du buffer
 * \param message Message à afficher avant la saisie
 *
 * Utilise fgets() pour une lecture sécurisée et supprime
 * le caractère de retour à la ligne (\n) à la fin.
 */
void lire_binaire(char *buffer, int taille, const char *message) {
    printf("%s", message);

    // Lire l'entrée utilisateur avec fgets (plus sûr que scanf)
    if (fgets(buffer, taille, stdin) != NULL) {
        // Supprimer le retour à la ligne (\n) à la fin de la chaîne
        // fgets inclut le \n dans la chaîne si l'entrée est courte enough
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';  // Remplacer \n par \0 (fin de chaîne)
        }
    }
}

/**
 * \brief Lit l'exposant pour l'exponentiation modulaire
 * \return L'exposant saisi (0 si entrée invalide)
 *
 * L'exposant est limité à 64 bits car on utilise unsigned long long.
 * C'est pour la version optimisée ExpModInt().
 */
unsigned long long lire_exposant() {
    char buffer[100];
    printf("Entrez l'exposant (entier < 64 bits): ");

    // Lire et convertir l'exposant en entier non-signé
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        unsigned long long exp;
        // %llu est le format pour unsigned long long
        if (sscanf(buffer, "%llu", &exp) == 1) {
            return exp;
        }
    }
    // Retourner 0 en cas d'erreur de lecture
    return 0;
}

// ============================================================================
// ==================== FONCTIONS AJOUTÉES: ENTRÉE DÉCIMALE ====================
// ============================================================================

/**
 * \brief Demande à l'utilisateur de choisir le format d'entrée
 * \param message Message à afficher avant le choix
 * \return 'b' pour binaire, 'd' pour décimal
 *
 * Boucle jusqu'à ce que l'utilisateur entre 'b' ou 'd'.
 * Accepte les majuscules et minuscules.
 */
char choisir_format_entree(const char *message) {
    char choix;
    char buffer[10];

    // Boucle infinie qui s'arrête quand on a un choix valide
    while (1) {
        printf("%s", message);
        printf("Format (b=binaire, d=décimal): ");

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            choix = buffer[0];  // Prendre le premier caractère

            // Vérifier si c'est un choix valide
            if (choix == 'b' || choix == 'B') {
                return 'b';
            } else if (choix == 'd' || choix == 'D') {
                return 'd';
            }
        }

        // Message d'erreur si choix invalide
        printf("Choix invalide. Entrez 'b' pour binaire ou 'd' pour décimal.\n");
    }
}

/**
 * \brief Lit un nombre dans le format spécifié
 * \param message Message d'invite
 * \param format 'b' pour binaire, 'd' pour décimal
 * \return Le nombre lu sous forme de BigBinary
 *
 * Si format = 'd': lit un entier long long et utilise creerBigBinaryDepuisDecimal()
 * Si format = 'b': lit une chaîne binaire et utilise creerBigBinaryDepuisChaine()
 *
 * En cas d'erreur de lecture, retourne zéro.
 */
BigBinary lire_nombre(const char *message, char format) {
    char buffer[BUFFER_SIZE];

    printf("%s", message);

    if (format == 'd') {
        // === ENTRÉE DÉCIMALE ===
        printf("(décimal): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            long long valeur;
            // %lld est le format pour long long
            if (sscanf(buffer, "%lld", &valeur) == 1) {
                // Conversion décimal -> BigBinary avec notre nouvelle fonction
                return creerBigBinaryDepuisDecimal(valeur);
            }
        }
        // Erreur: utiliser 0 par défaut
        printf("Entrée invalide. Utilisation de 0 par défaut.\n");
        return creerZero();
    } else {
        // === ENTRÉE BINAIRE ===
        printf("(binaire): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Supprimer le retour à la ligne
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n') {
                buffer[len-1] = '\0';
            }
            // Conversion chaîne binaire -> BigBinary
            return creerBigBinaryDepuisChaine(buffer);
        }
        // Erreur: utiliser 0 par défaut
        printf("Entrée invalide. Utilisation de 0 par défaut.\n");
        return creerZero();
    }
}

// ============================================================================
// ======================== PHASE 1: TESTS DE BASE ===========================
// ============================================================================

/**
 * \brief Teste l'addition de deux BigBinary
 *
 * Demande à l'utilisateur le format d'entrée (binaire ou décimal),
 * lit les deux nombres, effectue l'addition et affiche le résultat.
 *
 * === Phase 1, §5.1: Addition ===
 *
 * L'algorithme naïf de l'école primaire:
 * - On additionne bit par bit de droite à gauche
 * - On propage la retenue (0 ou 1 en binaire)
 */
void test_addition() {
    char format;

    printf("\n=== ADDITION ===\n");

    // Demander le format d'entrée
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    // Lire les deux nombres
    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B", format);

    // Afficher les nombres saisis
    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    // Effectuer l'addition
    BigBinary resultat = Addition(A, B);
    printf("\nA + B = ");
    afficheBigBinary(resultat);

    // Libérer la mémoire (important pour éviter les fuites!)
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&resultat);
}

/**
 * \brief Teste la soustraction de deux BigBinary (A >= B requis)
 *
 * === Phase 1, §5.1: Soustraction ===
 *
 * L'algorithme de l'école primaire avec emprunt (borrow):
 * - On soustrait bit par bit de droite à gauche
 * - Si le bit de A est plus petit, on "emprunte" 1 au bit suivant
 *
 * Note: La soustraction donne un résultat négatif si A < B,
 * mais notre implémentation ne supporte pas les nombres négatifs.
 */
void test_soustraction() {
    char format;

    printf("\n=== SOUSTRACTION ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B", format);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    // Vérifier que A >= B (notre fonction ne supporte pas A < B)
    if (Inferieur(A, B)) {
        printf("\nERREUR: A < B. La soustraction nécessite A >= B.\n");
    } else {
        // Effectuer la soustraction
        BigBinary resultat = Soustraction(A, B);
        printf("\nA - B = ");
        afficheBigBinary(resultat);
        libereBigBinary(&resultat);
    }

    libereBigBinary(&A);
    libereBigBinary(&B);
}

// ============================================================================
// ======================= PHASE 2: TESTS AVANCÉS ============================
// ============================================================================

/**
 * \brief Teste la multiplication égyptienne
 *
 * === Phase 2, §4: Multiplication Égyptienne ===
 *
 * Algorithme ancien basé sur:
 * - Le parcours des bits de B du LSB vers le MSB
 * - Si le bit est 1, ajouter A au résultat
 * - Doubler A à chaque étape
 *
 * Exemple: 13 × 11
 * Bits de 11: 1, 1, 0, 1 (du LSB au MSB)
 * Étapes: 13 + 26 + 52 + 104 = 143 ✓
 */
void test_multiplication() {
    char format;

    printf("\n=== MULTIPLICATION ÉGYPTIENNE ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B", format);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    // Effectuer la multiplication égyptienne
    BigBinary resultat = MultiplicationEgyptienne(A, B);
    printf("\nA × B = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&resultat);
}

/**
 * \brief Teste le calcul du PGCD par l'algorithme binaire d'Euclide
 *
 * === Phase 1, §3.4-§3.6: Algorithme Binaire d'Euclide ===
 *
    Principes utilisés:
    1. PGCD(a, 0) = a
    2. Si a et b sont pairs: PGCD(a, b) = 2 × PGCD(a/2, b/2)
    3. Si a pair, b impair: PGCD(a, b) = PGCD(a/2, b)
    4. Si a impair, b pair: PGCD(a, b) = PGCD(a, b/2)
    5. Si les deux impairs: PGCD(a, b) = PGCD(a-b, b)

    Avantages:
    - Pas de division euclidienne!
    - Juste des additions, soustractions et décalages
    - Parfait pour les processeurs de faible puissance (cartes à puce)
 */
void test_pgcd() {
    char format;

    printf("\n=== PGCD (Algorithme Binaire d'Euclide) ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B", format);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("B = ");
    afficheBigBinary(B);

    // Calculer le PGCD
    BigBinary resultat = PGCD(A, B);
    printf("\nPGCD(A, B) = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&resultat);
}

/**
 * \brief Teste le calcul du modulo (division-free)
 *
 * === Phase 2, §2: Modulo sans division ===
 *
    Algorithme de l'antiphérèse (comme Euclide à l'origine):
    - Au lieu de diviser, on soustrait des multiples de B
    - On trouve la plus grande puissance de 2^k × B <= A
    - On soustrait et on répète jusqu'à A < B

    Exemple: 192 mod 33 = 27
    192 - 4×33 = 192 - 132 = 60
    60 - 1×33 = 60 - 33 = 27
    27 < 33, donc le reste est 27 ✓
 */
void test_modulo() {
    char format;

    printf("\n=== MODULO (Division-Free) ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B (B != 0)", format);

    // Vérifier que B n'est pas nul (division par zéro!)
    if (estNul(B)) {
        printf("\nERREUR: B ne peut pas être 0!\n");
    } else {
        printf("\nA = ");
        afficheBigBinary(A);
        printf("B = ");
        afficheBigBinary(B);

        // Calculer le modulo
        BigBinary resultat = Modulo(A, B);
        printf("\nA mod B = ");
        afficheBigBinary(resultat);

        libereBigBinary(&resultat);
    }

    libereBigBinary(&A);
    libereBigBinary(&B);
}

/**
 * \brief Teste l'exponentiation modulaire rapide
 *
 * === Phase 2, §3: Exponentiation Rapide (Binary Exponentiation) ===
 *
    Idée: Décomposer l'exposant en binaire et réutiliser les puissances.
    Pour calculer M^e mod n:
    1. Réduire M modulo n d'abord
    2. Parcourir les bits de l'exposant
    3. À chaque bit 1: multiplier le résultat par la base
    4. Mettre la base au carré à chaque étape
    5. Réduire modulo n à chaque opération

    Complexité: O(log e) multiplications au lieu de O(e)
 */
void test_exponentiation() {
    char format;
    unsigned long long exp;

    printf("\n=== EXPONENTIATION MODULAIRE ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et n\n");

    // Lire la base (A) et le module (n) selon le format choisi
    BigBinary A = lire_nombre("Entrez A", format);
    // L'exposant est toujours lu en décimal (limité à 64 bits)
    exp = lire_exposant();
    BigBinary n = lire_nombre("Entrez n (modulo)", format);

    printf("\nA = ");
    afficheBigBinary(A);
    printf("exposant = %llu\n", exp);
    printf("n = ");
    afficheBigBinary(n);

    // Calculer A^exp mod n avec la version optimisée
    BigBinary resultat = ExpModInt(A, (unsigned int)exp, n);
    printf("\nA^exposant mod n = ");
    afficheBigBinary(resultat);

    libereBigBinary(&A);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

// ============================================================================
// ========================== PHASE 3: TESTS RSA =============================
// ============================================================================

/**
 * \brief Teste le chiffrement RSA
 *
 * === Phase 2, §5: RSA - Chiffrement ===
 *
    Le chiffrement RSA:
    C = M^e mod N

    où:
    - M est le message (converti en nombre)
    - e est l'exposant public (doit être premier avec φ(N))
    - N = p × q est le module (produit de deux nombres premiers)

    La sécurité repose sur la difficulté de factoriser N.
 */
void test_rsa_chiffrement() {
    char format;

    printf("\n=== RSA CHIFFREMENT ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour M, e et n\n");

    // Lire le message, l'exposant public et le module
    BigBinary M = lire_nombre("Entrez M (message)", format);
    BigBinary e = lire_nombre("Entrez e (exposant public)", format);
    BigBinary n = lire_nombre("Entrez n (modulus)", format);

    printf("\nM = ");
    afficheBigBinary(M);
    printf("e = ");
    afficheBigBinary(e);
    printf("n = ");
    afficheBigBinary(n);

    // Chiffrer: C = M^e mod n
    BigBinary resultat = RSA_encrypt(M, e, n);
    printf("\nC = M^e mod n = ");
    afficheBigBinary(resultat);

    libereBigBinary(&M);
    libereBigBinary(&e);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

/**
 * \brief Teste le déchiffrement RSA
 *
 * === Phase 2, §5: RSA - Déchiffrement ===
 *
    Le déchiffrement RSA:
    M = C^d mod N

    où d est l'exposant privé, l'inverse de e modulo φ(N).
    On a: e × d ≡ 1 (mod φ(N))

    Donc: (M^e)^d = M^(e×d) = M^(1 + k×φ(N)) = M × (M^φ(N))^k ≡ M × 1^k ≡ M (mod N)
 */
void test_rsa_dechiffrement() {
    char format;

    printf("\n=== RSA DÉCHIFFREMENT ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour C, d et n\n");

    // Lire le message chiffré, l'exposant privé et le module
    BigBinary C = lire_nombre("Entrez C (message chiffré)", format);
    BigBinary d = lire_nombre("Entrez d (exposant privé)", format);
    BigBinary n = lire_nombre("Entrez n (modulus)", format);

    printf("\nC = ");
    afficheBigBinary(C);
    printf("d = ");
    afficheBigBinary(d);
    printf("n = ");
    afficheBigBinary(n);

    // Déchiffrer: M = C^d mod n
    BigBinary resultat = RSA_decrypt(C, d, n);
    printf("\nM = C^d mod n = ");
    afficheBigBinary(resultat);

    libereBigBinary(&C);
    libereBigBinary(&d);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

/**
 * \brief Teste les fonctions de comparaison
 *
 * === Phase 1, §5.1: Fonctions de comparaison ===
 *
    Trois comparaisons disponibles:
    - Egal(A, B): true si A == B
    - Inferieur(A, B): true si A < B
    - Superieur(A, B): true si A > B

    L'algorithme compare d'abord le nombre de bits,
    puis les bits un par un du MSB vers le LSB.
 */
void test_comparaison() {
    char format;

    printf("\n=== COMPARAISON ===\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B", format);

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

/**
 * \brief Teste la Division Euclidienne
 *
 * === Phase 2, §2: Division Euclidienne (sans division) ===
 *
    Calcule quotient Q et reste R tels que:
    A = Q × B + R avec 0 ≤ R < B

    Même principe que le Modulo, mais on accumule
    les puissances de 2 dans le quotient.

    Exemple: 23 ÷ 5
    Q = 4, R = 3 car 23 = 4×5 + 3

    Étapes:
    1. Trouver k tel que 2^k × B ≤ A
    2. Soustraire 2^k × B de A
    3. Ajouter 2^k au quotient
    4. Répéter jusqu'à R < B
 */
void test_division_euclidienne() {
    char format;

    printf("\n=== DIVISION EUCLIDIENNE ===\n");
    printf("Calcule quotient et reste tels que A = quotient × B + reste\n\n");
    format = choisir_format_entree("Choisissez le format d'entrée pour A et B\n");

    BigBinary A = lire_nombre("Entrez A", format);
    BigBinary B = lire_nombre("Entrez B (B != 0)", format);

    // Vérifier division par zéro
    if (estNul(B)) {
        printf("\nERREUR: Division par zéro!\n");
    } else {
        printf("\nA = ");
        afficheBigBinary(A);
        printf("B = ");
        afficheBigBinary(B);

        // Calculer division euclidienne
        DivisionResult res = DivisionEuclidienne(A, B);

        printf("\nRésultat:\n");
        printf("Quotient = ");
        afficheBigBinary(res.quotient);
        printf("Reste    = ");
        afficheBigBinary(res.reste);

        // === VÉRIFICATION ===
        // On vérifie que: quotient × B + reste = A
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

/**
 * \brief Affiche des exemples de tests prédéfinis
 *
    Ces exemples correspondent aux tests du cahier des charges.
    Ils vérifient que nos fonctions donnent les bons résultats.
 */
void tester_avec_exemples() {
    printf("\n=== TESTS PRÉDÉFINIS ===\n");

    // Test 1: Addition - 10 + 6 = 16
    printf("\n1. Addition: 1010 + 110 = 10000 (10 + 6 = 16)\n");
    BigBinary a1 = creerBigBinaryDepuisChaine("1010");  // 10
    BigBinary b1 = creerBigBinaryDepuisChaine("110");   // 6
    BigBinary r1 = Addition(a1, b1);
    printf("Résultat: ");
    afficheBigBinary(r1);  // Devrait être 10000 (16)
    libereBigBinary(&a1); libereBigBinary(&b1); libereBigBinary(&r1);

    // Test 2: PGCD - PGCD(51, 57) = 3
    printf("\n2. PGCD: PGCD(110011, 111001) = 11 (51, 57) = 3\n");
    BigBinary a2 = creerBigBinaryDepuisChaine("110011");  // 51
    BigBinary b2 = creerBigBinaryDepuisChaine("111001"); // 57
    BigBinary r2 = PGCD(a2, b2);
    printf("Résultat: ");
    afficheBigBinary(r2);  // Devrait être 11 (3)
    libereBigBinary(&a2); libereBigBinary(&b2); libereBigBinary(&r2);

    // Test 3: Modulo - 23 mod 5 = 3
    printf("\n3. Modulo: 10111 mod 101 = 11 (23 mod 5 = 3)\n");
    BigBinary a3 = creerBigBinaryDepuisChaine("10111");  // 23
    BigBinary b3 = creerBigBinaryDepuisChaine("101");    // 5
    BigBinary r3 = Modulo(a3, b3);
    printf("Résultat: ");
    afficheBigBinary(r3);  // Devrait être 11 (3)
    libereBigBinary(&a3); libereBigBinary(&b3); libereBigBinary(&r3);

    // Test 4: Multiplication - 13 × 11 = 143
    printf("\n4. Multiplication: 1101 × 1011 = 10001111 (13 × 11 = 143)\n");
    BigBinary a4 = creerBigBinaryDepuisChaine("1101");   // 13
    BigBinary b4 = creerBigBinaryDepuisChaine("1011");   // 11
    BigBinary r4 = MultiplicationEgyptienne(a4, b4);
    printf("Résultat: ");
    afficheBigBinary(r4);  // Devrait être 10001111 (143)
    libereBigBinary(&a4); libereBigBinary(&b4); libereBigBinary(&r4);
}

// ============================================================================
// =========================== BOUCLE PRINCIPALE ==============================
// ============================================================================

/**
 * \brief Point d'entrée du mode interactif
 *
    Boucle principale qui:
    1. Affiche le menu
    2. Lit le choix de l'utilisateur
    3. Appelle la fonction de test correspondante
    4. Attend que l'utilisateur appuie sur Entrée
    5. Répète jusqu'à ce que l'utilisateur choisisse 0 (quitter)
 */
int main() {
    int choix;
    char buffer[10];

    // === NETTOYER stdin ===
    // setbuf(stdin, NULL) désactive le buffering de stdin
    // Cela évite des problèmes avec getchar() après fgets()
    setbuf(stdin, NULL);

    // === BOUCLE PRINCIPALE ===
    // Tant que choix != 0, on continue
    do {
        // Afficher le menu
        affiche_menu();

        // Lire le choix de l'utilisateur
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Convertir la chaîne en entier
            if (sscanf(buffer, "%d", &choix) != 1) {
                choix = -1;  // Valeur invalide si scanf échoue
            }
        } else {
            choix = 0;  // Quitter si erreur de lecture
        }

        // === INTERRUPTEUR (SWITCH) ===
        // Appeler la fonction correspondant au choix
        switch (choix) {
            case 1:  // Addition
                test_addition();
                break;
            case 2:  // Soustraction
                test_soustraction();
                break;
            case 3:  // Multiplication
                test_multiplication();
                break;
            case 4:  // PGCD
                test_pgcd();
                break;
            case 5:  // Modulo
                test_modulo();
                break;
            case 6:  // Exponentiation
                test_exponentiation();
                break;
            case 7:  // RSA Chiffrement
                test_rsa_chiffrement();
                break;
            case 8:  // RSA Déchiffrement
                test_rsa_dechiffrement();
                break;
            case 9:  // Comparaison
                test_comparaison();
                break;
            case 10: // Division Euclidienne
                test_division_euclidienne();
                break;
            case 11: // Tests prédéfinis
                tester_avec_exemples();
                break;
            case 0:  // Quitter
                printf("\nAu revoir !\n");
                break;
            default: // Choix invalide
                printf("\nChoix invalide. Veuillez réessayer.\n");
                break;
        }

        // === ATTENTE AVANT DE CONTINUER ===
        // Si l'utilisateur n'a pas choisi de quitter,
        // attendre qu'il appuie sur Entrée
        if (choix != 0) {
            printf("\nAppuyez sur Entrée pour continuer...");
            // Vider le buffer stdin (en cas de caractères en trop)
            while (getchar() != '\n');
            // Attendre la touche Entrée
            getchar();
        }

    } while (choix != 0);  // Répéter jusqu'à choix == 0

    return 0;
}
