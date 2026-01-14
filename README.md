# BigBinary - Arithmétique Binaire en C

Bibliothèque C pour manipuler des **grands entiers binaires** et calculer le **PGCD** via l'algorithme binaire d'Euclide.

## Compilation

```bash
make        # Compiler
./bigbinary # Exécuter
make clean  # Nettoyer
```

## Fonctionnalités

| Fonction | Description |
|----------|-------------|
| `Addition(A, B)` | A + B |
| `Soustraction(A, B)` | A - B (A ≥ B) |
| `PGCD(A, B)` | Plus grand diviseur commun |
| `Modulo(A, B)` | A mod B |
| `ExpMod(M, e, n)` | M^e mod n |
| `RSA_encrypt/decrypt` | Chiffrement RSA |

## Exemple

```c
#include "bigbinary.h"

int main() {
  BigBinary a = creerBigBinaryDepuisChaine("110011");  // 51
  BigBinary b = creerBigBinaryDepuisChaine("111001");  // 57
  
  BigBinary pgcd = PGCD(a, b);
  afficheBigBinary(pgcd);  // 11 (= 3)
  
  libereBigBinary(&a);
  libereBigBinary(&b);
  libereBigBinary(&pgcd);
  return 0;
}
```

## Structure

```c
typedef struct {
  int *Tdigits;  // Bits (MSB → LSB)
  int Taille;    // Nombre de bits
  int Signe;     // +1, -1, ou 0
} BigBinary;
```

## Fichiers

```
bigbinary.h/c  → Bibliothèque
main.c         → Tests
Makefile       → Compilation
```

---
*Projet ESIEA 3A - Langage C (2025-2026)*
