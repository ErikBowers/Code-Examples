///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Program name: bowerser.buildrooms.c
//Programmer name: Erik Bowers
//		   OSU CS344, Winter term 2018
//		   Assignment 2
//
//Purpose:Create 7 random rooms connected together for program bowerser.adventure.c to use.
//	  A new directory is created, using this process's PID, to hold the files.
//	  The 7 rooms are all put into seperate files for the adventure program to access.
//	  The following outline is the one given in class, added to as needed.
//
//Notes: Comments are above the line or block of code they're describing.
//	 Debug lines will be commented out, with a comment of DEBUG following the line.
//	 	Most debug lines removed to increase clarity and flow.
//	 Function names use CamelCase, starting with a capital letter.
//	 Variable names use camelCase, starting with a lower-case letter.
//	 The opening brace for loops and functions is on the same line as the decleration.
//	 	The closing brace for loops and functions has a comment explaing what it's closing
//	 Because C89 doesn't have booleans, ints are used instead.
//	 	0 is false, 1 is true. All lines using these pseudo-booleans have a comment following
//	 	indicating if it is true or false. (Mostly for debugging, but they read nicely too)
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

////////////////////////////////////////////////////////
//Struct: Room
//	  Holds all the requisite data for a room
struct Room{
	int id; //what location in the array this room's name is from. Makes for easy looping and such
	char* name; //The name of the room
	int type; //1 for start room, 2 for mid room, and 3 for end room. Everything else is an error
	struct Room* connections[6]; //An array of pointers to all the rooms it's connected to
	int connectionsUsed; //How many of its six connections its used
};//end struct Room

//////////////////////////////////////////////////////
//The struct that holds all the rooms
//globally delcared for all the functions to use it easily
struct Room rooms[7];

//////////////////////////////////////////////////////////
//Function: GetRandomRoom()
//	    Takes no input returns a random Room struct from the rooms array.
int runCount = 1;//Modifies the srand seed even if the time hasn't changed since the last use
struct Room GetRandomRoom(){
	srand(time(NULL) * (runCount));
	int rNum = (rand() % 7);
	runCount++;
	//printf("%d Random room name: %s\n", rNum, rooms[rNum].name);//DEBUG
	return rooms[rNum];
}//end temp struct placement

////////////////////////////////////////////////////////////
//Function: IsGraphFull()
//	    Takes no input, returns an int (boolean)
//	    Returns 1/true if all the rooms have three to six outbound connections
//	    returns 0/false otherwise
int IsGraphFull(){
	int isValid = 1; //true
	int i;
	for(i = 0;i < 7;i++){
		if(rooms[i].connectionsUsed < 3){
			isValid = 0;//false
		}
	}//end for loop
	return isValid;
}//end IsGraphFull() function

////////////////////////////////////////////////////////////
//Function: AddRandomConnection()
//	    Takes no input, returns no input
//          Adds a random, valid outbound connection from Room A to Room B and Room B to Room A
//          Selects both rooms itself, randomly.
//          	Won't select invalid posibilities:
//          	 Connecting a room to itself.
//          	 Connecting two rooms that already have a connection to each other.
//          	 Connecting where a room already has six outbound connections.
void AddRandomConnection(){
	struct Room A;
	struct Room B;
	
	while(1){
		A = GetRandomRoom();
		if(CanAddConnectionFrom(A) == 1)
		break;
	}
	do{
		B = GetRandomRoom();
		//printf("Random Room B: %s, A: %s\n", B.name, A.name);//DEBUG
	}
	while(CanAddConnectionFrom(B) == 0 || IsSameRoom(A, B) == 1 || ConnectionAlreadyExists(&A, &B) == 1);

	//printf("Making connection between %s and %s\n", A.name, B.name);//DEBUG
	int kitty = ConnectRoom(A, B);//ConnectRoom does A to B and B to A, so only one call
	//kitty is just here to stop some annoying conflicting type warnings on ConnectRoom function
	//ConnectRoom wants to return an int and who am I to tell it otherwise?
}//end AddRandomConnection() function

