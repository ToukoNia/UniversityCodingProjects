#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include "userManagement.h"
#include "setupStructs.h"

void drinkPotion(int *health, int maxHealth, int healing);
void changeHealth(int *health, int amount,int maxHealth);
void addObjectToInv(struct inventory *inv, void *equipment, int type, int index);
int pickupItem(struct inventory *inv, void *equipment, int type);
void printPotionInventory(struct inventory inv);
void printInventory(struct inventory inv);
void removeItemFromArray(void *arr[MAX_ITEMS], int index,int *count);
void choosePotion(struct gameController *player);
void printItemInfo(void *item, int type);
int equipWeapon(struct weapon *equippedWeapon,struct weapon wp);
void printPlayerStats(struct gameController player);
int mainMenu(struct gameController *player);

#endif /* GAME_CONTROLLER_H */
