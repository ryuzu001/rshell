#include <unistd.h> //execvp
#include <cstdio> //perror
#include <string> //string
#include <cstdlib> //exit
#include "exitshell.h"

exitshell::exitshell(){}

void exitshell::exit_shell(){
    // char * args[2];
    // string arg1 = "echo";
    
    // args[0] = (char*) arg1.c_str();  //prints 'exiting...' then exits
    // args[1] = NULL;
    
    // if( execvp (args [0],args) == -1){  //execute, with error checking
    //     perror ("exit");                // kills current process, no children/parents created
    // }
    exit(0);
}