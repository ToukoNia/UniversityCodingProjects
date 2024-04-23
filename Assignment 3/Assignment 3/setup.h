#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED

#include "setupStructs.h" // Include the necessary structs
#include "proceduralGen.h"

#define MAX_HEALTH 10
#define LINEMAX 200

// Function prototypes
FILE *openFile(const char *filename,const char *type);
int setupMonsters(const char *filename, struct monster monsters[]);
int setupItems(const char *filename, struct item items[]);
int setupWeapons(const char *filename, struct weapon weapons[]);
int setup(struct gameController *player,struct monster monsters[NUM_MONSTERS],struct objects *lists,struct roomNode rooms[MAX_ROOMS],int loadSave);

void savePlayerState(const char *filename,struct gameController player,const char *filename2);
void saveInventoryState(const char *filename,struct inventory inv);
void saveRoomState(const char *filename,struct roomNode rooms[MAX_ROOMS]);
void saveMonsterHealth(const char *filename,struct monster monsters[NUM_MONSTERS]);
void saveGame(struct gameController player, struct roomNode rooms[MAX_ROOMS],struct monster monsters[NUM_MONSTERS]);

int loadRooms(char *filename,struct roomNode rooms[MAX_ROOMS]);
int loadInventory(const char *filename,struct inventory inv,struct objects lists);
int loadGameSave(const char *filename, struct gameController *player,struct weapon weapons[NUM_MONSTERS]);

void printStartup();

#endif // SETUP_H_INCLUDED
