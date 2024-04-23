#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proceduralGen.h"

#define NUM_ROOM_TYPES 15
#define CHANCE_RATE 4
#define LINEMAX 512

int placeConnections(int connections[4],int chance,int index,int *lastSide){    //will place connections randomly across the rooms such that
    for (int side=*lastSide;side<4;side++){
        if (connections[side]==-1){     //if not already a room there
            if ((rand()%chance)==0){    //if 1 in chance occurs, place connection
                connections[side]=index;
                *lastSide=side;         //update the last side such that when this node is called again it doesn't get done twice
                return side;            //if connection placed, return value to halt this.
            }
        }
    }
    return -1;
}
/*
int recursivePlaceConnections(struct roomNode rooms[MAX_ROOMS],int index,int *nextIndex,int total){
    printf("%d\n",*nextIndex);
    unsigned int chance=2+(total/4);
    if (*nextIndex<MAX_ROOMS-1){
        for (int side=0;side<4;side++){
            if (rooms[index].connections[side]==-1){
                if ((rand()%chance)==0){
                    rooms[index].connections[side]=*nextIndex;
                    rooms[*nextIndex].connections[(side+2)%4]=index;
                    (*nextIndex)++;
                    total+=recursivePlaceConnections(rooms,*nextIndex-1,*nextIndex,total);
                }
            }
        }
    }
    return total;
}
*/

int DFS(struct roomNode rooms[MAX_ROOMS]){  //preforms a Depth First Search of the rooms array, calling PlaceConnections to randomly place connections and then moving to the next node till the dungeon is created
    int nextIndex,toVisit[MAX_ROOMS],toVisitCount,side,chance;        //initialise variables
    toVisit[0]=1;
    toVisitCount=chance=1;
    nextIndex=2;
    chance=1;
    while(toVisitCount){                                                //loops till no nodes left to vist
        if (nextIndex%CHANCE_RATE==2){                                        //increases chance of no room as more rooms get placed
            chance+=1;
        }
        if (toVisit[toVisitCount-1]!=-1){                               //if the node that was here has been fully visited, skip it (more for error checking)
            side=placeConnections(rooms[toVisit[toVisitCount-1]].connections,chance,nextIndex,&rooms[toVisit[toVisitCount-1]].lastSide);
            if (side==-1){                                                      //if no new room placed, node has been fully visited
                toVisit[toVisitCount]=-1;
                toVisitCount--;
            } else{
                rooms[toVisit[toVisitCount-1]].connectionCount+=1;
                side = (side+2)%4;                                               //transfers it to the alternate direction
                rooms[nextIndex].connections[side]=toVisit[toVisitCount-1];  //assigns the index of the room before it to the correct spot on itself
                toVisit[toVisitCount]=nextIndex;                               //adds the index of the new room to the toVisit array
                toVisitCount++;                                                 //increment counts
                nextIndex++;
            }
            if (nextIndex==MAX_ROOMS){                                          //if the max number of rooms has been placed, exit
                toVisitCount=0;
            }
        }
    }
    return nextIndex;   //returns the next index. Because this function doesn't include room 0, it does not need to be deincrememnted
}


