#ifndef test
#define test

char *concatenate(char *str, char *pStr);
int countCharacters(char *str);
pid_t saferFork();
char* lookForPath(char **envp);
int cdCommand(char **vector);
int determinePipe(char *str);
int lookForPipes(char *str);
int selectCommand(char **envp, char **argv, char *str, char delim);
int determinePipes(char *str, char** envp, char **argv);
int lookForBackground(char *str);

#endif
