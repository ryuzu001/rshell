#include <vector>   //vector
#include <unistd.h> //execvp
#include <cstdio> //perror
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

#include "semicolon.h"
#include "execute.h"

using namespace std;

semicolon::semicolon(){}

semicolon::semicolon(vector<char*> l, vector<char*> r){
    this->left = l;
    this->right = r;
}

vector<char*> semicolon::executeStatement(){
    execute temp;
    temp.executeStatement(left);
    return right;
}