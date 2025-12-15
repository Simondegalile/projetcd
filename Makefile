# Makefile pour le projet BigBinary
# Algorithme Binaire de Calcul du PGCD

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = bigbinary
TARGET_PHASE2 = test_phase2
TARGET_INTERACTIVE = interactive
OBJS = main.o bigbinary.o
OBJS_PHASE2 = test_phase2.o bigbinary.o
OBJS_INTERACTIVE = interactive.o bigbinary.o

# Règle par défaut - compilation seulement
all: $(TARGET) $(TARGET_PHASE2) $(TARGET_INTERACTIVE)

# Compilation de l'exécutable principal
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation de l'exécutable de test phase 2
$(TARGET_PHASE2): $(OBJS_PHASE2)
	$(CC) $(CFLAGS) -o $(TARGET_PHASE2) $(OBJS_PHASE2)

# Compilation de l'exécutable interactif
$(TARGET_INTERACTIVE): $(OBJS_INTERACTIVE)
	$(CC) $(CFLAGS) -o $(TARGET_INTERACTIVE) $(OBJS_INTERACTIVE)

# Compilation des fichiers objets
main.o: main.c bigbinary.h
	$(CC) $(CFLAGS) -c main.c

test_phase2.o: test_phase2.c bigbinary.h
	$(CC) $(CFLAGS) -c test_phase2.c

interactive.o: interactive.c bigbinary.h
	$(CC) $(CFLAGS) -c interactive.c

bigbinary.o: bigbinary.c bigbinary.h
	$(CC) $(CFLAGS) -c bigbinary.c

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJS) $(OBJS_PHASE2) $(OBJS_INTERACTIVE) \
	      $(TARGET) $(TARGET).exe \
	      $(TARGET_PHASE2) $(TARGET_PHASE2).exe \
	      $(TARGET_INTERACTIVE) $(TARGET_INTERACTIVE).exe

# Exécution du programme principal
run: $(TARGET)
	./$(TARGET)

# Exécution des tests phase 2
test: $(TARGET_PHASE2)
	./$(TARGET_PHASE2)

# Exécution du programme interactif
run-interactive: $(TARGET_INTERACTIVE)
	./$(TARGET_INTERACTIVE)

# Aide
help:
	@echo "Makefile pour le projet BigBinary"
	@echo ""
	@echo "Commandes disponibles:"
	@echo "  make                - Compile tous les programmes"
	@echo "  make run            - Compile et exécute le programme principal"
	@echo "  make test           - Compile et exécute les tests phase 2"
	@echo "  make run-interactive - Compile et execute le programme interactif"
	@echo "  make clean          - Supprime les fichiers compiles"
	@echo "  make help           - Affiche cette aide"

.PHONY: all clean run test run-interactive help
