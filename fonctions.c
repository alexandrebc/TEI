#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "fonctions.h"

#define ILESTNOMBRE valeurIndiv[0] >= '0' && valeurIndiv[0] <= '9'
#define ILESTVARIABLE (valeurIndiv[0] >= 'a' && valeurIndiv[0] <= 'z') && (strlen(valeurIndiv) == 1)
#define PRIORITEDEUX valeurIndiv[0] == '+' || valeurIndiv[0] == '-'
#define PRIORITETROIS valeurIndiv[0] == '*' || valeurIndiv[0] == '/'
#define PRIORITEQUATRE valeurIndiv[0] == '^'

// Creation de la Pile
void creePile(pileLIFO *p) {
    p->debut = NULL;
    p->taille = 0;
}

// Fonction pour mettre un objet dans de la pile
int push(pileLIFO *p, char *valeurToken, int typeToken) {
    tokenRPN *nouveauToken;
    int tailleVToken = strlen(valeurToken);

    // Code pour valider si est possible de faire la reallocation de memoire pour la pile
    if( (nouveauToken = (tokenRPN*) malloc (sizeof(tokenRPN))) == NULL)
        return -1;
    if( (nouveauToken->valeur = (char*) malloc (tailleVToken+1 * sizeof(char))) == NULL)
        return -1;

    // Si la reallocation étais possible on pendre la pile ancien et copie dans une nouvelle de taille plus large
    strcpy(nouveauToken->valeur, valeurToken);
    nouveauToken->type = typeToken;

    // Faire le 'link' entre la pile et le prochain valeur
    nouveauToken->prochaine = p->debut;

    p->debut = nouveauToken;
    p->taille++;

    return 0;
}

// Fonction pour retirer un objet de la pile
int pop(pileLIFO *p) {
    tokenRPN *p_token;

    if(p->taille == 0)
        return -1;

    p_token = p->debut;
    p->debut = p->debut->prochaine;

    free(p_token->valeur);

    p->taille--;

    return 0;
}

// Fonction pour montrer les objets de la pile avec ses token
void montrerPile(pileLIFO *p){
    tokenRPN *actuel;
    int i;
    actuel = p->debut;


    for(i=0; i<p->taille; i++){
        printf("%s\t%s\n", traduireType(actuel->type), actuel->valeur);
        actuel = actuel->prochaine;
    }
}

// Fonction pour prendre chaque valeur individuelle de la chaîne de caractères et traduire : dit quel ses token e fait le calcul
void traduireString(pileLIFO *p, char *stringRPN) {
    char *valeurIndiv = (char*) strtok(stringRPN, " ");

    while(valeurIndiv != NULL){
        // calculRPN(valeurIndiv, verifieType(valeurIndiv));
        push(p, valeurIndiv, verifieType(valeurIndiv));
        valeurIndiv = (char*) strtok(NULL, " ");
    }

    free(valeurIndiv);
}

// Recupere le valeur implicit de token de la pile et montre en texte
char *traduireType(int valeurType) {
    char *type = (char*)calloc(20, sizeof(char));

    switch(valeurType){
        case 0:
            strcpy(type, "constante");
            break;
        case 1:
            strcpy(type, "operateur binaire");
            break;
        case 2:
            strcpy(type, "variable");
            break;
        case 3:
            strcpy(type, "fonction");
            break;
    }

    return type;
}

// Verifie qu'est-ce que est chaque valeur individuelle : operateur binaire, variable, fonction ou constante
int verifieType(char *valeurIndiv) {
    int tailleValeur = strlen(valeurIndiv);

    if(ILESTNOMBRE)
        return 0;
    else if(PRIORITEDEUX || PRIORITETROIS || PRIORITEQUATRE)
        return 1;
    else if(tailleValeur == 1)
        return 2;
    else
        return 3;
}

// Verifie si la chaîne est du type RPN ou Infixée
int verifieRPN(char *stringRPN) {
    if(( ( stringRPN[strlen(stringRPN)-2] == ' ' ) && ( tolower(stringRPN[strlen(stringRPN)-1]) >= 'a' && tolower(stringRPN[strlen(stringRPN)-1]) <= 'z' )) || stringRPN[strlen(stringRPN)-1] == ')' || ( stringRPN[strlen(stringRPN)-1] >= '0' && stringRPN[strlen(stringRPN)-1] <= '9' ))
        return 0;
    else
        return 1;
}

