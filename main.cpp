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


int main(){
    
  while(1){
    string exitStr = "exit";     //string to indicate exit shell
    string userInput;  
    vector<char*> toExecute;
    
    //WILL NEED TO PUT THIS IN A LOOP LATER
    cout << "$ ";
    getline(cin, userInput);
    
    /* perhaps we need two strtok, so that one can ignore 
    whitespace, and one can distinguish ; && ||*/

    char * str = new char[userInput.length() + 1];  // char array named str
    
    strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
    // (COMMENTS)  Remove any text after a '#' character  //
    
    string COMMENT = "#";
    
    str = strtok (str,COMMENT.c_str());      // break at every COMMENT (#)
    
    // Comments now removed
    
    char * pch;  
    string delim = " ";    // delimiters for strtok    
  
    pch = strtok (str,delim.c_str());      // break at every delimiter
    
    while (pch != NULL){
        // cout << pch << endl;
        toExecute.push_back(pch);
        pch = strtok (NULL, delim.c_str());
    }
    
    // Probably put this in a loop later
    if(toExecute.size() == 0){
        printf("error: nothing to execute\n");
        exit(0);
    }
    
    if(toExecute.at(0) == exitStr){
        exitshell temp;
        temp.exit_shell();
        exit(0);
    }
    execute te;
    te.executestatement(toExecute);
    // te.executestatement(toExecute);
    
    delete [] str;  //no memory leaks
    // delete [] pch;
  }
    
    
    
    
}