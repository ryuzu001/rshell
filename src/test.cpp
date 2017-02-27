#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>   //stat()
#include <unistd.h>
#include <vector>

#include "test.h"

using namespace std;

test::test() {}    //constuctor

bool test::executeStatement(vector<char*> v){
    bool result;
    struct stat st;
    
    bool flagE = true;   //flags
    bool flagF = false;
    bool flagD = false;
    bool noFlags = true;
    char* filePath;       //file path
    
    if(v.size() == 0){
        printf("this is not a test command!");
        return false;
    }
    // if(v.at(0) == "test"){
    //     printf("this is a test command!");
    // }
    // if(v.at(0) == "["){
    //     printf("this is a test command!");
    // }
    
    v.erase(v.begin());         //erase first element, the "[" or "test"
    
    string strE = "-e";
    string strF = "-f";
    string strD = "-d";
    
    char* dashE = new char[1000];
    char* dashF = new char[1000];
    char* dashD = new char[1000];
    
    strcpy(dashE, strE.c_str());
    strcpy(dashF, strF.c_str());
    strcpy(dashD, strD.c_str());
    
    while(1){           // find flags
        char* pch;
        pch = strchr(v.at(0), '/');
        if(!strcmp(v.at(0), dashE)){
            noFlags = false;
            flagE = true;
        }
        else if(!strcmp(v.at(0), dashF)){
            noFlags = false;
            flagF = true;
        }
        else if(!strcmp(v.at(0), dashD)){
            noFlags = false;
            flagD = true;
        }
        else if(pch != NULL){                 //  "/" is found in the char* at v.at(0)
            break;
        }
        else{
            printf("invalid flag: %s\n", v.at(0));
        }
        v.erase(v.begin());
    }
    if(noFlags){
        flagE = true;
    }
    filePath = v.at(0);
    // cout << "this is the filePath: " << filePath << endl;
    // cout << "with\n";
    // if(flagE){
    //     cout << "flag E\n";
    // }
    // if(flagF){
    //     cout << "flag F\n";
    // }
    // if(flagD){
    //     cout << "flag D\n";
    // }
    
    // now everything is set up, flags accounted for and filePath accounted for.
    
    if(flagE){
        result = true;
    }
    
    if(stat(filePath, &st)){               // exists
        if(!flagF){                          // isreg = registry (file)
            result = S_ISREG(st.st_mode);
        }
        else if(!flagD){                     // isdir = directory
            result = S_ISDIR(st.st_mode);
        }
        else{ 
            result = false;     // does not exist, -e fails
        }
    }
    if(result){
        printf("(True)");
    }
    else{
        printf("(False)");
    }
    return result;
}
