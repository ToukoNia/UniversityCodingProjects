// roomOptions.h

#ifndef ROOMOPTIONS_H
#define ROOMOPTIONS_H

#include "combat.h"

void removeMonster(int *monsterIndex);
void switchRoom(int *currentRoomIndex, int room, int *lastRoomIndex);
void printRoomDirections(struct roomNode room,int first);
int enterRoom(struct gameController *player,struct roomNode *room, struct monster *enemy, struct objects lists);

#endif // ROOMOPTIONS_H
