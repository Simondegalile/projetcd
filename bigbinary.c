/**
 * \file bigbinary.c
 * \brief Implémentation de la bibliothèque BigBinary
 *
 * Ce fichier contient toutes les fonctions pour manipuler des grands entiers binaires.
 * Le projet est divisé en 3 phases selon le cahier des charges ESIEA.
 *
 * === PHASE 1: Opérations de base ===
 * - Structure BigBinary avec stockage des bits en MSB-first
 * - Addition et soustraction (algorithmes naïfs de l'école primaire)
 * - Fonctions de comparaison (Egal, Inferieur, Superieur)
 *
 * === PHASE 2: Opérations avancées ===
 * - Multiplication Égyptienne (algorithme ancien, efficace en binaire)
 * - PGCD par l'algorithme Binaire d'Euclide
 * - Modulo et Division Euclidienne (sans division, par antiphérèse)
 * - Exponentiation modulaire rapide (binary exponentiation)
 *
 * === PHASE 3: RSA (Bonus) ===
 * - Chiffrement et déchiffrement RSA simplifié
 */

#include "bigbinary.h"
#include <limits.h>

// ============================================================================
// ==================== PHASE 1: INITIALISATION ET MANIPULATION ===============
// ============================================================================

/**
 * \brief Initialise un BigBinary vide avec tous les bits à zéro
 * \param taille Nombre de bits à allouer
 * \param signe Signe du nombre (+1, -1, ou 0 pour nul)
 * \return Un BigBinary initialisé avec des zéros
 *
 * Cette fonction alloue dynamiquement un tableau de 'taille' entiers
 * et initialise tous les bits à 0. Utile pour créer des structures
 * temporaires avant de les remplir.
 *
 * === Phase 1, §6.1: Structure de données ===
 */
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;              // Nombre de bits significatifs
    nb.Signe = signe;                // +1 (positif), -1 (négatif), 0 (nul)
    nb.Tdigits = malloc(sizeof(int) * taille);  // Allocation dynamique

    // Initialiser tous les bits à 0
    for (int i = 0; i < taille; ++i) {
        nb.Tdigits[i] = 0;
    }
    return nb;
}

/**
 * \brief Crée un BigBinary depuis une chaîne de caractères binaire
 * \param chaîne Chaîne contenant uniquement des caractères '0' et '1'
 * \return Un BigBinary représentant le nombre binaire
 *
 * === Phase 1, §6.5: Initialiser un "BigBinary" ===
 *
 * Algorithme:
 * 1. Compter le nombre de bits valides dans la chaîne
 * 2. Allouer le tableau de bits
 * 3. Remplir le tableau en convertissant les caractères en entiers
 * 4. Détecter si tous les bits sont à 0 (dans ce cas, signe = 0)
 *
 * Stockage: Tdigits[0] = MSB (bit de poids fort), Tdigits[Taille-1] = LSB
 */
BigBinary creerBigBinaryDepuisChaine(const char *chaine) {
    BigBinary nb;
    int n = strlen(chaine);
    nb.Taille = 0;

    // Étape 1: Compter uniquement les caractères valides ('0' ou '1')
    // On ignore les autres caractères s'il y en a
    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            nb.Taille++;
        }
    }

    // Étape 2: Allouer le tableau de bits
    nb.Tdigits = malloc(sizeof(int) * nb.Taille);
    nb.Signe = +1;  // Par défaut, on suppose positif

    // Variables pour le remplissage
    int index = 0;      // Position actuelle dans le tableau de sortie
    int tousZeros = 1;  // Flag pour détecter si tous les bits sont à 0

    // Étape 3: Remplir le tableau bit par bit
    for (int i = 0; i < n; ++i) {
        if (chaine[i] == '0' || chaine[i] == '1') {
            // Convertir le caractère '0' ou '1' en entier 0 ou 1
            nb.Tdigits[index] = chaine[i] - '0';

            // Si on trouve un bit à 1, alors ce n'est pas tous des zéros
            if (nb.Tdigits[index] == 1) {
                tousZeros = 0;
            }
            index++;
        }
    }

    // Étape 4: Si tous les bits sont à 0, le nombre est nul (signe = 0)
    if (tousZeros) nb.Signe = 0;

    return nb;
}

/**
 * \brief Crée un BigBinary depuis un entier décimal (long long)
 * \param nombre Entier décimal à convertir
 * \return Un BigBinary représentant le nombre décimal en binaire
 *
 * === AJOUT: Conversion décimal vers binaire ===
 *
 * Algorithme de conversion:
 * 1. Gérer le cas spécial du zéro
 * 2. Déterminer le signe et la valeur absolue
 * 3. Compter le nombre de bits nécessaires (via décalages)
 * 4. Remplir le tableau de bits de droite à gauche (division par 2)
 *
 * Note: On utilise unsigned long long pour gérer LLONG_MIN car
 * abs(LLONG_MIN) déborde en C.
 */
