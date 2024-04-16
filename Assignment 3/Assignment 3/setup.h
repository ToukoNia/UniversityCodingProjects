#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED

#include "setupStructs.h" // Include the necessary structs
#define NUM_MONSTERS 4
#define NUM_ROOMS 2
#define MAX_HEALTH 10
#define LINEMAX 200

// Function prototypes
int setupMonsters(const char *filename, struct monster monsters[]);
int setupItems(const char *filename, struct item items[]);
int setupWeapons(const char *filename, struct weapon weapons[]);
int setup(struct gameController *player,struct monster monsters[NUM_MONSTERS],struct objects *lists);

void savePlayerState(const char *filename,struct gameController player,const char *filename2);
void saveInventoryState(const char *filename,struct inventory inv);
void saveRoomState(const char *filename,struct roomNode rooms[NUM_ROOMS]);
void saveGame(struct gameController player, struct roomNode rooms[NUM_ROOMS]);

#endif // SETUP_H_INCLUDED
