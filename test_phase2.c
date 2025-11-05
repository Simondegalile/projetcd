#include "bigbinary.h"

void afficher_test_separateur(const char* nom_test) {
    printf("\n");
    printf("========================================\n");
    printf("  %s\n", nom_test);
    printf("========================================\n");
}

void tester_multiplication_egyptienne() {
    afficher_test_separateur("TEST: MULTIPLICATION ÉGYPTIENNE");
    
    // Test 1: 13 × 11 = 143
    printf("\nTest 1: 13 × 11\n");
    BigBinary a1 = creerBigBinaryDepuisChaine("1101");  // 13 en décimal
    BigBinary b1 = creerBigBinaryDepuisChaine("1011");  // 11 en décimal
    
    printf("A = ");
    afficheBigBinary(a1);
    printf("   (13 en decimal)\n");
    printf("B = ");
    afficheBigBinary(b1);
    printf("   (11 en decimal)\n");
    
    BigBinary produit1 = MultiplicationEgyptienne(a1, b1);
    printf("A × B = ");
    afficheBigBinary(produit1);
    printf("   (143 en decimal)\n");
    
    libereBigBinary(&a1);
    libereBigBinary(&b1);
    libereBigBinary(&produit1);
    
    // Test 2: 7 × 5 = 35
    printf("\nTest 2: 7 × 5\n");
    BigBinary a2 = creerBigBinaryDepuisChaine("111");   // 7 en décimal
    BigBinary b2 = creerBigBinaryDepuisChaine("101");   // 5 en décimal
    
    printf("A = ");
    afficheBigBinary(a2);
    printf("   (7 en decimal)\n");
    printf("B = ");
    afficheBigBinary(b2);
    printf("   (5 en decimal)\n");
    
    BigBinary produit2 = MultiplicationEgyptienne(a2, b2);
    printf("A × B = ");
    afficheBigBinary(produit2);
    printf("   (35 en decimal)\n");
    
    libereBigBinary(&a2);
    libereBigBinary(&b2);
    libereBigBinary(&produit2);
}

void tester_modulo_division_free() {
    afficher_test_separateur("TEST: MODULO (DIVISION-FREE)");
    
    // Test avec l'exemple du document : 192 mod 33 = 27
    printf("\nTest: 192 mod 33 = 27\n");
    BigBinary a = creerBigBinaryDepuisChaine("11000000");  // 192 en décimal
    BigBinary b = creerBigBinaryDepuisChaine("100001");    // 33 en décimal
    
    printf("A = ");
    afficheBigBinary(a);
    printf("   (192 en decimal)\n");
    printf("B = ");
    afficheBigBinary(b);
    printf("   (33 en decimal)\n");
    
    BigBinary mod = Modulo(a, b);
    printf("A mod B = ");
    afficheBigBinary(mod);
    printf("   (27 en decimal)\n");
    
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&mod);
    
    // Test supplémentaire: 23 mod 5 = 3
    printf("\nTest: 23 mod 5 = 3\n");
    BigBinary a2 = creerBigBinaryDepuisChaine("10111");  // 23 en décimal
    BigBinary b2 = creerBigBinaryDepuisChaine("101");    // 5 en décimal
    
    printf("A = ");
    afficheBigBinary(a2);
    printf("   (23 en decimal)\n");
    printf("B = ");
    afficheBigBinary(b2);
    printf("   (5 en decimal)\n");
    
    BigBinary mod2 = Modulo(a2, b2);
    printf("A mod B = ");
    afficheBigBinary(mod2);
    printf("   (3 en decimal)\n");
    
    libereBigBinary(&a2);
    libereBigBinary(&b2);
    libereBigBinary(&mod2);
}

