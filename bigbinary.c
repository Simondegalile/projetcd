#include "bigbinary.h"

// ========== FONCTIONS D'INITIALISATION ET MANIPULATION DE BASE ==========

// Initialisation manuelle d'un BigBinary vide (tout est à zéro)
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    nb.Tdigits = malloc(sizeof(int) * taille);
    for (int i = 0; i < taille; ++i) {
        nb.Tdigits[i] = 0;
    }
    return nb;
}

// Création d'un BigBinary depuis une chaîne binaire
BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    BigBinary nb;
    int n = strlen(chaine);
    nb.Taille = 0;

    // Comptons uniquement les caractères valides ('0' ou '1')
    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Taille++;
        }
    }
    
    nb.Tdigits = malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;
    int index = 0;
    int tousZeros = 1;

    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Tdigits[index] = chaine[i] - '0';
            if (nb.Tdigits[index] == 1) tousZeros = 0;
            index++;
        }
    }

    if (tousZeros) nb.Signe = 0;
    
    return nb;
}

// Création d'un BigBinary représentant zéro
BigBinary creerZero() {
    BigBinary nb;
    nb.Taille = 1;
    nb.Signe = 0;
    nb.Tdigits = malloc(sizeof(int));
    nb.Tdigits[0] = 0;
    return nb;
}

// Affichage du nombre binaire
void afficheBigBinary(BigBinary nb) {
    if (nb.Signe == -1) printf("-");
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }
    for (int i = 0; i < nb.Taille; ++i) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

// Libération de la mémoire
void libereBigBinary(BigBinary *nb) {
    if (nb->Tdigits != NULL) {
        free(nb->Tdigits);
        nb->Tdigits = NULL;
    }
    nb->Taille = 0;
    nb->Signe = 0;
}

// Copie d'un BigBinary
BigBinary copieBigBinary(BigBinary nb) {
    BigBinary copie;
    copie.Taille = nb.Taille;
    copie.Signe = nb.Signe;
    copie.Tdigits = malloc(sizeof(int) * nb.Taille);
    for (int i = 0; i < nb.Taille; ++i) {
        copie.Tdigits[i] = nb.Tdigits[i];
    }
    return copie;
}

// Calcul du nombre de bits significatifs (supprime les zéros de tête)
void normaliser(BigBinary *nb) {
    if (nb->Taille == 0) {
        nb->Signe = 0;
        return;
    }
    
    // Trouver le premier bit non nul
    int premierePos = 0;
    while (premierePos < nb->Taille && nb->Tdigits[premierePos] == 0) {
        premierePos++;
    }
    
    // Si tous les bits sont à 0
    if (premierePos == nb->Taille) {
        nb->Signe = 0;
        nb->Taille = 1;
        nb->Tdigits[0] = 0;
        return;
    }
    
    // Si des zéros de tête existent, les supprimer
    if (premierePos > 0) {
        int nouvelleTaille = nb->Taille - premierePos;
        int *nouveauTableau = malloc(sizeof(int) * nouvelleTaille);
        for (int i = 0; i < nouvelleTaille; ++i) {
            nouveauTableau[i] = nb->Tdigits[premierePos + i];
        }
        free(nb->Tdigits);
        nb->Tdigits = nouveauTableau;
        nb->Taille = nouvelleTaille;
    }
}

// ========== PHASE 1: OPÉRATIONS DE BASE ==========

// Comparaison : renvoie true si A == B
bool Egal(BigBinary A, BigBinary B) {
    // Si les signes sont différents
    if (A.Signe != B.Signe) return false;
    
    // Si les deux sont nuls
    if (A.Signe == 0 && B.Signe == 0) return true;
    
    // Si les tailles sont différentes
    if (A.Taille != B.Taille) return false;
    
    // Comparer bit par bit
    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] != B.Tdigits[i]) return false;
    }
    
    return true;
}

