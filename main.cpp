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


using namespace std;

int main(){
    string userInput;  
    vector<char*> toExecute;
    
    cout << "$ ";
    getline(cin, userInput);
    
    /* perhaps we need two strtok, so that one can ignore 
    whitespace, and one can distinguish ; && ||*/

    char * str = new char[userInput.length() + 1];  // char array named str
    
    strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
    char * pch;  
    string delim = " ";    // delimiters for strtok
    
    // (COMMENTS)  Remove any text after a '#' character  //
    
    char * pch2;
    string COMMENT = "#";
    
    pch2 = strtok (str,COMMENT.c_str());      // break at every COMMENT (#)
    
    str = pch2;
    
    // Comments now removed
  
    pch = strtok (str,delim.c_str());      // break at every delimiter
    while (pch != NULL){
        // cout << pch << endl;
        toExecute.push_back(pch);
        pch = strtok (NULL, delim.c_str());
    }
    
    execute(toExecute);
    
    
    delete [] str;  //no memory leaks
    // delete [] pch;
}