void tester_exponentiation_document() {
    afficher_test_separateur("TEST: EXPONENTIATION MODULAIRE (DONNÉES DU DOCUMENT)");
    
    printf("\nTest: A^17 mod N = 2\n");
    printf("Ce test utilise les nombres fournis dans le document\n\n");
    
    // N = (10000000000000000000000000000000000000000001111111010110100101011011011001011001010111010)₂
    BigBinary N = creerBigBinaryDepuisChaine(
        "10000000000000000000000000000000000000000001111111010110100101011011011001011001010111010"
    );
    printf("N = ");
    afficheBigBinary(N);
    printf("   (modulus RSA)\n");
    
    // A = (10100011000010111111011001110010101101100100111110001101000001001111000000110000111010111)₂
    BigBinary A = creerBigBinaryDepuisChaine(
        "10100011000010111111011001110010101101100100111110001101000001001111000000110000111010111"
    );
    printf("\nA = ");
    afficheBigBinary(A);
    printf("   (base)\n");
    
    // e = 17
    BigBinary e = creerBigBinaryDepuisChaine("10001");  // 17 en binaire
    printf("\ne = ");
    afficheBigBinary(e);
    printf("   (17 en decimal)\n");
    
    printf("\nCalcul de A^17 mod N...\n");
    printf("(Ce calcul peut prendre quelques secondes)\n");
    
    BigBinary resultat = ExpMod(A, e, N);
    
    printf("\nRésultat: A^17 mod N = ");
    afficheBigBinary(resultat);
    printf("   (devrait être 10 = 2 en decimal)\n");
    
    // Vérifier si le résultat est 2
    BigBinary deux = creerBigBinaryDepuisChaine("10");  // 2 en binaire
    if (Egal(resultat, deux)) {
        printf("\n✓ TEST RÉUSSI! Le résultat est bien 2\n");
    } else {
        printf("\n✗ TEST ÉCHOUÉ: Le résultat n'est pas 2\n");
    }
    
    libereBigBinary(&N);
    libereBigBinary(&A);
    libereBigBinary(&e);
    libereBigBinary(&resultat);
    libereBigBinary(&deux);
}

void tester_rsa_exemple_jouet() {
    afficher_test_separateur("TEST: RSA EXEMPLE JOUET (p=1009, q=1201)");
    
    printf("\nParamètres RSA:\n");
    printf("  p = 1009\n");
    printf("  q = 1201\n");
    printf("  N = p × q = 1211809\n");
    printf("  φ(N) = (p-1)(q-1) = 1209600\n");
    printf("  E = 101 (exposant public)\n");
    printf("  D = 251501 (exposant privé)\n");
    
    // N = 1211809 = (100100111101111100001)₂
    BigBinary N = creerBigBinaryDepuisChaine("100100111101111100001");
    printf("\nN = ");
    afficheBigBinary(N);
    printf("   (1211809 en decimal)\n");
    
    // Message M = 99999 = (11000011010011111)₂
    BigBinary M = creerBigBinaryDepuisChaine("11000011010011111");
    printf("\nMessage original M = ");
    afficheBigBinary(M);
    printf("   (99999 en decimal)\n");
    
    // E = 101 = (1100101)₂
    BigBinary E = creerBigBinaryDepuisChaine("1100101");
    printf("\nExposant public E = ");
    afficheBigBinary(E);
    printf("   (101 en decimal)\n");
    
    // Chiffrement: C = M^E mod N
    printf("\n--- CHIFFREMENT ---\n");
    printf("Calcul de C = M^E mod N...\n");
    BigBinary C = RSA_encrypt(M, E, N);
    printf("Message chiffré C = ");
    afficheBigBinary(C);
    printf("   (561752 en decimal attendu)\n");
    
    // D = 251501 = (111101011001001101)₂
    BigBinary D = creerBigBinaryDepuisChaine("111101011001001101");
    printf("\n--- DÉCHIFFREMENT ---\n");
    printf("Exposant privé D = ");
    afficheBigBinary(D);
    printf("   (251501 en decimal)\n");
    
    printf("Calcul de M' = C^D mod N...\n");
    printf("(Ce calcul peut prendre plusieurs secondes)\n");
    BigBinary M_dechiffre = RSA_decrypt(C, D, N);
    printf("Message déchiffré M' = ");
    afficheBigBinary(M_dechiffre);
    
    // Vérification
    if (Egal(M, M_dechiffre)) {
        printf("\n✓ RSA FONCTIONNE! M = M'\n");
    } else {
        printf("\n✗ ERREUR: M ≠ M'\n");
    }
    
    libereBigBinary(&N);
    libereBigBinary(&M);
    libereBigBinary(&E);
    libereBigBinary(&C);
    libereBigBinary(&D);
    libereBigBinary(&M_dechiffre);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     TESTS PHASE 2 - MULTIPLICATION ET MODULO              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    tester_multiplication_egyptienne();
    tester_modulo_division_free();
    tester_exponentiation_document();
    tester_rsa_exemple_jouet();
    
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     FIN DES TESTS PHASE 2                                  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}

