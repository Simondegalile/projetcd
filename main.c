#include "bigbinary.h"

int main() {
    printf("=== Test de la bibliothèque BigBinary - Phase 1 ===\n\n");

    // Test 1: Création depuis une chaîne binaire
    printf("Test 1: Création depuis une chaîne binaire\n");
    BigBinary a = creerBigBinaryDepuisChaine("110011");
    printf("a = ");
    afficheBigBinary(a);
    printf("Nombre de bits: %d\n\n", getNombreDigits(a));

    // Test 2: Création d'un autre nombre
    printf("Test 2: Création d'un deuxième nombre\n");
    BigBinary b = creerBigBinaryDepuisChaine("111001");
    printf("b = ");
    afficheBigBinary(b);
    printf("Nombre de bits: %d\n\n", getNombreDigits(b));

    // Test 3: Comparaison - Egal
    printf("Test 3: Comparaison - Égalité\n");
    BigBinary c = creerBigBinaryDepuisChaine("110011");
    printf("a == c ? %s\n", Egal(a, c) ? "true" : "false");
    printf("a == b ? %s\n\n", Egal(a, b) ? "true" : "false");

    // Test 4: Comparaison - Inférieur
    printf("Test 4: Comparaison - Inférieur\n");
    printf("a < b ? %s\n", Inferieur(a, b) ? "true" : "false");
    printf("b < a ? %s\n\n", Inferieur(b, a) ? "true" : "false");

    // Test 5: Addition
    printf("Test 5: Addition\n");
    printf("a = ");
    afficheBigBinary(a);
    printf("b = ");
    afficheBigBinary(b);
    BigBinary somme = BigBinary_Addition(a, b);
    printf("a + b = ");
    afficheBigBinary(somme);
    printf("\n");

    // Test 6: Soustraction
    printf("Test 6: Soustraction\n");
    printf("b = ");
    afficheBigBinary(b);
    printf("a = ");
    afficheBigBinary(a);
    BigBinary diff = BigBinary_Soustraction(b, a);
    printf("b - a = ");
    afficheBigBinary(diff);
    printf("\n");

    // Test 7: Exemple du document (51 et 57)
    printf("Test 7: Exemple du document PGCD(51, 57)\n");
    BigBinary num51 = creerBigBinaryDepuisChaine("110011");  // 51 en binaire
    BigBinary num57 = creerBigBinaryDepuisChaine("111001");  // 57 en binaire
    printf("51 (binaire) = ");
    afficheBigBinary(num51);
    printf("57 (binaire) = ");
    afficheBigBinary(num57);

    printf("\nCalcul: 57 - 51\n");
    BigBinary step1 = BigBinary_Soustraction(num57, num51);
    printf("57 - 51 = ");
    afficheBigBinary(step1);  // Devrait donner 6 = 000110

    printf("\nCalcul: 51 - 6*8 = 51 - 48 = 3\n");
    BigBinary num3 = creerBigBinaryDepuisChaine("000011");  // 3 en binaire
    printf("3 (binaire) = ");
    afficheBigBinary(num3);
    printf("\n");

    // Test 8: Addition avec retenue
    printf("Test 8: Addition avec retenue\n");
    BigBinary d = creerBigBinaryDepuisChaine("1111");  // 15
    BigBinary e = creerBigBinaryDepuisChaine("0001");  // 1
    printf("1111 + 0001 = ");
    BigBinary somme2 = BigBinary_Addition(d, e);
    afficheBigBinary(somme2);  // Devrait donner 10000 (16)
    printf("\n");

    // Test 9: Nombre zéro
    printf("Test 9: Nombre zéro\n");
    BigBinary zero = creerBigBinaryDepuisChaine("0000");
    printf("zero = ");
    afficheBigBinary(zero);
    printf("Signe de zero: %d\n\n", zero.Signe);

    // Libération de la mémoire
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&c);
    libereBigBinary(&somme);
    libereBigBinary(&diff);
    libereBigBinary(&num51);
    libereBigBinary(&num57);
    libereBigBinary(&step1);
    libereBigBinary(&num3);
    libereBigBinary(&d);
    libereBigBinary(&e);
    libereBigBinary(&somme2);
    libereBigBinary(&zero);

    printf("=== Tests terminés avec succès ===\n");
    return 0;
}
