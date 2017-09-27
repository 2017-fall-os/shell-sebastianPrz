Shell lab part 1

Description: This program will use the tokenizer created on the previous lab, and it will tokenize words given the delimiter parameter. This, with the purpose of creating a path that will be able to run any given shell command.
The command will be taken from the user, and then it is going to check if the command contains a working path, in case it does not, it will tokenize the environmental variables and it will look for the one that contains the path.
Then that path will be tokenized and it will modify it so that it can be used, to then append the command given by the user.

Funtionality;
1) Type "make"
2) Type "./test"
3) Then the '$' character will appear, meaning that you will be able to input your command.
4) To exit input "exit"

Code Directory:
mytoc.h --file that has declared the methods used in mytoc.c
mytoc.c --file that contains the functions used to tokenize a word, as well as helper methods used in test.c
test.h --file that has declared the methods used in test.c
test.c --file that uses the given command by the user and runs the command if also a path is given, if not it will look for the path and will run the command