// STRICTLY FOR TESTING PURPOSES. WILL NOT BE INCLUDED IN THE FINAL PROJECT


#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

int main(){
    
    // char* args[3];
    // string ls = "ls -l";
    // string dash_l = "";
    
    // args[0] = (char*) ls.c_str();
    // args[1] = (char*) dash_l.c_str();
    // args[2] = NULL;
    
    // // for(int x = 0; x < 2; x++){
    // //     cout << args[x] << " ";
    // // }
    
    // if( execvp (args [0],args) == -1){
    //     perror ("exec");
    // }
    
    string userInput = "HELLO THERE MY NAME IS RYAN#YOOOOOOOOOO";
    vector<char*> toExecute;
    
    char * str = new char[userInput.length() + 1];  // char array named str
    
    strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
    printf("Before: %s\n", str);
    
    char * pch2;  
    string delim = "#";    // delimiters for strtok
  
    pch2 = strtok (str,delim.c_str());      // break at every delimiter
    toExecute.push_back(pch2);
    cout << "After: ";
    for(int x = 0; x < toExecute.size(); x++){
        cout << toExecute.at(x) << " ";
    }
    
}