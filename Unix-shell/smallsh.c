/////////////////////////////////////////////////////////////////////////////////////////
//PROGRAMMER: Erik Bowers
//PROGRAM NAME: smallsh.c
//////////////////////////////////////////////////////////////////////////////////////////
//PROGRAM: Assignment 3 for CS344 at OSU, Winter term 2018
//	basic UNIX shell.
//	Accepts input and output redirection as well as running commands
//		in the background.
//	BUILT IN COMMANDS:
//	                 status:
//				return exit status of last command.
//		             cd:
//				change directory to one given
//				If no path is given, go HOME.
//			   exit:
//				exit the shell.
//			 ctrl+z:
//			 	Toggle run in foreground-only mode.
//			 	While in foreground-only mode, any attempts
//			 	to run commands in the background will be
//			 	ignored.
//
///////////////////////////////////////////////////////////////////////////////////////////
//NOTE: Comments are either above the line or block of code they're describing
//		or on the same line.
//	Debug lines will be commented out, with a comment of DEBUG following the line.
//		Most DEBUG lines removed to increase clarity and flow.
//	Variable names use camelCase, starting with a lower-case letter.
//	THe opening brace for loops/functions/conditionals/etc is on the same
//		line as the declaration. The closing brace is on its own line,
//		with a comment explaining what it's closing.
//	Programmed in C89, so ints are used for booleans
//		0 is false, 1 is true. Lines using these pseudo-booleans
//		have a comment following indicating if it's true or false.
//		(mostly for debugging, but they read well too)
//
//	This code's base was the userinput_adv.c code given in class, mostly
//	dealing with getting good user input in a shell environment. 
//	Obviously this code was heavily modified as needed.
//
//	brennan.io/2015/01/16/write-a-shell-iin-c/ was a tutorial referenced
//	quite often for the beginning of this shell. A lot of what they did
//	was different than how we were instructed, and copying their code wouldn't
//	have helped me learn, but having a tutorial that was going through
//	making a shell was key in knowing what questions to ask myself. So while
//	the code wasn't really helpful, the tutorial itself still was.
////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

///////////////////////////////////
//Global variables and signal handlers
///////////////////////////////////
int foregroundOnlyMode = 0; //false
int wasChanged = 0; //false

//catches any ctrl+c/SIGINT that's sent
//not just set to ignore for debugging reasons
void catchSIGINT(int signo){
	//char* message = "SIGINT. Use exit to stop.\n";//DEBUG
	//write(STDOUT_FILENO, message, 26);
}//end catchSIGINT

//catch any ctrl+z/SIGTSTP that's sent
//but don't actually do anything with that right now, just cycle
	//some global variables so that it can be checked before user input
void catchSIGTSTP(int signo){
	if(foregroundOnlyMode == 0){

		foregroundOnlyMode = 1;
		wasChanged = 1;
	}else{

		foregroundOnlyMode = 0;
		wasChanged = 1;
	}
}//end catchSIGTSTP

///////////////////////////
//main() start!
/////////////////////////
void main(){
	struct sigaction SIGINT_action = {0}, SIGTSTP_action = {0};
	
	SIGINT_action.sa_handler = catchSIGINT;
	sigfillset(&SIGINT_action.sa_mask);
	sigaction(SIGINT, &SIGINT_action, NULL);
	
	SIGTSTP_action.sa_handler = catchSIGTSTP;
	sigfillset(&SIGTSTP_action.sa_mask);
	SIGTSTP_action.sa_flags = 0;
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);


	int exitStatus = 0; //clean exit unless told otherwise
	int numCharsEntered = -5;
	int currentChar = -5;
	size_t bufferSize = 0;
	char* lineEntered = NULL;//line entered can be 2048 bytes/chars
	lineEntered = malloc(2048 * sizeof(char));
	memset(lineEntered, '\0', sizeof(2048 * sizeof(char)));
	char* lineEnteredToken = NULL;
	char** enteredCommandAndArguments;
	int position = 0;
	char* command = NULL;
	pid_t spawnPid = -5;
	int childExitStatus = -5;
	int numberOfTokens = -5;
	int runInBackground = 0; //false
	int numberOfChildProcesses = 0;
	int i;
	pid_t backgroundPID = NULL;
	
