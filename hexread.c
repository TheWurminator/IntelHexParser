#include<stdio.h>
#include<stdlib.h>
int main(void){
	
	FILE *fptr = fopen("hexread.hex", "rb");
	FILE *print = fopen("output.hex", "w");
	FILE *test = fopen("values.bin", "wb");
	if(!fptr){
		printf("Could not open the file for writing\n");
		return 1;
	}
	//Make a buffer of some kind (Not this below)
	//Allocates it on the heap, this is a buffer
	char *str = malloc(sizeof(char)*100000);
	int i = 0;
	//Reading in the file to a buffer
	while(!feof(fptr)){
		unsigned char a = 0;
		//printf("%c",fptr);

		fscanf(fptr, "%c", &str[i]);
		//The written colon will be overwritten
		if(str[i] != ':'){
			//printf("%x", a);
			//fwrite(&str[i], sizeof(char), (size_t)1, print);
			//printf("%c", str[i]);
			//Increment the counter if i
			i++;
		}
		//Otherwise do nothing
	
	}



	//Second counter
	int j = 0;
	//Making a copy of the original buffer
	char *info = str;
	
	while((j/2)<=i){
		unsigned char hexbyte = 0;
		//Reading in two hex characters == 1 byte

		//Scanning the value into a byte
		sscanf(info+j, "%2hhx", &hexbyte);
		//Writing the straight binary byte to the file
		fwrite(&hexbyte, sizeof(char), (size_t)1, test);
		//Increment by two characters, because each is half of a byte
		j+=2;
		
		fprintf(print,"We got byte %02x, which is displayed as \'%c\'\n", hexbyte, hexbyte);

	}





}