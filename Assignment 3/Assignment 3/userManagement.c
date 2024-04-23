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

void printHLine(int lineNum){
    printf("%.*s\n", lineNum, line);
}
void printSeparator() {
    printHLine(LINEMAX);
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

int sanitisedUserDirectionInput(int connections[4],int notFirst) {
    char direction;
    int index;
    int count=0;
    int isValidInput = 0;
    char prompt[50];

    if (!notFirst){
        connections[2]=0;
    }

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
                if (connections[0]!=-1){
                    printf("You leave on the North exit.\n");
                    isValidInput = 1;
                    index=0;
                }
                break;
            case 'E':
                if (connections[1]!=-1){
                    printf("You leave on the East exit.\n");
                    isValidInput = 1;// Check if East is a valid direction
                    index=1;
                }
                    break;
            case 'S':
                printf("You leave on the South exit.\n");
                if (connections[2]!=-1){
                    isValidInput = 1;
                    index=2;
                }  // Check if South is a valid direction
                break;
            case 'W':
                printf("You leave on the West exit.\n");
                if (connections[3]!=-1){
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
void waitForEnter(int reset){
    printf("Press enter to continue: ");
    if (reset){
        resetInputBuffer();
    }
    while (getchar() != '\n');
}
void combatHowToPlay(){
    printSeparator();
    printf("COMBAT");
    printf("\nCombat will be turn based, you will attack and then the monster will take its turn.\n");
    printf("You will be given the opportunity to look at your enemy, which reads the description to you.\n");
    printf("In combat, you will have 3 options: to fight, to drink a potion or to run.\n");
    printf("If you fight, you attack. This deals an amount of damage within the range provided.\n");
    printf("If you choose to drink a potion, you get to select and consume the potion. You also get the opportunity to look at the monster again, allowing you to see their health and description.\n");
    printf("If you run, you head back to the last room you where in.\n");
}

int startMenu(int max_items){
    printSeparator();
    printf("Welcome to Death's Run!\n");
    printf("This is a procedurally generated text based adventure game written by Nia Touko.\n\n");
    printf("In the small town you grew up in, there has been a dungeon for as long as you can remember. Adventurers go into the dungeon repeatedly every few weeks to get some of the treasures; hoping to make a fortune for themselves.\n");
    printf("You are one of these few unfortunate adventurers.\n");

    waitForEnter(0);

    printSeparator();
    printf("HOW TO PLAY\n");
    printSeparator();
    printf("You will be given a series of prompts as you progress through the dungeon.\n These will guide you around the dungeon.\n");
    printf("When entering a room, you can look around it, you can move to another room, or you can open your menu.\n");
    printf("You can look around a room to see its description and to look for items in the room.\n");
    printf("You can open the player menu when not in combat. This will let you:\nopen your inventory, open the map, view your stats, drink a potion or exit the game (and save it).\n");
    printf("Items come in 3 types.\n\n");
    printSeparator();
    printf("Potions can be used to regain health. However, some are poisonous, so be careful! You can store up to %d of these in your inventory\n",max_items);
    printf("Items can be stored for the end. You can store up to %d in your inventory.\nBoth of these types of items have a value, which is used at the end to calculate winnings.\n",max_items);
    printf("Weapons change how much damage you can do per hit. You can only store one type of weapon at a time\n");
    printf("As you go round, you may encounter a monster.\n");
    waitForEnter(0);
    combatHowToPlay();
    waitForEnter(0);
    printSeparator();
    printf("The game ends when either you die, you chose to exit, or you leave through the south exit of the Starting Room.\n");
    printf("If you die or leave via the menu (and do not save), your score will be 0.\n");
    printf("Your score is counted from the value of all your items.\n");
    printf("\nMost importantly, have fun!\n");

    char choice,options[2]={'Y','N'};
    choice=sanitisedUserInput("Would you like to load a save game, or start a new one? (Y/N) ",options,2);
    if (choice=='Y'){
        return 1;
    }
    return 0;
}