/////////////////////////////////////////////////////////////////
//Function: CanAddConnectionFrom(struct Room x)
//	    Takes a Room as input, returns an int (boolean)
//	    Returns 1/true if there are fewer than six outbound connections
//	    Returns 0/false otherwise (there are six or more outbound connections
//Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
int CanAddConnectionFrom(struct Room x){
	int isValid = 0;//false
	if(x.connectionsUsed < 6){
		isValid = 1;
		//printf("Input Room %s has %d connections\n", x.name, x.connectionsUsed);//DEBUG
	}//end if
	return isValid;
}//end CanAddConnectionFrom(struct Room x) function

//////////////////////////////////////////////////////////////////
//Function: ConnectionAlreadyExists(Room x, Room y)
//	    Takes two Rooms as input, returns an int (boolean)
//	    Returns 1/true if a connection between Room x and Room y already exists
//	    Returns 0/false if there isn't a connection between Room x and Room y
int ConnectionAlreadyExists(struct Room* x, struct Room* y){
	int xLocation, yLocation, i;
	int connectionExists = 0;//false
	//find out what room x is
	for(i = 0;i < 7;i++){
		if(rooms[i].id == x->id){
			xLocation = i;
		}//end if
	}//end for loop
	//find out what room y is
	for(i = 0;i < 7;i++){
		if(rooms[i].id == y->id){
			yLocation = i;
		}//end if
	}//end for loop

	//printf("\nInside ConnectionAlreadyExists\n");//DEBUG
	//printf("xLocation: %d, yLocation: %d, x.id %d, y.id %d\n", xLocation, yLocation, x->id, y->id);//DEBUG
	//printf("name: %s, name: %s\n", rooms[xLocation].name, rooms[yLocation].name);//DEBUG
	
	//check all connections in x to see if any have the id of y
	for(i = 0;i < rooms[xLocation].connectionsUsed;i++){
		if(strcmp(rooms[yLocation].name, rooms[xLocation].connections[i]->name) == 0){
		//printf(", %s == %s", rooms[yLocation].name, rooms[xLocation].connections[i]->name);//DEBUG		
		connectionExists = 1;//true
		}//end if
	}//end for loop
	for(i = 0;i < rooms[yLocation].connectionsUsed;i++){
		if(strcmp(rooms[xLocation].name, rooms[yLocation].connections[i]->name) == 0){
		//printf(", %d == %d", rooms[xLocation].id, rooms[yLocation].connections[i]->id);//DEBUG
		connectionExists = 1;//true
		}//end if
	}//end for loop
	//printf(" connectionExists: %d\n", connectionExists);//DEBUG
	return connectionExists;
}//end ConnectionAlreadyExists(struct Room* x, struct Room* y) function

///////////////////////////////////////////////////////////////////////
//Function: ConnectRoom(struct Room x, struct Room y)
//	    Takes Room x and Room y as input, returns nothing
//	    	The returning of 1 is to stop type errors.
//	    	Had I more time I'd dig more into solving
//	    	those errors, but this will work for now.
//	    Creates a connection between Room x and Room y
//	    Note: Does not check if connection is valid
//	    Also, gcc gives a type warning for this function, but
//	    the whole thing works flawlessly and I have no idea
//	    why it's giving the warning. I really miss IDEs
//	    that give good descriptions of warnings.
int ConnectRoom(struct Room x, struct Room y){
	int temp = x.connectionsUsed;
	int xLocation, yLocation, i;
	//find out what room x is
	for(i = 0;i < 7;i++){
		if(strcmp(rooms[i].name, x.name) == 0){
			xLocation = i;
		}//end if
	}//end for loop
	//find out what room y is
	for(i = 0;i < 7;i++){
		if(strcmp(rooms[i].name, y.name) == 0){
			yLocation = i;
		}//end if
	}//end for loop
	rooms[xLocation].connections[rooms[xLocation].connectionsUsed] = &rooms[yLocation];
	rooms[yLocation].connections[rooms[yLocation].connectionsUsed] = &rooms[xLocation];
	rooms[xLocation].connectionsUsed++;
	rooms[yLocation].connectionsUsed++;
	return 1;//stops type warnings
	//printf("I'm done recafoobelling the energy-matron; or connecting the rooms, whatever.\n");//DEBUG
}//end ConnectRoom(Room x, Room y) function

