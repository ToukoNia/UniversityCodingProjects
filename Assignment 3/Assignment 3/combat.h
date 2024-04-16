#ifndef COMBAT_H
#define COMBAT_H

#include "gameController.h"
#define MONSTER_MAX_HEALTH 20

int calculateDamage(int lLimit, int uLimit);

void damageMonster(int damage, struct monster *subject);

void playerAttack(struct monster *subject, struct weapon attack);

void displayMonsterInfo(struct monster subject);

void monsterAttack(int *health,int maxHealth,int lDamage, int uDamage);

int combatInitiate(struct gameController *player, struct monster enemy);

#endif /* COMBAT_H */
