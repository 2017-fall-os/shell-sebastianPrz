#include "mytoc.h"
#include <stdlib.h>
#include <stdio.h>
#include "test.h"
/*Takes a char * and a char */
//stores the words on an array
//depending on the number of words
char ** mytoc(char *str, char delim){
  char ** vector;
  char * pStr;
  pStr = str;
  int index, wordLength, position;
  int numWords = countWords(str, delim);
  vector = (char **)calloc(numWords + 1, sizeof(char *));    //allocates n + 1
  position = 0;
  while(*pStr != '\0'){
    wordLength = countLengthWord(pStr, delim);
    if(wordLength > 0){                                      //just when there is an actual word
      vector[position] = (char *) malloc((wordLength + 1));  //allocate n + 1
      for(index = 0; index < wordLength; index++, pStr++){   //going from 0 to the length of the word
	vector[position][index] = *pStr;                     //store each character into array
	//printf("%c", *pStr);	
      }
      //printf("\n");
      vector[position][index] = '\0';                        //add 0 terminator
      position++;
    }
    else      //there is no word there
      pStr++;
  }
  vector[numWords] = '\0';
  return vector;
}
// It will compare two words, added so I can exit when typing exit
int compareWord(char *str, char *pStr){
  int same = 0;
  while((*str != '\0') || (*pStr != '\0')){                //for two words to be equal none of them needs to finish before
    if(*str != *pStr){                                     //if at the same index the letter is not the same then it's not equal
      break;
    }
    else{
      if(*(str + 1) == '\0' && *(pStr + 1) == '\0'){       //if the next character on both of the words is '\0' then we are at the last character
	same = 1;                                          //meaning that we reached the end having the letters being equal
      }
    }
    str++;
    pStr++;
  }
  return same;
}

//counts the number of characters for each word
//it will return the number of characters found before reaching to the delimiter or the end of the word
int countLengthWord(char * str, char delim){
  int wordLength = 0;
  char* pStr = str; 
  for(*pStr; *pStr != delim && *pStr != '\0'; pStr++){
    wordLength++;
  }
  return wordLength;
}
//counts the number of words
int countWords(char *str, char delim){
  int numWords, index;
  numWords = 0;
  char *pStr;
  pStr = str;
  if(*pStr == '\0'){                                 //if the word is empty then return 0
    numWords = 0;
  }
  else{                                            
    if(pStr[0] != delim){                            //if the first character is not the delimiter, then there is a word there
      numWords++;
    }
    while(*pStr != '\0'){                            //checking the word for the '\0' character
      if(*pStr == delim){                            //if there is a delimiter and the next char is not one
	if(*(pStr + 1) != delim){
	  if(*(pStr + 1) != '\0'){                   //and if the next char is not the end of the word
	    numWords++;                              //then that means there is a word there
	  }
	}
      }
      pStr++;
    }
  }
  return numWords;
}

/*
function to copy a char* into another char*
 */
char *copyWord(char *src, char *destination){
  destination = malloc(sizeof(char) * countCharacters(src) + 1);   //it will assign the memory given the char*
  while(*src != '\0'){                      //until we reach the end of the char*
    *destination = *src;                    //copy the value into the destination
    src++;                                  //increase both of the pointers
    destination++;
  }
  *destination = '\0';                      //add the '\0' char to the end of the char*
  return destination;
}
