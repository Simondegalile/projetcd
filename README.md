# Projet BigBinary - Algorithme Binaire de Calcul du PGCD

## Description

Ce projet implémente une bibliothèque en langage C pour la manipulation de **grands entiers en binaire** (BigBinary). Il permet d'effectuer des opérations arithmétiques sur des entiers de taille arbitraire, dépassant les limites des types natifs du C (32 ou 64 bits).

Le projet culmine avec l'implémentation de l'**algorithme binaire d'Euclide** pour le calcul du PGCD et une version simplifiée du **chiffrement RSA**.

## Structure du Projet

```
projetcd/
├── bigbinary.h        # Fichier d'en-tête avec les déclarations
├── bigbinary.c        # Implémentation des fonctions BigBinary
├── main.c             # Programme de test
├── Makefile           # Fichier de compilation
└── README.md          # Ce fichier
```

## Fonctionnalités

### Phase 1 : Opérations de Base
- ✅ Initialisation et gestion mémoire des BigBinary
- ✅ Addition de grands entiers binaires
- ✅ Soustraction de grands entiers binaires (A ≥ B)
- ✅ Comparaisons : égalité, inférieur, supérieur
- ✅ Affichage en binaire

### Phase 2 : Opérations Avancées
- ✅ Division et multiplication par 2 (décalage de bits)
- ✅ **Algorithme Binaire d'Euclide** pour le calcul du PGCD
  - Utilise uniquement des additions, soustractions et décalages
  - Pas de division euclidienne nécessaire
- ✅ Calcul du modulo
- ✅ Exponentiation modulaire rapide

### Phase 3 : RSA Simplifié (Bonus)
- ✅ Chiffrement RSA : C = M^e mod N
- ✅ Déchiffrement RSA : M = C^d mod N

## Compilation et Exécution

### Compilation
```bash
make
```

### Exécution
```bash
make run
```
ou directement :
```bash
./bigbinary
```

### Nettoyage
```bash
make clean
```

## Utilisation de la Bibliothèque

### Exemple de Base

```c
#include "bigbinary.h"

int main() {
    // Création de nombres binaires
    BigBinary a = creerBigBinaryDepuisChaine("1010");  // 10 en décimal
    BigBinary b = creerBigBinaryDepuisChaine("110");   // 6 en décimal
    
    // Addition
    BigBinary somme = Addition(a, b);
    printf("Résultat: ");
    afficheBigBinary(somme);  // Affiche: 10000 (16 en décimal)
    
    // Libération mémoire
    libereBigBinary(&a);
    libereBigBinary(&b);
    libereBigBinary(&somme);
    
    return 0;
}
```

### Calcul du PGCD

```c
// Exemple du document: PGCD(51, 57) = 3
BigBinary a = creerBigBinaryDepuisChaine("110011");  // 51
BigBinary b = creerBigBinaryDepuisChaine("111001");  // 57

BigBinary pgcd = PGCD(a, b);
afficheBigBinary(pgcd);  // Affiche: 11 (3 en décimal)

libereBigBinary(&a);
libereBigBinary(&b);
libereBigBinary(&pgcd);
```

### Exponentiation Modulaire et RSA

```c
// Chiffrement RSA simplifié
BigBinary message = creerBigBinaryDepuisChaine("1010");      // Message: 10
BigBinary e = creerBigBinaryDepuisChaine("111");             // e = 7
BigBinary n = creerBigBinaryDepuisChaine("10001111");        // n = 143

BigBinary chiffre = RSA_encrypt(message, e, n);
printf("Message chiffré: ");
afficheBigBinary(chiffre);

// Déchiffrement
BigBinary d = creerBigBinaryDepuisChaine("1100111");         // d = 103
BigBinary dechiffre = RSA_decrypt(chiffre, d, n);
printf("Message déchiffré: ");
afficheBigBinary(dechiffre);

// Libération mémoire
libereBigBinary(&message);
libereBigBinary(&e);
libereBigBinary(&n);
libereBigBinary(&d);
libereBigBinary(&chiffre);
libereBigBinary(&dechiffre);
```

## Structure BigBinary

```c
typedef struct {
    int *Tdigits;  // Tableau de bits (MSB à gauche, LSB à droite)
    int Taille;    // Nombre de bits significatifs
    int Signe;     // +1 (positif), -1 (négatif), 0 (nul)
} BigBinary;
```

### Convention de Stockage
- `Tdigits[0]` : bit de poids fort (MSB - Most Significant Bit)
- `Tdigits[Taille-1]` : bit de poids faible (LSB - Least Significant Bit)

Cette convention facilite les opérations de division par 2 (suppression du bit de poids faible).

## Algorithme Binaire d'Euclide

L'algorithme binaire d'Euclide calcule le PGCD de deux entiers sans utiliser la division euclidienne, en exploitant les propriétés suivantes :

| Condition | Formule |
|-----------|---------|
| a pair et b pair | PGCD(a, b) = 2 × PGCD(a/2, b/2) |
| a pair et b impair | PGCD(a, b) = PGCD(a/2, b) |
| a impair et b pair | PGCD(a, b) = PGCD(a, b/2) |
| a et b impairs | PGCD(a, b) = PGCD(a - b, b) |
| b = 0 | PGCD(a, 0) = a |

En binaire, la division par 2 devient un simple décalage de bits, ce qui rend cet algorithme particulièrement efficace pour les processeurs.

## Tests Inclus

Le programme `main.c` contient une suite de tests complète :

1. **Tests Phase 1** : Addition, soustraction, comparaisons
2. **Tests Phase 2** : Division/multiplication par 2, PGCD, modulo, exponentiation modulaire
3. **Tests Phase 3** : Chiffrement et déchiffrement RSA
4. **Tests avec grands nombres** : Nombres de 90+ bits

## Références

1. [Algorithme d'Euclide - Wikipedia](https://fr.wikipedia.org/wiki/Algorithme_d%27Euclide)
2. [Algorithme Binaire d'Euclide - Wikipedia](https://fr.wikipedia.org/wiki/Algorithme_binaire_de_calcul_du_PGCD)
3. Donald E. Knuth, *The Art of Computer Programming*

## Auteur

Projet ESIEA 3ème Année - Language C
Année Académique 2025-2026

## Licence

Ce projet est à usage éducatif dans le cadre du cours de Language C de l'ESIEA.
