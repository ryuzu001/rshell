#ifndef TEST_H_
#define TEST_H_

#include "execute.h"
using namespace std;

class test:public execute{
    
    protected:
    
    char * command;
    
    public:
    
    test();
    bool executeStatement(vector<char*>);
    
};
#endif