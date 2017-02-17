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
#include "connector.h"


using namespace std;

void displayShell();
vector<char*> charstarToVector(char*, string);

void printInfo(){
    printf("$ ");
}

void checkUserInput(vector<char*> input){
    string exitStr = "exit";     // string to indicate exit shell
    execute te;
    if(input.size() == 0){               // no input, recursively call itself
        displayShell();
        te.exit_shell();
    }
    if(input.at(0) == exitStr){          // exit command
        te.exit_shell();
    }    
}

// string executeSemicolon(string input, string delim){
//     execute te;
//     int index = 0;    // incase of ending semicolon
    
//     for(unsigned i = 0; i < input.size(); ++i){
//         if(input.at(i) == ';'){
//             index = i;
//             break;  //first semicolon only
//         }
//     }
//     string firstCommand = input.substr(0, index);
//     string secondCommand = input.substr(index + 1);
    
//     char * str = new char[firstCommand.length() + 1];
//     strcpy(str, firstCommand.c_str()); 
    
//     vector<char*> com1 = charstarToVector(str, delim);
    
//     checkUserInput(com1);
    
//     te.executeStatement(com1);
    
//     delete[] str;
    
//     return secondCommand;
// }

bool executeSemicolon(string input, string &newInput, string delim){
    execute te;
    int index = 0;    // incase of ending semicolon
    
    for(unsigned i = 0; i < input.size(); ++i){
        if(input.at(i) == ';'){
            index = i;
            break;  //first semicolon only
        }
    }
    string firstCommand = input.substr(0, index);
    string secondCommand = input.substr(index + 1);
    
    char * str = new char[firstCommand.length() + 1];
    strcpy(str, firstCommand.c_str()); 
    
    vector<char*> com1 = charstarToVector(str, delim);
    
    checkUserInput(com1);
    
    if(!(te.executeStatement(com1))){
        return false;
    }
    
    delete[] str;
    
    newInput = secondCommand;
    
    return true;
}

bool hasSemicolon(string userInput){
    for(unsigned i = 0; i < userInput.size(); ++i){
        if(userInput.at(i) == ';'){
            return true;
        }
    }
    return false;
}

bool hasAnd(string userInput){
    if(userInput.size() == 0){
        return false;
    }
    for(unsigned i = 0; i < userInput.size() - 2; ++i){
        if(userInput.at(i) == '&' && userInput.at(i + 1) == '&'){
            return true;
        }
    }
    return false;
}

bool hasOr(string userInput){
    if(userInput.size() == 0){
        return false;
    }
    for(unsigned i = 0; i < userInput.size() - 2; ++i){
        if(userInput.at(i) == '|' && userInput.at(i + 1) == '|'){
            return true;
        }
    }
    return false;    
}

bool hasConnector(string userInput){
    if(hasSemicolon(userInput) || hasAnd(userInput) || hasAnd(userInput)){
        return true;
    }
    return false;
}

vector<char*> charstarToVector(char* str, string delim){
    vector<char*> result;
    char * pch = strtok (str, delim.c_str());
    while(pch != NULL){
        result.push_back(pch);
        pch = strtok (NULL, delim.c_str());
    }
    return result;
}

void displayShell(){
    string comment = "#";        // character used to indicate comment
    string delim = " ";          // delimiters for strtok 
    string userInput;  
    string revisedInput;
    
    while(1){
        printInfo();
        execute te;
        vector<char*> toExecute;      // reset toExecute to empty
        getline(cin, userInput);
        
        while(hasConnector(userInput)){
            if(hasSemicolon(userInput)){
                executeSemicolon(userInput, revisedInput, delim);
                userInput = revisedInput;
            }
            if(hasAnd(userInput)){
                
            }
            if(hasOr(userInput)){
                
            }
        }
        
        char * str = new char[userInput.length() + 1];  // char array named str
        strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
        str = strtok (str, comment.c_str());     // break at every comment 
        
        toExecute = charstarToVector(str, delim);

        checkUserInput(toExecute);
        
        te.executeStatement(toExecute);
        
        delete [] str;
    }    
}

int main(){
    displayShell();      /* Since we cannot call main(), I needed another 
                            function to call itself, so I made main just call
                            that function.
                         */
}