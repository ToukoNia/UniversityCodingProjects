#ifndef SETUPSTRUCTS_H_INCLUDED
#define SETUPSTRUCTS_H_INCLUDED

#define DESC_SIZE 200
#define NAME_LEN 20
#define MAX_ITEMS 8
#define NUM_ITEMS 3
#define NUM_WEAPONS 5
#define NUM_POTIONS 6


struct monster{
    char name[NAME_LEN];
    char description[DESC_SIZE];
    int health;
    int lDamage;
    int uDamage;
};

struct base{
    char name[NAME_LEN];
    char description[DESC_SIZE];
    int value;
    int index;
};

struct item{
    struct base base;
};

struct weapon{
    struct base base;
    int lDamage;
    int uDamage;
};

struct potion{
    struct base base;
    int healing;
};

struct inventory {
    struct potion potions[MAX_ITEMS];
    struct item items[MAX_ITEMS];
    int itemCount;
    int potionCount;
};

struct objects{
    struct item items[NUM_ITEMS];
    struct potion potions[NUM_POTIONS];
    struct weapon weapons[NUM_WEAPONS];
};

struct roomNode{
    char type[NAME_LEN];                  //sets up the room type for outputting to the uer
    char description[DESC_SIZE];          //the description to be read to the user
    int objectIndex;                  //needs to be an index, as the weapons and potions are in seperate lists. -1 if none.
    int objectList;                 //indicates which list the object belongs to.
    int monsterIndex;               //same as above for monsters, -1 if none
    int connections[4];             //array of the indices of the rooms that it is connected to
    int connectionCount;
};

struct gameController{
    char name[NAME_LEN];
    int currentRoomIndex;
    int lastRoomIndex;
    int health;
    int maxHealth;
    struct inventory inv;
    struct weapon equippedWeapon;
};

#endif // STRUCTS_H_INCLUDED
