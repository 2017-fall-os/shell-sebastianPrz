#ifndef test
#define test

char *concatenate(char *str, char *pStr);
int countCharacters(char *str);
pid_t saferFork();
char* lookForPath(char **envp);
int cdCommand(char **vector);
int determinePipe(char *str);
int lookForPipes(char *str);
int selectCommand(char **vector, char **envp, int pipe, char *str, char delim);

#endif
