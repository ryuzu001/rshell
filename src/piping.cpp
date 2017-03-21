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

vector<vector<char*> > piping::splitCommand(string s){ //split at every "|"
    vector<char*> vCommands;
    char* ss = new char[s.size() + 1];
    strcpy(ss, s.c_str());
    
    char * st = strtok(ss, "|");
    while(st != NULL){
        vCommands.push_back(st);
        st = strtok(NULL, "|");
    }
    
    vector<vector<char*> > final;
    for(unsigned i = 0; i < vCommands.size(); i++){
        final.push_back(charToVector(vCommands.at(i)));
    }
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

void piping::runPipe2(vector<vector<char*> > v, int(&fd)[2]){
    
    int stdout_copy = dup(1);
    if(stdout_copy == -1){perror("dup");}
    
    if(hasString(v.at(0), "<")){
        for(unsigned i = 0; i < v.at(0).size(); i++){
            if(strcmp(v.at(0).at(i), "<") == 0){
                v.at(0).erase(v.at(0).begin() + i);
                break;
            }
        }
    }
    if(hasString(v.at(1), ">")){
        char* file;
        for(unsigned i = 0; i < v.at(1).size(); i++){
            if(!strcmp(v.at(1).at(i), ">")){
                file = v.at(1).at(i + 1);
                v.at(1).erase(v.at(1).begin() + i, v.at(1).end());
                break;
            }
        }
        if(close(1) == -1){perror("close");}
        int out = open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    if(hasString(v.at(1), ">>")){
        char* file;
        for(unsigned i = 0; i < v.at(1).size(); i++){
            if(!strcmp(v.at(1).at(i), ">>")){
                file = v.at(1).at(i + 1);
                v.at(1).erase(v.at(1).begin() + i, v.at(1).end());
                break;
            }
        }
        if(close(1) == -1){perror("close");}
        int out = open(file, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    char* arg1[1000];
    char* arg2[1000];
    for(unsigned i = 0; i < v.at(0).size(); i++){
        arg1[i] = v.at(0).at(i);
    }
    arg1[v.at(0).size()] = NULL;
    for(unsigned i = 0; i < v.at(1).size(); i++){
        arg2[i] = v.at(1).at(i);
    }
    arg2[v.at(1).size()] = NULL;
    
    
    int newFD[2];
    if(pipe(newFD) == -1){perror("pipe");}
    pid_t pid = fork();
    if(pid == -1){perror("fork");}
    else if (pid == 0){
        if(close(STDOUT_FILENO) == -1){perror("close");}
        if(dup(newFD[1]) == -1){perror("dup");}
        if(close(newFD[0]) == -1){perror("close");}
        if(close(newFD[1]) == -1){perror("close");}
        if(execvp(arg1[0],arg1) == -1){perror("exec1");}
        _exit(2);
    }
    pid_t pid2 = fork();
    if(pid2 == -1){perror("fork");}
    else if(pid2 == 0){
        if(close(STDIN_FILENO) == -1){perror("close");}
        if(dup(newFD[0]) == -1){perror("dup");}
        if(close(newFD[1]) == -1){perror("close");}
        if(close(newFD[0]) == -1){perror("close");}
        if(execvp(arg2[0],arg2) == -1){perror("exec2");}
        _exit(2);
    }
    if(close(newFD[0]) == -1){perror("close");}
    if(close(newFD[1]) == -1){perror("close");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    
    
    if(dup2(stdout_copy, 1) == -1){perror("dup2");}
    if(close(stdout_copy) == -1){perror("close");}
    return;
}

void piping::runPipe3(vector<vector<char*> > v, int(&fd)[2]){
    int stdout_copy = dup(1);
    if(stdout_copy == -1){perror("dup");}
    
    if(hasString(v.at(0), "<")){
        for(unsigned i = 0; i < v.at(0).size(); i++){
            if(strcmp(v.at(0).at(i), "<") == 0){
                v.at(0).erase(v.at(0).begin() + i);
                break;
            }
        }
    }
    if(hasString(v.at(2), ">")){
        char* file;
        for(unsigned i = 0; i < v.at(2).size(); i++){
            if(!strcmp(v.at(2).at(i), ">")){
                file = v.at(2).at(i + 1);
                v.at(2).erase(v.at(2).begin() + i, v.at(2).end());
                break;
            }
        }
        if(close(1) == -1){perror("close");}
        int out = open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    if(hasString(v.at(2), ">>")){
        char* file;
        for(unsigned i = 0; i < v.at(1).size(); i++){
            if(!strcmp(v.at(2).at(i), ">>")){
                file = v.at(2).at(i + 1);
                v.at(2).erase(v.at(2).begin() + i, v.at(2).end());
                break;
            }
        }
        if(close(1) == -1){perror("close");}
        int out = open(file, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    char* arg1[1000];
    char* arg2[1000];
    char* arg3[1000];
    for(unsigned i = 0; i < v.at(0).size(); i++){
        arg1[i] = v.at(0).at(i);
    }
    arg1[v.at(0).size()] = NULL;
    for(unsigned i = 0; i < v.at(1).size(); i++){
        arg2[i] = v.at(1).at(i);
    }
    arg2[v.at(1).size()] = NULL;
    for(unsigned i = 0; i < v.at(2).size(); i++){
        arg3[i] = v.at(2).at(i);
    }
    arg3[v.at(2).size()] = NULL;
    
    int pipe1[2];
    int pipe2[2];
    
    if(pipe(pipe1) == -1){perror("pipe");}
    pid_t pid = fork();
    if(pid == -1){perror("fork");}
    else if (pid == 0){
        if(dup2(pipe1[1], 1) == -1){perror("dup2");}
        if(close(pipe1[0]) == -1){perror("close");}
        if(close(pipe1[1]) == -1){perror("close");}
        if(execvp(arg1[0], arg1) == -1){perror("exec");}
        _exit(2);
    }
    
    if(pipe(pipe2) == -1){perror("pipe");}
    
    pid_t pid2 = fork();
    if(pid2 == -1){perror("fork");}
    else if(pid2 == 0){
        if(dup2(pipe1[0], 0) == -1){perror("dup2");}
        if(dup2(pipe2[1], 1) == -1){perror("dup2");}
        if(close(pipe1[0]) == -1){perror("close");}
        if(close(pipe1[1]) == -1){perror("close");}
        if(close(pipe2[0]) == -1){perror("close");}
        if(close(pipe2[1]) == -1){perror("close");}
        if(execvp(arg2[0], arg2) == -1){perror("exec");}
        _exit(2);
    }
    if(close(pipe1[0]) == -1){perror("close");}
    if(close(pipe1[1]) == -1){perror("close");}
    
    pid_t pid3 = fork();
    if(pid3 == -1){perror("fork");}
    else if (pid3 == 0){
        if(dup2(pipe2[0], 0) == -1){perror("dup2");}
        if(close(pipe2[0]) == -1){perror("close");}
        if(close(pipe2[1]) == -1){perror("close");}
        if(execvp(arg3[0], arg3) == -1){perror("exec");}
        _exit(2);
    }
    if(close(pipe2[0]) == -1){perror("close3");}
    if(close(pipe2[1]) == -1){perror("close4");}
    
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    
    if(dup2(stdout_copy, 1) == -1){perror("dup2");}
    if(close(stdout_copy) == -1){perror("close");}
    
    return;
}

void piping::runPipe4(vector<vector<char*> > v, int(&fd)[2]){
    int stdout_copy = dup(1);
    if(stdout_copy == -1){perror("dup");}   //copy stdout fd for later use
    
    if(hasString(v.at(0), "<")){   // remove <
        for(unsigned i = 0; i < v.at(0).size(); i++){
            if(strcmp(v.at(0).at(i), "<") == 0){
                v.at(0).erase(v.at(0).begin() + i);    // get rid of <
                break;
            }
        }
    }
    if(hasString(v.at(3), ">")){  // change stdout
        char* file;
        for(unsigned i = 0; i < v.at(3).size(); i++){
            if(!strcmp(v.at(3).at(i), ">")){
                file = v.at(3).at(i + 1);                        // gets file name, and erases from > onward
                v.at(3).erase(v.at(3).begin() + i, v.at(3).end());
                break;
            }
        }
        if(close(1) == -1){perror("close_1");}
        int out = open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    if(hasString(v.at(3), ">>")){   // change stdout
        char* file;
        for(unsigned i = 0; i < v.at(3).size(); i++){
            if(!strcmp(v.at(3).at(i), ">>")){
                file = v.at(3).at(i + 1);
                v.at(3).erase(v.at(3).begin() + i, v.at(3).end());    //same
                break;
            }
        }
        if(close(1) == -1){perror("close_2");}    // append cause >>
        int out = open(file, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    char* arg1[1000];
    char* arg2[1000];
    char* arg3[1000];
    char* arg4[1000];
    for(unsigned i = 0; i < v.at(0).size(); i++){   // get arguments
        arg1[i] = v.at(0).at(i);
    }
    arg1[v.at(0).size()] = NULL;
    for(unsigned i = 0; i < v.at(1).size(); i++){
        arg2[i] = v.at(1).at(i);
    }
    arg2[v.at(1).size()] = NULL;
    for(unsigned i = 0; i < v.at(2).size(); i++){
        arg3[i] = v.at(2).at(i);
    }
    arg3[v.at(2).size()] = NULL;
    for(unsigned i = 0; i < v.at(3).size(); i++){
        arg4[i] = v.at(3).at(i);
    }
    arg4[v.at(3).size()] = NULL;
    
    int pipe1[2]; // pipe
    int pipe2[2];
    int pipe3[2];
    
    if(pipe(pipe1) == -1){perror("pipe");} // pipe(pipe1)
    pid_t pid = fork();                    // fork()
    if(pid == -1){perror("fork");}
    else if (pid == 0){
        //get input from stdin (done)
        if(dup2(pipe1[1], 1) == -1){perror("dup2");}     // set output to pipe1
        if(close(pipe1[0]) == -1){perror("close_3");}      //close pipe1
        if(close(pipe1[1]) == -1){perror("close_4");}
        if(execvp(arg1[0], arg1) == -1){perror("exec");} //execute
        _exit(2);
    }
    
    if(pipe(pipe2) == -1){perror("pipe");}      //pipe(pipe2)
     
    pid_t pid2 = fork();                        //fork()
    if(pid2 == -1){perror("fork");}
    else if(pid2 == 0){
        if(dup2(pipe1[0], 0) == -1){perror("dup2");}     //get input from pipe1
        if(dup2(pipe2[1], 1) == -1){perror("dup2");}     //set output to pipe2
        if(close(pipe1[0]) == -1){perror("close_5");}      // close everything
        if(close(pipe1[1]) == -1){perror("close_6");}
        if(close(pipe2[0]) == -1){perror("close_7");}
        if(close(pipe2[1]) == -1){perror("close_8");}
        if(execvp(arg2[0], arg2) == -1){perror("exec");}  //execute
        _exit(2);
    }
    if(close(pipe1[0]) == -1){perror("close");}   //close pipe1, unused
    if(close(pipe1[1]) == -1){perror("close");}
    
    if(pipe(pipe3) == -1){perror("pipe");}            //pipe(pipe3)
    
    pid_t pid3 = fork();                            //fork()
    if(pid3 == -1){perror("fork");}
    else if (pid3 == 0){
        if(dup2(pipe2[0], 0) == -1){perror("dup2");}   //get input from pipe2
        if(dup2(pipe3[1], 1) == -1){perror("dup2");}     //set output to pipe3
        if(close(pipe2[0]) == -1){perror("close_9");}      // close everything
        if(close(pipe2[1]) == -1){perror("close_10");}
        if(close(pipe3[0]) == -1){perror("close_11");}
        if(close(pipe3[1]) == -1){perror("close_12");}
        if(execvp(arg3[0], arg3) == -1){perror("exec");}   //execute
        _exit(2);  
    }
    if(close(pipe2[0]) == -1){perror("close_13");}   //close pipe2, now unused
    if(close(pipe2[1]) == -1){perror("close_14");}
    
    pid_t pid4 = fork();
    if(pid4 == -1){perror("fork");}
    else if (pid4 == 0){
        if(dup2(pipe3[0], 0) == -1){perror("dup2_9");}   //get input from pipe3
        // set output to stdout, done already (or could be changed above)
        if(close(pipe3[0]) == -1){perror("close_15");}      // close everything
        if(close(pipe3[1]) == -1){perror("close_17");}
        if(execvp(arg4[0], arg4) == -1){perror("exec");}   //execute
        _exit(2);
    }
    if(close(pipe3[0]) == -1){perror("close_18");}   //close pipe3
    if(close(pipe3[1]) == -1){perror("close_19");}
    
    if(wait(NULL) == -1){perror("wait");}        //wait for all children to finish
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}

    if(dup2(stdout_copy, 1) == -1){perror("dup2");}   // restore stdout
    if(close(stdout_copy) == -1){perror("close_20");}
    
    return;
    
}

void piping::runPipe5(vector<vector<char*> > v, int(&fd)[2]){
    int stdout_copy = dup(1);
    if(stdout_copy == -1){perror("dup");}   //copy stdout fd for later use
    
    if(hasString(v.at(0), "<")){   // remove <
        for(unsigned i = 0; i < v.at(0).size(); i++){
            if(strcmp(v.at(0).at(i), "<") == 0){
                v.at(0).erase(v.at(0).begin() + i);    // get rid of <
                break;
            }
        }
    }
    if(hasString(v.at(v.size() - 1), ">")){  // change stdout
        char* file;
        for(unsigned i = 0; i < v.at(v.size() - 1).size(); i++){
            if(!strcmp(v.at(v.size() - 1).at(i), ">")){
                file = v.at(v.size() - 1).at(i + 1);                        // gets file name, and erases from > onward
                v.at(v.size() - 1).erase(v.at(v.size() - 1).begin() + i, v.at(v.size() - 1).end());
                break;
            }
        }
        if(close(1) == -1){perror("close");}
        int out = open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    if(hasString(v.at(v.size() - 1), ">>")){  // change stdout
        char* file;
        for(unsigned i = 0; i < v.at(v.size() - 1).size(); i++){
            if(!strcmp(v.at(v.size() - 1).at(i), ">>")){
                file = v.at(v.size() - 1).at(i + 1);                        // gets file name, and erases from > onward
                v.at(v.size() - 1).erase(v.at(v.size() - 1).begin() + i, v.at(v.size() - 1).end());
                break;
            }
        }
        if(close(1) == -1){perror("close");}
        int out = open(file, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, append, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    char* arg1[1000];
    char* arg2[1000];
    char* arg3[1000];
    char* arg4[1000];
    char* arg5[1000];
    
    for(unsigned i = 0; i < v.at(0).size(); i++){   // get arguments
        arg1[i] = v.at(0).at(i);
    }
    arg1[v.at(0).size()] = NULL;
    for(unsigned i = 0; i < v.at(1).size(); i++){
        arg2[i] = v.at(1).at(i);
    }
    arg2[v.at(1).size()] = NULL;
    for(unsigned i = 0; i < v.at(2).size(); i++){
        arg3[i] = v.at(2).at(i);
    }
    arg3[v.at(2).size()] = NULL;
    for(unsigned i = 0; i < v.at(3).size(); i++){
        arg4[i] = v.at(3).at(i);
    }
    arg4[v.at(3).size()] = NULL;
    for(unsigned i = 0; i < v.at(4).size(); i++){
        arg5[i] = v.at(4).at(i);
    }
    arg5[v.at(4).size()] = NULL;
    
    int pipe1[2]; // pipe
    int pipe2[2];
    int pipe3[2];
    int pipe4[2];
    
    if(pipe(pipe1) == -1){perror("pipe");} // pipe(pipe1)
    pid_t pid = fork();                    // fork()
    if(pid == -1){perror("fork");}
    else if (pid == 0){
        //get input from stdin (done)
        if(dup2(pipe1[1], 1) == -1){perror("dup2");}     // set output to pipe1
        if(close(pipe1[0]) == -1){perror("close_3");}      //close pipe1
        if(close(pipe1[1]) == -1){perror("close_4");}
        if(execvp(arg1[0], arg1) == -1){perror("exec");} //execute
        _exit(2);
    }
    if(pipe(pipe2) == -1){perror("pipe");}      //pipe(pipe2)
     
    pid_t pid2 = fork();                        //fork()
    if(pid2 == -1){perror("fork");}
    else if(pid2 == 0){
        if(dup2(pipe1[0], 0) == -1){perror("dup2");}     //get input from pipe1
        if(dup2(pipe2[1], 1) == -1){perror("dup2");}     //set output to pipe2
        if(close(pipe1[0]) == -1){perror("close_5");}      // close everything
        if(close(pipe1[1]) == -1){perror("close_6");}
        if(close(pipe2[0]) == -1){perror("close_7");}
        if(close(pipe2[1]) == -1){perror("close_8");}
        if(execvp(arg2[0], arg2) == -1){perror("exec");}  //execute
        _exit(2);
    }
    if(close(pipe1[0]) == -1){perror("close");}   //close pipe1, unused
    if(close(pipe1[1]) == -1){perror("close");}
    
    if(pipe(pipe3) == -1){perror("pipe");}            //pipe(pipe3)
    
    pid_t pid3 = fork();                            //fork()
    if(pid3 == -1){perror("fork");}
    else if (pid3 == 0){
        if(dup2(pipe2[0], 0) == -1){perror("dup2");}   //get input from pipe2
        if(dup2(pipe3[1], 1) == -1){perror("dup2");}     //set output to pipe3
        if(close(pipe2[0]) == -1){perror("close_9");}      // close everything
        if(close(pipe2[1]) == -1){perror("close_10");}
        if(close(pipe3[0]) == -1){perror("close_11");}
        if(close(pipe3[1]) == -1){perror("close_12");}
        if(execvp(arg3[0], arg3) == -1){perror("exec");}   //execute
        _exit(2);  
    }
    if(close(pipe2[0]) == -1){perror("close_13");}   //close pipe2, now unused
    if(close(pipe2[1]) == -1){perror("close_14");}
    
    if(pipe(pipe4) == -1){perror("pipe");}
    
    pid_t pid4 = fork();
    if(pid4 == -1){perror("fork");}
    else if (pid4 == 0){
        if(dup2(pipe3[0], 0) == -1){perror("dup2_9");}   //get input from pipe3
        if(dup2(pipe4[1], 1) == -1){perror("dup2");}     //set output to pipe4
        if(close(pipe3[0]) == -1){perror("close_15");}      // close everything
        if(close(pipe3[1]) == -1){perror("close_17");}
        if(close(pipe4[0]) == -1){perror("close_15");}      // close everything
        if(close(pipe4[1]) == -1){perror("close_17");}
        if(execvp(arg4[0], arg4) == -1){perror("exec");}   //execute
        _exit(2);
    }
    if(close(pipe3[0]) == -1){perror("close_18");}   //close pipe3
    if(close(pipe3[1]) == -1){perror("close_19");}
    
    pid_t pid5 = fork();
    if(pid5 == -1){perror("fork");}
    else if(pid5 == 0){
        if(dup2(pipe4[0], 0) == -1){perror("dup2");}   //get input from pipe4
        //set output to stdout(should be already good)
        if(close(pipe4[0]) == -1){perror("close");}      // close everything
        if(close(pipe4[1]) == -1){perror("close");}
        if(execvp(arg5[0], arg5) == -1){perror("exec");}   //execute
        _exit(2);
    }
    if(close(pipe4[0]) == -1){perror("close_18");}   //close pipe4
    if(close(pipe4[1]) == -1){perror("close_19");}
    
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    
    if(dup2(stdout_copy, 1) == -1){perror("dup2");}   // restore stdout
    if(close(stdout_copy) == -1){perror("close_20");}
    
    return;
}

void piping::runPipe6(vector<vector<char*> > v, int(&fd)[2]){
    int stdout_copy = dup(1);
    if(stdout_copy == -1){perror("dup");}   //copy stdout fd for later use
    
    if(hasString(v.at(0), "<")){   // remove <
        for(unsigned i = 0; i < v.at(0).size(); i++){
            if(strcmp(v.at(0).at(i), "<") == 0){
                v.at(0).erase(v.at(0).begin() + i);    // get rid of <
                break;
            }
        }
    }
    if(hasString(v.at(v.size() - 1), ">")){  // change stdout
        char* file;
        for(unsigned i = 0; i < v.at(v.size() - 1).size(); i++){
            if(!strcmp(v.at(v.size() - 1).at(i), ">")){
                file = v.at(v.size() - 1).at(i + 1);                        // gets file name, and erases from > onward
                v.at(v.size() - 1).erase(v.at(v.size() - 1).begin() + i, v.at(v.size() - 1).end());
                break;
            }
        }
        if(close(1) == -1){perror("close_0");}
        int out = open(file, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, truncate, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    if(hasString(v.at(v.size() - 1), ">>")){  // change stdout
        char* file;
        for(unsigned i = 0; i < v.at(v.size() - 1).size(); i++){
            if(!strcmp(v.at(v.size() - 1).at(i), ">>")){
                file = v.at(v.size() - 1).at(i + 1);                        // gets file name, and erases from > onward
                v.at(v.size() - 1).erase(v.at(v.size() - 1).begin() + i, v.at(v.size() - 1).end());
                break;
            }
        }
        if(close(1) == -1){perror("close_00");}
        int out = open(file, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        //open file, create, write only, append, read and write permissions for all users
        if(out == -1){perror("open");}
        if(dup2(out, 1) == -1){perror("dup2");}
    }
    char* arg1[1000];
    char* arg2[1000];
    char* arg3[1000];
    char* arg4[1000];
    char* arg5[1000];
    char* arg6[1000];
    
    for(unsigned i = 0; i < v.at(0).size(); i++){   // get arguments
        arg1[i] = v.at(0).at(i);
    }
    arg1[v.at(0).size()] = NULL;
    for(unsigned i = 0; i < v.at(1).size(); i++){
        arg2[i] = v.at(1).at(i);
    }
    arg2[v.at(1).size()] = NULL;
    for(unsigned i = 0; i < v.at(2).size(); i++){
        arg3[i] = v.at(2).at(i);
    }
    arg3[v.at(2).size()] = NULL;
    for(unsigned i = 0; i < v.at(3).size(); i++){
        arg4[i] = v.at(3).at(i);
    }
    arg4[v.at(3).size()] = NULL;
    for(unsigned i = 0; i < v.at(4).size(); i++){
        arg5[i] = v.at(4).at(i);
    }
    arg5[v.at(4).size()] = NULL;
    for(unsigned i = 0; i < v.at(5).size(); i++){
        arg6[i] = v.at(5).at(i);
    }
    arg6[v.at(5).size()] = NULL;
    
    int pipe1[2]; // pipes
    int pipe2[2];
    int pipe3[2];
    int pipe4[2];
    int pipe5[2];
    
    if(pipe(pipe1) == -1){perror("pipe");} // pipe(pipe1)
    pid_t pid = fork();                    // fork()
    if(pid == -1){perror("fork");}
    else if (pid == 0){
        //get input from stdin (done)
        if(dup2(pipe1[1], 1) == -1){perror("dup2");}     // set output to pipe1
        if(close(pipe1[0]) == -1){perror("close_3");}      //close pipe1
        if(close(pipe1[1]) == -1){perror("close_4");}
        if(execvp(arg1[0], arg1) == -1){perror("exec");} //execute
        _exit(2);
    }
    if(pipe(pipe2) == -1){perror("pipe");}      //pipe(pipe2)
     
    pid_t pid2 = fork();                        //fork()
    if(pid2 == -1){perror("fork");}
    else if(pid2 == 0){
        if(dup2(pipe1[0], 0) == -1){perror("dup2");}     //get input from pipe1
        if(dup2(pipe2[1], 1) == -1){perror("dup2");}     //set output to pipe2
        if(close(pipe1[0]) == -1){perror("close_5");}      // close everything
        if(close(pipe1[1]) == -1){perror("close_6");}
        if(close(pipe2[0]) == -1){perror("close_7");}
        if(close(pipe2[1]) == -1){perror("close_8");}
        if(execvp(arg2[0], arg2) == -1){perror("exec");}  //execute
        _exit(2);
    }
    if(close(pipe1[0]) == -1){perror("close_0");}   //close pipe1, unused
    if(close(pipe1[1]) == -1){perror("close_00");}
    
    if(pipe(pipe3) == -1){perror("pipe");}            //pipe(pipe3)
    
    pid_t pid3 = fork();                            //fork()
    if(pid3 == -1){perror("fork");}
    else if (pid3 == 0){
        if(dup2(pipe2[0], 0) == -1){perror("dup2");}   //get input from pipe2
        if(dup2(pipe3[1], 1) == -1){perror("dup2");}     //set output to pipe3
        if(close(pipe2[0]) == -1){perror("close_9");}      // close everything
        if(close(pipe2[1]) == -1){perror("close_10");}
        if(close(pipe3[0]) == -1){perror("close_11");}
        if(close(pipe3[1]) == -1){perror("close_12");}
        if(execvp(arg3[0], arg3) == -1){perror("exec");}   //execute
        _exit(2);  
    }
    if(close(pipe2[0]) == -1){perror("close_13");}   //close pipe2, now unused
    if(close(pipe2[1]) == -1){perror("close_14");}
    
    if(pipe(pipe4) == -1){perror("pipe");}
    
    pid_t pid4 = fork();
    if(pid4 == -1){perror("fork");}
    else if (pid4 == 0){
        if(dup2(pipe3[0], 0) == -1){perror("dup2_9");}   //get input from pipe3
        if(dup2(pipe4[1], 1) == -1){perror("dup2");}     //set output to pipe4
        if(close(pipe3[0]) == -1){perror("close_15");}      // close everything
        if(close(pipe3[1]) == -1){perror("close_17");}
        if(close(pipe4[0]) == -1){perror("close_15");}      // close everything
        if(close(pipe4[1]) == -1){perror("close_17");}
        if(execvp(arg4[0], arg4) == -1){perror("exec");}   //execute
        _exit(2);
    }
    if(close(pipe3[0]) == -1){perror("close_18");}   //close pipe3
    if(close(pipe3[1]) == -1){perror("close_19");}
    
    if(pipe(pipe5) == -1){perror("pipe");}
    
    pid_t pid5 = fork();
    if(pid5 == -1){perror("fork");}
    else if(pid5 == 0){
        if(dup2(pipe4[0], 0) == -1){perror("dup2");}   //get input from pipe4
        if(dup2(pipe5[1], 1) == -1){perror("dup2");}     //set output to pipe5
        if(close(pipe4[0]) == -1){perror("close_20");}      // close everything
        if(close(pipe4[1]) == -1){perror("close_21");}
        if(close(pipe5[0]) == -1){perror("close_22");}      // close everything
        if(close(pipe5[1]) == -1){perror("close_23");}
        if(execvp(arg5[0], arg5) == -1){perror("exec");}   //execute
        _exit(2);
    }
    if(close(pipe4[0]) == -1){perror("close_18");}   //close pipe4
    if(close(pipe4[1]) == -1){perror("close_19");}
    
    pid_t pid6 = fork();
    if(pid6 == -1){perror("fork");}
    else if(pid6 == 0){
        if(dup2(pipe5[0], 0) == -1){perror("dup2");}   //get input from pipe5
        // set output to stdout
        if(close(pipe5[0]) == -1){perror("close_24");}      // close everything
        if(close(pipe5[1]) == -1){perror("close_25");}
        if(execvp(arg6[0], arg6) == -1){perror("exec_1");}   //execute
        _exit(2);
    }
    if(close(pipe5[0]) == -1){perror("close");}   //close pipe5
    if(close(pipe5[1]) == -1){perror("close");}
    
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    if(wait(NULL) == -1){perror("wait");}
    
    if(dup2(stdout_copy, 1) == -1){perror("dup2");}   // restore stdout
    if(close(stdout_copy) == -1){perror("close");}
    
    return;
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
    
    for(unsigned i = 0; i < v.size(); i++){
        if(!strcmp(v.at(i), op) || !strcmp(v.at(i), cl) || !strcmp(v.at(i), cl2)){
            break;
        }
        else{
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
        if(dup2(fd[1], 0) == -1){   // copies the file descriptor of file to 0
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
    else if(v.size() == 1){
        execNoPipe(v.at(0), fd);
    }
    else if(v.size() == 2){
        runPipe2(v, fd);
    }
    else if(v.size() == 3){
        runPipe3(v, fd);
    }
    else if(v.size() == 4){
        runPipe4(v, fd);
    }
    else if(v.size() == 5){
        runPipe5(v, fd);
    }
    else if(v.size() == 6){
        runPipe6(v, fd);
    }
    else{
        cout << "oops! looks like you broke by program!\n";
        cout << "You have too many arguments in your piping statement! (that I programmed for)\n";
        cout << "There's only so much I can do by myself since my parter doesn't do too much...\n";
    }
}





void piping::executeStatement(string s){
    int fd[2];
    vector<vector<char*> > command;
    command = splitCommand(s);

    // if command size is more than 1, that means there's piping involved.
    
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
        execPipe(command, fd);
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




int (&fd)[2])


*/