///////////////////////////////////////////////////////////////////
//Function: IsSameRoom(struct Room x, struct Room y)
//	    Takes Room x and Room y as input, returns an int (boolean)
//	    Returns 1/true if Room x and Room y are the same room (have the same id)
//	    Returns 0/false if Room x and Room y are not the same room (have different ids)
int IsSameRoom(struct Room x, struct Room y){
	int result = 0;//false
	if(x.id == y.id){
		result = 1;//true
	}
	return result;
}//end IsSameRoom() function

///////////////////////////////////////////////////////////////////
//Function: PrintRoomOutboundConnections(struct Room* input)
//	    Takes a Room as input, returns nothing.
//	    Prints out name and id of room, then all of its connections (their name and id too)
//	    Everything sent to stdout
//	    A DEBUG function
void PrintRoomOutboundConnections(struct Room* input){
	printf("%s/%d has %d outbound connections\n", input->name, input->id, input->connectionsUsed);
	int location = 0;
	int i;
	for(i = 0;i < 7;i++){
		if(rooms[i].id == input->id){
			location = i;
		}//end if
	}//end for loop
	printf("%d, Room: (%s/%d)\n", location, rooms[location].name, rooms[location].id);
	for(i = 0;i < rooms[location].connectionsUsed;i++){
		printf("	(%s/%d)\n", rooms[location].connections[i]->name, rooms[location].connections[i]->id);
	}//end for loop
}//end PrintRoomOutboundConnections(struct Room* input) function

