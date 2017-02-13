#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

void execute(char* pch, string delim){
    while (pch != NULL){
        system(pch);
        pch = strtok(NULL, delim.c_str());
    }    
    delete[] pch;
}