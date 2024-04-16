#include <stdio.h>
#include <stdlib.h>
#include "setup.h"

FILE *openFile(const char *filename,const char *type) {
    FILE *file = fopen(filename, type);
    if (file == NULL) {
        printf("Error: Unable to open file %s.\n", filename);
        exit(1);
    }
    return file;
}

//Function to read monsters in from a file
int setupMonsters(const char *filename,struct monster monsters[NUM_MONSTERS]) {
    FILE *fin = openFile(filename,"r");
    char line[512]; // Assuming the maximum length of a line in the file
    int errorOccurred = 0; // Flag variable to track errors
    for (int i = 0; i < NUM_MONSTERS+1 && !errorOccurred; i++) { //loops until either an error is caused, or until all the monsters have populated the script
        if (fgets(line, sizeof(line), fin) != NULL) {
            if (i != 0){ // Skip the first line (header)
                // Parse the line and populate the monster structure
                if (sscanf(line, "\"%49[^\"]\",%d,%d,%d,\"%99[^\"]\"",
                           monsters[i-1].name,
                           &monsters[i-1].health,
                           &monsters[i-1].lDamage,
                           &monsters[i-1].uDamage,
                           monsters[i-1].description) != 5) //uses scanf to allocate all of the monster's values.
                {
                    fprintf(stderr, "Error: Invalid format in line %d of 'monster.txt'.\n", i+1);
                    errorOccurred = 1; // Set flag to indicate error
                }
            }
        } else {
            fprintf(stderr, "Error: Unable to read line %d of 'monster.txt'.\n", i+1);
            errorOccurred = 1; // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

// Function to read items from a file
int setupItems(const char *filename, struct item items[NUM_ITEMS]) {
    FILE *fin = openFile(filename,"r");
    char line[512]; // Assuming maximum length of a line
    int error_occurred=0;
    // Read items from file
    for (int i=0;i<NUM_ITEMS+1 && !error_occurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "\"%49[^\"]\",\"%99[^\"]\",%d",
                           items[i-1].base.name,
                           items[i-1].base.description,
                           &items[i-1].base.value) != 3){ //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of 'item.txt'.\n", i+1);
                    error_occurred=1;   // Set flag to indicate error
                }
                items[i-1].base.index=i-1;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of 'item.txt'.\n", i+1);
            error_occurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return error_occurred;
}

int setupWeapons(const char *filename, struct weapon weapons[NUM_WEAPONS]){
    FILE *fin = openFile(filename,"r");
    char line[512]; // Assuming maximum length of a line
    int error_occurred=0;
    // Read items from file
    for (int i=0;i<NUM_WEAPONS+1 && !error_occurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "\"%49[^\"]\",%d,%d,\"%99[^\"]\",%d",
               weapons[i-1].base.name,
               &weapons[i-1].lDamage,
               &weapons[i-1].uDamage,
               weapons[i-1].base.description,
               &weapons[i-1].base.value) != 5) { //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of 'weapons.txt'.\n", i+1);
                    error_occurred=1;   // Set flag to indicate error
                }
                weapons[i-1].base.index=i-1;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of 'weapons.txt'.\n", i+1);
            error_occurred=1;   // Set flag to indicate error
        }
    }
    weapons[0].base.index=-1;
    fclose(fin);
    return error_occurred;
}

int setupPotions(const char *filename, struct potion potions[NUM_POTIONS]){
    FILE *fin = openFile(filename,"r");
    char line[512]; // Assuming maximum length of a line
    int error_occurred=0;
    // Read items from file
    for (int i=0;i<NUM_POTIONS+1 && !error_occurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "\"%49[^\"]\",%d,\"%99[^\"]\",%d",
               potions[i-1].base.name,
               &potions[i-1].healing,
               potions[i-1].base.description,
               &potions[i-1].base.value) != 4) { //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of 'potions.txt'.\n", i+1);
                    error_occurred=1;   // Set flag to indicate error
                }
                potions[i-1].base.index=i-1;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of 'potions.txt'.\n", i+1);
            error_occurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return error_occurred;
}