// Comparaison : renvoie true si A < B (pour nombres positifs)
bool Inferieur(BigBinary A, BigBinary B) {
    // Si A est nul et B est positif
    if (A.Signe == 0 && B.Signe == 1) return true;
    if (A.Signe == 1 && B.Signe == 0) return false;
    if (A.Signe == 0 && B.Signe == 0) return false;
    
    // Si A a moins de bits que B, alors A < B
    if (A.Taille < B.Taille) return true;
    if (A.Taille > B.Taille) return false;
    
    // Même nombre de bits : comparer bit par bit du MSB au LSB
    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }
    
    // A == B
    return false;
}

// Comparaison : renvoie true si A > B
bool Superieur(BigBinary A, BigBinary B) {
    return !Inferieur(A, B) && !Egal(A, B);
}

// Addition de deux BigBinary positifs : A + B
BigBinary Addition(BigBinary A, BigBinary B) {
    // Déterminer la taille maximale + 1 pour la retenue éventuelle
    int tailleMax = (A.Taille > B.Taille) ? A.Taille : B.Taille;
    BigBinary resultat = initBigBinary(tailleMax + 1, 1);
    
    int retenue = 0;
    int i = 0;
    
    // Addition bit par bit de droite à gauche
    while (i < A.Taille || i < B.Taille || retenue) {
        int bitA = (i < A.Taille) ? A.Tdigits[A.Taille - 1 - i] : 0;
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        
        int somme = bitA + bitB + retenue;
        resultat.Tdigits[tailleMax - i] = somme % 2;
        retenue = somme / 2;
        
        i++;
    }
    
    // Normaliser le résultat
    normaliser(&resultat);
    
    return resultat;
}

// Soustraction de deux BigBinary positifs avec A >= B : A - B
BigBinary Soustraction(BigBinary A, BigBinary B) {
    // Vérifier que A >= B
    if (Inferieur(A, B)) {
        printf("Erreur: Soustraction avec A < B non supportée\n");
        return creerZero();
    }
    
    if (Egal(A, B)) {
        return creerZero();
    }
    
    BigBinary resultat = initBigBinary(A.Taille, 1);
    int emprunt = 0;
    
    // Soustraction bit par bit de droite à gauche
    for (int i = 0; i < A.Taille; ++i) {
        int bitA = A.Tdigits[A.Taille - 1 - i];
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;
        
        int diff = bitA - bitB - emprunt;
        
        if (diff < 0) {
            diff += 2;
            emprunt = 1;
        } else {
            emprunt = 0;
        }
        
        resultat.Tdigits[A.Taille - 1 - i] = diff;
    }
    
    // Normaliser le résultat
    normaliser(&resultat);
    
    return resultat;
}

// ========== PHASE 2: OPÉRATIONS AVANCÉES ==========

// Division par 2 (décalage à droite)
void divisePar2(BigBinary *nb) {
    if (nb->Signe == 0 || nb->Taille == 0) return;
    
    // Si le nombre est 1, il devient 0
    if (nb->Taille == 1 && nb->Tdigits[0] == 1) {
        nb->Tdigits[0] = 0;
        nb->Signe = 0;
        return;
    }
    
    // Décalage à droite : supprimer le bit de poids faible
    nb->Taille--;
    if (nb->Taille == 0) {
        nb->Signe = 0;
        nb->Taille = 1;
        nb->Tdigits[0] = 0;
    }
}

// Multiplication par 2 (décalage à gauche)
BigBinary multiplePar2(BigBinary nb) {
    if (nb.Signe == 0) return creerZero();
    
    BigBinary resultat = initBigBinary(nb.Taille + 1, nb.Signe);
    
    // Copier tous les bits
    for (int i = 0; i < nb.Taille; ++i) {
        resultat.Tdigits[i] = nb.Tdigits[i];
    }
    // Ajouter un 0 à droite (LSB)
    resultat.Tdigits[nb.Taille] = 0;
    
    return resultat;
}

// Teste si un BigBinary est pair
bool estPair(BigBinary nb) {
    if (nb.Signe == 0) return true;  // 0 est considéré comme pair
    // Un nombre est pair si son bit de poids faible (LSB) est 0
    return nb.Tdigits[nb.Taille - 1] == 0;
}

