#ifndef PILE_H_INCLUDED
#define PILE_H_INCLUDED

//Definition de tokens
typedef struct tokenRPN{
    /**
      * type
      * 0 -> constante
      * 1 -> opérateur binaire
      * 2 -> variable
      * 3 -> fonction
      */

    int type;
    char *valeur;
    struct tokenRPN *prochaine;
} tokenRPN;

//Definition du stack de operation
typedef struct opAssoc{
    /**
      * associativite
      * 0 -> droite
      * 1 -> gauche
      */

    int associativite;
    int priorite;
    char *valeur;
} opAssoc;

// Definition de la pile (LIFO - Last In First Out)
typedef struct pileLIFO{
    int taille;
    tokenRPN *debut;
} pileLIFO;

// Fonction pour crée la pile
void creePile(pileLIFO *p);

// Fonctions de la pile
int push(pileLIFO *p, char *valeurToken, int typeToken); // Placer un nouveau token
int pop(pileLIFO *p); // Retirer le dernier token
void montrerPile(pileLIFO *p); // Montre tout la pile

// Autres fonctions
void traduireString(pileLIFO *p, char *stringRPN); // Reçoit une string (RPN) et mettre chaque valeur dans une pile
int verifieType(char *valeurIndiv); // Verifie quel type correspond au nouveau valeur dans la pile
char *traduireType(int valeurType); // Quand le programme vas afficher les tokens; il montre le texte que match au type (int)
int verifieRPN(char *stringRPN); // Verefie si le chaine est RPN ou non
double calculRPN(pileLIFO *p); // Realise le calcul
char *shuntingYard(char *stringRPN); // Shunting Yard Algorithm - Infixe -> Postfixe

#endif
