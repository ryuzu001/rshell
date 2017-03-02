#include "execute.h"
#include <string>

#include <vector>
#ifndef CONNECTOR_H_
#define CONNECTOR_H_

using namespace std;

/* for commands with ; || or && */

class connector : public execute{
    protected:
     vector<char*> left;
     vector<char*> right;
     vector<char*> connectorType;
    public:
     connector();
     connector(vector<char*> l, vector<char*> r) : left(l), right(r) { }
     virtual bool executeStatement() = 0;
};


class andOp : public connector {
    public:
     andOp(){}
     andOp(vector<char*> l, vector<char*> r) : connector(l, r) { }
     bool executeStatement();
    
};


class orOp : public connector {
    public:
     orOp() {}
     orOp(vector<char*> l, vector<char*> r) : connector(l, r) { }
     bool executeStatement();
    
};


class semicolon : public connector{
    public:
     semicolon(){} 
     semicolon(vector<char*> l, vector<char*> r);
     bool executeStatement();
};


#endif