BigBinary creerBigBinaryDepuisDecimal(long long nombre) {
    BigBinary nb;

    // === CAS SPÉCIAL: ZÉRO ===
    // Le nombre 0 est un cas particulier: 1 bit à 0, signe nul
    if (nombre == 0) {
        nb.Taille = 1;           // 1 bit suffisant pour représenter 0
        nb.Signe = 0;            // Signe nul pour représenter zéro
        nb.Tdigits = malloc(sizeof(int));
        nb.Tdigits[0] = 0;       // Le bit unique est à 0
        return nb;
    }

    // === DÉTERMINER LE SIGNE ET VALEUR ABSOLUE ===
    int signe = (nombre > 0) ? +1 : -1;  // +1 si positif, -1 si négatif
    unsigned long long valeur_absolue;   // Type non-signé pour éviter le débordement

    // === CAS SPÉCIAL: LLONG_MIN ===
    // Le problème: abs(LLONG_MIN) déborde car LLONG_MIN = -2^63
    // et 2^63 ne peut pas être représenté en signed long long
    // Solution: caster en unsigned et ajouter 1
    if (nombre == LLONG_MIN) {
        // LLONG_MAX = 2^63 - 1, donc LLONG_MAX + 1 = 2^63
        valeur_absolue = (unsigned long long)LLONG_MAX + 1;
    } else {
        // Cas normal: valeur absolue simple
        valeur_absolue = (nombre > 0) ? (unsigned long long)nombre
                                      : (unsigned long long)(-nombre);
    }

    // === CALCULER LE NOMBRE DE BITS NÉCESSAIRES ===
    // On compte combien de fois on peut décaler à droite avant d'atteindre 0
    // Exemple: 13 (1101) -> 4 bits car 13 >> 3 = 1
    int bits_needed = 0;
    unsigned long long temp = valeur_absolue;
    while (temp > 0) {
        bits_needed++;     // Un bit de plus
        temp >>= 1;        // Décalage à droite = division par 2
    }

    // === REMPLIR LE TABLEAU DE BITS ===
    // Allocation du tableau
    nb.Taille = bits_needed;
    nb.Signe = signe;
    nb.Tdigits = malloc(sizeof(int) * bits_needed);

    // Remplissage de droite à gauche (du LSB vers le MSB)
    // On prend le reste de la division par 2 (bit de poids faible)
    for (int i = bits_needed - 1; i >= 0; i--) {
        nb.Tdigits[i] = valeur_absolue % 2;   // Bit actuel (0 ou 1)
        valeur_absolue /= 2;                  // Division entière par 2
    }

    return nb;
}

/**
 * \brief Crée un BigBinary représentant zéro
 * \return Un BigBinary initialisé à zéro
 *
 * Fonction utilitaire pour créer rapidement un BigBinary nul.
 * Utilise malloc pour allouer un seul bit à 0.
 */
BigBinary creerZero() {
    BigBinary nb;
    nb.Taille = 1;           // Un seul bit suffit pour zéro
    nb.Signe = 0;            // Signe nul = nombre égal à zéro
    nb.Tdigits = malloc(sizeof(int));
    nb.Tdigits[0] = 0;       // Bit unique = 0
    return nb;
}

/**
 * \brief Affiche un BigBinary en format binaire
 * \param nb BigBinary à afficher
 *
 * === Phase 1, §6.3: Fonction d'affichage ===
 *
 * Affiche d'abord le signe si négatif, puis tous les bits.
 * Si le nombre est nul (signe = 0), affiche simplement "0".
 */
