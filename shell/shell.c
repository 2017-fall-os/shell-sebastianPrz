#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mytoc.h"
#include "test.h"

char *userCommand(char ** envp){
  char **vars;
  while(*envp != '\0'){
    vars = mytoc(*envp, '=');
    if(compareWord(*vars, "PATH") == 1){
      return *vars;
    }
  }
  char **path = mytoc(vars, ':');
  return path;
}
