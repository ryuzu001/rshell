#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

#include "execute.cpp"
#include "exit_shell.cpp"


using namespace std;

int main(){
    string exitStr = "exit";     //string to indicate exit shell
    string userInput;  
    vector<char*> toExecute;
    
    cout << "-----------------------\n";
    cout << "|  Welcome to rshell  |\n";
    cout << "|  Type any command   |\n";
    cout << "| Type 'exit' to exit |\n";
    cout << "-----------------------\n";
    
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
        exit_shell();
        exit(0);
    }
    
    execute(toExecute);
    
    delete [] str;  //no memory leaks
    // delete [] pch;
}