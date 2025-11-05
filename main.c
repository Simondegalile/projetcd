#include "bigbinary.h"

void afficher_separateur() {
    printf("\n========================================\n");
}

void tester_phase1() {
    printf("\n=== TESTS PHASE 1: OPERATIONS DE BASE ===\n");
    
    // Test d'addition
    afficher_separateur();
    printf("Test Addition:\n");
    BigBinary a1 = creerBigBinaryDepuisChaine("1010");  // 10 en décimal
    BigBinary b1 = creerBigBinaryDepuisChaine("110");   // 6 en décimal
    printf("A = ");
    afficheBigBinary(a1);
    printf("B = ");
    afficheBigBinary(b1);
    
    BigBinary somme = Addition(a1, b1);
    printf("A + B = ");
    afficheBigBinary(somme);  // Devrait être 10000 (16 en décimal)
    
    libereBigBinary(&a1);
    libereBigBinary(&b1);
    libereBigBinary(&somme);
    
    // Test de soustraction
    afficher_separateur();
    printf("Test Soustraction:\n");
    BigBinary a2 = creerBigBinaryDepuisChaine("10000");  // 16 en décimal
    BigBinary b2 = creerBigBinaryDepuisChaine("110");    // 6 en décimal
    printf("A = ");
    afficheBigBinary(a2);
    printf("B = ");
    afficheBigBinary(b2);
    
    BigBinary diff = Soustraction(a2, b2);
    printf("A - B = ");
    afficheBigBinary(diff);  // Devrait être 1010 (10 en décimal)
    
    libereBigBinary(&a2);
    libereBigBinary(&b2);
    libereBigBinary(&diff);
    
    // Test de comparaison
    afficher_separateur();
    printf("Test Comparaisons:\n");
    BigBinary a3 = creerBigBinaryDepuisChaine("1010");
    BigBinary b3 = creerBigBinaryDepuisChaine("1010");
    BigBinary c3 = creerBigBinaryDepuisChaine("111");
    
    printf("A = ");
    afficheBigBinary(a3);
    printf("B = ");
    afficheBigBinary(b3);
    printf("C = ");
    afficheBigBinary(c3);
    
    printf("A == B ? %s\n", Egal(a3, b3) ? "OUI" : "NON");
    printf("A < C ? %s\n", Inferieur(a3, c3) ? "OUI" : "NON");
    printf("A > C ? %s\n", Superieur(a3, c3) ? "OUI" : "NON");
    
    libereBigBinary(&a3);
    libereBigBinary(&b3);
    libereBigBinary(&c3);
}

void tester_phase2() {
    printf("\n\n=== TESTS PHASE 2: OPERATIONS AVANCEES ===\n");
    
    // Test division par 2
    afficher_separateur();
    printf("Test Division par 2:\n");
    BigBinary a4 = creerBigBinaryDepuisChaine("1010");  // 10 en décimal
    printf("Avant: ");
    afficheBigBinary(a4);
    divisePar2(&a4);
    printf("Après division par 2: ");
    afficheBigBinary(a4);  // Devrait être 101 (5 en décimal)
    libereBigBinary(&a4);
    
    // Test multiplication par 2
    afficher_separateur();
    printf("Test Multiplication par 2:\n");
    BigBinary a5 = creerBigBinaryDepuisChaine("101");  // 5 en décimal
    printf("Avant: ");
    afficheBigBinary(a5);
    BigBinary double_a5 = multiplePar2(a5);
    printf("Après multiplication par 2: ");
    afficheBigBinary(double_a5);  // Devrait être 1010 (10 en décimal)
    libereBigBinary(&a5);
    libereBigBinary(&double_a5);
    
    // Test PGCD avec l'exemple du document: PGCD(51, 57) = 3
    afficher_separateur();
    printf("Test PGCD (exemple du document):\n");
    BigBinary a6 = creerBigBinaryDepuisChaine("110011");   // 51 en décimal
    BigBinary b6 = creerBigBinaryDepuisChaine("111001");   // 57 en décimal
    printf("A = ");
    afficheBigBinary(a6);
    printf("   (51 en decimal)\n");
    printf("B = ");
    afficheBigBinary(b6);
    printf("   (57 en decimal)\n");
    
    BigBinary pgcd = PGCD(a6, b6);
    printf("PGCD(A, B) = ");
    afficheBigBinary(pgcd);  // Devrait être 11 (3 en décimal)
    printf("   (3 en decimal)\n");
    
    libereBigBinary(&a6);
    libereBigBinary(&b6);
    libereBigBinary(&pgcd);
    
    // Test Modulo
    afficher_separateur();
    printf("Test Modulo:\n");
    BigBinary a7 = creerBigBinaryDepuisChaine("10111");  // 23 en décimal
    BigBinary b7 = creerBigBinaryDepuisChaine("101");    // 5 en décimal
    printf("A = ");
    afficheBigBinary(a7);
    printf("   (23 en decimal)\n");
    printf("B = ");
    afficheBigBinary(b7);
    printf("   (5 en decimal)\n");
    
    BigBinary mod = Modulo(a7, b7);
    printf("A mod B = ");
    afficheBigBinary(mod);  // 23 mod 5 = 3, donc devrait être 11
    printf("   (3 en decimal)\n");
    
    libereBigBinary(&a7);
    libereBigBinary(&b7);
    libereBigBinary(&mod);
    
    // Test Exponentiation Modulaire
    afficher_separateur();
    printf("Test Exponentiation Modulaire:\n");
    BigBinary base = creerBigBinaryDepuisChaine("11");     // 3 en décimal
    BigBinary exp = creerBigBinaryDepuisChaine("100");     // 4 en décimal
    BigBinary n = creerBigBinaryDepuisChaine("111");       // 7 en décimal
    printf("Base = ");
    afficheBigBinary(base);
    printf("   (3 en decimal)\n");
    printf("Exposant = ");
    afficheBigBinary(exp);
    printf("   (4 en decimal)\n");
    printf("Modulo = ");
    afficheBigBinary(n);
    printf("   (7 en decimal)\n");
    
    BigBinary resultat = ExpMod(base, exp, n);
    printf("Base^Exposant mod Modulo = ");
    afficheBigBinary(resultat);  // 3^4 mod 7 = 81 mod 7 = 4, donc devrait être 100
    printf("   (4 en decimal)\n");
    
    libereBigBinary(&base);
    libereBigBinary(&exp);
    libereBigBinary(&n);
    libereBigBinary(&resultat);
}

