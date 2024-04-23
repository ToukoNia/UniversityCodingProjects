#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
/*
This file does all the setup before the main code has to run
*/
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
    char line[LINEMAX]; // sets up framework for smallest line
    int errorOccurred = 0; // Flag variable to track errors
    for (int i = 0; i < NUM_MONSTERS+1 && !errorOccurred; i++) { //loops until either an error is caused, or until all the monsters have populated the script
        if (fgets(line, sizeof(line), fin) != NULL) {
            if (i != 0){ // Skip the first line (header)
                // Parse the line and populate the monster structure
                if (sscanf(line, "'%20[^']',%d,%d,%d,'%200[^']'",
                           monsters[i-1].name,
                           &monsters[i-1].health,
                           &monsters[i-1].lDamage,
                           &monsters[i-1].uDamage,
                           monsters[i-1].description) != 5) //uses scanf to allocate all of the monster's values.
                {
                    fprintf(stderr, "Error: Invalid format in line %d of '%s'.\n", i+1,filename);
                    errorOccurred = 1; // Set flag to indicate error
                }
                monsters[i-1].name[NAME_LEN-1]='\0';
                monsters[i-1].description[DESC_SIZE-1]='\0';  //termination character
            }
        } else {
            fprintf(stderr, "Error: Unable to read line %d of '%s'.\n", i+1,filename);
            errorOccurred = 1; // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

// Function to read items from a file
int setupItems(const char *filename, struct item items[NUM_ITEMS]) {
    FILE *fin = openFile(filename,"r");
    char line[LINEMAX]; // Assuming maximum length of a line
    int errorOccurred=0;
    // Read items from file
    for (int i=0;i<NUM_ITEMS+1 && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "'%20[^']','%200[^']',%d",
                           items[i-1].base.name,
                           items[i-1].base.description,
                           &items[i-1].base.value) != 3){ //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of 'item.txt'.\n", i+1);
                    errorOccurred=1;   // Set flag to indicate error
                }
                items[i-1].base.index=i-1;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of 'item.txt'.\n", i+1);
            errorOccurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

int setupWeapons(const char *filename, struct weapon weapons[NUM_WEAPONS]){
    FILE *fin = openFile(filename,"r");
    char line[LINEMAX]; // Assuming maximum length of a line
    int errorOccurred=0;
    // Read items from file
    for (int i=0;i<NUM_WEAPONS+1 && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "'%20[^']',%d,%d,'%200[^']',%d",
               weapons[i-1].base.name,
               &weapons[i-1].lDamage,
               &weapons[i-1].uDamage,
               weapons[i-1].base.description,
               &weapons[i-1].base.value) != 5) { //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of 'weapons.txt'.\n", i+1);
                    errorOccurred=1;   // Set flag to indicate error
                }
                weapons[i-1].base.index=i-1;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of 'weapons.txt'.\n", i+1);
            errorOccurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

int setupPotions(const char *filename, struct potion potions[NUM_POTIONS]){
    FILE *fin = openFile(filename,"r");
    char line[LINEMAX]; // Assuming maximum length of a line
    int errorOccurred=0;
    // Read items from file
    for (int i=0;i<NUM_POTIONS+1 && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "'%20[^']',%d,'%200[^']',%d",
               potions[i-1].base.name,
               &potions[i-1].healing,
               potions[i-1].base.description,
               &potions[i-1].base.value) != 4) { //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of 'potions.txt'.\n", i+1);
                    errorOccurred=1;   // Set flag to indicate error
                }
                potions[i-1].base.index=i-1;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of 'potions.txt'.\n", i+1);
            errorOccurred=1;   // Set flag to indicate error
        }
    }

    fclose(fin);
    return errorOccurred;
}

