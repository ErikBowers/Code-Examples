////////////////////////////////////////////////////////////////////////////////////////
//PROGRAMMER NAME: Erik Bowers
////////////////////////////////////////////////////////////////////////////////////////
//PROGRAM NAME: bowerser.adventure.c
////////////////////////////////////////////////////////////////////////////////////////
//PROGRAM:	Assignment 2 for CS344 at OSU, Winter term 2018
//		Text adventure style game. Reads room data from seven
//		files, created in bowerser.buildrooms.c, to create
//		a maze for the player to traverse through.
//		
//		Takes input from the user via STDIN, outputs
//		to STDOUT.
////////////////////////////////////////////////////////////////////////////////////////
//FUNCTIONS:
//		LocalTime()
//			   Creates a file named currentTime.txt and adds
//			   the current time to this file.
////////////////////////////////////////////////////////////////////////////////////////
//NOTE: Comments are either above the line or block of code they're describing or
//		on the same line.
//	Debug lines will be commented out, with a comment of DEBUG following the line.
//		Most debug lines removed to increase clarity and flow.
//	Variable names use camelCase, starting with a lower-case letter.
//	The opening brace for loops/functions/conditionals/etc is on the same line
//		as the decleration. The closing brace is on its own line, with
//		a comment explaining what it's closing.
//	Because C89 doesn't have booleans, ints are used instead.
//		0 is false, 1 is true. All lines using these pseudo-booleans have
//		a comment following indicating if it's true or false.
//		(Mostly for debugging, but they read nicely too).
///////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <assert.h>

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
char pathToTimeFile[40];

void* LocalTime(char directoryName[30]){
	pthread_mutex_lock(&myMutex);
	time_t currentTime = time(NULL);
	struct tm *tmp;
	char currentTimeString[100];
	tmp = localtime(&currentTime);
	strftime(currentTimeString, sizeof(currentTimeString), "%l:%M%P, %A, %B %d, %Y", tmp);
	//printf("The current time is: %d\n%s\n", currentTime, currentTimeString);//DEBUG
	char filePath[42];
	strcpy(filePath, directoryName);
	strcat(filePath, "/currentTime.txt");
	strcpy(pathToTimeFile, filePath);
	FILE *timeFile = fopen(filePath, "w+"); //open a file for writing: create one if needed, truncating if there already
	fprintf(timeFile, "%s\n", currentTimeString);
	fclose(timeFile);
	pthread_mutex_unlock(&myMutex);
}//end LocalTime() function


