#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "setup.h"
#include "roomOptions.h"

/*
Text Based Procedurally Generated Adventure Game
Instructions to play happen when you run the program
Error checking is done throughout to sanitise inputs.
Files are needed to store values. These are errorchecked, and suitable error codes are thrown when an error occurs.
*/

int main() {
    struct gameController player;
    struct monster monsters[NUM_MONSTERS];
    struct roomNode rooms[20];
    struct roomNode *currentRoom;
    struct objects lists;
    int error,score,halt=0;

    srand(time(0));
    error=setup(&player,monsters,&lists,rooms,startMenu(MAX_ITEMS));

    if (error){
        return 1;
    }

    while(!halt){
        currentRoom = &rooms[player.currentRoomIndex];
        if (currentRoom->monsterIndex!=-1){
            halt=enterRoom(&player,currentRoom,&monsters[currentRoom->monsterIndex],lists);
        } else{
            halt=enterRoom(&player,currentRoom,NULL,lists);
        }
        printSeparator();
        printf("\n");
    }
    if (halt==2){
        saveGame(player,rooms,monsters);
    } else if (halt==3){
        score=getPlayerScore(player);
        if (score>999){
            printf("Congrats! You are set for life.\n");
        } else{
            printf("You scored %d!\n",score);
            if (score>200){
                printf("You won't starve this month, congrats.");
            }
        }
    }


    return 0;
}