void afficheBigBinary(BigBinary nb) {
    // Afficher le signe moins pour les nombres négatifs
    if (nb.Signe == -1) printf("-");

    // Cas spécial: nombre nul ou taille nulle
    if (nb.Signe == 0 || nb.Taille == 0) {
        printf("0\n");
        return;
    }

    // Afficher chaque bit du MSB (index 0) au LSB (index Taille-1)
    for (int i = 0; i < nb.Taille; ++i) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

/**
 * \brief Libère la mémoire allouée pour un BigBinary
 * \param nb Pointeur vers le BigBinary à libérer
 *
 * === Phase 1, §6.3: Libération de la mémoire ===
 *
 * Important: Cette fonction doit être appelée pour chaque BigBinary
 * créé avec malloc pour éviter les fuites de mémoire.
 * On libère le tableau de bits et on met les champs à zéro.
 */
void libereBigBinary(BigBinary *nb) {
    // Vérifier si le pointeur est déjà NULL (double free protection)
    if (nb->Tdigits != NULL) {
        free(nb->Tdigits);     // Libérer la mémoire du tableau
        nb->Tdigits = NULL;    // Mettre à NULL pour éviter les accès invalides
    }
    nb->Taille = 0;            // Plus de bits
    nb->Signe = 0;             // Signe nul
}

/**
 * \brief Crée une copie indépendante d'un BigBinary
 * \param nb BigBinary à copier
 * \return Une copie.deep du BigBinary
 *
 * Utile quand on ne veut pas modifier l'original.
 * Alloue une nouvelle mémoire et copie tous les bits.
 */
BigBinary copieBigBinary(BigBinary nb) {
    BigBinary copie;

    // Copier les métadonnées
    copie.Taille = nb.Taille;
    copie.Signe = nb.Signe;

    // Allouer nouvelle mémoire pour les bits
    copie.Tdigits = malloc(sizeof(int) * nb.Taille);

    // Copier chaque bit un par un
    for (int i = 0; i < nb.Taille; ++i) {
        copie.Tdigits[i] = nb.Tdigits[i];
    }
    return copie;
}

/**
 * \brief Supprime les zéros de tête (normalise le BigBinary)
 * \param nb Pointeur vers le BigBinary à normaliser
 *
 * === Phase 1, §6.6: Le nombre de digits ===
 *
 * Après certaines opérations, on peut avoir des zéros au début
 * du tableau (MSB). Cette fonction les supprime pour maintenir
 * une représentation canonique (sans zéros inutiles).
 *
 * Exemple: 001101 devient 1101 après normalisation
 */
void normaliser(BigBinary *nb) {
    // Cas spéciale: si déjà vide, mettre signe à 0
    if (nb->Taille == 0) {
        nb->Signe = 0;
        return;
    }

    // === TROUVER LE PREMIER BIT NON-NUL ===
    // On parcourt depuis le début (MSB) pour trouver le premier 1
    int premierePos = 0;
    while (premierePos < nb->Taille && nb->Tdigits[premierePos] == 0) {
        premierePos++;
    }

    // === CAS: TOUS LES BITS SONT ZÉRO ===
    if (premierePos == nb->Taille) {
        nb->Signe = 0;           // Le nombre est nul
        nb->Taille = 1;          // Réduire à un seul bit
        nb->Tdigits[0] = 0;      // Ce bit est 0
        return;
    }

    // === CAS: IL Y A DES ZÉROS DE TÊTE À SUPPRIMER ===
    if (premierePos > 0) {
        // Calculer la nouvelle taille (taille originale - zéros de tête)
        int nouvelleTaille = nb->Taille - premierePos;

        // Allouer nouveau tableau de la bonne taille
        int *nouveauTableau = malloc(sizeof(int) * nouvelleTaille);

        // Copier les bits valides (sans les zéros de tête)
        for (int i = 0; i < nouvelleTaille; ++i) {
            nouveauTableau[i] = nb->Tdigits[premierePos + i];
        }

        // Libérer l'ancien tableau et remplacer par le nouveau
        free(nb->Tdigits);
        nb->Tdigits = nouveauTableau;
        nb->Taille = nouvelleTaille;
    }
}

// ============================================================================
// ========================= PHASE 1: OPÉRATIONS DE BASE ======================
// ============================================================================

/**
 * \brief Compare si deux BigBinary sont égaux
 * \param A Premier BigBinary
 * \param B Deuxième BigBinary
 * \return true si A == B, false sinon
 *
 * === Phase 1, §5.1: Fonctions de comparaison ===
 *
 * Algorithme de comparaison:
 * 1. Si les signes sont différents -> pas égaux
 * 2. Si les deux sont nuls -> égaux
 * 3. Si les tailles sont différentes -> pas égaux
 * 4. Comparer bit par bit (du MSB au LSB)
 */
bool Egal(BigBinary A, BigBinary B) {
    // Règle 1: Signes différents -> pas égaux
    if (A.Signe != B.Signe) return false;

    // Règle 2: Deux zéros sont toujours égaux
    if (A.Signe == 0 && B.Signe == 0) return true;

    // Règle 3: Tailles différentes -> pas égaux
    // (ex: 101 (3 bits) != 1101 (4 bits))
    if (A.Taille != B.Taille) return false;

    // Règle 4: Comparer chaque bit individuellement
    for (int i = 0; i < A.Taille; ++i) {
        if (A.Tdigits[i] != B.Tdigits[i]) return false;
    }

    // Tous les bits sont identiques -> égaux
    return true;
}

/**
 * \brief Compare si A est inférieur à B (pour nombres positifs)
 * \param A Premier BigBinary
 * \param B Deuxième BigBinary
 * \return true si A < B, false sinon
 *
 * === Phase 1, §5.1: Inferieur ===
 *
 * Comparaison par magnitude (nombre de bits puis bit par bit):
 * 1. Si A est nul et B positif -> A < B
 * 2. Si A a moins de bits que B -> A < B
 * 3. Si même nombre de bits, comparer du MSB au LSB
 *
 * Note: On suppose des nombres positifs pour la comparaison magnitude
 */
bool Inferieur(BigBinary A, BigBinary B) {
    // Cas 1: A nul et B positif -> A < B
    if (A.Signe == 0 && B.Signe == 1) return true;
    // Cas inverse: A positif et B nul -> A > B (donc pas <)
    if (A.Signe == 1 && B.Signe == 0) return false;
    // Cas 2: Deux zéros sont égaux (ni < ni >)
    if (A.Signe == 0 && B.Signe == 0) return false;

    // === COMPARAISON PAR NOMBRE DE BITS ===
    // Un nombre avec plus de bits est toujours plus grand
    // Ex: 1100 (4 bits, 12) > 101 (3 bits, 5)
    if (A.Taille < B.Taille) return true;
    if (A.Taille > B.Taille) return false;

    // === COMPARAISON BIT PAR BIT ===
    // Même nombre de bits: on compare du MSB au LSB
    for (int i = 0; i < A.Taille; ++i) {
        // Le premier bit différent détermine le résultat
        if (A.Tdigits[i] < B.Tdigits[i]) return true;
        if (A.Tdigits[i] > B.Tdigits[i]) return false;
    }

    // Tous les bits sont identiques -> A == B (donc pas <)
    return false;
}

/**
 * \brief Compare si A est supérieur à B
 * \param A Premier BigBinary
 * \param B Deuxième BigBinary
 * \return true si A > B, false sinon
 *
 * === Phase 1, §5.1: Superieur ===
 *
 * Simplification logique: A > B si et seulement si
 * A n'est pas inférieur à B ET A n'est pas égal à B
 */
bool Superieur(BigBinary A, BigBinary B) {
    return !Inferieur(A, B) && !Egal(A, B);
}

/**
 * \brief Additionne deux BigBinary positifs
 * \param A Premier opérande
 * \param B Deuxième opérande
 * \return Résultat de A + B
 *
 * === Phase 1, §5.1: Addition (algorithme naïf) ===
 *
 * Algorithme de l'école primaire adapté au binaire:
 * 1. On parcourt les bits de droite à gauche (LSB vers MSB)
 * 2. On additionne les bits avec la retenue
 * 3. Le bit de résultat est (somme % 2)
 * 4. La nouvelle retenue est (somme / 2)
 *
 * Exemple: 1010 (10) + 110 (6) = 10000 (16)
 *   1010
 * +  110
 * -----
 *  10000
 */
BigBinary Addition(BigBinary A, BigBinary B) {
    // === CALCULER LA TAILLE DU RÉSULTAT ===
    // La somme peut avoir un bit de plus que le plus grand opérande
    // Ex: 111 (7) + 1 (1) = 1000 (8) -> 3 bits + 1 bit = 4 bits
    int tailleMax = (A.Taille > B.Taille) ? A.Taille : B.Taille;
    BigBinary resultat = initBigBinary(tailleMax + 1, 1);

    // Variables pour l'algorithme
    int retenue = 0;  // Retenue qui circule (0 ou 1 en binaire)
    int i = 0;        // Index de position (depuis le LSB)

    // === BOUCLE PRINCIPALE: addition bit par bit ===
    // On continue tant qu'il reste des bits ou une retenue
    while (i < A.Taille || i < B.Taille || retenue) {
        // Récupérer le bit de A (0 si on a dépassé sa taille)
        int bitA = (i < A.Taille) ? A.Tdigits[A.Taille - 1 - i] : 0;
        // Récupérer le bit de B (0 si on a dépassé sa taille)
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;

        // Additionner: bitA + bitB + retenue précédente
        // Maximum possible: 1 + 1 + 1 = 3
        int somme = bitA + bitB + retenue;

        // Stocker le bit de résultat (somme modulo 2)
        resultat.Tdigits[tailleMax - i] = somme % 2;
        // Calculer la nouvelle retenue (division entière par 2)
        // Si somme = 0 ou 1 -> retenue = 0
        // Si somme = 2 ou 3 -> retenue = 1
        retenue = somme / 2;

        i++;  // Passer au bit suivant (plus significatif)
    }

    // === NORMALISER ===
    // Supprimer les zéros de tête inutiles
    // Ex: si résultat = 00010000, devient 10000
    normaliser(&resultat);

    return resultat;
}

/**
 * \brief Soustrait deux BigBinary positifs (A >= B requis)
 * \param A Premier opérande (doit être >= B)
 * \param B Deuxième opérande
 * \return Résultat de A - B
 *
 * === Phase 1, §5.1: Soustraction (algorithme naïf) ===
 *
 * Algorithme de l'école primaire avec emprunt (borrow):
 * 1. On parcourt les bits de droite à gauche (LSB vers MSB)
 * 2. Si bitA < bitB + emprunt, on "emprunte" 1 au bit suivant
 * 3. L'emprunt augmente le bitA de 2 (car 2 en binaire = 10)
 *
 * Exemple: 1100 (12) - 101 (5) = 0111 (7)
 *   1100
 * -  101
 * -----
 *   0111
 */
BigBinary Soustraction(BigBinary A, BigBinary B) {
    // Vérification de sécurité: A doit être >= B
    // Cette fonction ne supporte pas les nombres négatifs résultants
    if (Inferieur(A, B)) {
        printf("Erreur: Soustraction avec A < B non supportée\n");
        return creerZero();
    }

    // Si A == B, le résultat est 0
    if (Egal(A, B)) {
        return creerZero();
    }

    // Le résultat a la même taille que A (A > B donc même ou plus de bits)
    BigBinary resultat = initBigBinary(A.Taille, 1);
    int emprunt = 0;  // Emprunt à propagé (0 ou 1)

    // === BOUCLE PRINCIPALE: soustraction bit par bit ===
    // On parcourt de droite à gauche (LSB vers MSB)
    for (int i = 0; i < A.Taille; ++i) {
        // Récupérer le bit de A (depuis la droite)
        int bitA = A.Tdigits[A.Taille - 1 - i];
        // Récupérer le bit de B (0 si A est plus long)
        int bitB = (i < B.Taille) ? B.Tdigits[B.Taille - 1 - i] : 0;

        // Calculer la différence: bitA - bitB - emprunt
        int diff = bitA - bitB - emprunt;

        // === GÉRER L'EMPRUNT ===
        // Si diff < 0, cela signifie qu'on a besoin d'emprunter
        // On ajoute 2 (car on emprunte 1 qui vaut 2 en binaire)
        if (diff < 0) {
            diff += 2;   // 2 en binaire = 10, donc -1 + 2 = +1
            emprunt = 1; // L'emprunt se propage au bit suivant
        } else {
            emprunt = 0; // Plus d'emprunt nécessaire
        }

        // Stocker le bit de différence
        resultat.Tdigits[A.Taille - 1 - i] = diff;
    }

    // === NORMALISER ===
    // Supprimer les zéros de tête résultants
    normaliser(&resultat);

    return resultat;
}

// ============================================================================
// ======================= PHASE 2: OPÉRATIONS AVANCÉES =======================
// ============================================================================

/**
 * \brief Divise un BigBinary par 2 (décalage à droite)
 * \param nb Pointeur vers le BigBinary à diviser
 *
 * === Phase 2, §6.2: Division par 2 ===
 *
 * En binaire, diviser par 2 est非常简单: on supprime le bit de
 * poids faible (LSB). C'est juste un décalage à droite.
 *
 * Exemple: 1100 (12) / 2 = 110 (6)
 *          1100 >> 1 = 110
 *
 * Note: Si le nombre est impair, le LSB est perdu (division entière)
 */
void divisePar2(BigBinary *nb) {
    // Ne rien faire si le nombre est nul
    if (nb->Signe == 0 || nb->Taille == 0) return;

    // === CAS SPÉCIAL: LE NOMBRE EST 1 ===
    // 1 / 2 = 0, donc le nombre devient nul
    if (nb->Taille == 1 && nb->Tdigits[0] == 1) {
        nb->Tdigits[0] = 0;  // Devient 0
        nb->Signe = 0;       // Signe nul = nombre nul
        return;
    }

    // === DÉCALAGE À DROITE ===
    // Enlever le bit de poids faible = réduire la taille de 1
    nb->Taille--;

    // === CAS: LA TAILLE DEVIENT 0 ===
    // Cela ne devrait pas arriver avec des nombres valides,
    // mais par sécurité on gère ce cas
    if (nb->Taille == 0) {
        nb->Signe = 0;
        nb->Taille = 1;
        nb->Tdigits[0] = 0;
    }
}

/**
 * \brief Multiplie un BigBinary par 2 (décalage à gauche)
 * \param nb BigBinary à multiplier
 * \return Nouveau BigBinary égal à nb * 2
 *
 * === Phase 2, §6.2: Multiplication par 2 ===
 *
 * En binaire, multiplier par 2 est简单: on ajoute un 0 à la fin
 * (décalage à gauche). La taille augmente de 1.
 *
 * Exemple: 110 (6) * 2 = 1100 (12)
 *          110 << 1 = 1100
 */
BigBinary multiplePar2(BigBinary nb) {
    // 0 * 2 = 0
    if (nb.Signe == 0) return creerZero();

    // Créer un nouveau tableau avec un bit de plus
    BigBinary resultat = initBigBinary(nb.Taille + 1, nb.Signe);

    // === COPIE DES BITS EXISTANTS ===
    // Copier tous les bits de l'original vers le nouveau
    for (int i = 0; i < nb.Taille; ++i) {
        resultat.Tdigits[i] = nb.Tdigits[i];
    }

    // === AJOUT DU 0 À LA FIN ===
    // Ce 0 à la position LSB représente le décalage vers la gauche
    // Ex: 110 (6) -> 1100 (12): le 0 est ajouté à la fin
    resultat.Tdigits[nb.Taille] = 0;

    return resultat;
}

/**
 * \brief Teste si un BigBinary est pair
 * \param nb BigBinary à tester
 * \return true si pair, false si impair
 *
 * === Phase 2, §3.5: Test de parité ===
 *
 * Un nombre binaire est pair si et seulement si son bit de
 * poids faible (LSB) est 0. C'est immediate en binaire!
 *
 * Exemple: 1100 (12) -> LSB = 0 -> pair
 *          1101 (13) -> LSB = 1 -> impair
 */
bool estPair(BigBinary nb) {
    // Le nombre 0 est considéré comme pair par convention
    if (nb.Signe == 0) return true;

    // Vérifier le LSB (dernier bit du tableau)
    // Si c'est 0 -> pair, si c'est 1 -> impair
    return nb.Tdigits[nb.Taille - 1] == 0;
}

/**
 * \brief Teste si un BigBinary est nul
 * \param nb BigBinary à tester
 * \return true si nul (zéro), false sinon
 *
 * Vérification simple: le signe est 0
 */
bool estNul(BigBinary nb) {
    return nb.Signe == 0;
}

/**
 * \brief Multiplication de deux BigBinary par la méthode égyptienne
 * \param A Premier opérande
 * \param B Deuxième opérande
 * \return Résultat de A * B
 *
 * === Phase 2, §4: Multiplication Égyptienne ===
 *
 * Algorithme ancien (encore plus vieux qu'Euclide!) qui utilise
 * le fait que multiplier par 2 est facile (décalage) et que
 * tout nombre peut être décomposé en somme de puissances de 2.
 *
 * Principe: Pour calculer A * B
 * 1. On parcourt les bits de B du LSB vers le MSB
 * 2. Pour chaque bit de B qui est 1, on ajoute A (multiplicande) au résultat
 * 3. À chaque étape, on double A (A = A * 2)
 *
 * Exemple: 13 * 11 avec 13 = 1101 et 11 = 1011
 * Bits de 11 (du LSB au MSB): 1, 1, 0, 1
 *
 * Étape 1: bit = 1, A = 13, résultat = 13
 * Étape 2: bit = 1, A = 26, résultat = 13 + 26 = 39
 * Étape 3: bit = 0, A = 52, résultat = 39 (pas d'ajout)
 * Étape 4: bit = 1, A = 104, résultat = 39 + 104 = 143 ✓
 */
BigBinary MultiplicationEgyptienne(BigBinary A, BigBinary B) {
    // === CAS SIMPLES ===
    // Si A ou B est nul, le résultat est nul
    if (estNul(A) || estNul(B)) {
        return creerZero();
    }

    // Initialiser le résultat à 0
    BigBinary resultat = creerZero();

    // Créer une copie de A qu'on va doubler à chaque itération
    // (multiplicande courant)
    BigBinary multiplicande = copieBigBinary(A);

    // === BOUCLE PRINCIPALE ===
    // On parcourt B de droite à gauche (du LSB au MSB)
    // Tdigits[Taille-1] = LSB, Tdigits[0] = MSB
    for (int i = B.Taille - 1; i >= 0; i--) {
        // === SI LE BIT EST 1: AJOUTER LE MULTIPLICANDE ===
        if (B.Tdigits[i] == 1) {
            BigBinary temp = Addition(resultat, multiplicande);
            libereBigBinary(&resultat);
            resultat = temp;  // Nouveau résultat
        }

        // === DOUBLER LE MULTIPLICANDE POUR L'ÉTAPE SUIVANTE ===
        // (sauf à la dernière itération où ça ne sert à rien)
        if (i > 0) {
            BigBinary temp = multiplePar2(multiplicande);
            libereBigBinary(&multiplicande);
            multiplicande = temp;  // Nouveau multiplicande
        }
    }

    // Libérer le multiplicande temporaire
    libereBigBinary(&multiplicande);
    return resultat;
}

/**
 * \brief Calcule le PGCD de deux BigBinary par l'algorithme binaire d'Euclide
 * \param A Premier nombre
 * \param B Deuxième nombre
 * \return PGCD(A, B)
 *
 * === Phase 1, §3.4-§3.6: Algorithme Binaire d'Euclide ===
 *
    ================================
    Principes de l'algorithme binaire:
    ================================
    1. PGCD(a, 0) = a                                    (trivial)
    2. Si a et b sont pairs: PGCD(a, b) = 2 * PGCD(a/2, b/2)
    3. Si a est pair et b impair: PGCD(a, b) = PGCD(a/2, b)
    4. Si a est impair et b pair: PGCD(a, b) = PGCD(a, b/2)
    5. Si a et b sont impairs: PGCD(a, b) = PGCD(a-b, b)

    ================================
    Avantages du binaire:
    ================================
    - Pas besoin de division euclidienne!
    - Diviser par 2 = simple décalage
    - Multiplier par 2 = simple décalage avec ajout de 0
    - Utilise seulement additions, soustractions et décalages

    ================================
    Exemple: PGCD(51, 57)
    ================================
    51 = 110011, 57 = 111001
    Étapes:
    1. Les deux sont impairs -> PGCD(51-57, 57) = PGCD(6, 57)
    2. 6 est pair, 57 impair -> PGCD(3, 57)
    3. Les deux impairs -> PGCD(3, 57-3) = PGCD(3, 54)
    4. 54 pair -> PGCD(3, 27)
    5. Les deux impairs -> PGCD(3, 27-3) = PGCD(3, 24)
    6. 24 pair -> PGCD(3, 12)
    7. 12 pair -> PGCD(3, 6)
    8. 6 pair -> PGCD(3, 3)
    9. PGCD(3, 3) = 3 ✓
 */
BigBinary PGCD(BigBinary A, BigBinary B) {
    // === CAS TRIVIAUX ===
    // Si A == B, le PGCD est A (ou B)
    if (Egal(A, B)) {
        return copieBigBinary(A);
    }

    // Créer des copies pour ne pas modifier les originaux
    BigBinary a = copieBigBinary(A);
    BigBinary b = copieBigBinary(B);

    // PGCD(a, 0) = a et PGCD(0, b) = b
    if (estNul(b)) {
        libereBigBinary(&b);
        return a;
    }
    if (estNul(a)) {
        libereBigBinary(&a);
        return b;
    }

    // === ÉLIMINER LES FACTEURS 2 COMMUNS ===
    // Compteur pour se souvenir combien de fois on a divisé par 2
    int facteur2 = 0;

    // Tant que a ET b sont pairs, on divise les deux par 2
    // Ce sont des facteurs 2 communs qu'on pourra réintégrer à la fin
    while (estPair(a) && estPair(b)) {
        divisePar2(&a);  // a = a / 2
        divisePar2(&b);  // b = b / 2
        facteur2++;      // Compter les divisions par 2
    }

    // === ALGORITHME BINAIRE D'EUCLIDE ===
    // Tant que a != b, appliquer les règles de simplification
    while (!Egal(a, b)) {
        if (estPair(a)) {
            // Règle 3: a pair, b impair -> PGCD(a, b) = PGCD(a/2, b)
            divisePar2(&a);
        } else if (estPair(b)) {
            // Règle 4: a impair, b pair -> PGCD(a, b) = PGCD(a, b/2)
            divisePar2(&b);
        } else {
            // Règle 5: Les deux sont impairs
            // PGCD(a, b) = PGCD(|a-b|, min(a, b))
            // Ici a et b sont positifs et a != b, donc |a-b| = max(a,b) - min(a,b)
            if (Superieur(a, b)) {
                // a > b -> PGCD(a, b) = PGCD(a-b, b)
                BigBinary diff = Soustraction(a, b);
                libereBigBinary(&a);
                a = diff;
                divisePar2(&a);  // (a-b) est toujours pair après soustraction!
            } else {
                // b > a -> PGCD(a, b) = PGCD(a, b-a)
                BigBinary diff = Soustraction(b, a);
                libereBigBinary(&b);
                b = diff;
                divisePar2(&b);  // (b-a) est toujours pair!
            }
        }
    }

    // === RÉINTÉGRER LES FACTEURS 2 ===
    // On avait divisé a et b par 2, facteur2 fois
    // Donc on doit multiplier le résultat par 2^facteur2
    BigBinary resultat = a;  // a == b à ce stade
    for (int i = 0; i < facteur2; ++i) {
        BigBinary temp = multiplePar2(resultat);
        libereBigBinary(&resultat);
        resultat = temp;
    }

    libereBigBinary(&b);
    return resultat;
}

/**
 * \brief Calcule le modulo: A mod B (sans utiliser la division)
 * \param A Dividende
 * \param B Diviseur (doit être non nul)
 * \return Résultat de A % B
 *
 * === Phase 2, §2: Calcul Modulaire "Division-Free" ===
 *
    ================================
    Principe de l'antiphérèse:
    ================================
    On utilise la relation: A mod B = A - q*B où q est le quotient
    Mais au lieu de calculer q par division, on trouve la plus grande
    puissance de 2 telle que 2^k * B <= A.

    ================================
    Algorithme:
    ================================
    1. Tant que A >= B:
       a. Trouver le plus grand k tel que 2^k * B <= A
       b. Remplacer A par A - 2^k * B
    2. Quand A < B, A est le reste!

    ================================
    Exemple: 192 mod 33
    ================================
    192 / 33 ≈ 5.8, donc 2^2 = 4 <= 5.8 < 2^3 = 8
    Donc on cherche 2^k * B avec k tel que 2^k * B <= 192

    Étape 1: 2^5 * 33 = 1056 > 192 (trop grand)
             2^4 * 33 = 528 > 192 (trop grand)
             2^3 * 33 = 264 > 192 (trop grand)
             2^2 * 33 = 132 <= 192 ✓
             192 - 132 = 60

    Étape 2: 60 >= 33?
             2^1 * 33 = 66 > 60 (trop grand)
             2^0 * 33 = 33 <= 60 ✓
             60 - 33 = 27

    Étape 3: 27 < 33? OUI!
    Donc 192 mod 33 = 27 ✓
 */
BigBinary Modulo(BigBinary A, BigBinary B) {
    // === VÉRIFICATIONS ===
    if (estNul(B)) {
        fprintf(stderr, "Erreur: Division par zéro dans Modulo\n");
        return creerZero();
    }

    // === CAS SIMPLES ===
    // Si A < B, alors A mod B = A
    if (Inferieur(A, B)) {
        return copieBigBinary(A);
    }
    // Si A == B, alors A mod B = 0
    if (Egal(A, B)) {
        return creerZero();
    }

    // Créer une copie de A pour le reste
    BigBinary reste = copieBigBinary(A);

    // === BOUCLE PRINCIPALE: tant que reste >= B ===
    while (!Inferieur(reste, B)) {
        // Trouver la plus grande puissance de 2 telle que 2^k * B <= reste
        // On commence avec B et on le double jusqu'à dépasser reste
        BigBinary B_decale = copieBigBinary(B);

        // Doubler B_decale (= multiplier par 2^k) jusqu'à dépasser reste
        while (true) {
            BigBinary B_double = multiplePar2(B_decale);

            // Si 2^(k+1) * B > reste, on s'arrête
            // B_decale représente maintenant 2^k * B
            if (Superieur(B_double, reste)) {
                libereBigBinary(&B_double);
                break;  // B_decale est la plus grande puissance valide
            }

            // Sinon, continuer avec la prochaine puissance
            libereBigBinary(&B_decale);
            B_decale = B_double;
        }

        // Soustraire 2^k * B du reste
        BigBinary nouveau_reste = Soustraction(reste, B_decale);
        libereBigBinary(&reste);
        libereBigBinary(&B_decale);
        reste = nouveau_reste;
    }

    return reste;
}

/**
 * \brief Division Euclidienne: calcule quotient et reste
 * \param A Dividende
 * \param B Diviseur
 * \return Structure DivisionResult avec quotient et reste
 *
 * === Phase 2, §2: Division Euclidienne (sans division) ===
 *
    ================================
    Même principe que Modulo:
    ================================
    Au lieu de juste soustraire, on accumule les puissances de 2
    dans le quotient. On cherche A = Q*B + R avec R < B.

    ================================
    Exemple: 23 / 5
    ================================
    23 = 4*5 + 3, donc quotient = 4, reste = 3

    Étape 1: Trouver k tel que 2^k * 5 <= 23
             2^2 = 4, 4*5 = 20 <= 23 ✓
             2^3 = 8, 8*5 = 40 > 23 ✗
             Donc k = 2, on soustrait 20: reste = 3
             On ajoute 2^2 = 4 au quotient

    Étape 2: reste = 3 < 5, donc on s'arrête
    Résultat: quotient = 4, reste = 3 ✓
 */
DivisionResult DivisionEuclidienne(BigBinary A, BigBinary B) {
    DivisionResult result;

    // === CAS SPÉCIAUX ===
    if (estNul(B)) {
        fprintf(stderr, "Erreur: Division par zéro dans DivisionEuclidienne\n");
        result.quotient = creerZero();
        result.reste = creerZero();
        return result;
    }

    // Si A < B: quotient = 0, reste = A
    if (Inferieur(A, B)) {
        result.quotient = creerZero();
        result.reste = copieBigBinary(A);
        return result;
    }

    // Si A == B: quotient = 1, reste = 0
    if (Egal(A, B)) {
        result.quotient = creerBigBinaryDepuisChaine("1");
        result.reste = creerZero();
        return result;
    }

    // Initialiser reste = A, quotient = 0
    BigBinary reste = copieBigBinary(A);
    BigBinary quotient = creerZero();

    // === BOUCLE PRINCIPALE ===
    while (!Inferieur(reste, B)) {
        // Trouver le plus grand k tel que 2^k * B <= reste
        BigBinary B_decale = copieBigBinary(B);
        BigBinary puissance2 = creerBigBinaryDepuisChaine("1");  // = 2^0

        // Trouver k maximal tel que 2^k * B <= reste
        while (true) {
            BigBinary B_double = multiplePar2(B_decale);

            if (Superieur(B_double, reste)) {
                // 2^(k+1) * B > reste, donc k est maximal
                libereBigBinary(&B_double);
                break;
            }

            // Continuer: 2^(k+1) * B <= reste
            libereBigBinary(&B_decale);
            B_decale = B_double;

            // Mettre à jour la puissance de 2 (2 * 2^k = 2^(k+1))
            BigBinary p2 = multiplePar2(puissance2);
            libereBigBinary(&puissance2);
            puissance2 = p2;
        }

        // Soustraire 2^k * B du reste
        BigBinary nouveau_reste = Soustraction(reste, B_decale);
        libereBigBinary(&reste);
        reste = nouveau_reste;

        // Ajouter 2^k au quotient
        BigBinary nouveau_quotient = Addition(quotient, puissance2);
        libereBigBinary(&quotient);
        quotient = nouveau_quotient;

        // Libérer les temporaires
        libereBigBinary(&B_decale);
        libereBigBinary(&puissance2);
    }

    result.quotient = quotient;
    result.reste = reste;
    return result;
}

/**
 * \brief Libère la mémoire d'un DivisionResult
 * \param res Pointeur vers le DivisionResult à libérer
 */
void libereDivisionResult(DivisionResult *res) {
    libereBigBinary(&res->quotient);
    libereBigBinary(&res->reste);
}

// ============================================================================
// ==================== PHASE 2: EXPONENTIATION MODULAIRE =====================
// ============================================================================

/**
 * \brief Exponentiation modulaire (version avec exposant BigBinary)
 * \param M Base
 * \param exp Exposant (BigBinary)
 * \param n Module
 * \return M^exp mod n
 *
 * === Phase 2, §3: Exponentiation rapide modulaire ===
 *
    ================================
    Binary Exponentiation (Exponentiation par décalages):
    ================================
    L'idée est de décomposer l'exposant en binaires et de
    réutiliser les résultats intermédiaires.

    Pour calculer M^exp:
    1. Si exp = 0: retourner 1
    2. Si le bit de poids faible de exp est 1:
       résultat = résultat * base
    3. base = base * base
    4. exp = exp / 2 (décalage à droite)
    5. Répéter jusqu'à exp = 0

    Exemple: M^13 mod n où 13 = 1101 (binaire)
    bits de 13 (du LSB au MSB): 1, 0, 1, 1

    résultat = 1
    Itération 1 (bit = 1): résultat = 1 * M = M
    Itération 2 (bit = 0): résultat = M * M^0 = M
    Itération 3 (bit = 1): résultat = M * M^2 = M^3
    Itération 4 (bit = 1): résultat = M^3 * M^8 = M^11... pas tout à fait

    Le vrai algorithme parcourt les bits et combine les puissances
    de manière cumulative avec modulo à chaque étape.
 */
BigBinary ExpMod(BigBinary M, BigBinary exp, BigBinary n) {
    // Si l'exposant est 0, M^0 = 1
    if (estNul(exp)) {
        BigBinary un = creerBigBinaryDepuisChaine("1");
        return un;
    }

    // Initialisations
    BigBinary resultat = creerBigBinaryDepuisChaine("1");
    BigBinary base = Modulo(M, n);      // M réduit modulo n
    BigBinary exposant = copieBigBinary(exp);

    // Boucle tant que l'exposant n'est pas nul
    while (!estNul(exposant)) {
        // Si le bit de poids faible (LSB) de l'exposant est 1
        // On multiplie le résultat par la base courante
        if (exposant.Tdigits[exposant.Taille - 1] == 1) {
            BigBinary temp = multiplicationMod(resultat, base, n);
            libereBigBinary(&resultat);
            resultat = temp;
        }

        // Mettre la base au carré (modulo n)
        BigBinary temp = multiplicationMod(base, base, n);
        libereBigBinary(&base);
        base = temp;

        // Diviser l'exposant par 2 (décalage à droite)
        divisePar2(&exposant);
    }

    libereBigBinary(&base);
    libereBigBinary(&exposant);
    return resultat;
}

/**
 * \brief Exponentiation modulaire (version optimisée pour exp < 64 bits)
 * \param M Base
 * \param exp Exposant (unsigned int, plus petit et plus rapide)
 * \param n Module
 * \return M^exp mod n
 *
 * === Phase 2, §3: Version optimisée pour petits exposants ===
 *
    Même algorithme que ExpMod mais avec un exposant unsigned int.
    Utilise les opérateurs bit à bit natifs du C pour plus d'efficacité.
 */
BigBinary ExpModInt(BigBinary M, unsigned int exp, BigBinary n) {
    // M^0 = 1 par définition mathématique
    if (exp == 0) {
        return creerBigBinaryDepuisChaine("1");
    }

    // Initialisations
    BigBinary resultat = creerBigBinaryDepuisChaine("1");
    BigBinary base = Modulo(M, n);  // Réduire M modulo n d'abord

    unsigned int e = exp;  // Copie locale de l'exposant

    // === BINARY EXPONENTIATION ===
    while (e > 0) {
        // Si le bit de poids faible est 1 (e & 1)
        if (e & 1) {
            // résultat = résultat * base (mod n)
            BigBinary temp = multiplicationMod(resultat, base, n);
            libereBigBinary(&resultat);
            resultat = temp;
        }

        // base = base * base (mod n)
        BigBinary temp = multiplicationMod(base, base, n);
        libereBigBinary(&base);
        base = temp;

        // Décalage à droite: e = e / 2
        e >>= 1;
    }

    libereBigBinary(&base);
    return resultat;
}

/**
 * \brief Multiplication modulaire optimisée
 * \param A Premier opérande
 * \param B Deuxième opérande
 * \param n Module
 * \return (A * B) mod n
 *
 * === Phase 2, §3: Éviter les débordements ===
 *
    Problème: Si A et B sont très grands, A * B peut dépasser
    la capacité de nos structures.

    Solution: On utilise la multiplication égyptienne avec modulo
    à chaque étape pour garder les nombres petits.

    Algorithme:
    - On parcourt les bits de B du MSB au LSB
    - À chaque bit: résultat = résultat * 2 (mod n)
    - Si le bit de B est 1: résultat = résultat + base (mod n)
 */
BigBinary multiplicationMod(BigBinary A, BigBinary B, BigBinary n) {
    // Cas trivial: 0 * anything = 0
    if (estNul(A) || estNul(B)) {
        return creerZero();
    }

    BigBinary resultat = creerZero();
    BigBinary base = Modulo(A, n);  // Réduire A modulo n

    // Parcourir chaque bit de B du MSB (index 0) au LSB
    for (int i = 0; i < B.Taille; i++) {
        // === DOUBLER LE RÉSULTAT (shift left) ===
        // résultat = résultat * 2 mod n
        BigBinary temp = multiplePar2(resultat);
        libereBigBinary(&resultat);
        resultat = Modulo(temp, n);
        libereBigBinary(&temp);

        // === SI LE BIT EST 1: AJOUTER LA BASE ===
        if (B.Tdigits[i] == 1) {
            // résultat = résultat + base mod n
            BigBinary somme = Addition(resultat, base);
            libereBigBinary(&resultat);
            resultat = Modulo(somme, n);
            libereBigBinary(&somme);
        }
    }

    libereBigBinary(&base);
    return resultat;
}

// ============================================================================
// =========================== PHASE 3: RSA (BONUS) ===========================
// ============================================================================

/**
 * \brief Chiffrement RSA: C = M^e mod N
 * \param message Message à chiffrer (BigBinary)
 * \param e Exposant public
 * \param n Module RSA
 * \return Message chiffré C
 *
 * === Phase 2, §5: RSA - Chiffrement ===
 *
    L'algorithme RSA utilise l'exponentiation modulaire:
    - C = M^e mod N

    où:
    - M est le message (converti en nombre)
    - e est l'exposant public (choisi pour être premier avec φ(N))
    - N = p * q est le module (produit de deux nombres premiers)
 */
BigBinary RSA_encrypt(BigBinary message, BigBinary e, BigBinary n) {
    // Le chiffrement RSA est simplement une exponentiation modulaire
    return ExpMod(message, e, n);
}

/**
 * \brief Déchiffrement RSA: M = C^d mod N
 * \param cipher Message chiffré
 * \param d Exposant privé
 * \param n Module RSA
 * \return Message déchiffré M
 *
 * === Phase 2, §5: RSA - Déchiffrement ===
 *
    L'algorithme RSA repose sur le fait que:
    (M^e)^d ≡ M mod N

    où d est l'inverse de e modulo φ(N) = (p-1)(q-1)
    et d est calculé par l'algorithme d'Euclide étendu.

    Le déchiffrement utilise la même opération que le chiffrement:
    - M = C^d mod N
 */
BigBinary RSA_decrypt(BigBinary cipher, BigBinary d, BigBinary n) {
    // Le déchiffrement RSA est aussi une exponentiation modulaire
    return ExpMod(cipher, d, n);
}
