#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "roomOptions.h"

void removeMonster(int *monsterIndex){
    *monsterIndex=-1;
}

void switchRoom(int *currentRoomIndex,int room,int *lastRoomIndex){
    *lastRoomIndex=*currentRoomIndex;
    *currentRoomIndex=room;
}

void printRoomDirections(struct roomNode room,int first) {
    int count=0;

    if (room.connectionCount>1){
        printf("%d exits, including the one you entered from.\n", room.connectionCount);
        printf("These are on the ");
    } else if (first){
        printf("You can only see one exit in front of you.\n");
        printf("This is on the ");
    }else{
        printf("It appears to be a dead end, you can only return the way you came.\n");
        printf("This is on the ");
    }

    for (int i = 0; i < 4; i++) {
        if (room.connections[i] != -1) {
            if (count) {
                if (i == room.connectionCount - 1) {
                    printf(", and ");
                } else {
                    printf(", ");
                }
            }
            if (i == 0) {
                printf("North");
                count++;
            } else if (i == 1) {
                printf("East");
                count++;
            } else if (i == 2) {
                printf("South");
                count++;
            } else if (i == 3) {
                printf("West");
                count++;
            }
        }
    }
    printf(" side.\n");
}

void lookAroundRoom(struct gameController *player, struct roomNode *room,struct objects lists){
    char choice,validOptions[2]={'P','L'};
    printf("You look around the room.\n\n%s\n",room->description);
    printRoomDirections(*room,player->currentRoomIndex==player->lastRoomIndex);
    if (room->objectIndex!=-1){
        void *object;
        printf("There is an item on the floor!\n");
        switch (room->objectList){
            case 0:
                object=(void *)&lists.items[room->objectIndex];
                printf("\nType: Item\n");
                break;
            case 1:
                object=(void *)&lists.potions[room->objectIndex];
                printf("\nType: Potion\n");
                break;
            case 2:
                object=(void *)&lists.weapons[room->objectIndex];
                printf("\nType: Weapon\n");
                printf("Current Equipped Weapon Stats:\n");
                printItemInfo(&player->equippedWeapon,2);
                break;
        }
        printItemInfo(object,room->objectList);
        choice = sanitisedUserInput("Would you like to [P]ick it up or [L]eave it? ",validOptions,2);
        if (choice == 'P'&&room->objectList!=2){
            room->objectIndex=pickupItem(&player->inv,object,room->objectList);
        } else if (choice == 'P'&&room->objectList==2){
            room->objectIndex=equipWeapon(player,*(struct weapon*)object);
        } else if (choice == 'L'){
            printf("You left the item.\n");
        }
    }
}

int roomOptions(struct gameController *player,struct roomNode *room, struct objects lists){
    int index,checked,flag;
    char choice,validOptions[3]={'L','O','E'};
    do{
        choice = sanitisedUserInput("Would you like to [L]ook around the room, [O]pen the menu or [E]xit the room? ",validOptions,3);
        if (choice=='L'){
            lookAroundRoom(player,room,lists);
            checked=1;
        }
        else if (choice=='O'){
            flag=mainMenu(player);
            if (flag){
                printf("You are exiting the program.\n Thank you for playing.\n");
                return 1;
            }
        }
        else if (choice == 'E') {
            if (!checked){
                printRoomDirections(*room,player->currentRoomIndex==player->lastRoomIndex);
            }
            index=sanitisedUserDirectionInput(room->connections);
            if (index!=-1){
                switchRoom(&player->currentRoomIndex,room->connections[index],&player->lastRoomIndex);
            } else{
                printf("You stayed in the room.\n");
                choice=='S';    //continues the loop
            }
        }
    } while (choice!='E');
    return 0;
}

int enterRoom(struct gameController *player,struct roomNode *room,struct monster *enemy, struct objects lists){
    int combatOutcome,flag;
    flag=combatOutcome=1;

    printf("You have entered a %s.\n",room->type);

    if (room->monsterIndex!=-1){
        combatOutcome=combatInitiate(player,*enemy);
        if (combatOutcome==1){
            removeMonster(&room->monsterIndex);
        }
    }
    if (combatOutcome==1) {
        flag=roomOptions(player,room,lists);
        if (flag){
            return 2;
        }
    }
    else if (combatOutcome==0){
        printf("You have fled to the last room you were in.\n");
        switchRoom(&player->currentRoomIndex,player->lastRoomIndex,&player->lastRoomIndex);
    }

    if (combatOutcome==2){   //Raises exit flag
        return 1;
    }

    return 0;

}