// Teste si un BigBinary est nul
bool estNul(BigBinary nb) {
    return nb.Signe == 0;
}

// Multiplication Égyptienne : A * B
// Principe : Pour multiplier A par B, on parcourt les bits de B
// Si le bit i de B est 1, on ajoute A*(2^i) au résultat
BigBinary MultiplicationEgyptienne(BigBinary A, BigBinary B) {
    if (estNul(A) || estNul(B)) {
        return creerZero();
    }
    
    BigBinary resultat = creerZero();
    BigBinary multiplicande = copieBigBinary(A);  // A qu'on va doubler à chaque étape
    
    // Parcourir B de droite à gauche (du LSB au MSB)
    for (int i = B.Taille - 1; i >= 0; i--) {
        // Si le bit actuel de B est 1
        if (B.Tdigits[i] == 1) {
            // Ajouter le multiplicande au résultat
            BigBinary temp = Addition(resultat, multiplicande);
            libereBigBinary(&resultat);
            resultat = temp;
        }
        
        // Doubler le multiplicande pour la prochaine itération (sauf à la dernière)
        if (i > 0) {
            BigBinary temp = multiplePar2(multiplicande);
            libereBigBinary(&multiplicande);
            multiplicande = temp;
        }
    }
    
    libereBigBinary(&multiplicande);
    return resultat;
}

// Calcul du PGCD par l'algorithme binaire d'Euclide
BigBinary PGCD(BigBinary A, BigBinary B) {
    // Créer des copies pour ne pas modifier les originaux
    BigBinary a = copieBigBinary(A);
    BigBinary b = copieBigBinary(B);
    
    // PGCD(a, 0) = a
    if (estNul(b)) {
        libereBigBinary(&b);
        return a;
    }
    if (estNul(a)) {
        libereBigBinary(&a);
        return b;
    }
    
    // Compteur pour les facteurs de 2
    int facteur2 = 0;
    
    // Tant que a et b sont pairs, diviser par 2 et compter
    while (estPair(a) && estPair(b)) {
        divisePar2(&a);
        divisePar2(&b);
        facteur2++;
    }
    
    // Algorithme binaire d'Euclide
    while (!Egal(a, b)) {
        if (estPair(a)) {
            divisePar2(&a);
        } else if (estPair(b)) {
            divisePar2(&b);
        } else {
            // Les deux sont impairs
            if (Superieur(a, b)) {
                BigBinary diff = Soustraction(a, b);
                libereBigBinary(&a);
                a = diff;
                divisePar2(&a);  // (a - b) / 2
            } else {
                BigBinary diff = Soustraction(b, a);
                libereBigBinary(&b);
                b = diff;
                divisePar2(&b);  // (b - a) / 2
            }
        }
    }
    
    // Multiplier le résultat par 2^facteur2
    BigBinary resultat = a;
    for (int i = 0; i < facteur2; ++i) {
        BigBinary temp = multiplePar2(resultat);
        libereBigBinary(&resultat);
        resultat = temp;
    }
    
    libereBigBinary(&b);
    return resultat;
}