void tester_phase3() {
    printf("\n\n=== TESTS PHASE 3: RSA (BONUS) ===\n");
    
    afficher_separateur();
    printf("Test RSA simplifié:\n");
    printf("Utilisation de petits nombres pour la démonstration\n");
    printf("p = 11, q = 13, n = 143, phi = 120\n");
    printf("e = 7 (exposant public), d = 103 (exposant privé)\n\n");
    
    // Message original
    BigBinary message = creerBigBinaryDepuisChaine("1010");  // 10 en décimal
    printf("Message original M = ");
    afficheBigBinary(message);
    printf("   (10 en decimal)\n");
    
    // Clé publique: (e, n)
    BigBinary e = creerBigBinaryDepuisChaine("111");           // 7 en décimal
    BigBinary n = creerBigBinaryDepuisChaine("10001111");      // 143 en décimal
    
    // Chiffrement: C = M^e mod n
    printf("\nChiffrement...\n");
    BigBinary chiffre = RSA_encrypt(message, e, n);
    printf("Message chiffré C = ");
    afficheBigBinary(chiffre);
    
    // Clé privée: d
    BigBinary d = creerBigBinaryDepuisChaine("1100111");       // 103 en décimal
    
    // Déchiffrement: M = C^d mod n
    printf("\nDéchiffrement...\n");
    BigBinary dechiffre = RSA_decrypt(chiffre, d, n);
    printf("Message déchiffré M' = ");
    afficheBigBinary(dechiffre);
    printf("   (devrait être identique au message original)\n");
    
    // Vérification
    if (Egal(message, dechiffre)) {
        printf("\n✓ RSA fonctionne correctement! Message = Message déchiffré\n");
    } else {
        printf("\n✗ Erreur: Le message déchiffré ne correspond pas à l'original\n");
    }
    
    libereBigBinary(&message);
    libereBigBinary(&e);
    libereBigBinary(&n);
    libereBigBinary(&d);
    libereBigBinary(&chiffre);
    libereBigBinary(&dechiffre);
}

void tester_grands_nombres() {
    printf("\n\n=== TESTS AVEC GRANDS NOMBRES ===\n");
    
    afficher_separateur();
    printf("Test avec nombres de 90+ bits:\n");
    
    // Nombres de test du document
    BigBinary A1 = creerBigBinaryDepuisChaine("10101010101001110011101000111110100001011101001000101100011010110000111000000110100011101001");
    BigBinary B1 = creerBigBinaryDepuisChaine("11001011011111111000110000111010000000110101011000000100000010110101101001100011110001011000");
    
    printf("A1 = ");
    afficheBigBinary(A1);
    printf("B1 = ");
    afficheBigBinary(B1);
    
    printf("\nCalcul de A1 + B1...\n");
    BigBinary somme = Addition(A1, B1);
    printf("Résultat = ");
    afficheBigBinary(somme);
    
    libereBigBinary(&A1);
    libereBigBinary(&B1);
    libereBigBinary(&somme);
    
    afficher_separateur();
    printf("Test PGCD avec grands nombres:\n");
    BigBinary PGCD_A = creerBigBinaryDepuisChaine("111100011110001001111100100001010110100110010110000100111001100110010111001010111101101110");
    BigBinary PGCD_B = creerBigBinaryDepuisChaine("11100110011111011000000111111110110001110000011001111100111011011010101110110111000110111");
    
    printf("A = ");
    afficheBigBinary(PGCD_A);
    printf("B = ");
    afficheBigBinary(PGCD_B);
    
    printf("\nCalcul du PGCD...\n");
    BigBinary pgcd_result = PGCD(PGCD_A, PGCD_B);
    printf("PGCD(A, B) = ");
    afficheBigBinary(pgcd_result);
    
    libereBigBinary(&PGCD_A);
    libereBigBinary(&PGCD_B);
    libereBigBinary(&pgcd_result);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  PROJET LANGUAGE C - ALGORITHME BINAIRE D'EUCLIDE         ║\n");
    printf("║  Manipulation de Grands Entiers en Binaire (BigBinary)    ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // Exécuter les tests
    tester_phase1();
    tester_phase2();
    tester_phase3();
    tester_grands_nombres();
    
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  TESTS TERMINÉS                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}
