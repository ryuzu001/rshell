#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

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
    
    cout << "Here are your commands: " << endl;
  
    pch = strtok (str,delim.c_str());      // break at every ';'
    while (pch != NULL){
        cout << pch << "\n";
        pch = strtok (NULL, delim.c_str());
    }
    
    /*reset pch for another go*/
    strcpy(str, userInput.c_str());
    pch = strtok (str,delim.c_str());
    
    cout << "Executing your commands: " << endl;
    
    while (pch != NULL){
        system(pch);
        pch = strtok(NULL, delim.c_str());
    }
    
    
    delete [] str;  //no memory leaks
    delete [] pch;
}