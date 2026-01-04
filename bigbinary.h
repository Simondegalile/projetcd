#ifndef BIGBINARY_H
#define BIGBINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BASE 2  // La base du nombre (2 pour binaire)

// Structure pour représenter un grand entier en binaire
typedef struct {
    int *Tdigits;  // Tableau de bits : Tdigits[0] = bit de poids fort (MSB), Tdigits[Taille - 1] = bit de poids faible (LSB)
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 pour positif, -1 pour négatif, 0 pour nul
} BigBinary;

// ========== FONCTIONS D'INITIALISATION ET MANIPULATION DE BASE ==========

// Initialisation manuelle d'un BigBinary vide (tout est à zéro)
BigBinary initBigBinary(int taille, int signe);

// Création d'un BigBinary depuis une chaîne binaire
BigBinary creerBigBinaryDepuisChaine(const char *chaine);

// Création d'un BigBinary représentant zéro
BigBinary creerZero();

// Affichage du nombre binaire
void afficheBigBinary(BigBinary nb);

// Libération de la mémoire
void libereBigBinary(BigBinary *nb);

// Copie d'un BigBinary
BigBinary copieBigBinary(BigBinary nb);

// Calcul du nombre de bits significatifs (supprime les zéros de tête)
void normaliser(BigBinary *nb);

// ========== PHASE 1: OPÉRATIONS DE BASE ==========

// Addition de deux BigBinary positifs : A + B
BigBinary Addition(BigBinary A, BigBinary B);

// Soustraction de deux BigBinary positifs avec A >= B : A - B
BigBinary Soustraction(BigBinary A, BigBinary B);

// Comparaison : renvoie true si A == B
bool Egal(BigBinary A, BigBinary B);

// Comparaison : renvoie true si A < B
bool Inferieur(BigBinary A, BigBinary B);

// Comparaison : renvoie true si A > B
bool Superieur(BigBinary A, BigBinary B);

// ========== PHASE 2: OPÉRATIONS AVANCÉES ==========

// Division par 2 (décalage à droite)
void divisePar2(BigBinary *nb);

// Multiplication par 2 (décalage à gauche)
BigBinary multiplePar2(BigBinary nb);

// Teste si un BigBinary est pair
bool estPair(BigBinary nb);

// Teste si un BigBinary est nul
bool estNul(BigBinary nb);

// Multiplication Égyptienne : A * B
BigBinary MultiplicationEgyptienne(BigBinary A, BigBinary B);

// Calcul du PGCD par l'algorithme binaire d'Euclide
BigBinary PGCD(BigBinary A, BigBinary B);

// Calcul du modulo : A mod B
BigBinary Modulo(BigBinary A, BigBinary B);

// Structure pour le résultat de la Division Euclidienne
typedef struct {
    BigBinary quotient;
    BigBinary reste;
} DivisionResult;

// Division Euclidienne : calcule quotient et reste tels que A = quotient * B + reste
DivisionResult DivisionEuclidienne(BigBinary A, BigBinary B);

// Libération de la mémoire d'un DivisionResult
void libereDivisionResult(DivisionResult *res);

// Exponentiation modulaire : M^exp mod n
// exp est passé comme BigBinary mais peut être simplifié en int si < 64 bits
BigBinary ExpMod(BigBinary M, BigBinary exp, BigBinary n);

// Exponentiation modulaire avec exposant unsigned int (pour e < 64 bits)
BigBinary ExpModInt(BigBinary M, unsigned int exp, BigBinary n);

// Fonction auxiliaire pour la multiplication modulaire : (A * B) mod n
BigBinary multiplicationMod(BigBinary A, BigBinary B, BigBinary n);

// ========== PHASE 3: RSA (BONUS) ==========

// Chiffrement RSA : C = M^e mod N
BigBinary RSA_encrypt(BigBinary message, BigBinary e, BigBinary n);

// Déchiffrement RSA : M = C^d mod N
BigBinary RSA_decrypt(BigBinary cipher, BigBinary d, BigBinary n);

#endif // BIGBINARY_H
