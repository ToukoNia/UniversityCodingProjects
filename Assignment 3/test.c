#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "userOptions.h"

void resetInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);    //resets buffer
}

char sanitisedUserInput(char* message,char option1,char option2){
    int error;
    int store;
    do{
        printf("%s",message);   //prints the required question for the user input
        error=scanf(toupper("%s"),&store);  //if non-integer passed in, returns not 1
        if ((error!=option1)&&error!=option2)||error!=1){
            printf("Invalid Input");
            resetInputBuffer;
        }
    } while ((error!=option1)&&error!=option2)||error!=1);
    return store;
}
int enterRoom(struct roomNode room){
    char choice;
    printf("You have entered a %s.\n",room.type);

    if (room.monsterIndex!=-1){
        combatInitiate(monsters[room.monsterIndex]);
    }
    else {
        choice = sanitisedUserInput("Would you like to [L]ook around the room or [E]xit the room the way you came? ",'L','E');
        if (choice=='L'){
            printf("You look around the room.\n%s\n",room.description);
            if (size(room.connections)-1>2){
                printf("You see %d exits, plus the one you came in on.\n",size(room.connections)-1);
                printf("These are on the ")
                for (int i=0;i<4;i++){
                    if (room.connections[i]!=null&&j!=size(room.connections)-1){
                        printf("%s side,");
                        j++;
                    }

                }
            } else{
                printf("It appears to be a dead end.\n")
            }
        }

    }

}
