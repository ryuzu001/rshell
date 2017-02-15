#include <unistd.h> //execvp
#include <cstdio> //perror
#include <string> //string
#include "exitshell.h"

exitshell::exitshell(){};

void exitshell::exit_shell(){
    char * args[3];
    string arg1 = "echo";
    string arg2 = "exiting...";
    
    args[0] = (char*) arg1.c_str();  //prints exiting... then exits
    args[1] = (char*) arg2.c_str();
    args[2] = NULL;
    
    if( execvp (args [0],args) == -1){  //execute, with error checking
        perror ("exit");
    }
}