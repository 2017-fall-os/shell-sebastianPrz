#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.h"
#include "test.h"

int main(){

  char delim = ' ';
  int maxSize = 50;
  int size;
  
  printf("Enter string to tokenize (less than 50 characters)\n");
  while(1){
    char * str = (char *) malloc(50 * sizeof(char));
    write(1, "$", 1);
    fgets(str, maxSize, stdin);
    if(*str != '\0'){
      char ** vector = mytoc(str, delim);
      char * temp;
      for(temp = str; *temp; temp++)
	;
      size = temp - str + 1;
      if(size > 50){
	printf("The size was exceeded\n");
	return 0;
      }
      printf("Hit 'Enter' with no text to exit\n");
    }
    if(*str == '\n'){
      break; 
    }
  }  
  return 0;
}