void setupGameController(struct gameController *player,struct weapon fist){
    resetInputBuffer();
    printf("Please enter your player's name: ");
    fgets(player->name, sizeof(player->name), stdin);
    player->name[strcspn(player->name, "\n")] = '\0';
    player->inv.itemCount=player->inv.potionCount=0;
    player->health=player->maxHealth=MAX_HEALTH;
    player->currentRoomIndex=0;
    player->lastRoomIndex = 0;
    player->equippedWeapon=fist;
}

void setupRooms(const char *filename,struct roomNode rooms[MAX_ROOMS]){
    GenerateRooms(filename,rooms);
}

int setup(struct gameController *player,struct monster monsters[NUM_MONSTERS],struct objects *lists,struct roomNode rooms[MAX_ROOMS],int loadSave){
    int error=0;

    // Read monsters from file
    error += setupMonsters("monsters.txt", monsters);

    // Read items from file
    error += setupItems("items.txt", lists->items);

    // Read weapons from file
    error += setupWeapons("weapons.txt", lists->weapons);

    // Read potions from file
    error += setupPotions("potions.txt", lists->potions);


    if (loadSave){
        error+=loadGameSave("savePlayer.txt",player,lists->weapons);
        error+=loadRooms("saveRooms.txt",rooms);
        error+=loadInventory("saveInv.txt",player->inv,*lists);

    } else{
        setupRooms("roomTypes.txt",rooms);
        setupGameController(player,lists->weapons[0]);
    }
    if (error) {
        return 1;
    }

    return 0;
}

void savePlayerState(const char *filename,struct gameController player,const char *filename2){
    FILE *fin = openFile(filename,"w");
    fprintf(fin,"name,currentRoomIndex,lastRoomIndex,health,itemCount,potionCount.\n");
    fprintf(fin,"'%s',",player.name);
    fprintf(fin,"%d,",player.currentRoomIndex);
    fprintf(fin,"%d,",player.lastRoomIndex);
    fprintf(fin,"%d,",player.health);
    fprintf(fin,"%d,",player.inv.itemCount);
    fprintf(fin,"%d,",player.inv.potionCount);
    fprintf(fin,"%d",player.equippedWeapon.base.index);
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
    for (int i=0;i<inv.potionCount;i++){
        fprintf(fin,"%d",inv.potions[i].base.index);
        if (i!=inv.potionCount-1){
            fprintf(fin,",");
        }
    }
    fclose(fin);
}

void saveMonsterHealth(const char *filename,struct monster monsters[NUM_MONSTERS]){
    FILE *fin=openFile(filename,"w");
    fprintf(fin,"Health of monsters.\n");
    for (int i=0;i<NUM_MONSTERS;i++){
        fprintf(fin,"%d\n",monsters[i].health);
    }
    fclose(fin);
}
void saveRoomState(const char *filename,struct roomNode rooms[MAX_ROOMS]){
    FILE *fin = openFile(filename,"w");
    fprintf(fin,"type,objectIndex,objectList,monsterIndex,north,east,south,west\n");
     for (int i=0;i<MAX_ROOMS;i++){
        fprintf(fin,"'%s','%s',%d,%d,%d,%d,%d,%d,%d\n",rooms[i].type,rooms[i].description,rooms[i].objectIndex,rooms[i].objectList,rooms[i].monsterIndex,rooms[i].connections[0],rooms[i].connections[1],rooms[i].connections[2],rooms[i].connections[3]);
    }
    fclose(fin);
}

void saveGame(struct gameController player,struct roomNode rooms[MAX_ROOMS],struct monster monsters[NUM_MONSTERS]){
    char choice,validChoices[2]={'Y','N'};
    choice=sanitisedUserInput("Would you like to save the game? Previous saves will be deleted. (Y/N) ", validChoices, 2);
    if (choice=='Y'){
        savePlayerState("savePlayer.txt",player,"saveInv.txt");
        saveRoomState("saveRooms.txt",rooms);
        saveMonsterHealth("saveMonsters.txt",monsters);
    }
}

