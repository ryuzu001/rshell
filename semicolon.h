#ifndef SEMICOLON_H_
#define SEMICOLON_H_

#include <vector>
#include "connector.h"

using namespace std;

class semicolon:public connector{
    protected:
    
    vector<char*>left;      // left command
    vector<char*>right;     // right command
    
    public:
    
    semicolon();   // constructor
    semicolon(vector<char*>, vector<char*>);
    virtual vector<char*> executeStatement() = 0;
};

#endif