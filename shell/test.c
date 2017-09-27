#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mytoc.h"
#include "test.h"
#include <errno.h>

int main(int argc, char **argv, char **envp){
  int size = 1024;
  int i = 0;
  char *str = (char *)malloc(size * sizeof(char));
  char delim = ' ';
  char *path;
  char **posPath;
  char *realPath;
  char *cmd;
  char **vector;
  pid_t pid;  

  while(1){
    write(1, "$ ", 2);
    scanf("%[^\n]%*c",str);                        //get input from user, avoiding \n
    if(compareWord("exit", str) == 1){
	return 0;
    }
    vector = mytoc(str, delim);
    cmd = vector[0];
    if(access(cmd, F_OK) == 0){                    //access function will check if the path exists, return 0 if it does
      pid = saferFork();                           //calling saferFork, from the examples
      if(pid == 0){                                //if it returs no error
        execve(cmd, vector, envp);                 //call execve, in case there was given a correct path
      }
      else{                                        //in case it is still executing the child process
	wait(NULL);
      }
    }
        
    while(*envp != (char*)0){                      
      char ** tempVector = mytoc(envp[i], '=');    //tokenizing environmental variables
      if(compareWord(tempVector[0], "PATH") == 1){ //looking for the one containing the PATH
	path = tempVector[1];                      //when found the path, save its contents on var path   
	break;
      }
      envp++;
      free(tempVector);
    }
    posPath = mytoc(path, ':');                    //tokenizing the given path
    while(*posPath){                               //traversing the possible paths
      realPath = concatenate(*posPath, "/");       //adding '/' to the possible paths
      realPath = concatenate(realPath, cmd);       //adding the given command by the user after '/'
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

		
