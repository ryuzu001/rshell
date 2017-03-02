#include <vector>   
#include <unistd.h> 
#include <cstdio> 
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

#include "execute.h"
#include "exitshell.h"
#include "test.h"

/* This will execute any ONE command given a vector of individual words in the command
 
   for example, "echo hello there" or "ls -l". Multiple commands will need multiple execute calls.
   
   will NOT work with "echo hello there; ls -l"
*/

using namespace std;

execute::execute(){}

bool execute::executeStatement(vector<char*> arg){
    char* test1 = new char[1000];
    char* test2 = new char[1000];
    
    string t1 = "test";
    string t2 = "[";
    
    strcpy(test1, t1.c_str());
    strcpy(test2, t2.c_str());
    
    if(!strcmp(test1, arg.at(0)) || !strcmp(test2, arg.at(0))){
        test exe;
        return exe.executeStatement(arg);
    }
    
    bool result = true;
    char* args[1000];      // cause the -pedantic flag throws an error with the other declaration
    
    // char* args[vectorSize + 1];             // vector to array
                                               //      |
    for(unsigned i = 0; i < arg.size(); i++){  //      |
        args[i] = arg.at(i);                   //      |
    }                                          //      V
    
    args[arg.size()] = NULL;  // final entry needs to be NULL
    
    

    
    int pid = fork();    //RUN SYSCALL -->ERROR CHECK
        
        if(pid < 0){       //failed to fork, error
            perror("fork");
            exit(1);
        }
        else if(pid == 0){ //child process
            if( execvp (args [0],args) < 0){  //execute, with error checking
                result = false;
                perror ("exec");
                _exit(2);     //end child
            }
            _exit(2);
        }
        else{ //parent process
        
            /* WIFEXITED(s)  Evaluates to a non-zero value if status 
               was returned for a child process that exited normally.*/
               
            int status;
            if(waitpid(pid, &status, 0) < 0){  //execute
                perror("waitpid");
            }
            if(WEXITSTATUS(status) != 0){    //need to check if child process exited
                result = false;  // did not exit normally; return false
            }
        }
    
    return result;

}

void execute::exit_shell(){
    exitshell temp;
    temp.exit_shell();
}