#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fonctions.h"

double valeurRPN[10];
int iPos; 

int main() {
// Initialization
    pileLIFO pile;
    char *stringRPN = malloc(100 * sizeof(char));
    // int typeString;
    valeurRPN[0] = 0;

    creePile(&pile);

    // printf("RPN? 0 1 : ");
        // scanf("%d", &typeString);
        // fflush(stdin);

    scanf("%[^\n]s", stringRPN); printf("\n");

    if(!(verifieRPN(stringRPN))) {
        stringRPN = shuntingYard(stringRPN);
        printf("Il est infixee !\n");
    }


    printf("Le RPN : %s\n\n", stringRPN);
    traduireString(&pile, stringRPN);
    montrerPile(&pile);
    printf("o i e\n");
    printf("\nCalcul RPN : %.2f\n", calculRPN(&pile));
    free(stringRPN);

    return 0;
}
