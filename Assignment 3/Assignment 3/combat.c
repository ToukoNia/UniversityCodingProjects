#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "combat.h"


int calculateDamage(int lLimit,int uLimit){
    return -(rand()%(uLimit-lLimit+1)+lLimit);
}

void playerAttack(struct monster *subject,struct weapon attack){
    int dmg;
    printf("You attacked with your %s.\n",attack.base.name);
    dmg=calculateDamage(attack.lDamage,attack.uDamage);
    changeHealth(&subject->health,dmg,MONSTER_MAX_HEALTH);
    printf("You dealt %d damage.\n",-dmg);
    printf("The %s has %d health left.\n",subject->name,subject->health);
}

void monsterAttack(int *health,int maxHealth,int lDamage, int uDamage){
    int dmg;
    printf("The monster attacks you!\n");
    dmg=calculateDamage(lDamage,uDamage);
    printf("It does %d damage.\n",-dmg);
    changeHealth(health,dmg,maxHealth);
    printf("You have %d health left.\n",*health);
}

void displayMonsterInfo(struct monster subject){ //prints out the monster information to the end user
    printSeparator();
    printf("Name: %s\n", subject.name);
    printf("Health: %d\n", subject.health);
    printf("Description: %s\n", subject.description);
    printSeparator();
    printf("\n");
}

int combatInitiate(struct gameController *player,struct monster *enemy){
    char choice;
    int dmg,ran,enemyDead,playerDead;
    char validOptions1[2]={'Y','N'};
    char validOptions2[3]={'F','P','R'};

    ran=enemyDead=playerDead=0;             //sets them all to zero

    printf("You encounter a %s.\n",enemy->name);
    printf("You have entered combat with the %s.\n",enemy->name);
    choice=sanitisedUserInput("Would you like to look at the monster? (Y/N) ",validOptions1,2);
    if (choice=='Y'){
        displayMonsterInfo(*enemy);
    }
    while (!enemyDead&&!ran&&!playerDead){
        printSeparator();
        printf("It is your turn.\n");
        choice=sanitisedUserInput("Would you like to [F]ight, use a [P]otion or [R]un? ",validOptions2,3);
        switch (choice){   //matches the user input to the correct function to continue
            case 'F':
                playerAttack(enemy,player->equippedWeapon);
                break;
            case 'P':
                choosePotion(player);
                choice=sanitisedUserInput("Would you like to look at the monster? (Y/N) ",validOptions1,2);
                if (choice=='Y'){
                    displayMonsterInfo(*enemy);
                }
                break;
            case 'R':
                printf("You have fled combat. The enemy can attack as you leave.\n");
                ran=1;
                break;
        }

        printSeparator();
        if (enemy->health<=0){
            enemyDead=1;
        } else{
            waitForEnter(1);
            printSeparator();
            printf("It is the %s's turn.\n",enemy->name);
            monsterAttack(&player->health,player->maxHealth,enemy->lDamage,enemy->uDamage);
        }
        if (player->health==0){
            playerDead=1;
        }
    }
    if (ran){
        return 0;
    }
    else if (enemyDead){
        printf("You killed the %s.\n",enemy->name);
        return 1;
    }
    else if (playerDead){
        printf("You have died.\n");
        return 2;
    }
}

