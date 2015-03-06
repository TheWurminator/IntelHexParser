//Readinv2.c
//This had to be created because some lines do not end with 10 or 13, so the entire file must be tokenized by 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct instruction{
	unsigned char record;
	unsigned char bytecount;
	unsigned int address;
	unsigned char * data;
	struct instruction * next;

};

struct master{
	struct instruction * head;
	struct instruction * tail;
};

void printeverything(struct master ** headref);
void addnode(struct master **headref, unsigned char record, unsigned char bytecount, unsigned int address, int upper16, unsigned char * data);
int main(void){

	unsigned char bytecount = 0;	//Bytes of data to be written
	int upper16 = 0;
	unsigned char record = 0;	//Stores the record type
	int datacounter = 0;		//Used to do something						
	int i = 0;					//Line counter thing
	unsigned char * data = NULL; //Making the data buffer

	//NEED TO CREATE A DATA UNSIGNED CHAR ARRAY
	//Making a head for the data
	struct instruction * head = NULL;
	struct instruction * tempo = NULL;
	struct master * mast = (struct master *)malloc(sizeof(struct master));
	mast->head = NULL;
	mast->tail = NULL;
	FILE * fptr = fopen("hexread.hex", "r");				//Opening the hex file to read
	char check = 0;
	char * filebuf  = (char * )malloc(70000*sizeof(char));		//File Buffer
	check = fgetc(fptr);	//Getting first character
	while(!feof(fptr)){
		filebuf[i] = check;
		i++;
		check = fgetc(fptr);
	}
	fclose(fptr); //Done with the file

	const char s[2] = ":";			//Delimiter
	char *token;					//Where each token will be stored
	token = strtok(filebuf,s);		//Getting the first token
	while(token!=NULL){				//Stepping through all of the tokens
		char * line = (char *)malloc(sizeof(token));		//Making a character array from the token
		line = token;
		//Reusing check
		i = 0;
		//Must sanitize each line
		while(line[i] != 0){
			if(line[i] == 13 || line[i] == 10){		//If there are any newline or cr chars, remove them
				line[i] = 0;
			}
			i++;
		}
		//printf("%d", i);
		//printf("\n%s\n", token);
		//Now we must step through each instruction
		unsigned int address = 0;				//Put inside of the loop because it will be reset every time
		int linecounter = 0; 					//Since the colons have been eliminated, we can have this be 0
		int power = 1;							//Used when converting the bytecount to base 10
		while(linecounter <= 1){
			if(line[linecounter] < 58 && line[linecounter] > 47){						//0-9
				//printf("here");
				bytecount = bytecount + (line[linecounter]-48) * (pow(16,power));
				//printf("\n%d\n", bytecount);
			}
			else{
				bytecount = bytecount + (line[linecounter]-55) * (pow(16, power));		//A-F
				//printf("\n%d\n", bytecount);
			}
			power--;
			linecounter++;

		}	
		//printf("Got here1\n");
		data = (unsigned char *)malloc(bytecount * sizeof(char)); //Must allocate bytecount * 2, because there are two hex chars in a byte
		power = 3;
		while(linecounter <= 5){
			//printf("\n%d %c\n", line[linecounter], line[linecounter]);
			if(line[linecounter] < 58 && line[linecounter] > 47){						//0-9
				address = address + (line[linecounter]-48) * (pow(16,power));
			}
			else{
				address = address + (line[linecounter]-55) * (pow(16,power));			//A-F
			}
			power--;	//Decrement power, 
			linecounter++; //Increment linecounter

		}
		//printf("Got here2\n");
		linecounter++; //Stepping over the 0, because it is not necessary to read in
		//printf("%d", linecounter);
		int j = 0;
		while(linecounter == 7){
			record = line[linecounter] - 48;					//Only 0-9, so subtract 48 from ASCII
			//printf("\n%c\n", line[linecounter]);
			if(line[linecounter] == '0'){						//Only Data
			
				//Uses regular address value, reads in bytecount bytes
				power = 1;		//Need to reset the power every time this runs
				//Moving over to the data
				linecounter++;
				//j==0
				while(j < bytecount){
				//Read in byte by byte
					if(power == -1){	//If the power has been decreased twice, reset it
						datacounter++;	//Moving one position in the array, since one byte has passed
						power = 1;		//Resetting the power
					}
					printf(" ");			//Used to separate the conditional statements
					if(line[linecounter] < 58 && line[linecounter] > 47){								//0-9
						//DATACOUNTER is used for the data char array
						//data[dc]+=line[linecounter]-48 * 16^power
						data[datacounter] = data[datacounter] + (line[linecounter]-48) * pow(16, power);
					}
					else{																				//A-F
						data[datacounter] = data[datacounter] + (line[linecounter]-55) * pow(16,power);
					}
					linecounter++;	//Incrementing the linecounter
					datacounter++;	//Incrementing the datacounter
					power--; //Decrementing the power
					j++;	//Incrementing the loop counter
				}

			}
			//0 case completed
			//COMPLETE
			else if(line[linecounter] == '1'){					//EOF

				//Data field is empty, address is typically 0000
				printf("\nThe end of the file has been reached\n");
				linecounter++;
				
			}
			//First case complete

			else if(line[linecounter] == '4'){	//Put this into the upper 16 bits of the address
				j=0;							//Resetting the counter
				power = 3;						//Setting the power to 3 since we will be reading in a 2 byte value
				upper16 = 0; 					//Resetting upper16 and modifying the value
				linecounter++;
				while(j<bytecount){
					if(line[linecounter] < 58 && line[linecounter] > 47){								//0-9
						upper16 = upper16 + (line[linecounter]-48) * pow(16, power);
					}
					else{																				//A-F
						upper16 = upper16 + (line[linecounter]-55) * pow(16, power);
					}
					linecounter++;
					j++;

				}
				upper16 = upper16 << 16; 		//Shifting the value into the upper 16 bits of the integer

				//32 bit addressing. Address field is IGNORED. The two encoded, big endian data bytes specify the upper 16
				//bits of the of the 32 bit absolute address for all subsequent "00" records, which apply until the next "04"
				//record. If no type "04" record precedes an "00" record, then the upper 16 bits default to 0000. 

			}

			//This is just loading into the data array, not doing anything with it
			else if(line[linecounter] == '5'){	//Start Linear Address, supposed to load into EIP
				power = 3;
				j=0;
				while(j<bytecount){
					if(line[linecounter] < 58 && line[linecounter] > 47){								//0-9
						data[datacounter] = data[datacounter] + (line[linecounter]-48) * pow(16, power);
					}
					else{																				//A-F
						data[datacounter] = data[datacounter] + (line[linecounter]-55) * pow(16,power);
					}
					linecounter++;	//Must increment linecounter
					datacounter++;	//Must increment datacounter
					j++;

				}
				//The address field is NOT USED. Byte count is 04. Bytes represent the 32-bit value loaded into the EIP
				//Register of the 80386 and higher CPU.

			}
			else{
				return 3;
			}
		}

		linecounter = 0; //Resetting the linecounter, because everything that was needed has been parsed
		addnode(&mast, record, bytecount, address, upper16, data);
		data = NULL;
		i=0;
		datacounter = 0;
		address = 0;
		bytecount = 0;
		record = 0;
		line = NULL;
		token = strtok(NULL,s);			//Moving onto the next instruction
		//printf("%s", token);

	}

	//printf("About to print");
	//printf("%d", mast->head->)
	printeverything(&mast);
	 //feof
	return 0;
}



