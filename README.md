# Projet BigBinary - Calcul sur des entiers en précision arbitraire

## Description

Ce projet implémente une bibliothèque en C pour la manipulation de grands entiers en **binaire** (représentation en base 2). Il permet d'effectuer des opérations arithmétiques sur des nombres qui ne peuvent pas être représentés exactement sur 32 ou 64 bits standard.

Ce projet est basé sur l'algorithme binaire d'Euclide pour le calcul du PGCD (Plus Grand Commun Diviseur).

## Fichiers du projet

- **bigbinary.h** : Fichier d'en-tête contenant la structure `BigBinary` et les déclarations des fonctions
- **bigbinary.c** : Implémentation des fonctions de manipulation des grands entiers binaires
- **main.c** : Programme de test démontrant l'utilisation de la bibliothèque
- **Makefile** : Fichier de compilation automatique

## Structure BigBinary

```c
typedef struct {
    int *Tdigits;  // Tableau de bits (MSB à gauche, LSB à droite)
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 (positif), -1 (négatif), 0 (nul)
} BigBinary;
```

## Fonctionnalités - Phase 1 ✅

### Fonctions de base
- `createBigBinary(int size)` : Crée un BigBinary vide
- `initBigBinary(int taille, int signe)` : Initialise un BigBinary à zéro
- `creerBigBinaryDepuisChaine(const char *chaine)` : Crée un BigBinary depuis une chaîne binaire (ex: "1010101")
- `afficheBigBinary(BigBinary nb)` : Affiche un BigBinary
- `libereBigBinary(BigBinary *nb)` : Libère la mémoire
- `getNombreDigits(BigBinary nb)` : Retourne le nombre de bits

### Opérations arithmétiques
- `BigBinary_Addition(BigBinary A, BigBinary B)` : Addition (algorithme naïf)
- `BigBinary_Soustraction(BigBinary A, BigBinary B)` : Soustraction (avec A ≥ B)

### Fonctions de comparaison
- `Egal(BigBinary A, BigBinary B)` : Teste l'égalité
- `Inferieur(BigBinary A, BigBinary B)` : Teste si A < B

## Compilation

### Avec GCC (Linux/MinGW)
```bash
make
```

### Compilation manuelle
```bash
gcc -o bigbinary_test main.c bigbinary.c -Wall -I.
```

### Avec MSVC (Windows)
```cmd
cl /Fe:bigbinary_test.exe main.c bigbinary.c
```

## Utilisation

```bash
./bigbinary_test
```

## Exemple d'utilisation

```c
#include "bigbinary.h"

int main() {
    // Création de nombres binaires
    BigBinary a = creerBigBinaryDepuisChaine("110011");  // 51 en décimal
    BigBinary b = creerBigBinaryDepuisChaine("111001");  // 57 en décimal

    // Addition
    BigBinary somme = BigBinary_Addition(a, b);
    printf("Somme = ");
    afficheBigBinary(somme);

    // Libération mémoire
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&somme);

    return 0;
}
```

## Phase 2 (À venir)

- Calcul du PGCD avec l'algorithme binaire d'Euclide
- Calcul du modulo
- Exponentiation rapide modulaire

## Phase 3 (Bonus)

- Chiffrement RSA
- Déchiffrement RSA

## Notes importantes

⚠️ **Convention de stockage des bits** : Les bits sont stockés avec le MSB (Most Significant Bit) à gauche :
- `Tdigits[0]` = bit de poids fort
- `Tdigits[Taille-1]` = bit de poids faible

⚠️ **Règles scanf/printf** :
1. Pendant le développement : vous pouvez faire absolument TOUT CE QUE VOUS VOULEZ
2. Pour le code "rendu" : votre code devra faire EXACTEMENT CE QUI SERA DEMANDÉ (ni plus ni moins)

## Références

- [Algorithme d'Euclide - Wikipedia](https://fr.wikipedia.org/wiki/Algorithme_d%27Euclide)
- [Algorithme binaire d'Euclide - Wikipedia](https://fr.wikipedia.org/wiki/Algorithme_binaire_de_calcul_du_PGCD)
- [Division algorithm - Wikipedia](https://en.wikipedia.org/wiki/Division_algorithm)

## Auteur

Projet réalisé dans le cadre du cours "Langage C - PROJET S5" à l'ESIEA (3A S5 FISA/FISE)
Année Académique : 2025-2026

## Licence

Ce projet est à but éducatif.
