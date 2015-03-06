#include<stdio.h>
#include<stdlib.h>

//This program will find every record code in a hex file that you give it and print out a text file called records.txt
//Which will contain all of the records

int main(void){
	FILE * fptr = fopen("hexread.hex", "r");
	FILE * print = fopen("records.txt", "w");
	char * line = (char *)malloc(100 * sizeof(char));
	while(!feof(fptr)){
		char check = 0;
		int i = 0;
		check = fgetc(fptr);
		if(feof(fptr)){
			break;
		}
		while(check != 13){
			printf("%c\n", check);
			line[i]=check;
			check = fgetc(fptr); // This will be 13
			i++;
		}
		check = fgetc(fptr);			// This will be 10
		fprintf(print, "CODE: %c\n", line[8]);
		for(i=0; i<100; i++){
			line[i] = 0;
		}
		
		//fprintf(print, "%d\n", fgetc(fptr));

	}
	return 0;
}