void GenerateRooms(char *filename,struct roomNode rooms[MAX_ROOMS]){    //Function to procedurally generate the rooms
    int numberRooms,suggestedMonster,roomIndex,total,wCount,halt,loop;      //initialise variables
    int endCount,deadEndIndices[MAX_ROOMS],conCount,connectedIndices[MAX_ROOMS];
    char name[NAME_LEN], description[DESC_SIZE], line[LINEMAX];

    endCount=halt=loop=conCount=0;

    for (int i=0;i<MAX_ROOMS;i++){  //sets up the initial values to expected values for the DFS function
        rooms[i].lastSide=0;
        for (int j=0;j<4;j++){
            rooms[i].connections[j]=-1;
        }
    }
    rooms[0].connections[0]=1;
    rooms[0].connectionCount=1; //sets up the initial values
    rooms[1].connections[2]=0;  //links back to the starting rooms

    numberRooms=DFS(rooms);
    for (int i = 1; i < numberRooms - 1; i++) { //computes how many deadends vs open rooms
        if (rooms[i].connectionCount == 1) {
            deadEndIndices[endCount] = i;
            endCount++;
        } else {
            connectedIndices[conCount] = i;
            conCount++;
        }
    }


    FILE *fin = fopen(filename, "r"); // Open the file for reading

    if (fin == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fgets(line,sizeof(line), fin); //skip first line
    wCount=(NUM_WEAPONS-1);     //sets wCount to the weapon indice for the most powerful weapon (else it becomes almost impossible to kill the minotaur.

    if (conCount>NUM_ROOM_TYPES-4){ //if the conCount is greater than the number of rooms satisfying this condition, loop till all rooms are occupied. (2 rooms are for the dead ends, 2 are for the special rooms). These numbers are hardcoded as changing them will mean the loop in this function will not work without alterations
        loop=conCount-(NUM_ROOM_TYPES-4);   //sets the loop amount.
    }

    for (int i=0;i<NUM_ROOM_TYPES&&!halt;i++){
        if (fgets(line,sizeof(line), fin) != NULL){
            if (sscanf(line, "'%[^']','%[^']',%d", name, description, &suggestedMonster) == 3) {
                if (i==0){
                    constructRoom(name, description, suggestedMonster, &rooms[0],1);
                    rooms[0].objectIndex=-1;
                }else if (i==1){
                    constructRoom(name, description, suggestedMonster, &rooms[numberRooms-1],1);
                    rooms[numberRooms-1].objectList=0;
                    rooms[numberRooms-1].objectIndex=NUM_ITEMS-1;
                } else if (i==2){
                    if (endCount){
                        total=endCount/2;
                        while(total<endCount){
                            roomIndex=getIndex(&endCount,deadEndIndices);
                            constructRoom(name, description, suggestedMonster, &rooms[roomIndex],0);
                            placeRandomItem(&rooms[roomIndex],&wCount);
                        }
                    }
                } else if (i==3){
                    if (endCount){
                        while(endCount!=0){
                            roomIndex=getIndex(&endCount,deadEndIndices);
                            constructRoom(name, description, suggestedMonster, &rooms[roomIndex],0);
                            placeRandomItem(&rooms[roomIndex],&wCount);
                        }
                    }
                } else{
                    // Assign room type to rooms randomly
                    if (conCount){
                        do{
                            roomIndex=getIndex(&conCount,connectedIndices);
                            constructRoom(name, description, suggestedMonster, &rooms[roomIndex],0);
                            placeRandomItem(&rooms[roomIndex],&wCount);
                            if (loop>0){
                                loop--;
                            }
                        } while(loop);
                    }
                } if (!endCount&&!conCount&&i){
                    halt=1;
                }
            } else {
                printf("line %d\n",i+1);
                fprintf(stderr, "Error: Invalid format in 'roomTypes.txt'\n");
                halt=1;
            }
        } else{
            printf("line %d\n",i+1);
            fprintf(stderr, "Error: Invalid format in 'roomTypes.txt'\n");
            halt=1;
        }
    }
}


int getIndex(int *cnt,int indices[MAX_ROOMS]) { //generates a random index, and then removes it from array.
    int temp,roomIndex;
    roomIndex = rand() % *cnt;
    temp=indices[roomIndex];
    indices[roomIndex]=indices[*cnt-1];
    (*cnt)--;
    return temp;
}

void constructRoom(char *name, char *description, int suggestedMonster, struct roomNode *room,int specialRoom){     //takes a each of the values that need to be assigned to the room, and assigns them
    strcpy(room->type, name);
    room->type[NAME_LEN - 1] = '\0';    //end value to prevent a memory overflow error
    strncpy(room->description, description, DESC_SIZE - 1);
    room->description[DESC_SIZE - 1] = '\0'; //same as above
    if (!specialRoom){          //if its not the starting room or the Throne Room
        if (suggestedMonster==-1){  //if its not got a suggested monster
            if (!(rand()%3)){
              suggestedMonster=(rand()%NUM_MONSTERS);  //1 in 3 chance. Places a random monster
            }
        }
    }
    room->monsterIndex = suggestedMonster;
}

void placeRandomItem(struct roomNode *room,int *wCount){
    int index,count,found=0;

    if (*wCount==0){    //if the value stored in wCount equals 0, then it means that the next weapon would be the unarmed state, which we don't want to place. Hence, it'll stop placing weapons
        room->objectList=rand()%2;
    } else{
        room->objectList=rand()%3;
    }
    if (rand()%2==0){       //randomises if it should place an item or not
        switch (room->objectList){
            case 0:
                room->objectIndex=rand()%(NUM_ITEMS-1);     //generates the random item index, not including the minotaur horn
                break;
            case 1:
                room->objectIndex=rand()%NUM_POTIONS;       //generates the random potion index
                break;
            case 2:
                room->objectIndex=*wCount;      //since only 1 of these will be placed per game, no need to random what room as the function recieves a random room.
                (*wCount)--;

                break;
        }
    } else{
        room->objectIndex=-1;                       //signifies that the room does not contain an item
    }
}
