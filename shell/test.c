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
  char **words;
  char *cmd;
  char *slash = "/";
  char **vector;
  int pid;  

  while(1){
    write(1, "$", 2);
    fgets(str, size, stdin);
    if(compareWord("exit\n", str) == 1){
	return 0;
    }
    vector = mytoc(str, delim);
    //    cmd = vector[0];
    for(int i = 0; envp[i] != (char*)0; i++){
      char ** tempVector = mytoc(envp[i], '=');
      if(compareWord(tempVector[0], "PATH") == 1){
	path = tempVector[1];
	break;
      }
      free(tempVector);
    }
    posPath = mytoc(path, ':');

    if(access(vector[0], F_OK) == 0){
      pid = fork();
      if(pid == 0){
	int executed = execve(vector[0], vector, envp);
	if(executed == 0){
	  return 0;
	}
	else{
	  printf("There was an error while trying to execute the command\n");
	}
      }
      else{
	wait(NULL);
	return 0;
      }
    }
    else{
      while(posPath){
	realPath = concatenate(*posPath, "/");
	realPath = concatenate(realPath, cmd);
        pid = fork();
	if(pid == 0){
	  int executed = execve(realPath, vector, envp);
	  if(executed == 0){
	    return 0;
	  }
	  else{
	    wait(NULL);
	  }
	  break;
	}
	posPath++;
      }
    }
  }
  return 0;
}

char *concatenate(char *str, char *pStr){
  int lengthStr = countCharacters(str);
  int lengthPStr = countCharacters(pStr);
  char count = lengthStr + lengthPStr + 1;
  char * strConcat = (char *) calloc(count, sizeof(char *));
  for(int i = 0; i < count; i++){
    if(i < lengthStr){
      strConcat[i] = str[i];
    }
    else{
      strConcat[i] = pStr[i - lengthStr];
    }
  }
  strConcat[count] = '\0';
  return strConcat;
  
}

int countCharacters(char *str){
  char * temp;
  int size = 0;
  for(temp = str; *temp; temp++)
    ;
  size = temp - str;
  return size;
}

		
