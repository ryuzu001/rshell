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
    char* test1 = new char[1000];
    string t1 = "test";
    strcpy(test1, t1.c_str());
    
    char* noFlag = new char[1000];
    string nf = "-e";
    strcpy(noFlag, nf.c_str());
    
    bool result = true;
    struct stat st;
    char* flag;
    bool extraChar;
    
    bool flagF = false;
    bool flagD = false;
    char* filePath;       //file path
    
    if(v.size() == 0){
        printf("this is not a test command!");
        return false;
    }
    
    if(!strcmp(test1,v.at(0))){
        extraChar = false;
    }
    else{
        extraChar = true;
    }
    
    v.erase(v.begin());         //erase first element, the "[" or "test"
    
    if(extraChar){
        if(v.size() == 3){
            flag = v.at(0);
            v.erase(v.begin());
        }
        else{
            flag = noFlag;
        }
    }
    else{
        if(v.size() == 2){
            flag = v.at(0);
            v.erase(v.begin());
        }
        else{
            flag = noFlag;
        }
    }
    
    filePath = v.at(0);
    
    string strF = "-f";
    string strD = "-d";
    string strE = "-e";
    
    char* dashF = new char[1000];
    char* dashD = new char[1000];
    char* dashE = new char[1000];
    
    strcpy(dashF, strF.c_str());
    strcpy(dashD, strD.c_str());
    strcpy(dashE, strE.c_str());
    
    if(!strcmp(flag, dashF)){
        flagF = true;
    }
    else if(!strcmp(flag, dashD)){
        flagD = true;
    }
    
    cout << "filePath: " << filePath << endl;
    
    // now everything is set up, flags accounted for and filePath accounted for.
    
    
    if(stat(filePath, &st) == 0){               // exists
        if(flagF){                          // isreg = registry (file)
            result = S_ISREG(st.st_mode);
        }
        else if(flagD ){                     // isdir = directory
            result = S_ISDIR(st.st_mode);
        }
    }
    else{         //does not exist, -e flag fails
        result = false;
    }
    if(result){
        printf("(True)\n");
    }
    else{
        printf("(False)\n");
    }
    return result;
}
