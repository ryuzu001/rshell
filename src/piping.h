#ifndef PIPING_H
#define PIPING_H
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;

class piping{
    public:
    piping();
    vector<vector<char*> > splitCommand(string);
    bool hasInOut(char*);
    vector<char*> charToVector(char*);
    bool hasString(vector<char*>, string);
    void executeStatement(string);
    void execNoPipe(vector<char*>, int(&)[2]);
    void execPipe(vector<vector<char*> >, int(&)[2]);
};



#endif