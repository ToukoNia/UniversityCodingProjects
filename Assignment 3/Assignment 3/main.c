#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "setup.h"
#include "roomOptions.h"

//Text Based Adventure Game


void ConstructRoom(struct roomNode* room, const char name[NAME_LEN], const char description[DESC_SIZE],
                   const int objectIndex, const int objectList, const int monsterIndex,
                   int north,int east,int south,int west){
    strcpy(room->type,name);
    strcpy(room->description,description);
    room->objectIndex=objectIndex;
    room->objectList=objectList;
    room->monsterIndex=monsterIndex;

    room->connections[0] = north;
    room->connections[1] = east;
    room->connections[2] = south;
    room->connections[3] = west;
    room->connectionCount=0;
    if (north != -1) room->connectionCount++;
    if (east != -1) room->connectionCount++;
    if (south != -1) room->connectionCount++;
    if (west != -1) room->connectionCount++;
}



/*
void SetupRooms(){
    struct roomNode path[13];
    ConstructRoom(&path[0],"Hallway","A long hallway, with cobbled stone walls. You can see an opening at the end.",&path[1]);
    ConstructRoom(&path[1],"Large Room","A large room with shadows cast by flickering lights. It has slime covering the walls.",-1,2,0,11);
    ConstructRoom(&path[2],"Hallway","It is a long hallway. There is a torch on the stone walls.",3,-1,-1,1);
    ConstructRoom(&path[3],"Hallway","There is a torch on the stone walls, with a door at the end.",3,-1,-1,1);

}
*/

// Function to open a file and return a file pointer

int main() {
    struct gameController player;
    struct monster monsters[NUM_MONSTERS];
    struct roomNode rooms[NUM_ROOMS];
    struct roomNode *currentRoom;
    struct objects lists;
    int halt=0,error=0;
    error=setup(&player,monsters,&lists);

    ConstructRoom(&rooms[0],"Hallway","It's a long hallway, with cobbled stone walls. You can see an opening at the end.",0,1,-1,1,-1,-1,-1);
    ConstructRoom(&rooms[1],"Large Room","It's a large room with shadows cast by flickering lights. It has slime covering the walls.",-1,0,0,-1,-1,0,-1);

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
        saveGame(player,rooms);
    }
}