int main(int argc, char* argv[]){
	//lock the mutex
	pthread_mutex_lock(&myMutex);

	//find the most recently used bowerser.rooms.PID directory
	int newestDirTime = -1;
	char targetDirPrefix[32] = "bowerser.rooms.";
	char newestDirName[256];
	memset(newestDirName, '\0', sizeof(newestDirName));
	DIR* dirToCheck;
	struct dirent *fileInDir;
	struct stat dirAttributes;
	dirToCheck = opendir("."); //Open the directory the program was run in
	
	//Really just the loop from the lecture notes.
	//First, we check to see that we can actually check the dir this program is run from
	if(dirToCheck > 0){
		//We're going to check every directory inside of this directory.
		while((fileInDir = readdir(dirToCheck)) != NULL){
			//make sure it has the prefix we're looking for
			if(strstr(fileInDir->d_name, targetDirPrefix) != NULL){
				//printf("Found the prefix: %s\n", fileInDir->d_name);//DEBUG
				//get its name and its stat struct
				stat(fileInDir->d_name, &dirAttributes);
				//check to see if its st_mtime is newer than our current newest directory
				if((int)dirAttributes.st_mtime > newestDirTime){
					//It is, so swap the stored data over to this directory
					newestDirTime = (int)dirAttributes.st_mtime;
					memset(newestDirName, '\0', sizeof(newestDirName));
					strcpy(newestDirName, fileInDir->d_name);
					//printf("Newer subdir: %s, new time: %d\n", fileInDir->d_name, newestDirTime);//DEBUG
				}//end inner if
			}//end middle if
		}//end while
	}//end outer if
	closedir(dirToCheck);
	//printf("Newest entry found is: %s\n", newestDirName);//DEBUG

	//Now that we have the directory, open all its files
	char pathName[32];
	strcpy(pathName, newestDirName);
	strcat(pathName, "/room0");
	FILE *startRoom = fopen(pathName, "r");//room0 is always the start room

	strcpy(pathName, newestDirName);
	strcat(pathName, "/room1");
	FILE *midRoom_1 = fopen(pathName, "r");

	strcpy(pathName, newestDirName);
	strcat(pathName, "/room2");
	FILE *midRoom_2 = fopen(pathName, "r");

	strcpy(pathName, newestDirName);
	strcat(pathName, "/room3");
	FILE *midRoom_3 = fopen(pathName, "r");

	strcpy(pathName, newestDirName);
	strcat(pathName, "/room4");
	FILE *midRoom_4 = fopen(pathName, "r");

	strcpy(pathName, newestDirName);
	strcat(pathName, "/room5");
	FILE *midRoom_5 = fopen(pathName, "r");
	
	strcpy(pathName, newestDirName);
	strcat(pathName, "/room6");
	FILE *endRoom = fopen(pathName, "r");//room6 is always the end room


	char pathTaken[512];//I want to make sure even a comically long path gets stored.
	memset(pathTaken, '\0', sizeof(pathTaken));
	int numberOfMoves = 0;
	int maxLineSize = 24; //23 is max line size in a room, with the extra for a null terminator
	char* line;
	line = malloc(maxLineSize);
	char junk[20];
	memset(junk, '\0', sizeof(junk));
	char alsoJunk[20];
	memset(alsoJunk, '\0', sizeof(alsoJunk));
	char data[20];//I bet all of these could be 10, but I need to double check first
	memset(data, '\0', sizeof(data));
	char name[8];
	memset(name, '\0', sizeof(name));
	char connection[6][9];//A room can have 6 connections: a name can have 8 characters, and 1 more for \0
	int i = 0;
	for(i = 0;i < 6;i++){
		memset(connection[i], '\0', 9 * sizeof(char));
	}//end for loop. connection should all be clear now
	int loopCount = 0;
	char connectionType[10];
	memset(connectionType, '\0', sizeof(connectionType));
	int numberOfConnections = 0;
	char listOfConnections[60];
	memset(listOfConnections, '\0', sizeof(listOfConnections));
        i = 0;
	char userInput[20];//larger than 8 so that the user can't enter the wrong thing and break things too easily
	memset(userInput, '\0', sizeof(userInput));

	//Get the name of the start room, as well as its list of connections
	while(fgets(line, maxLineSize, startRoom) != NULL){
	//Each line of the file has string lines for human readablity and one of actual data
	//So we can strip the first two strings as junk, and just grab the data.
	sscanf(line, "%s %s %s", junk, alsoJunk, data);
	//printf("Data: %s\n", data);//DEBUG
	//The first line is the name, so just place that right into data
	if(loopCount == 0){
		strcpy(name, data);
		//printf("Name: %s\n", name);//DEBUG
	}else{
		//if it's the room type then place data into connectionType
		if( (strcmp(data, "START_ROOM") == 0) ){ // || (strcmp(data, "MID_ROOM") == 0) || (strcmp(data, "END_ROOM") == 0) ){
			strcpy(connectionType, data);
			//printf("Connection Type: %s\n", connectionType);//DEBUG
		//Otherwise, it's a connection and should be placed into the array of connections
		//And the number of connections should be increased accordingly
		}else{
			//printf("connection[%d]: %s\n", i, data);//DEBUG
			strcpy(connection[i], data);
			i++;
			numberOfConnections++;
		     }
	}//end nested if/else
	loopCount++;
	}//end pre-game-loop while loop
	
	//reset startRoom's file pointer
	rewind(startRoom);//learned from stackoverflow.com/questions/32366665/resetting-pointer-to-the-start-of-file
	
	//printf("\nOut of the file loop.\n");//DEBUG
	//Display game prompt
	printf("CURRENT LOCATION: %s\n", name);
	for(i = 0;i < numberOfConnections;i++){
		//Append each name to the list of names
		strcat(listOfConnections, connection[i]);
		//printf("connection[%d]: %s\n", i, connection[i]);//DEBUG
		strcat(listOfConnections, ", ");
	}//end for loop
	//Remove the last two characters from listOfConnections, ", ", and add a period
	//stackoverflow.com/questions/1726298/strip-first-and-last-character-from-c-string
	//I thought strlen would return the length of an array, not what's in it!
	//Wow, something string wise that C does better than Java!
	listOfConnections[strlen(listOfConnections)-2] = '\0';
	strcat(listOfConnections, ".");
	printf("POSSIBLE CONNECTIONS: %s\n", listOfConnections);
	printf("WHERE TO? >");//then read input
	

////////////////////////////////////////////////////////////////////////
	//game loop!
	int hasWon = 0; //false
	int isValidMove = 0;//false
	while(hasWon == 0){
	//take input from user
	//check input against everything in connection[]
	//if the name isn't in there, display error and repeat all that room data again.
	//if the name is there, find out which room file has that name and load up that file's data
	//and move to that room. If it's not the end room, repeat this process.
	while(isValidMove == 0){
		scanf("%s", userInput);
		//Now check to see if any of the current connections has the input name
		for(i = 0;i < numberOfConnections;i++){
			if(strcmp(userInput, connection[i]) == 0){
				//that room is a valid move
				isValidMove = 1;//true
			}//end if
		}//end for loop
		
		//if it's a valid move, move there. If not, tell the player such and ask again
		if(isValidMove == 1){
			//printf("\nYes, that's a valid move!\n");//DEBUG
			numberOfMoves++;
			strcat(pathTaken, userInput);
			strcat(pathTaken, "\n");
		}else{
			if(strcmp(userInput, "time") == 0){
				//Naw, it's cool. Don't do anything here, the other loop will display the time, since
				//that's where all the move data is done anyway
				isValidMove = 1;
			}
			else{
				printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
				//reprint all the room stuff
				printf("CURRENT LOCATION: %s\n", name);
				printf("POSSIBLE CONNECTIONS: %s\n", listOfConnections);
				printf("WHERE TO? >");
			}
		}//end if/else
	}//end isValidMove while loop
	isValidMove = 0;//set back to zero so that above runs next time through
		//if the user input time, then get that and don't muck with the other loop
		if(strcmp(userInput, "time") == 0){
			pthread_mutex_unlock(&myMutex);
			LocalTime(newestDirName);
			pthread_mutex_lock(&myMutex);
			FILE *timeFile = fopen(pathToTimeFile, "r");
			char currentTime[100];
			fgets(currentTime, 100 * sizeof(char), timeFile);
			printf("\n%s\n", currentTime);
			fclose(timeFile);
		}//end time if
		else{
		//check room 0 (the user could move back to start technically)
		//If that's not the correct room, check room 1. Continue on down until
		//the correct room is found. Because the input was validated above, there
		//shouldn't be any way to get here with invalid data!
		i = 0;
		numberOfConnections = 0;
		strcpy(listOfConnections, "");//clear listOfConnections
		fgets(line, maxLineSize, startRoom);
		sscanf(line, "%s %s %s", junk, alsoJunk, data);
		//printf("User Input: %s, Name: %s\n", userInput, data);//DEBUG
		if(strcmp(data, userInput) == 0){
			//that's the correct room! Move data to there!
			strcpy(name, data);//we just compared the name, so grab that
			//Update the rest of the room data (connections and type)
			while(fgets(line, maxLineSize, startRoom) != NULL){
				sscanf(line, "%s %s %s", junk, alsoJunk, data);
				//If it's the room type, put it into connectionType
				if(strcmp(data, "START_ROOM") == 0 ){
					strcpy(connectionType, data);
				}//end if
				//Otherwise, it must be a connection
				else{
					strcpy(connection[i], data);
					i++;
					numberOfConnections++;
				}//end else
			}//end while
			//now, create a list of all the connections
			for(i = 0;i < numberOfConnections;i++){
				strcat(listOfConnections, connection[i]);
				strcat(listOfConnections, ", ");
			}//for loop
			//Fix up listOfConnections formatting
			listOfConnections[strlen(listOfConnections)-2] = '\0';
			strcat(listOfConnections, ".");
		}//end first if
		else{
			fgets(line, maxLineSize, midRoom_1);
			sscanf(line, "%s %s %s", junk, alsoJunk, data);
			//printf("User Input: %s, Name: %s\n", userInput, data);
			if(strcmp(data, userInput) == 0){
				//that's the correct room! Move data to there!
				strcpy(name, data);//We just compared the name, so grab that
				//Update the rest of the room data
				while(fgets(line, maxLineSize, midRoom_1) != NULL){
					sscanf(line, "%s %s %s", junk, alsoJunk, data);
					//If it's the room type, put it into connectionType
					if(strcmp(data, "MID_ROOM") == 0){
						strcpy(connectionType, data);
					}//end if
					//Otherwise, it must be a connection
					else{
						strcpy(connection[i], data);
						i++;
						numberOfConnections++;
					}//end else
				}//end while
				//now, create a list of all the connections
				for(i = 0;i < numberOfConnections;i++){
					strcat(listOfConnections, connection[i]);
					strcat(listOfConnections, ", ");
				}//for loop
				//Fix up listOfConnections formatting
				listOfConnections[strlen(listOfConnections)-2] = '\0';
				strcat(listOfConnections, ".");
			}//end second if
			else{
				fgets(line, maxLineSize, midRoom_2);
				sscanf(line, "%s %s %s", junk, alsoJunk, data);
				//printf("User Input: %s, Name: %s\n", userInput, data);//DEBUG
				if(strcmp(data, userInput) == 0){
					//that's the correct room! Move data to there!
					strcpy(name, data);//We just compared the name, so grab that
					//update the rest of the room data
					while(fgets(line, maxLineSize, midRoom_2) != NULL){
						sscanf(line, "%s %s %s", junk, alsoJunk, data);
						//If it's the room type, put it into connectionType
						if(strcmp(data, "MID_ROOM") == 0){
							strcpy(connectionType, data);
						}//end if
						//Otherwise, it must be a connection
						else{
							strcpy(connection[i], data);
							i++;
							numberOfConnections++;
						}//end else
					}//end while
					//now, create a list of all the connections
					for(i = 0;i < numberOfConnections;i++){
						strcat(listOfConnections, connection[i]);
						strcat(listOfConnections, ", ");
					}//for loop
					//Fix up listOfConnections formatting
					listOfConnections[strlen(listOfConnections)-2] = '\0';
					strcat(listOfConnections, ".");
				}//end third if
				else{
					fgets(line, maxLineSize, midRoom_3);
					sscanf(line, "%s %s %s", junk, alsoJunk, data);
					//printf("User Input: %s, Name: %s\n", userInput, data);//DEBUG
					if(strcmp(data, userInput) == 0){
						//that's the correct room! Move data to there!
						strcpy(name, data);//We just compared the name, so grab that
						//update the rest of the room data
						while(fgets(line, maxLineSize, midRoom_3) != NULL){
							sscanf(line, "%s %s %s", junk, alsoJunk, data);
							//If it's the room type, put it into connectionType
							if(strcmp(data, "MID_ROOM") == 0){
								strcpy(connectionType, data);
							}//end if
							//Otherwise, it must be a connection
							else{
								strcpy(connection[i], data);
								i++;
								numberOfConnections++;
							}//end else
						}//end while
						//now, create a list of all the connections
						for(i = 0;i < numberOfConnections;i++){
							strcat(listOfConnections, connection[i]);
							strcat(listOfConnections, ", ");
						}//end for loop
						//Fix up listOfConnections formatting
						listOfConnections[strlen(listOfConnections)-2] = '\0';
						strcat(listOfConnections, ".");
					}//end fourth if
					else{
						fgets(line, maxLineSize, midRoom_4);
						sscanf(line, "%s %s %s", junk, alsoJunk, data);
						//printf("User Input: %s, Name: %s\n", userInput, data);//DEBUG
						if(strcmp(data, userInput) == 0){
							//that's the correct room! Move data to there!
							strcpy(name, data);//we just compared the name, so grab that
							//Update the rest of the room data
							while(fgets(line, maxLineSize, midRoom_4) != NULL){
								sscanf(line, "%s %s %s", junk, alsoJunk, data);
								//If it's the room type, put it into connectionType
								if(strcmp(data, "MID_ROOM") == 0){
									strcpy(connectionType, data);
								}//end if
								//otherwise, it must be a connection
								else{
									strcpy(connection[i], data);
									i++;
									numberOfConnections++;
								}//end else
							}//end while
							//now, create a list of all the connections
							for(i = 0;i < numberOfConnections;i++){
								strcat(listOfConnections, connection[i]);
								strcat(listOfConnections, ", ");
							}//end for loop
							//Fix up listOfConnections formatting
							listOfConnections[strlen(listOfConnections)-2] = '\0';
							strcat(listOfConnections, ".");
						}//end fifth if
						else{
							fgets(line, maxLineSize, midRoom_5);
							sscanf(line, "%s %s %s", junk, alsoJunk, data);
							//printf("User Input: %s, Name: %s\n", userInput, data);//DEBUG
							if(strcmp(data, userInput) == 0){
								//that's the correct room! Move data to there!
								strcpy(name, data);//we just compared the name, so grab that
								//Update the rest of the room data
								while(fgets(line, maxLineSize, midRoom_5) != NULL){
									sscanf(line, "%s %s %s", junk, alsoJunk, data);
									//If it's the room type, put it into connectionType
									if(strcmp(data, "MID_ROOM") == 0){
										strcpy(connectionType, data);
									}//end if
									//otherwise, it must be a connection
									else{
										strcpy(connection[i], data);
										i++;
										numberOfConnections++;
									}//end else
								}//end while
								//now, create the list of all the connections
								for(i = 0;i < numberOfConnections;i++){
									strcat(listOfConnections, connection[i]);
									strcat(listOfConnections, ", ");
								}//end for loop
								//Fix up listOfConnections formatting
								listOfConnections[strlen(listOfConnections)-2] = '\0';
								strcat(listOfConnections, ".");
							}//end sixth if. I'm sorry you have to read through all of this.
								//But I'm more sorry I had to write and debug it all
							else{
								fgets(line, maxLineSize, endRoom);
								sscanf(line, "%s %s %s", junk, alsoJunk, data);
								//printf("User Input: %s, Name: %s\n", userInput, data);//DEBUG
								if(strcmp(data, userInput) == 0){
									//that's the correct room! Move data to there!
									//Also that's the end room, so you won!
									strcpy(name, data);//We just compared the name, so grab that
									//it's the final room, so I don't actually need to copy anything over
									//But still, update the rest of the room data
									while(fgets(line, maxLineSize, endRoom) != NULL){
										sscanf(line, "%s %s %s", junk, alsoJunk, data);
										//If it's room type, put it into connectionType
										if(strcmp(data, "END_ROOM") == 0){
											strcpy(connectionType, data);
										}//end if
										//Otherwise, it must be connection
										else{
											strcpy(connection[i], data);
											i++;
											numberOfConnections++;
										}//end else
									}//end while
									//now, create the list of all the connections
									for(i = 0;i < numberOfConnections;i++){
										strcat(listOfConnections, connection[i]);
										strcat(listOfConnections, ", ");
									}//end for loop
									//fix up listOfConnections formatting
									listOfConnections[strlen(listOfConnections)-2] = '\0';
									strcat(listOfConnections, ".");
								}//end seventh if
								else{
									//printf("Something has gone horribly wrong. How did you get here?!\n");
									printf("\nHUH?! WHAT DID YOU DO?! TRY AGAIN!\n");
								}//end else
							}//end midRoom_5 else
						}//end midRoom_4 else
					}//end midRoom_3 else
				}//end midRoom_2 else
			}//end midRoom_1 else
		}//end startRoom else, and this entire ugly if/else block
		}//end else put in for time if. So /now/ it's the end of that whole mess.
	
	//rewind all file pointers
	rewind(startRoom);
	rewind(midRoom_1);
	rewind(midRoom_2);
	rewind(midRoom_3);
	rewind(midRoom_4);
	rewind(midRoom_5);
	rewind(endRoom);

	//now we have moved over to the correct room
	//check to see if it's the end room, and if it is set hasWon to 1/true and exit the game loop.
	//If it's not the end room, then print out all the room data for the new room and loop through this again.	
	if(strcmp(connectionType, "END_ROOM") == 0){
		//We're at the end room! Yay!
		hasWon = 1;//true
	}
	else{
		//Since the player hasn't won, get another path option
		printf("CURRENT LOCATION: %s\n", name);
		printf("POSSIBLE CONNECTIONS: %s\n", listOfConnections);
		printf("WHERE TO? >");
	}//end else

	}//end game loop

	//Let the player know they have won and display their number of moves and path taken
	printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", numberOfMoves);
	printf("%s", pathTaken);
	exit(EXIT_SUCCESS);//make sure exit code is 0, since this all worked
}//end main()













