void setupGameController(struct gameController *player,struct weapon fist){
    strcpy(player->name,"Alexys");
    player->inv.itemCount=player->inv.potionCount=0;
    player->health=player->maxHealth=MAX_HEALTH;
    player->currentRoomIndex=0;
    player->lastRoomIndex = 0;
    player->equippedWeapon=fist;
}

int setup(struct gameController *player,struct monster monsters[NUM_MONSTERS],struct objects *lists){
    int error=0;
    // Read monsters from file
    error += setupMonsters("monsters.txt", monsters);

    // Read items from file
    error += setupItems("items.txt", lists->items);

    // Read weapons from file
    error += setupWeapons("weapons.txt", lists->weapons);

    // Read potions from file
    error += setupPotions("potions.txt", lists->potions);

    if (error) {
        return 1;
    }
    setupGameController(player,lists->weapons[0]);

    return 0;
}

void savePlayerState(const char *filename,struct gameController player,const char *filename2){
    FILE *fin = openFile(filename,"w");
    fprintf(fin,"name,currentRoomIndex,lastRoomIndex,health,itemCount,potionCount.\n");
    fprintf(fin,"%s,",player.name);
    fprintf(fin,"%d,",player.currentRoomIndex);
    fprintf(fin,"%d,",player.lastRoomIndex);
    fprintf(fin,"%d,",player.health);
    fprintf(fin,"%d,",player.inv.itemCount);
    fprintf(fin,"%d",player.inv.potionCount);
    fclose(fin);
    saveInventoryState(filename2,player.inv);
}

void saveInventoryState(const char *filename,struct inventory inv){
    FILE *fin = openFile(filename,"w");
    fprintf(fin,"First %d items are items, and then the next %d are potions.\n",inv.itemCount,inv.potionCount);
    for (int i=0;i<inv.itemCount;i++){
        fprintf(fin,"%d",inv.items[i].base.index);
        if (i!=inv.itemCount-1){
            fprintf(fin,",");
        }
    }
    fprintf(fin,"\n");
    for (int i=0;i<inv.itemCount;i++){
        fprintf(fin,"%d",inv.potions[i].base.index);
        if (i!=inv.itemCount-1){
            fprintf(fin,",");
        }
    }
    fclose(fin);
}

void saveRoomState(const char *filename,struct roomNode rooms[NUM_ROOMS]){
    FILE *fin = openFile(filename,"w");
    fprintf(fin,"type,objectIndex,objectList,monsterIndex,north,east,south,west\n");
     for (int i=0;i<NUM_ROOMS;i++){
        fprintf(fin,"%s,%d,%d,%d,%d,%d,%d,%d\n",rooms[i].type,rooms[i].objectIndex,rooms[i].objectList,rooms[i].monsterIndex,rooms[i].connections[0],rooms[i].connections[1],rooms[i].connections[2],rooms[i].connections[3]);
    }
}

void saveGame(struct gameController player,struct roomNode rooms[NUM_ROOMS]){
    char choice,validChoices[2]={'Y','N'};
    choice=sanitisedUserInput("Would you like to save the game? Previous saves will be deleted. (Y/N) ", validChoices, 2);
    if (choice=='Y'){
        savePlayerState("savePlayer.txt",player,"saveInv.txt");
        saveRoomState("saveRooms.txt",rooms);
    }
}

void loadGameSave(struct gameController *player,struct roomNode *rooms[NUM_ROOMS],struct objects lists){
    char line[500];
    FILE *fin = openFile("savePlayer.txt,"w");
    fgets(line, sizeof(line), fin);
    if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
        if (sscanf(line, "%19[^,],%d,%d,%d,%d,%d", player->name, player.currentRoomIndex, player.lastRoomIndex,
                player.health, player.inv.itemCount, player.inv.potionCount)!=6){
                    fprintf(stderr, "Error: Invalid format in line %d of 'savePlayer.txt'.\n", i+1);
                    error_occurred=1;   // Set flag to indicate error
                }
        }
    fclose(fin);

}


