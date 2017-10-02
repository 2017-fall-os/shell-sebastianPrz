#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mytoc.h"
#include "test.h"
#include <errno.h>


int isPipe = 0;

int main(int argc, char **argv, char **envp){
  int size = 1024;
  int i = 0;
  char *str = (char *)malloc(size * sizeof(char));
  char delim = ' ';
  char **vector;
  
  while(1){
    write(1, "$ ", 2);
    scanf("%[^\n]%*c",str);                        //get input from user, avoiding \n
    if(compareWord("exit", str) == 1){
	return 0;
    }
    if(compareWord("\n", str) == 1){   //I have to fix this
      break;
    }
    vector = mytoc(str, delim);
    selectCommand(vector, envp, isPipe, str, delim);
  }
  
  return 0;
}

/*
Function will determine which of all the possible commands is the one entered
 */

int selectCommand(char **vector, char **envp, int isPipe, char *str, char delim){

  char *path;
  char **posPath;
  char *realPath;
  char *temp;
  pid_t pid;

  if(cdCommand(vector) == 1){                      //checking if the given command is cd
    if(countWords(str, delim) == 1){              //in case there was not given a directory, emulating the actual cd command it should go up one
      temp = "..";
      chdir(temp);
    }
    else{                                          //in case there was given a directory to change
      chdir(vector[1]);
    }
  }
  else if(lookForPipes(str) > 1){            //there are pipes that need to be considered
    isPipe = 1;
    

    
  }
  else{
    if(access(vector[0], F_OK) == 0){                    //access function will check if the path exists, return 0 if it does
      pid = saferFork();                           //calling saferFork, from the examples
      if(pid == 0){                                //if it returs no error
	execve(vector[0], vector, envp);                 //call execve, in case there was given a correct path
      }
      else{                                        //in case it is still executing the child process
	wait(NULL);
      }
      }
    path = lookForPath(envp);
    posPath = mytoc(path, ':');                    //tokenizing the given path
    while(*posPath){                               //traversing the possible paths
      realPath = concatenate(*posPath, "/");       //adding '/' to the possible paths
      realPath = concatenate(realPath, vector[0]);       //adding the given command by the user after '/'
      if(access(realPath, F_OK) == 0){             //access function will check if the new path exists
	  pid = saferFork();
	  if(pid == 0){
	    execve(realPath, vector, envp);
	  }
	  else{
	    wait(NULL);
	  }
      }  
      free(*posPath);
      posPath++;
    }
  }
  return 0;
}


/*
Look for PATH
 */
char* lookForPath(char **envp){
  int i = 0;
  char *path;
  while(*envp != (char*)0){                      
    char ** tempVector = mytoc(envp[i], '=');    //tokenizing environmental variables
    if(compareWord(tempVector[0], "PATH") == 1){ //looking for the one containing the PATH
      path = tempVector[1];                      //when found the path, save its contents on var path   
      break;
    }
    envp++;
    free(tempVector);
  }
  return path;
}

/*
Function to check if the entered command is cd
 */
int cdCommand(char **vector){
  if(compareWord(vector[0], "cd")){
    return 1;
  }
  return 0;
}

/*
In case the entered command words as a pipe
 */
char **determinePipes(char *str){
  int fileD[2];
  pid_t pid = saferFork();
  char **vector = mytoc(str, '|');
  int numWords = countWords(str, '|');
  int stdIn2 = dup (0);
  int stdOut2 = dup(1);
  fileD[0] = 0;
  fileD[1] = 1;
  for(int i = 0; i < numWords - 1; i++){
    pipe(fileD);
    
    if(pid == 0){
      
    
      close(fileD[0]);                          //Closing the input side of the pipe
      dup2(fileD[1], 0);
    
    }
    else{
      close(fileD[1]);
    }
  }
  return 0;
}

/*
Look for pipe passing the command as parameter and looking for '|' char
 */
int lookForPipes(char *str){
  return (countWords(str, '|'));
}


/*
concatenate function, will create a new char * adding the content 
from the first parameter and the second one
 */
char *concatenate(char *str, char *pStr){
  int lengthStr = countCharacters(str);                          //count number of characters
  int lengthPStr = countCharacters(pStr);
  char count = lengthStr + lengthPStr + 1;                       //adding the number of both parameters and adding 1 for the '\0'
  char * strConcat = (char *) calloc(count, sizeof(char *));     //assigning memory
  for(int i = 0; i < count; i++){
    if(i < lengthStr){                        //if the counter is less than the length, that means we are in the first word
      strConcat[i] = str[i];                  //copy it directly
    }
    else{                                     //else, we are in the second word
      strConcat[i] = pStr[i - lengthStr];     //copy the ith element minus the length of the first word, so that we can get the right element
    }
  }
  strConcat[count] = '\0';                    //0 terminate the char *
  return strConcat;
  
}
/*
Taken from the demos, it will wait a second for the fork function to get called
 */
pid_t saferFork(){
  //fprintf(stderr, "process <%d> calling fork() in 5 seconds...", getpid());
  sleep(1);
  //fprintf(stderr, "calling fork now!\n");
  return fork();
}
/*
Taken from the tokenizer demos, it counts how many times the pointer is increased, meaning how many characters there are
 */
int countCharacters(char *str){
  char * temp;
  int size = 0;
  for(temp = str; *temp; temp++)
    ;
  size = temp - str;
  return size;
}

		
