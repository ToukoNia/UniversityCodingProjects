#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resetInputBuffer();
void printSeparator();
int sanitisedUserNumberInput(char* message, int lBound, int uBound);
char sanitisedUserInput(char* message,char options[],int numOptions);
int sanitisedUserDirectionInput(int connections[4]);

#endif /* USER_MANAGEMENT_H */
