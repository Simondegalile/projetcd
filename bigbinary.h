#ifndef BIGBINARY_H
#define BIGBINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BASE 2  // La base du nombre (2 pour binaire)

// Structure pour représenter un grand entier en binaire
typedef struct {
    int *Tdigits;  // Tableau de bits : Tdigits[0] = bit de poids fort, Tdigits[Taille - 1] = bit de poids faible
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 pour positif, -1 pour négatif, 0 pour nul
} BigBinary;

// Fonctions de base

// Crée un BigBinary vide de taille donnée
BigBinary createBigBinary(int size);

// Initialise un BigBinary à zéro
BigBinary initBigBinary(int taille, int signe);

// Crée un BigBinary depuis une chaîne binaire (ex: "1010101")
BigBinary creerBigBinaryDepuisChaine(const char *chaine);

// Affiche un BigBinary
void afficheBigBinary(BigBinary nb);

// Libère la mémoire d'un BigBinary
void libereBigBinary(BigBinary *nb);

// Retourne le nombre de digits (bits) d'un BigBinary
int getNombreDigits(BigBinary nb);

// Opérations de base

// Addition de deux BigBinary (algorithme naïf)
BigBinary BigBinary_Addition(BigBinary A, BigBinary B);

// Soustraction de deux BigBinary (A - B, avec A >= B)
BigBinary BigBinary_Soustraction(BigBinary A, BigBinary B);

// Fonctions de comparaison

// Teste l'égalité de deux BigBinary
bool Egal(BigBinary A, BigBinary B);

// Teste si A < B
bool Inferieur(BigBinary A, BigBinary B);

#endif // BIGBINARY_H