/////////////////////////////////
//Shell while loop. Leave this and exit the shell
///////////////////////////////

	while(1){
		numCharsEntered = 0;//reset
		runInBackground = 0;//reset
		//get input from user
		while(1){
				//see if our background process has terminated
				pid_t tempBackgroundPID = waitpid(backgroundPID, &childExitStatus, WNOHANG);
				if(tempBackgroundPID > 0){
					printf("Background pid %d terminated. Signal %d\n", tempBackgroundPID, childExitStatus);
					fflush(stdout);
					numberOfChildProcesses--;
				}//end background child check if
				//see if ctrl+z/SIGTSTP has been entered since last run
				if(wasChanged == 1){
					//entered foreground only mode, tell the user such
					if(foregroundOnlyMode == 1){
						printf("Entering foreground-only mode (& is now ignored)\n");
						fflush(stdout);
						wasChanged = 0;	
					}else{
						//exited foreground only mode, tell the user such
						printf("Exiting foreground-only mode\n");
						fflush(stdout);
						wasChanged = 0;
					}
				}//end SIGTSTP if 
			
			printf(":");//command prompt
			fflush(stdout);
			numCharsEntered = getline(&lineEntered, &bufferSize, stdin);//get the input from the user
			if(numCharsEntered == -1){
				clearerr(stdin);
			}else
				break; //Bail out, we've got our input
		}//end inner while

		//remove the \n that getline adds. If there was just a newline, then command will be NULL, and we check for that later
 		//printf("__Line entered:%s", lineEntered);//DEBUG
		lineEntered[strcspn(lineEntered, "\n")] = '\0';
		//printf("Here is the cleaned line: \"%s\"\n", lineEntered);//DEBUG
	
		//I would rather use sscanf but I have no idea how to do that with a possibility of 512 arguments
		//so strtok it is! Everything is just space delimited
		position = 0;//reset position
		lineEnteredToken = strtok(lineEntered, " ");
		//malloc space for enteredCommandAndArguments, but also clear it all out from the last time it was used
		enteredCommandAndArguments = malloc(513 * sizeof(char));//Max number of arguments is 512, + 1 for command
		//memset(enteredCommandAndArguments, '\0', sizeof(513 * sizeof(char)));
		while(lineEnteredToken != NULL){
			enteredCommandAndArguments[position] = lineEnteredToken;
			lineEnteredToken = strtok(NULL, " ");
			//printf("%d:%s\t", position, enteredCommandAndArguments[position]);//DEBUG
			position++;
		}
		numberOfTokens = position;
		//Now, the command and all the arguments should be inside this nifty char**
		//enteredCommandAndArguments[0] is always a command, and can now be compared as such
		//printf("location 0:%s\n", enteredCommandAndArguments[0]);//DEBUG
		command = enteredCommandAndArguments[0];
		//printf("Command:%s\n", command);//DEBUG

	//////////////////////////////////////////////
	//Parse input, checking for commands and executing as needed
	//////////////////////////////////////////////
	
	//check if command is a NULL (meaning it was just a newline), as it breaks things if it is NULL and we use strcmp on it
	if(command == NULL){
		printf("\n");
		fflush(stdout);
	}//end NULL if
	else{	
	//check for $$ in one of the arguments, and expand it to be the process ID if it's there
	for(i = 0;i < numberOfTokens;i++){
		char temp[256];//we don't have a maximum argument size, but 256 seems plenty large
		memset(temp, '\0', sizeof(temp));
		//copy the command/argument into temp so we can actually do things with it without seg faulting
		strcpy(temp, enteredCommandAndArguments[i]);

		//adapted from code from class lecture where we're finding a terminating sequence and removing it
		//If there is the sequence "$$" in temp then we'll remove those characters and add the process ID
			//to the end of temp
		if(strstr(temp, "$$") != NULL){
			//some basic array math to turn an address into a location in the array
			int terminalLocation = strstr(temp, "$$") - temp;
			//printf("terminalLocation:%d, numberOfTokens:%d\n", terminalLocation, numberOfTokens);//DEBUG
			pid_t tempProcessID = getpid();
			strcpy(temp, enteredCommandAndArguments[i]);
			temp[terminalLocation] = '\0';
			temp[terminalLocation + 1] = '\0';
			sprintf(temp, "%s%d", temp, tempProcessID);
			enteredCommandAndArguments[i] = temp;
		}	
	}//end PID expansion for loop
		//status check
		//return with the exit status of the last child process to exit
		if(strcmp(command, "status") == 0){
			printf("exit value %d\n", childExitStatus);
			fflush(stdout);
		}//end status if
		else{//status else
			//Check input to see if it's "exit", and exit(0) if it is
			if(strcmp(command, "exit") == 0){
				exit(0);
			}//end exit if
			else{//exit else
				//check input to see if it's $$, and output the PID if it is
				//this shouldn't ever trigger anymore, but it's too deep into the nested if/else to easily remove it
				if(strcmp(command, "$$") == 0){
					pid_t tempID = getpid();
					printf("%d\n", tempID);
					fflush(stdout);
				}//end $$ expansion
				else{//$$ else
					//check input to see if it's "cd"
					if(strcmp(command, "cd") == 0){
						//This triggers if no arguments were sent with cd or if someone tried to run cd in the background
						if(enteredCommandAndArguments[1] == NULL || strcmp(enteredCommandAndArguments[1], "&") == 0){
							chdir(getenv("HOME"));//change directory to home, if input was just cd
							//printf("Entered cd with no arguments, go home\n");//DEBUG
							printf("\n");
							fflush(stdout);
						}
						else{	
							//then it must have an argument!
							//And we can just use enteredCommandAndArguments[1] for that!
							//So if someone tried to run this in the background it'll ignore it too
							int tempValue = chdir(enteredCommandAndArguments[1]);
							if(tempValue == -1){
								printf("cd failed. No such directory.\n");
								fflush(stdout);
							}//end inner if
							printf("\n");
							fflush(stdout);
							//printf("Location 1:%s\n", enteredCommandAndArguments[1]);//DEBUG
						}//end nested if/else
					}//end cd if
					else{//cd else
						//Check for comments
						//if( (strstr(lineEntered, "#") != NULL) ){
						if( (strstr(command, "#") != NULL) ){
							//printf("That was a comment\n");//DEBUG
							printf("\n");
							fflush(stdout);
							//If it just has a # in the middle of the command it'll get caught by this
							//but since we aren't testing for invalid syntax or middle of the line comments this should be okay
							//That was a comment, so do nothing loop back around
						}//end comment if
						else{//comment else
							//welp, guess this command will require us to fork() and exec()
							//The fork() will have the same code and variables but independent of this parent process
							//printf("08th team, move out! We got a fork() and an exec()!\n");//DEBUG
							childExitStatus = 0; //reset
							runInBackground = 0; //reset
							
							//loop through and look for "&", and if it's the last command then set runInBackground to 1/true
							//if it's in foreground-only mode then this variable will be ignored, so safe to set it here.
							if( strcmp(enteredCommandAndArguments[numberOfTokens - 1], "&") == 0){
								runInBackground = 1;//true	
							}//end if
						
							spawnPid = fork();
							//pid_t whatProcessIsThis = getpid();//DEBUG
							//printf("Just forked. Process: %d\n", whatProcessIsThis);//DEBUG

							//switch statement. If it's the parent, call waitpid as needed. If it's the child, swap file descriptors as needed then run exec
							switch(spawnPid){
								case -1:{perror("Abandon Ship!\n");exitStatus = 1; exit(1); break; }
								case 0:{
										//printf("Child process start!\n");
										//We want to now parse through the arguments array and do things
										//if there are any special symbols <, >, or &
										//< says that we should read from the next input instead of stdin
										//	so, dup2(targetFD, 0)
										//> says that we should print to the next input instead of stdout
										//	so, dup2(targetFD, 1)
										//& says that this process should be run in the background
										//	so just swap over stdin and stdout to dev/null unless
										//	they were already swapped with this call
	
										//loop through all of the arguments, seeing if any is one of them ^
										int result = -5;
										int count = 0;
										//char** tempArguments = NULL;
										//int characterSkipped = 0; //false
										//tempArguments = malloc(513 * sizeof(char));//TODO remove this and all its crap
										//exitStatus = 0; //reset from last time.
										pid_t childPID = getpid();
										//printf("I'm the child process: PID:%d\n", childPID);//DEBUG
										//fflush(stdout);//DEBUG
										
										for(position = 0; position < numberOfTokens; position++){
											//if enteredCommandAndArguments[position] is "<"
											//input from a file, so swap FDs
											if(strcmp(enteredCommandAndArguments[position], "<") == 0){
												//set "<" to NULL so it can't be read by exec()
												enteredCommandAndArguments[position] = NULL;
												//[position + 1] must be the file name!
												//printf("Trying to open file with name: %s\n", enteredCommandAndArguments[position + 1]);//DEBUG
												int sourceFD;
												sourceFD = open(enteredCommandAndArguments[position + 1], O_RDONLY);//I use vim with a monitor in portrait instead of landscape, and these nested statements suck. Seeing 3x number of lines is nice though
												//printf("Reading from a file: %s\tFD:%d\n", enteredCommandAndArguments[position + 1], sourceFD);//DEBUG
												if(sourceFD == -1){perror(enteredCommandAndArguments[position + 1]); exit(1);}
												result = dup2(sourceFD, 0);
												if(result == -1){perror("source dup2() failed"); exit(1);}
												//printf("Reading from a file: %s\tresult:%d\n", enteredCommandAndArguments[position + 1], result);//DEBUG
											}//end < if
											else{
												//if enteredCommandAndArguments[position] is ">"
												//output to, and likely create, a file
												if(strcmp(enteredCommandAndArguments[position], ">") == 0){
													//set ">" to NULL so it can't be read by exec()
													enteredCommandAndArguments[position] = NULL;
													//printf("Attempting to create file with name: %s\n", enteredCommandAndArguments[position + 1]);//DEBUG
													int targetFD;
													targetFD = open(enteredCommandAndArguments[position + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
													//printf("Printing to file: %s\tFD:%d\n", enteredCommandAndArguments[position + 1], targetFD);//DEBUG
													if(targetFD == -1){perror("open failed"); exit(1);}
													result = dup2(targetFD, 1);
													//printf("Print to file: %s\tresult:%d\n", enteredCommandAndArguments[position + 1], result);//DEBUG
													if(result == -1){perror("target dup2 failed"); exit(1);}
												}//end > if
												else{
													//if entered command is "&" AND it's the last argument
													if(strcmp(enteredCommandAndArguments[position], "&") == 0 && position == (numberOfTokens - 1) ){
														//printf("Found &. foregroundOnlyMode?:%d\n", foregroundOnlyMode);
														//fflush(stdin);
														runInBackground = 1;
														//only swap over the file descriptors if we aren't in foreground only mode
														if(foregroundOnlyMode == 0){
															//runInBackground = 1;//true. Changing here doesn't change the parent's version, derp
															//check if stdin has been redirected, and redirect it if it hasn't
															if( STDIN_FILENO == fileno(stdin) ){
																//printf("change stdin to /dev/null\n");//DEBUG
																//fflush(stdin);//DEBUG
																int devNullFD;
																devNullFD = open("/dev/null", O_RDONLY);
																if(devNullFD == -1){perror("/dev/null open failed"); exit(1);}
																result = dup2(devNullFD, 0); //change stdin to be dev/null/	
																if(result == -1){perror("stdin -> dev/null change failed"); exit(1);}
																//printf("devNullFD = %d, result = %d\n", devNullFD, result);//DEBUG
																//fflush(stdin);//DEBUG
															}
															//check if stdout has been redirected, and redirect if it hasn't
															if( STDOUT_FILENO == fileno(stdout) ){
																//printf("change stdout to /dev/null\n");//DEBUG
																//fflush(stdout);//DEBUG
																int devNullFD;
																devNullFD = open("/dev/null", O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
																if(devNullFD == -1){perror("/dev/null open failed"); exit(1);}
																result = dup2(devNullFD, 1); //change stdout to be dev/null/
																if(result == -1){perror("stdout -> dev/null change failed"); exit(1);}
															}
																
														}//end outer if check
														enteredCommandAndArguments[position] = NULL;//set & to null so it can't be sent to exec()
													}//end & if
												}//end inner else
											}//end outer else
										}//end for loop
										
										//call exec using the command and list of arguments
										//if the exec runs, everything after it is never executed. so perror will only trigger on an error
										if(execvp(command, enteredCommandAndArguments) < 0){
											perror(command);
											exit(1);
										}//end exec if
										break;
									}//end case 0
								default:{
									//printf("Parent process doin' things!\n");//DEBUG
									//sleep(1);//testing if an issue was caused by waitpid not waiting. It wasn't.
									//pid_t parentPID = getpid();//DEBUG
									//printf("I'm the parent PID:%d\n", parentPID);//DEBUG
									//fflush(stdout);//DEBUG
									numberOfChildProcesses++;
									//always execute in the foreground if we're in foreground only mode, otherwise only execute if we weren't told to run in the background
									if(foregroundOnlyMode == 1 || (foregroundOnlyMode == 0 && runInBackground == 0) ){
										//printf("foreground child\n(foregroundmode:%d, runInBackground:%d\n", foregroundOnlyMode, runInBackground);//DEBUG
										//fflush(stdout);//DEBUG
										pid_t actualPID = waitpid(spawnPid, &childExitStatus, 0);//run in foreground, waiting for the child to finish
										numberOfChildProcesses--;
									}else{
										//run in the background
										printf("background pid is %d\n", spawnPid);
										fflush(stdout);
										pid_t backgroundPID = waitpid(spawnPid, &childExitStatus, WNOHANG);//background process, so don't wait for the child
									}//if/else
									
									//update childExitStatus
									if(WIFEXITED(childExitStatus) != 0){
										childExitStatus = WEXITSTATUS(childExitStatus);
									}else{
										if(WIFSIGNALED(childExitStatus) != 0)
											childExitStatus = WTERMSIG(childExitStatus);
									}
									//sleep(1);//DEBUG checking that my double :: isn't from a waiting error. It wasn't
									//printf("Parent is back in control!\n");//DEBUG
									//fflush(stdout);//DEBUG
									break;
								}//end default
							}//end switch statement
						}//end fork and exec call
					}	
				}
			}	
		}//end exit's if/elxe
	}//end huge if/else block

	//Free the memory allocated by getline() because I'm a responsible programmer	
	free(lineEntered);
	//free(enteredCommandAndArguments);//that broke things in amazing ways
	lineEntered = NULL;//reset lineEntered
	command = NULL;//reset command
	//Now, loop back up and get input again!
	}//end outer while

}//end main
