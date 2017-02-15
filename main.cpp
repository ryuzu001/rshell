#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>   //getenv
#include <sys/types.h>
#include <unistd.h>   //getlogin
#include <sys/wait.h>
#include <cstddef>
#include <vector>

#include "execute.h"
#include "exitshell.h"


using namespace std;

void printInfo(){
    printf("$ ");
}

void displayShell(){
    string exitStr = "exit";     // string to indicate exit shell
    string comment = "#";        // character used to indicate comment
    string delim = " ";          // delimiters for strtok 
    string userInput;  
    
    while(1){
        printInfo();
        execute te;
        vector<char*> toExecute;      // reset toExecute to empty
        char * pch;                   // reset pch
        getline(cin, userInput);
        
        char * str = new char[userInput.length() + 1];  // char array named str
        strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
        str = strtok (str, comment.c_str());     // break at every comment 
        pch = strtok (str, delim.c_str());       // break at every delimiter
    
        while (pch != NULL){                     // adds the rest of the commands to toExecute
            toExecute.push_back(pch);
            pch = strtok (NULL, delim.c_str());
        }
        
        if(toExecute.size() == 0){               // no input, recursively call itself
            displayShell();
            te.exit_shell();
        }
        if(toExecute.at(0) == exitStr){          // exit command
            te.exit_shell();
        }
        if(te.executeStatement(toExecute)){      //tests if command was sucessful
            cout << "sucessful\n";
        }
        else{
            cout << "unsucessful\n";
        }
        delete [] str;
    }    
}

int main(){
    displayShell();      /* Since we cannot call main(), I needed another 
                            function to call itself, so I made main just call
                            that function.
                         */
}