// Calcul du modulo : A mod B
// Algorithme "division-free mod" basé sur l'antiphérèse
// On utilise la propriété : Modulo(A, B) = Modulo(A - 2^k * B, B)
// où k est le plus grand entier tel que A - 2^k * B >= 0
BigBinary Modulo(BigBinary A, BigBinary B) {
    if (estNul(B)) {
        printf("Erreur: Division par zéro\n");
        return creerZero();
    }
    
    // Si A < B alors A mod B = A
    if (Inferieur(A, B)) {
        return copieBigBinary(A);
    }
    
    // Si A == B alors A mod B = 0
    if (Egal(A, B)) {
        return creerZero();
    }
    
    // Algorithme division-free mod
    BigBinary reste = copieBigBinary(A);
    
    // Boucle principale : tant que reste >= B
    while (!Inferieur(reste, B)) {
        // Trouver le plus grand k tel que reste >= 2^k * B
        BigBinary B_decale = copieBigBinary(B);
        int k = 0;
        
        // On décale B à gauche jusqu'à ce que 2^(k+1) * B > reste
        while (true) {
            BigBinary B_double = multiplePar2(B_decale);
            
            // Si 2^(k+1) * B > reste, on s'arrête
            if (Superieur(B_double, reste)) {
                libereBigBinary(&B_double);
                break;
            }
            
            // Sinon, on continue avec 2^(k+1) * B
            libereBigBinary(&B_decale);
            B_decale = B_double;
            k++;
        }
        
        // Maintenant, on a 2^k * B <= reste < 2^(k+1) * B
        // On soustrait 2^k * B de reste
        BigBinary nouveau_reste = Soustraction(reste, B_decale);
        libereBigBinary(&reste);
        libereBigBinary(&B_decale);
        reste = nouveau_reste;
    }
    
    return reste;
}

// Exponentiation modulaire : M^exp mod n
BigBinary ExpMod(BigBinary M, BigBinary exp, BigBinary n) {
    // Si l'exposant est 0, retourner 1
    if (estNul(exp)) {
        BigBinary un = creerBigBinaryDepuisChaine("1");
        return un;
    }
    
    // Résultat initialisé à 1
    BigBinary resultat = creerBigBinaryDepuisChaine("1");
    BigBinary base = Modulo(M, n);  // M mod n
    BigBinary exposant = copieBigBinary(exp);
    
    // Algorithme d'exponentiation rapide (binary exponentiation)
    while (!estNul(exposant)) {
        // Si le bit de poids faible de l'exposant est 1
        if (exposant.Tdigits[exposant.Taille - 1] == 1) {
            // resultat = (resultat * base) mod n
            // Comme on n'a pas de multiplication directe, on utilise l'addition répétée
            // ou on implémente une multiplication
            BigBinary temp = multiplicationMod(resultat, base, n);
            libereBigBinary(&resultat);
            resultat = temp;
        }
        
        // base = (base * base) mod n
        BigBinary temp = multiplicationMod(base, base, n);
        libereBigBinary(&base);
        base = temp;
        
        // Diviser l'exposant par 2
        divisePar2(&exposant);
    }
    
    libereBigBinary(&base);
    libereBigBinary(&exposant);
    
    return resultat;
}

// Fonction auxiliaire: Multiplication de deux BigBinary avec modulo
// A * B mod n (utilise la multiplication égyptienne)
BigBinary multiplicationMod(BigBinary A, BigBinary B, BigBinary n) {
    if (estNul(A) || estNul(B)) {
        return creerZero();
    }
    
    BigBinary resultat = creerZero();
    BigBinary temp_A = Modulo(A, n);
    
    // Parcourir chaque bit de B de droite à gauche
    for (int i = B.Taille - 1; i >= 0; i--) {
        if (B.Tdigits[i] == 1) {
            // resultat = (resultat + temp_A) mod n
            BigBinary somme = Addition(resultat, temp_A);
            libereBigBinary(&resultat);
            resultat = Modulo(somme, n);
            libereBigBinary(&somme);
        }
        
        if (i > 0) {
            // temp_A = (temp_A * 2) mod n
            BigBinary double_temp = multiplePar2(temp_A);
            libereBigBinary(&temp_A);
            temp_A = Modulo(double_temp, n);
            libereBigBinary(&double_temp);
        }
    }
    
    libereBigBinary(&temp_A);
    return resultat;
}

// ========== PHASE 3: RSA (BONUS) ==========

// Chiffrement RSA : C = M^e mod N
BigBinary RSA_encrypt(BigBinary message, BigBinary e, BigBinary n) {
    return ExpMod(message, e, n);
}

// Déchiffrement RSA : M = C^d mod N
BigBinary RSA_decrypt(BigBinary cipher, BigBinary d, BigBinary n) {
    return ExpMod(cipher, d, n);
}
