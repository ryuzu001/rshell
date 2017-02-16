#ifndef CONNECTOR_H_
#define CONNECTOR_H_

using namespace std;

/* for commands with ; || or && */

class connector{
    public:
    virtual vector<char*> executeStatement() = 0;
};

#endif