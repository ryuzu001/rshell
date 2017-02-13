#include <vector>   //vector
#include <unistd.h> //execvp

/* This will execute any ONE command given a vector of individual words in the command
 
   for example, "echo hello there" or "ls -l". Multiple commands will need multiple execute calls.
*/

using namespace std;

void execute(vector<char*> arg){
    char* args[arg.size() + 1];                // vector to array
                                               //      |
    for(unsigned i = 0; i < arg.size(); i++){  //      |
        args[i] = arg.at(i);                   //      |
    }                                          //      V
    
    args[arg.size()] = NULL;  // final entry needs to be NULL
    
    if( execvp (args [0],args) == -1){  //execute, with error checking
        perror ("exec");
    }

}