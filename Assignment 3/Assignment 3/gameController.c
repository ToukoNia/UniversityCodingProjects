#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameController.h"

#define MAX_ITEMS 10

void drinkPotion(int *health, int maxHealth, int healing){ //manages drinking the potion, and changing health.
    int prevHealth=*health;
    changeHealth(health,healing,maxHealth);
    if (healing>0){
        printf("You drank a healing potion! You regain %d health.\n",*health-prevHealth);
    }
    else{
        printf("Ouch! You drank a poison potion. You lost %d health.\n",*health-prevHealth);
    }
}

void changeHealth(int *health, int amount,int healthMax){   //function to increase code readability, and it means health can never go below 0 or above the max health
    *health=(*health+amount > 0) ? ((*health+amount < healthMax) ? *health+amount : healthMax) : 0;
}

void addObjectToInv(struct inventory *inv,void *equipment,int type,int index){  //this object takes either a potion or weapon and adds it to the respective inventory slot.
    if (type==1){
        inv->potions[index]=*(struct potion*)equipment;
    }
    else if (type == 0){
        inv->items[index]=*(struct item*)equipment;
    }
    else{
        printf("ERROR: INVALID TYPE");
    }
    printf("You picked up %s!\n",((struct base*)equipment)->name);
}

int pickupItem(struct inventory *inv,void *equipment,int type){ //handles the logic for figuring out if an item can be picked up
    char choice;
    int index;
    int objectIndex=-1;
    char validChoices[2]={'Y','N'};

    if ((type == 0 && inv->itemCount == 10) || (type == 1 && inv->potionCount == 10)) {
        choice = sanitisedUserInput("Your inventory is full, would you like to drop something? (Y/N) ", validChoices, 2);
        if (choice == 'Y') {
            if (type == 0) {
                printInventory(*inv);
            } else {
                printPotionInventory(*inv);
            }
            index = sanitisedUserNumberInput("Enter a number between 1 and 10 of the item you would like to drop: ", 1, 10) - 1;
            if (type == 0) {
                printf("You dropped %s.\n", inv->items[index].base.name);
            } else {
                printf("You dropped %s.\n", inv->potions[index].base.name);
            }
            objectIndex = (type == 0) ? inv->items[index].base.index : inv->potions[index].base.index;
            addObjectToInv(inv, equipment, type, index);
        } else {
            printf("Your inventory is full and you declined to pick it up.\n");
        }
    } else {
        addObjectToInv(inv, equipment, type, (type == 0) ? inv->itemCount : inv->potionCount);
        inv->itemCount += (type == 0);
        inv->potionCount += (type == 1);
    }

    return objectIndex;
}

void printPotionInventory(struct inventory inv){    //prints out the potion inventory
    printSeparator();
    if (inv.potionCount>0){
        printf("Potion Inventory:\n");
        for (int i=0;i<inv.potionCount;i++){
            printf("%d: %s\n",i+1,inv.potions[i].base.name);
        }
    }
    else{
        printf("Potion Inventory is currently empty.\n");
    }
    printSeparator();
}

void printInventory(struct inventory inv){  //prints out the standard inventory
    printSeparator();
    if (inv.itemCount>0){
        printf("General Inventory:\n");
        for (int i=0;i<inv.itemCount;i++){
            printf("%d: %s\n",i+1,inv.items[i].base.name);
        }
    }
    else{
        printf("General Inventory is currently empty.\n");
    }
    printSeparator();
}

void removeItemFromArray(void *arr[MAX_ITEMS], int index,int *count){   //remove an object from the respective array, while still allowing other functions to work
    if (*count==1){
        arr[index]=NULL;
    } else{
        for (int i=index;i<*count;i++){
            if (i==index){
                arr[i]=NULL;
            }
            else{
                arr[i-1]=arr[i];
            }
        }
        arr[*count]=NULL;
    }
    *count-=1;
}

void choosePotion(struct gameController *player){   //handles the logic
    int index=0;
    char validChoices[2]={'Y','N'};
    char choice;
    if (player->inv.potionCount!=0){
        printf("\n");
        if (player->inv.potionCount>1){
            printPotionInventory(player->inv);
            index=sanitisedUserNumberInput("Enter the number of the potion you would like to drink: ",1,player->inv.potionCount)-1;
            printf("This is your %s.\n",player->inv.potions[index].base.name);
            choice = sanitisedUserInput("Would you like to drink it? (Y/N) ",validChoices,2);
        } else if (player->inv.potionCount==1){
            printf("Your only potion is your %s.\n",player->inv.potions[index].base.name);
            choice = sanitisedUserInput("Would you like to drink it? (Y/N) ",validChoices,2);
        }
        if (choice=='Y'){
            drinkPotion(&player->health,player->maxHealth,player->inv.potions[index].healing);  //passes in the values required to preform drink potion
            removeItemFromArray(&player->inv.potions,index,&player->inv.potionCount);
        } else{
            printf("You did not drink the potion.\n");
        }

    } else{
        printf("You have no potions.\n");
    }
}


void printItemInfo(void *item,int type){
    struct base *object = (struct base*) item;
    printSeparator();
    printf("ITEM INFO:\n");
    printf("Name: %s\n",object->name);
    printf("Description: %s\n",object->description);
    if (type==2){
        struct weapon *weaponObject = (struct weapon*) item;
        printf("Damage Range: %d-%d.\n",weaponObject->lDamage,weaponObject->uDamage);
    }
    printSeparator();
}

int equipWeapon(struct weapon *equippedWeapon,struct weapon wp){
    int index;
    printSeparator();
    index=equippedWeapon->base.index;
    printf("You equipped %s instead of %s.\n",wp.base.name,equippedWeapon->base.name);
    printf("Stat Changes:\n");
    printf("Lower Damage: %d -> %d.\n",equippedWeapon->lDamage,wp.lDamage);
    printf("Upper Damage: %d -> %d.\n",equippedWeapon->uDamage,wp.uDamage);
    *equippedWeapon=wp;
    printSeparator();
    return index;

}
void printPlayerStats(struct gameController player){
    printSeparator();
    printf("Player Name: %s\n",player.name);
    printf("Health: %d\n",player.health);
    printf("Current Weapon:\n");
    printItemInfo(&player.equippedWeapon,2);
    printf("You currently have %d items, of which %d are potions.\n",player.inv.itemCount+player.inv.potionCount,player.inv.potionCount);
    printSeparator();
}
int mainMenu(struct gameController *player){
     char choice;
     char validOptions1[4]={'O','V','D','E'};
     char validOptions2[2]={'Y','N'};
     printSeparator();
     printf("MAIN MENU:\n");
     choice = sanitisedUserInput("Would you like to [O]pen your inventory, [V]iew your stats, [D]rink a potion or [E]xit the game? ",validOptions1,4);
     switch (choice){
        case 'O':
            printInventory(player->inv);
            printPotionInventory(player->inv);
            break;
        case 'V':
            printPlayerStats(*player);
            break;
        case 'D':
            choosePotion(player);
            break;
        case 'E':
            choice = sanitisedUserInput("Are you sure you would like to exit? (Y/N) ",validOptions2,2);
            if (choice == 'Y'){
                printSeparator();
                printf("\n");
                return 2;
            }
            break;
     }
    return 0;
}