// Fait le calcul RPN
// void calculRPN(char *valeurToken, int typeToken) {
double calculRPN(pileLIFO *p) {
   
    int typeToken;
    double valeurRPN[10];
    int iPos = 0;
    
    // Pour avoir le debut de la pile pour faire le calcul correct
    //*******//
    
    int i, tStack = 0;
    char *auxStack[100];
    char *valeurToken;
    
    tokenRPN *actuel = p->debut;
    
    for(i=0; i<p->taille; i++) {
        auxStack[tStack++] = actuel->valeur;
        actuel = actuel->prochaine;
    }
    
    //******//
    
    while(tStack > 0) {
        valeurToken = auxStack[--tStack];
        typeToken = verifieType(valeurToken);
        char op = valeurToken[0];
    
        if(typeToken == 0) {
            valeurRPN[iPos] = atoi(valeurToken);
            iPos++;
        } else {
            // Operateur Binaire
            if(typeToken == 1) {
                if(op == '+')
                    valeurRPN[iPos-2] = valeurRPN[iPos-2] + valeurRPN[iPos-1];
                if(op == '-')
                    valeurRPN[iPos-2] = valeurRPN[iPos-2] - valeurRPN[iPos-1];
                if(op == '*')
                    valeurRPN[iPos-2] = valeurRPN[iPos-2] * valeurRPN[iPos-1];
                if(op == '/')
                    valeurRPN[iPos-2] = valeurRPN[iPos-2] / valeurRPN[iPos-1];
                if(op == '^')
                    valeurRPN[iPos-2] = pow(valeurRPN[iPos-2],valeurRPN[iPos-1]);
    
                iPos --;
            }
    
            // Variable
            if(typeToken == 2) {
                char term;
                int valeurVariable;
    
                printf("Il y a une variable! ");
                    do{
                        printf("Mettre un valeur valide pour %c : ", op);
                        fflush(stdin);
                        scanf("%d%c", &valeurVariable, &term);
                    } while (term != '\n');
                    valeurRPN[iPos] = valeurVariable;
                iPos++;
            }
    
            // Fonctions
            if(typeToken == 3) {
                // Les fonctions inverse
                if(!strcmp(valeurToken, "asin"))
                    valeurRPN[iPos-1] = asin(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "acos"))
                    valeurRPN[iPos-1] = acos(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "atan"))
                    valeurRPN[iPos-1] = atan(valeurRPN[iPos-1]);
                
                // Les fonctions
                if(!strcmp(valeurToken, "sin"))
                    valeurRPN[iPos-1] = sin(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "cos"))
                    valeurRPN[iPos-1] = cos(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "tan"))
                    valeurRPN[iPos-1] = tan(valeurRPN[iPos-1]);
                
                // Les Fonctions hiperboliques
                if(!strcmp(valeurToken, "sinh"))
                    valeurRPN[iPos-1] = sinh(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "cosh"))
                    valeurRPN[iPos-1] = cosh(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "tanh"))
                    valeurRPN[iPos-1] = tanh(valeurRPN[iPos-1]);
                
                // Les autres fonctions
                if(!strcmp(valeurToken, "sqrt")) // Racine carree
                    valeurRPN[iPos-1] = sqrt(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "exp")) // Exponenciel
                    valeurRPN[iPos-1] = exp(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "ln")) // Logaritme naturel ?
                    valeurRPN[iPos-1] = log(valeurRPN[iPos-1]);
                if(!strcmp(valeurToken, "log")) // Logaratime base Dix ?
                    valeurRPN[iPos-1] = log10(valeurRPN[iPos-1]);
    
            }
        }
    }
    
    return valeurRPN[0];
}

// Algoritme 'Shunting Yard' pour faire la conversion du type Infixée pour le RPN
char *shuntingYard(char *stringRPN){
#define spush(x) conversionStack[l_stack++].valeur = x
#define spop()   conversionStack[--l_stack].valeur

    char *valeurIndiv = (char*) strtok(stringRPN, " ");
    char *postFixe = malloc(100 * sizeof(char));
    opAssoc conversionStack[100];
    int l_stack = 0;
    int flag = 0;

    strcpy(postFixe, "");

    while(valeurIndiv != NULL) {
        // Si le caractère est constante mettre dans la pile
        if((ILESTNOMBRE) || (ILESTVARIABLE)) {
            strcat(postFixe, valeurIndiv);
            strcat(postFixe, " ");
        
        // Si non, on doit verifier sa priorité et associativité    
        } else {
            if (PRIORITEDEUX) {
                conversionStack[l_stack].priorite = 2;
                conversionStack[l_stack].associativite = 1;
            }
            else if (PRIORITETROIS) {
                conversionStack[l_stack].priorite = 3;
                conversionStack[l_stack].associativite = 1;
            }
            
            else {
                conversionStack[l_stack].priorite = 0;
                conversionStack[l_stack].associativite = 0;
            }
            
            // Verification s'il y a la presence d'un ()
            if (valeurIndiv[0] == '(')
                flag = l_stack;
            
            // Verification de la priorite de l'algoritme
            if (l_stack && conversionStack[l_stack-1].priorite == conversionStack[l_stack].priorite) {
                strcat(postFixe, spop());
                strcat(postFixe, " ");
                
                conversionStack[l_stack].priorite = conversionStack[l_stack+1].priorite;
            }
            
            spush(valeurIndiv);
        }
        
        if(valeurIndiv[0] == ')') {
                l_stack--;
                while(l_stack != flag+1) {
                    strcat(postFixe, spop());
                    strcat(postFixe, " ");
                }
                l_stack--;
        }
        
        valeurIndiv = (char*) strtok(NULL, " ");
    }
    
    // Si il a déjà fini fait le pop jusqu'à le fin
    while(l_stack) {
        strcat(postFixe, spop());
        strcat(postFixe, " ");
    }

    free(valeurIndiv);
    return postFixe;

}
