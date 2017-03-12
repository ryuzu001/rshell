#include <vector>   
#include <unistd.h> 
#include <fcntl.h>
#include <cstdio> 
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>

#include "piping.h"

using namespace std;

const int PIPE_READ = 0;
const int PIPE_WRITE = 1;

piping::piping(){}

bool piping::hasInOut(char* c){
    for(unsigned i = 0; i < strlen(c); i++){
        if(c[i] == '>'){
            return true;
        }
        if(c[i] == '<'){
            return true;
        }
    }
    return false;
}
vector<char*> piping::charToVector(char* c){
    vector<char*> result;
    char* tok = strtok(c, " \t");
    while(tok != NULL){
        result.push_back(tok);
        tok = strtok(NULL, " \t");
    }
    return result;
}

vector<vector<char*> > piping::splitCommand(string s){
    vector<char*> vCommands;
    char* ss = new char[s.size() + 1];
    strcpy(ss, s.c_str());
    
    char * st = strtok(ss, "|");
    while(st != NULL){
        vCommands.push_back(st);
        st = strtok(NULL, "|");
    }
    // for(unsigned i = 0; i < vCommands.size(); i++){
    //     cout << vCommands.at(i) << endl;
    // }
    // cout << "\n\n\n";
    
    //things are good until here
    
    vector<vector<char*> > final;
    for(unsigned i = 0; i < vCommands.size(); i++){
        final.push_back(charToVector(vCommands.at(i)));
    }
    // for(unsigned i = 0; i < final.size(); i++){
    //     final.at(i).push_back(NULL);
    // }
    return final;
}

bool piping::hasString(vector<char*> v, string s){
    char* st = new char[s.size() + 1];
    strcpy(st, s.c_str());
    for(unsigned x = 0; x < v.size(); x++){
        if(v.at(x) != NULL){
            if(!strcmp(v.at(x), st)){
                return true;
            }
        }
    }
    return false;
}

void piping::execNoPipe(vector<char*> v, int (&fd)[2]){
    char* file;
    char* args[1000];
    int placeholder = 0;
    char* op = new char[10];
    char* cl = new char[10];
    char* cl2 = new char[10];
    
    string op_s = "<";
    string cl_s = ">";
    string cl2_s = ">>";
    
    strcpy(op, op_s.c_str());
    strcpy(cl, cl_s.c_str());
    strcpy(cl2, cl2_s.c_str());
    
    // cout << "vsize: " << v.size();
    for(unsigned i = 0; i < v.size(); i++){
        if(!strcmp(v.at(i), op) || !strcmp(v.at(i), cl) || !strcmp(v.at(i), cl2)){
            break;
        }
        else{
            // cout << v.at(i) << " ";
            args[i] = v.at(i);
            placeholder++;
        }
    }
    args[placeholder + 1] = NULL;
    if(hasString(v,"<")){
        for(unsigned i = 0; i < v.size(); i++){
            if(!strcmp(v.at(i), "<")){
                file = v.at(i + 1);
                break;
            }
        }
        fd[1] = open(file, O_RDONLY);   //fd is set to the file descriptor of file
        if(fd[1] == -1){
            perror("open");
        }
        if(dup2(fd[1], 0) == -1){   // copies the file descriptor of file to fd[PIPE_READ]
            perror("dup2_1");
        }
        if(close(fd[1]) == -1){
            perror("close_1");
        }
    }
    if(hasString(v, ">")){
        for(unsigned i = 0; i < v.size(); i++){
            if(!strcmp(v.at(i), ">")){
                file = v.at(i + 1);
                break;
            }
        }
        fd[0] = open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(fd[0] == -1){
            perror("open");
        }
        if(dup2(fd[0], 1) == -1){
            perror("dup2_2");
        }
        if(close(fd[0]) == -1){
            perror("close_2");
        }
    }
    if(hasString(v, ">>")){
        for(unsigned i = 0; i < v.size(); i++){
            if(!strcmp(v.at(i), ">>")){
                file = v.at(i + 1);
                break;
            }
        }
        fd[0] = open(file, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, APPEND, read and write permissions for all users
        if(fd[0] == -1){
            perror("open");
        }
        if(-1 == dup2(fd[0], 1)){
            perror("dup2");
        }
        if(close(fd[0]) == -1){
            perror("close");
        }
    }
    
    execvp(args[0], args);
}

void piping::execPipe(vector<vector<char*> > v, int(&fd)[2]){
    if(v.size() == 0){
        return;
    }
    
    pid_t pid;
    
    if(pipe(fd) == -1){
        perror("pipe");
    }
    pid = fork();
    if(pid < 0){
        perror("fork");
    }
    else if(pid == 0){
        if(hasString(v.at(0), "<") || hasString(v.at(0), ">") || hasString(v.at(0), ">>")){
            execNoPipe(v.at(0), fd);
        }
        if(close(fd[1]) == -1){
            perror("close");
        }
        dup(fd[0]);
    }
    else{
        v.erase(v.begin());
        execPipe(v, fd);
    }
}

void piping::executeStatement(string s){
    int fd[2];
    vector<vector<char*> > command;
    command = splitCommand(s);
    
    // bool result = true;
    
    // for(unsigned i = 0; i < command.size(); i++){
    //     for(unsigned j = 0; j < command.at(i).size(); j++){
    //         cout << command.at(i).at(j) << " ";
    //     }
    //     cout << endl;
    // }
    
    // if command size is more than 1, that means there's piping involved.
    
    // cout << command.size() << endl;
    
    if(command.size() == 1){   // no piping
        pid_t pid = fork();
        if(pid < 0){
            perror("fork");
        }
        else if(pid == 0){
            execNoPipe(command.at(0), fd);
        }
        else{
            int status;
            if(waitpid(pid, &status, 0) < 0){  //wait
                perror("waitpid");
            }
        }
    }
    else{    //PIPING
        
    }
}








/*

cat < test.txt
cat < test.txt > file
cat < test.txt >> file
cat < test.txt > newFile
cat < test.txt >> newFile
echo A > existingInputFile
echo A >> existingInputFile

cat < test.txt | tr A-Z a-z
cat < test.txt | tr a-z A-Z

cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2




int (&fd)[2]){


*/

