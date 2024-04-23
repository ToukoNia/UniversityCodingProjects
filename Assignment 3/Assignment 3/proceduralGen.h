#ifndef PROCEDURALGEN_H
#define PROCEDURALGEN_H

#include "setupStructs.h"
#define MAX_ROOMS 20

int placeConnections(int connections[4], int chance, int index, int *lastSide);
int DFS(struct roomNode rooms[MAX_ROOMS]);
void assignRooms(struct roomNode rooms[MAX_ROOMS]);

void GenerateRooms(char *filename,struct roomNode rooms[MAX_ROOMS]);
int getIndex(int *cnt, int indices[MAX_ROOMS]);
void constructRoom(char *name, char *description, int suggestedMonster, struct roomNode *room,int specialRoom);
void placeRandomItem(struct roomNode *room,int *wCount);

#endif /* PROCEDURALGEN_H */
