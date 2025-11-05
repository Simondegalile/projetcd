# Makefile pour le projet BigBinary
# Algorithme Binaire de Calcul du PGCD

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = bigbinary
TARGET_PHASE2 = test_phase2
OBJS = main.o bigbinary.o
OBJS_PHASE2 = test_phase2.o bigbinary.o

# Règle par défaut
all: $(TARGET) $(TARGET_PHASE2)

# Compilation de l'exécutable principal
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation de l'exécutable de test phase 2
$(TARGET_PHASE2): $(OBJS_PHASE2)
	$(CC) $(CFLAGS) -o $(TARGET_PHASE2) $(OBJS_PHASE2)

# Compilation des fichiers objets
main.o: main.c bigbinary.h
	$(CC) $(CFLAGS) -c main.c

test_phase2.o: test_phase2.c bigbinary.h
	$(CC) $(CFLAGS) -c test_phase2.c

bigbinary.o: bigbinary.c bigbinary.h
	$(CC) $(CFLAGS) -c bigbinary.c

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJS) $(OBJS_PHASE2) $(TARGET) $(TARGET).exe $(TARGET_PHASE2) $(TARGET_PHASE2).exe

# Exécution du programme principal
run: $(TARGET)
	./$(TARGET)

# Exécution des tests phase 2
test: $(TARGET_PHASE2)
	./$(TARGET_PHASE2)

# Aide
help:
	@echo "Makefile pour le projet BigBinary"
	@echo ""
	@echo "Commandes disponibles:"
	@echo "  make           - Compile tous les programmes"
	@echo "  make run       - Compile et exécute le programme principal"
	@echo "  make test      - Compile et exécute les tests phase 2"
	@echo "  make clean     - Supprime les fichiers compilés"
	@echo "  make help      - Affiche cette aide"

.PHONY: all clean run test help
