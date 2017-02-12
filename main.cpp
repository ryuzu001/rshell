#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
int main(){
    string userInput;  
    
    cout << "$ ";
    getline(cin, userInput);
    
    /* testing to make sure I understand strtok */
    char * str = new char[userInput.length() + 1];  // char array named str
    
    strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
    char * pch;  
    string delim = ";";    // delimiters for strtok
    
    cout << "\nHere are your commands: \n";
  
    pch = strtok (str,delim.c_str());      // break at every ';'
    while (pch != NULL){
        cout << pch << "\n";
        pch = strtok (NULL, delim.c_str());
    }
    
    delete [] str;  //no memory leaks
    delete [] pch;
}