void printeverything(struct master ** headref){
	//printf("In the function");
	FILE *fptr2 = fopen("printout2.txt", "w");
	struct instruction * temp = (*headref)->head;
	while(temp != NULL){
		fprintf(fptr2, "Record = %d\n", temp->record);
		fprintf(fptr2, "Address = %d\n", temp->address);
		fprintf(fptr2, "Bytecount = %d\n", temp->bytecount);
		int i = 0;
		fprintf(fptr2, "Data = ");
		while(i<temp->bytecount){
			fprintf(fptr2, "%d ", temp->data[i]);
			i++;
		}
		fprintf(fptr2, "\n\n");
		temp = temp->next;
	}
	if(temp == NULL){
		printf("NULLTEMP");
	}
	free(temp);
	return;
}

//This will add a node into the queue
void addnode(struct master **mastref, unsigned char record, unsigned char bytecount, unsigned int address, int upper16, unsigned char * data ){
	struct instruction * temp = (struct instruction *)malloc(sizeof(struct instruction));
	address = address + upper16;
	if((*mastref)->head == NULL){
		temp->record = record;
		temp->bytecount = bytecount;
		temp->address = address;
		temp->data = data;
		(*mastref)->head = temp;
		(*mastref)->tail = temp;
	}
	else if((*mastref)->head != NULL){
		temp->record = record;
		temp->bytecount = bytecount;
		temp->address = address;
		temp->data = data;
		(*mastref)->tail->next = temp;
		(*mastref)->tail = temp;
	}
		return;
}