#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(){
    
    char* args[3];
    string ls = "ls -l";
    string dash_l = "";
    
    args[0] = (char*) ls.c_str();
    args[1] = (char*) dash_l.c_str();
    args[2] = NULL;
    
    // for(int x = 0; x < 2; x++){
    //     cout << args[x] << " ";
    // }
    
    if( execvp (args [0],args) == -1){
        perror ("exec");
    }
}