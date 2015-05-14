//The purpose of this program is to parse all Intel Hex files that the user reads in
//It will put these into a linked list of instructions, which will be printed out to a file
//Currently working on printing it out to a file 03/04/2015
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
//Still have to reinitialize all of the values, except temp
//Could use a struct to model it in memory
struct instruction{
	unsigned char record;
	unsigned char bytecount;
	unsigned int address;
	unsigned char * data;
	struct instruction * next;

};
void printeverything(struct instruction ** headref);
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

	FILE * fptr = fopen("hexread.hex", "r");				//Opening the hex file to read
	
	char * line  = (char * )malloc(100*sizeof(char));		//Line Buffer

	while(!feof(fptr)){							//Will run until the file has ended, analyzes the line
		char check = fgetc(fptr);				//Get the first character in the file
		if(feof(fptr)){							//Used for the last case, where it will read in a character, which is EOF
			break;
		}
		int runtime = 0;
		while(check != 13 && check!=10){
			if(check == ':' && runtime != 0)						//Reading in a single line
			{
				break;
			}
			printf("%c", check);
			line[i]=check;
			check = fgetc(fptr);
			i++;
		}

		check = fgetc(fptr);					//This will be 10

		//printf("\nI is: %d\n", i);

		unsigned int address = 0;				//Put inside of the loop because it will be reset every time
		int linecounter = 1;					//Colon will be at 0, so I set it to 1
		int power = 1;							//Used when converting the bytecount to base 10
		//printf("%d", linecounter);
			//Getting the byte count
		

		//WORKS AS INTENDED	
		//Purpose of this is to get the number of data bytes
		while(linecounter <= 2){
			//printf("\nGot here\n");
			//printf("\n%d\n", line[linecounter]);
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
		//Linecounter should be 3 at this point

		//free(data); 												// May or may not be needed, may ruin everything
		data = (unsigned char *)malloc(bytecount *2* sizeof(char)); 	//Have to properly size 
		power = 3;		//Setting power to 3 because this is going to be reading in 4 hex bits == 2 bytes into an int (Lower 16 bits)
	
		//printf("ghere\n%d\n", linecounter);
		//Purpose is to read in the address in the line
		//3,4,5,6 == runs 4 times
		while(linecounter <= 6){
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
		linecounter++; //Do not care about the zero in front of the command, so I skipped it
		//Linecounter should be 8 at this point


		int j = 0;		//General counter

		//Purpose is to read in the record type, only 1 byte
		while(linecounter <=8){
			record = line[linecounter] - 48;					//THIS IS WHERE THE RECORD IS STORED
			//COMPLETE
			if(line[linecounter] == '0'){						//Only Data
			
				//Uses regular address value, reads in bytecount bytes
				power = 1;		//Need to reset the power every time this runs
				//Moving over to the data
				linecounter++;
				//j==0
				while(j < bytecount*2){
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
				goto END;

			}
			//First case complete

			else if(line[linecounter] == '4'){	//Put this into the upper 16 bits of the address
				j=0;							//Resetting the counter
				power = 3;						//Setting the power to 3 since we will be reading in a 2 byte value
				upper16 = 0; 					//Resetting upper16 and modifying the value
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
		//Linecounter 8 has ended
		//Linecounter = 8 + bytecount*2;
		linecounter = 0; //Resetting the linecounter, because everything that was needed has been parsed
		memset(line, 0, sizeof(char)*100); //Resetting the line buffer to 0 so it can be used again
		

		//This is the final part, loading the information into the struct
		//If the head has not been initialized
		if(head == NULL){
			address = address + upper16;	//Putting the offset in the read-in address
			head = (struct instruction *)malloc(1*sizeof(struct instruction));
			head->address = address;
			head->record = record;
			head->next = NULL;
			head->bytecount = bytecount;
			head->data = data;
		}
		else{
			address = address + upper16;	//Putting the offset in the read-in address
			struct instruction *tempo = (struct instruction *)malloc(1*sizeof(struct instruction));
			tempo->address = address;
			tempo->record = record;
			tempo->next = head;
			tempo->bytecount = bytecount;
			tempo->data = data;
			head = tempo;
		}
		i=0;
		datacounter = 0;
		address = 0;
		bytecount = 0;
		record = 0;
	}
	END:
	printeverything(&head);
	 //feof
	return 0;
}




void printeverything(struct instruction ** headref){
	FILE *fptr2 = fopen("printout.txt", "w");
	struct instruction * temp = (*headref);
	while(temp != NULL){
		fprintf(fptr2, "Record = %d\n", temp->record);
		fprintf(fptr2, "Address = %d\n", temp->address);
		fprintf(fptr2, "Bytecount = %d\n", temp->bytecount);
		int i = 0;
		fprintf(fptr2, "Data = ");
		while(i<temp->bytecount){
			fprintf(fptr2, "%d", temp->data[i]);
			i++;
		}
		fprintf(fptr2, "\n\n");
		temp = temp->next;
	}
}
