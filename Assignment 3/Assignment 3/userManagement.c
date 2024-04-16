#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userManagement.h"
#define line "________________________________________________________________________________________________________________________________________________________________________________________________________"
//line is 200 characters long
#define LINEMAX 120

void resetInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);    //resets buffer
}

void printSeparator() {
    printf("%.*s\n", LINEMAX, line);
}

int sanitisedUserNumberInput(char* message, int lBound, int uBound){
    int store;
    int error;
    do{
        printf("%s",message);   //prints the required question for the user input
        error=scanf(" %d",&store);  //if non-integer passed in, returns not 1
        if (store>uBound||store<lBound||error==0){  //checks that the input is valid, otherwise displays error message
            printf("Invalid Input. Please enter an integer between %d and %d.\n",lBound,uBound);
            resetInputBuffer(); //resets the buffer to prevent infinite looping
        } else if (error=EOF){
            printf("Error: End of file encountered.\n");
            exit(EXIT_FAILURE);  // Terminate program if end of file encountered
        }

    } while(store>uBound||store<lBound||error!=1);  //loops till valid input is declared
    return store;
}

char sanitisedUserInput(char* message,char options[],int numOptions){
    char store;
    int isValidInput = 0;
    int i;

    do {
        printf("%s", message);
        if (scanf(" %c", &store)!=1){
            resetInputBuffer();
        }
        else{
            store = toupper(store); // Convert input to uppercase

            for (i = 0; i < numOptions&&!isValidInput; i++) {
                if (store == toupper(options[i])) {
                    isValidInput = 1;
                }
            }

            if (!isValidInput) {
                printf("Invalid input. Please enter one of the following options: ");
                for (i = 0; i < numOptions; i++) {
                    printf("'%c'", toupper(options[i]));
                    if (i < numOptions - 1) {
                        printf(", ");
                    }
                }
                printf("\n");
                resetInputBuffer();
            }
        }
    } while (!isValidInput);
    return store;
}


int sanitisedUserDirectionInput(int connections[4]) {
    char direction;
    int index;
    int count=0;
    int isValidInput = 0;
    char prompt[50];

    strcpy(prompt, "Enter a direction (");
    for (int i = 0; i < 4; i++) {
        if (connections[i] != -1) {
            if (count != 0) {
                strcat(prompt, ", ");
            }
            if (i == 0) {
                strcat(prompt, "'N'");
                count++;
            }
            else if (i == 1) {
                strcat(prompt, "'E'");
                count++;
            }
            else if (i == 2) {
                strcat(prompt, "'S'");
                count++;
            }
            else if (i == 3) {
                strcat(prompt, "'W'");
                count++;
            }
        }
    }
    strcat(prompt, ") or 'C' to cancel: ");

    while (!isValidInput){
        printf("%s",prompt);
        scanf(" %c", &direction); // Space before %c to consume any leading whitespace
        direction = toupper(direction); // Convert input to uppercase

        switch (direction) {     // Check if North is a valid direction
            case 'N':
                if (connections[0]){
                    printf("You leave on the North exit.\n");
                    isValidInput = 1;
                    index=0;
                }
                break;
            case 'E':
                if (connections[1]){
                    printf("You leave on the East exit.\n");
                    isValidInput = 1;// Check if East is a valid direction
                    index=1;
                }
                    break;
            case 'S':
                printf("You leave on the South exit.\n");
                if (connections[2]){
                    isValidInput = 1;
                    index=2;
                }  // Check if South is a valid direction
                break;
            case 'W':
                printf("You leave on the West exit.\n");
                if (connections[3]){
                    isValidInput = 1;
                    index=3;
                }
                // Check if West is a valid direction
                break;
            case 'C':   //cancel case
                isValidInput=1;
                index=-1;
                break;
            default:
                printf("Invalid input.\n");
        }
    }
    return index;
}


