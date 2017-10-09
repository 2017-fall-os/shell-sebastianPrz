#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mytoc.h"
#include "test.h"
#include <errno.h>
#include <fcntl.h>

int main(int argc, char **argv, char **envp){
  int size = 1024;
  int i = 0;
  char *str = (char *)malloc(size * sizeof(char));
  char delim = ' ';
  
  while(1){
    write(1, "$ ", 2);
    scanf("%[^\n]%*c",str);                        //get input from user, avoiding \n
    if(compareWord("exit", str) == 1){
	return 0;
    }
    if(compareWord("\n", str) == 1){   //I have to fix this
      break;
    }
    selectCommand(envp, argv, str, delim);
  }  
  return 0;
}

/*
Function will determine which of all the possible commands is the one entered
 */
int selectCommand(char **envp, char **argv, char *str, char delim){
  char **vector = mytoc(str, delim);
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
    determinePipes(str, envp, argv); 
    
  }
  else if(lookForBackground(str) > 1){
    //    determineBackground(str, envp, argv);
  }
  else{
    if(access(vector[0], F_OK) == 0){                    //access function will check if the path exists, return 0 if it does
      pid = saferFork();                           //calling saferFork, from the examples
      if(pid < 0){
	printf("Error\n");
      }
      else if(pid == 0){                                //if it returs no error
	execve(vector[0], vector, envp);                 //call execve, in case there was given a correct path
	return 1;
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
	  if(pid < 0){
	    printf("Error\n");
	  }
	  else if(pid == 0){
	    execve(realPath, vector, envp);
	    return 1;
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
In case the entered command words as a pipe, used as template the demo provided
 */
int determinePipes(char *str,char** envp, char **argv){  
  int fd[2];                                //file descriptors
  char delim = ' ';
  pid_t pid = saferFork();
  char **vector = mytoc(str, '|');
  if(pid < 0){
    printf("Error\n");
  }
  else if(pid == 0){
    pipe(fd);
    pid_t pid = saferFork();
    if(pid < 0){
      printf("Error\n");
    }
    else if(pid == 0){                              //child
      close(1);                                     //closing output
      dup(fd[1]);                                   //pipe for writing
      close(fd[0]);                                 
      close(fd[1]);
      selectCommand(envp, argv, vector[0], delim);  //executing using first command
      exit(2);                                      
    }
    else{                                           //parent
      wait(NULL);                                   //wait for child if it hasn't finished yet
      close(0);                                     //closing input
      dup(fd[0]);                                   //pipe for reading
      close(fd[0]);
      close(fd[1]);
      selectCommand(envp, argv, vector[1], delim);  //executing using second command
      exit(2);
    }
  }
  else{
    wait(NULL);                                     //wait if necessary
    return 0;
  }
  close(0);                                         //closing input 
  close(1);                                         //closing output
}

/*
Look for pipe passing the command as parameter and looking for '|' char
 */
int lookForPipes(char *str){
  return (countWords(str, '|'));
}

/*
Look for pipe passing the command as parameter and looking for '|' char
 */
int lookForBackground(char *str){
  return (countWords(str, '&'));
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

		
