#include "execute.h"
#ifndef CONNECTOR_H_
#define CONNECTOR_H_

using namespace std;

/* for commands with || or && */

class connector : public execute{
    protected:
     vector<char*> left;
     vector<char*> right;
    public:
     connector(vector<char*> l, vector<char*> r) : left(l), right(r) { }
     virtual bool executeStatement() = 0;
};


class andOp : public connector {
    public:
     bool executeStatement();
    
};


class orOp : public connector {
    public:
     bool executeStatement();
    
};
#endif