/////////////////////////////////////////////////////////////////////
//Function: PlaceIntoFiles()
//	    Takes no input, returns nothing.
//	    Creates a directory using PID, and places each room into its own file.
//	    Formats all the Room data properly.
void PlaceIntoFiles(){
	int processID = getpid();
	//printf("Process ID: %d\n", processID);//DEBUG
	int file_descriptor, i, j;
	ssize_t nwritten;
	char dirName[20]; //large enough to hold bowerser.rooms.processID
	//create the name of the directory, using this program's PID
	sprintf(dirName, "bowerser.rooms.%d", processID);
	char pathName[50]; //just a random large size, I don't want to do math right now
	char tempPathName[50];
	strcpy(pathName, dirName);
	strcpy(tempPathName, pathName);
	//create a directory using the created path name
	int result = mkdir(dirName, 0755);
	//TODO check that mkdir worked before making the files inside that directory
	
	//For every Room in the rooms array, create a filePath name and create a file
	//then fill that file with all the data from that Room
	for(i = 0;i < 7;i++){
		char tempLine[23];
		char tempString[23];
		sprintf(tempPathName, "%s/room%d", pathName, i);
		//printf("File Name: %s\n", tempPathName);//DEBUG
		file_descriptor = open(tempPathName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		//format the room's name as required
		sprintf(tempLine, "ROOM NAME: %s\n", rooms[i].name);
		//Write the room's name to file
		nwritten = write(file_descriptor, tempLine, strlen(tempLine) * sizeof(char));
		//format and write each of the room's connections to the file
		for(j = 0;j < rooms[i].connectionsUsed;j++){
			strcpy(tempLine, "");
			sprintf(tempLine, "CONNECTION %d: %s\n", (j + 1), rooms[i].connections[j]->name);
			nwritten = write(file_descriptor, tempLine, strlen(tempLine) * sizeof(char));
		}//end for loop for adding connections to the file
		strcpy(tempLine, "");
		//format the room's connection type to the file
		if(rooms[i].type == 0){
			sprintf(tempLine, "ROOM TYPE: START_ROOM\n");
		}else{
			if(rooms[i].type == 2){
				sprintf(tempLine, "ROOM TYPE: END_ROOM\n");
			}else{
				sprintf(tempLine, "ROOM TYPE: MID_ROOM\n");
			}
		}//end nested if/else
		//write the room's connection type to the file
		nwritten = write(file_descriptor, tempLine, strlen(tempLine) * sizeof(char));
		//readdy tempPathName for the next loop around
		strcpy(tempPathName, pathName);
	}//end for loop
}//end PlaceIntoFiles() function


///////////////////////////////////////////////////////////////////////////
//Function: main
//	    Creates array of 10 names.
//	    Picks seven names at random, creates seven rooms
//	    Fills "graph" with required number of connections
//	    Creates a directory and seven files, places all
//	    the "graph" data into those files.
//	    Displays completed graph back to stdout DEBUG VERSION ONLY
int main(int argc, char* argv[]){
//hardcode all room names into an array of String literals
char* roomNames[10];
roomNames[0] = "Garage";
roomNames[1] = "Parlor";
roomNames[2] = "Bathroom";
roomNames[3] = "Atrium";
roomNames[4] = "Bedroom";
roomNames[5] = "Kitchen";
roomNames[6] = "Library";
roomNames[7] = "WarRoom";//I hope the WarRoom doesn't connect to the Armory, since there's no fighting in the War Room.
roomNames[8] = "Workshop";	//Ah, yes, 60's movie references. Clearly I know how to write topical comments!
roomNames[9] = "Armory";

//build graph. This loop creates seven rooms. Only when the graph is complete will they be placed into files.
//everything needs some content for loops later on, but room type is assigned here
int i = 0;
for(i = 0;i < 7;i++){
	if(i == 0)
	{
		rooms[i].type = 0;
	}else{
		if(i == 6)
		{
			rooms[i].type = 2;
		}else{
			rooms[i].type = 1;
		}
	     }//end nested if/else
	rooms[i].id = -1;
	rooms[i].connectionsUsed = 0;
}//end for loop

srand(time(NULL)); //Having to set the seed to use rand() and have it be different every execution is really dumb
			//It would be less dumb if K&R C book actually told you this, though.
			//assistence via: stackoverflow.com/questions/3067364/generating-random-numbers-in-c
			//specifically for using time as a seed, as srand itself is pretty straight forward
int randomNumber = 4; //chosen by a fair dice roll. xkcd.com/221
i = 0;
int j = 0;
int isValid = 1; //true (C89 lacking Booleans is just weird, but this works. 0 is false, anything else is true)

//pick seven rooms from the list of 10, randomly, filling rooms[]
for(i = 0;i < 7;i++){
	isValid = 0; //false
	while(isValid == 0){ //while isValid is false
		isValid = 1; //true
		randomNumber = (rand() % 10); //bad random number gnerator, but it works well enough here
		//printf("oh, hey, a random number!: %d\n", randomNumber); //DEBUG
		for(j = 0;j < 7;j++){
			if(randomNumber == rooms[j].id){
				//printf("Aww, %d is already in use\n", randomNumber); //DEBUG
				isValid = 0;
			}//end if
		}//end inner for loop
	}//end isValid while
	//we now have a valid number
	rooms[i].id = randomNumber;
	rooms[i].name = calloc(16, sizeof(char));//TODO free this memory. Maybe reduce it to 8, our max name size
	strcpy(rooms[i].name, roomNames[randomNumber]);
}//end for loop for populating rooms[]

//printf("Let's see what the rooms all are!\n");//DEBUG
//DEBUG printing loop
//for(i=0;i<7;i++){
//	printf("Name: %s, id: %d\n", rooms[i].name, rooms[i].id);
//}//end for loop
//printf("Let's complete the graph:\n");//DEBUG

//connect all the rooms until the graph is full (every room has 3 to 6 connections)
//i = 0;//DEBUG
while(IsGraphFull() == 0){
	//printf("\nAdd connection %d\n", i);//DEBUG
	AddRandomConnection();
	//i++;//DEBUG
}//end while loop

//printf("Let's print out the finished graph\n");//DEBUG
//print out all the rooms and their connections. DEBUG
//for(i = 0;i < 7;i++){
	//printf("Room %d, %s, %d\n", i, rooms[i].name, rooms[i].id);//DEBUG
	//PrintRoomOutboundConnections(&rooms[i]);//DEBUG
//}//end for loop
//
//Finally, take all that rooms data and place it into its files!
PlaceIntoFiles();

return 0;
}//end main
