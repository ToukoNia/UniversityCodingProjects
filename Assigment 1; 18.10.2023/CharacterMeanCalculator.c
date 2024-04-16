#include <stdio.h>

/*Here is a program to take in a series of inputs, store them in a file. This file is then reopenned and then
each character is checked to extract the numbers. This is then used to calculate an average of all the numbers
provided, which is then returned to the user.

Written by Nia Touko. 18/10/2023 15:37*/

float mean(int sum,int N){
    //function calculates the mean average of the inputs
    if (N==0){ //checks if the program has had a number entered
        return 0;
    }
    float average;
    average=sum/(float)N;
    return average;
}

void main(){
    //initilisation of variables

    char singleChar, character;
    char run = '1';
    int sum, count, singleInt;

    float avg;

    FILE *fptr;
    //will loop until the program halts
    while (run=='1'){

        //declaration of variables to reset them to base values (assists in the loop)
        sum=0, count=0, singleInt=0, avg=0, singleChar="a", character="a";

        //Opens file for writing.
        fptr=fopen("myFile.txt","w");


        printf("Please enter each of your characters with no spaces: ");

        //Takes the input in, and prints it to the file. Will loop till every inputed character has been read.
        while (singleChar!='\n'){
            scanf("%c",&singleChar);
            fprintf(fptr,"%c",singleChar);
        }
        fclose(fptr);

        //Opens file for reading. If no file exists, error
        fptr=fopen("myFile.txt","r");
        if (fptr==NULL){
            printf("ERROR! No such file\n");
            run=0;
        }else{
                //loops until the end of the file. This allows it to scan every character

                while (character!=EOF){
                    character=fgetc(fptr);
                    //checks if the character is an integer between set values
                    if (character>='0'&&character<='9'){
                        printf("%c",character);
                        //Preforming the operations on the values
                        singleInt=character-48;
                        sum = sum+singleInt;
                        count++;
                    }
                }
            fclose(fptr);

            //calls function to calculate average
            avg=mean(sum,count);


            if (avg!=0){
                //outputting to the user
                printf("\nHere is the sum of the values: %d \n",sum);
                printf("Here is the number of values: %d \n",count);
                printf("Here is the average to 2 decimal places %.2f \n",avg);

            } else{
                printf("No numbers are in the file.\n");
            }
            //Checks if the user wants to leave the loop
            printf("Press 1 to continue the program, or any other key to quit: ");
            scanf("%c",&run);
            scanf("%c"); //this catches the newline from the user input
            }
        }


}