int loadGameSave(const char *filename,struct gameController *player,struct weapon weapons[NUM_WEAPONS]){
    char line[LINEMAX];
    int errorOccurred=0;
    int equippedWeaponIndex=0;
    FILE *fin = openFile(filename,"r");
    fgets(line, sizeof(line), fin); //skips the first line

    if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
        if (sscanf(line, "%20[^,],%d,%d,%d,%d,%d,%d", player->name, &player->currentRoomIndex, &player->lastRoomIndex,
                &player->health, &player->inv.itemCount, &player->inv.potionCount, &equippedWeaponIndex) != 7){    //scans in the values and assigns them to the player struct
                    fprintf(stderr, "Error: Invalid format in line 2 of '%s'.\n",filename);
                    errorOccurred=1;   // Set flag to indicate error
                }
        }
    fclose(fin);
    player->equippedWeapon=weapons[equippedWeaponIndex];
    player->maxHealth=MAX_HEALTH;
    return errorOccurred;
}

int loadInventory(const char *filename,struct inventory inv,struct objects lists){
    int errorOccurred=0;
    char line[3];
    int index;
    FILE *fin = openFile(filename,"r");
    for (int i=0;i<inv.itemCount++ && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "%d", &index)!=1){
                    fprintf(stderr, "Error: Invalid format in line %d of '%s'.\n", i+1,filename);
                    errorOccurred=1;   // Set flag to indicate error
                } else{
                    inv.items[i-1]=lists.items[index];
                }
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of '%s'.\n", i+1,filename);
            errorOccurred=1;   // Set flag to indicate error
        }

    }
    for (int i=0;i<inv.potionCount++ && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "%d", &index)!=1){
                    fprintf(stderr, "Error: Invalid format in line %d of '%s'.\n", i+1,filename);
                    errorOccurred=1;   // Set flag to indicate error
                } else{
                    inv.potions[i-1]=lists.potions[index];
                }
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of '%s'.\n", i+1,filename);
            errorOccurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

int loadRooms(char *filename,struct roomNode rooms[MAX_ROOMS]){
    FILE *fin = openFile(filename,"r");
    char line[LINEMAX];
    int errorOccurred=0;
    for (int i=0;i<MAX_ROOMS+1 && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "'%20[^']','%200[^']',%d,%d,%d,%d,%d,%d,%d,%d",
                rooms[i-1].type,
                rooms[i-1].description,
                &rooms[i-1].objectIndex,
                &rooms[i-1].objectList,
                &rooms[i-1].monsterIndex,
                &rooms[i-1].connections[0],
                &rooms[i-1].connections[1],
                &rooms[i-1].connections[2],
                &rooms[i-1].connections[3]) != 9) { //scans in the values. If it errors, outputs to the user
                    fprintf(stderr, "Error: Invalid format in line %d of '%s'.\n", i+1,filename);
                    errorOccurred=1;   // Set flag to indicate error
                }
                rooms[i - 1].connectionCount = 0;
                rooms[i - 1].connectionCount = 0;
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of '%s'.\n", i+1,filename);
            errorOccurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

int loadMonsters(char *filename,struct monster monsters[NUM_MONSTERS]){
    FILE *fin =openFile(filename,"r");
    char line[LINEMAX];
    int errorOccurred=0;

    for (int i=0;i<NUM_MONSTERS+1 && !errorOccurred;i++){
        if (fgets(line, sizeof(line), fin) != NULL) {   //reads in the line
            if (i!=0){
                if (sscanf(line, "%d", &monsters[i-1].health)!=1){
                    fprintf(stderr, "Error: Invalid format in line %d of '%s'.\n", i+1,filename);
                    errorOccurred=1;   // Set flag to indicate error
                }
            }
        }
        else{
            fprintf(stderr, "Error: Unable to read line %d of '%s'.\n", i+1,filename);
            errorOccurred=1;   // Set flag to indicate error
        }
    }
    fclose(fin);
    return errorOccurred;
}

