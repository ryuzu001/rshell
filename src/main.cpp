#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>   //getenv
#include <sys/types.h>
#include <unistd.h>   //getlogin
#include <sys/wait.h>
#include <cstddef>
#include <vector>

#include "execute.h"
#include "connector.h"
#include "test.h"


using namespace std;

void displayShell();
vector<char*> charstarToVector(char*, string);

void printInfo(){
    printf("$ ");
}

void checkUserInput(vector<char*> input){
    string exitStr = "exit";     // string to indicate exit shell
    execute te;
    if(input.size() == 0){               // no input, recursively call itself
        displayShell();
        te.exit_shell();
    }
    if(input.at(0) == exitStr){          // exit command
        te.exit_shell();
    }    
}
void checkUserInput2(vector<string> input){
    string exitStr = "exit";     // string to indicate exit shell
    execute te;
    if(input.size() == 0){               // no input, recursively call itself
        displayShell();
        te.exit_shell();
    }
    if(input.at(0) == exitStr){          // exit command
        te.exit_shell();
    }    
}

// string executeSemicolon(string input, string delim){
//     execute te;
//     int index = 0;    // incase of ending semicolon
    
//     for(unsigned i = 0; i < input.size(); ++i){
//         if(input.at(i) == ';'){
//             index = i;
//             break;  //first semicolon only
//         }
//     }
//     string firstCommand = input.substr(0, index);
//     string secondCommand = input.substr(index + 1);
    
//     char * str = new char[firstCommand.length() + 1];
//     strcpy(str, firstCommand.c_str()); 
    
//     vector<char*> com1 = charstarToVector(str, delim);
    
//     checkUserInput(com1);
    
//     te.executeStatement(com1);
    
//     delete[] str;
    
//     return secondCommand;
// }
bool hasSemicolon(string userInput){
    if (userInput.find(";") != string::npos) {
        return true;
    }
    return false;
}

bool hasAnd(string userInput){
    if (userInput.find("&&") != string::npos) {
        return true;
    }
    return false;
}

bool isAnd(string input){
    if(input != "&&"){
        return false;
    }
    return true;
}

bool isSemi(string input){
    if(input != ";"){
        return false;
    }
    return true;
}

bool hasOr(string userInput){
    if (userInput.find("||") != string::npos) {
        return true;
    }
    return false;  
}

bool hasParenthesis(string userInput)
{
    if (userInput.find("(") != string::npos) {
        return true;
    }
    return false;
}

bool hasConnector(string userInput){
    if(hasSemicolon(userInput) || hasAnd(userInput) || hasOr(userInput)){
        return true;
    }
    return false;
}

bool isConnector(string i){
    if(i == ";"){
        return true;
    }
    if(i == "&&"){
        return true;
    }
    if(i == "||"){
        return true;
    }
    return false;
}

vector<int> readCmd(string input)
{
    vector<int> values;
    for (unsigned i = 0; i < input.size(); ++i)
    {
        if (input.at(i) == '&' || input.at(i) == '|')
        {
            values.push_back(i);
            i++;
        }
    }
    
    return values;
}

vector<string> parseUI(string input){
    vector<string> rawCommands;
    if(input.size() == 0){
        printf("empty");
    }
    char* str = new char[input.size() + 1];
    strcpy(str, input.c_str());
    
    char * pch;
    pch = strtok(str, " ");
    while(pch != NULL){
        rawCommands.push_back(pch);
        pch = strtok(NULL, " ");
    }
    return rawCommands;
    
}




vector<vector<unsigned> > findOpenParentheses(vector<string> v)
{
    vector<vector<unsigned> > toReturn(v.size()); //initialize vector to 0 parentheses for entire v
    for (unsigned i = 0; i < v.size(); ++i)
    {
        for (unsigned j = 0; j < v.at(i).length(); ++j)
        {
            if(v.at(i).at(j) == '(')
            {
                toReturn.at(i).push_back(j);
            }
        }
    }
    return toReturn;
}



int findClosed(vector<string> v, unsigned vIndex)
{
    int count = 1;
    for(unsigned i = vIndex; i < v.size(); ++i)
    {
        for(unsigned j =  1; j < v.at(i).size(); j++)
        {
            if(v.at(i).at(j) == '(')
            {
                count++;
            }
            if(v.at(i).at(j) == ')')
            {
                count--;
            }
            if(count == 0)
            {
                return i;
            }
        }
    }
    return 1; //needs to throw error, not enough parentheses
}



void executeVector(vector<string> v){
    if(v.at(0) == "&&" || v.at(0) == "||"){
        printf("error, no first argument");
        return;
    }
    bool previousCommandSucessfull = true;
    vector<char*> secondCommand;
    vector<vector<unsigned> > parentheses;
    execute e;
    unsigned int placeholder = 0;
    string connector = "semicolon";
    
    parentheses = findOpenParentheses(v);
    
    
    
    for(unsigned i = 0; i < v.size(); i++){
        vector<char*> singleCommand;// clear singleCommand
        if(hasParenthesis(v.at(i)))
        {
            vector<string> subExec; //clear vector of strings
            unsigned index = i;
            unsigned closedParen = findClosed(v, i);
            while(index <= closedParen) 
            {
                subExec.push_back(v.at(index));
                placeholder += subExec.back().size();
                index++;
            }
            
            
            for(unsigned p = 0; p < subExec.size(); ++p)
            {
                cout << "before removing (): ";
                cout << subExec.at(p); cout << endl;
            }
            
            
            //these will delete the last element in the vector, not parentheses. need to find them first.
            subExec.at(0) = subExec.at(0).substr(1, subExec.at(0).size() - 1);//should remove first parenthesis
            subExec.at(subExec.size() - 1) = subExec.at(subExec.size() - 1).substr(0, subExec.at(subExec.size() - 1).size() - 1);// should remove corresponding parenthesis
            for(unsigned p = 0; p < subExec.size(); ++p)
            {
                cout << "After removing (): ";
                cout << subExec.at(p);
                cout << endl;
            }
            
            
            
            if(connector == "semicolon"){
                cout << "263" << endl;
                executeVector(subExec); 
                previousCommandSucessfull = true;
            }
            if(connector == "and"){
                if(previousCommandSucessfull){
                    executeVector(subExec);
                    previousCommandSucessfull = true;
                }
                else{
                    previousCommandSucessfull = false;
                }
            }
            if(connector == "or"){
                if(previousCommandSucessfull){
                    previousCommandSucessfull = false;
                }
                else{
                    executeVector(subExec);
                    previousCommandSucessfull = true;
                }
            }
            
        connector = //should be whatever was after the parentheses    
            
            
        }

        else if(hasSemicolon(v.at(i))){
            string temp;                                  //get rid of semicolon
            temp = v.at(i).substr(0, v.at(i).length() - 1);
            v.at(i) = temp;
            while(placeholder <= i){       // <= because the ; will be at the end of a statement.
                char* temp = new char[v.at(placeholder).length() + 1];
                strcpy(temp, v.at(placeholder).c_str());
                singleCommand.push_back(temp);
                placeholder++;
            }
             //execute singleCommand
            if(connector == "semicolon"){
                if(e.executeStatement(singleCommand)){
                    previousCommandSucessfull = true;
                }
                else{
                    previousCommandSucessfull = false;
                }
            }
            if(connector == "and"){
                if(previousCommandSucessfull){
                    //execute
                    if(e.executeStatement(singleCommand)){
                        previousCommandSucessfull = true;
                    }
                    else{
                        previousCommandSucessfull = false;
                    }
                }
                else{
                    //dont
                }
            }
            if(connector == "or"){
                if(previousCommandSucessfull){
                    
                }
                else{
                    if(e.executeStatement(singleCommand)){
                        previousCommandSucessfull = true;
                    }
                    else{
                        previousCommandSucessfull = false;
                    }
                }
            }
            connector = "semicolon";
        }
        else if(isAnd(v.at(i))){
            // The string will be "&&" so there will be a before and after to execute. i is the index of &&
            while(placeholder < i){     //before
                char* temp = new char[v.at(placeholder).length() + 1];
                strcpy(temp, v.at(placeholder).c_str());
                singleCommand.push_back(temp);
                placeholder++;
            }
            placeholder ++;
            if(connector == "semicolon"){
                if(e.executeStatement(singleCommand)){
                    previousCommandSucessfull = true;
                }
                else{
                    previousCommandSucessfull = false;
                }
            }
            if(connector == "and"){
                if(previousCommandSucessfull){
                    //execute
                    if(e.executeStatement(singleCommand)){
                        previousCommandSucessfull = true;
                    }
                    else{
                        previousCommandSucessfull = false;
                    }
                }
                else{
                    //dont
                }
            }
            if(connector == "or"){
                if(previousCommandSucessfull){
                    
                }
                else{
                    if(e.executeStatement(singleCommand)){
                        previousCommandSucessfull = true;
                    }
                    else{
                        previousCommandSucessfull = false;
                    }
                }
            }
            connector = "and";
        }
        else if(hasOr(v.at(i))){
            while(placeholder < i){
                char* temp = new char[v.at(placeholder).length() + 1];
                strcpy(temp, v.at(placeholder).c_str());
                singleCommand.push_back(temp);
                placeholder++;
            }
            placeholder++;
            if(connector == "semicolon"){
                if(e.executeStatement(singleCommand)){
                    previousCommandSucessfull = true;
                }
                else{
                    previousCommandSucessfull = false;
                }
            }
            if(connector == "and"){
                if(previousCommandSucessfull){
                    if(e.executeStatement(singleCommand)){
                        previousCommandSucessfull = true;
                    }
                    else{
                        previousCommandSucessfull = false;
                    }
                }
            }
            if(connector == "or"){
                if(previousCommandSucessfull){}
                else{
                    if(e.executeStatement(singleCommand)){
                        previousCommandSucessfull = true;
                    }
                    else{
                        previousCommandSucessfull = false;
                    }
                }
            }
            connector = "or";
        }
    }
    vector<char*> lastCommand; 

    unsigned t = placeholder;
    while(t < v.size()){
        char* temp = new char[1000];
        strcpy(temp, v.at(t).c_str());
        lastCommand.push_back(temp); 
        t++;
    }
    // cout << "Placeholder: " << placeholder;
    // cout << "\nvsize" << v.size();
    // cout << "lastCommand";
    // for(unsigned x = 0; x < lastCommand.size();x++){
    //     cout << lastCommand.at(x) << " ";
    // }
    // cout << endl << "v: \n";
    // for(unsigned x = 0; x < v.size();x++){
    //     cout << v.at(x) << " ";
    // }
    if(connector == "semicolon"){
        e.executeStatement(lastCommand);
    }
    if(connector == "and" && previousCommandSucessfull){
        e.executeStatement(lastCommand);
    }
    if(connector == "or" && !previousCommandSucessfull){
        e.executeStatement(lastCommand);
    }
}

bool executeSemicolon(string input, string &newInput, string delim){
    execute te;
    int index = 0;    // incase of ending semicolon
    
    for(unsigned i = 0; i < input.size(); ++i){
        if(input.at(i) == ';'){
            index = i;
            break;  //first semicolon only
        }
    }
    string firstCommand = input.substr(0, index - 1);
    string secondCommand = input.substr(index + 1);
    
    char * str = new char[firstCommand.length() + 1];
    strcpy(str, firstCommand.c_str()); 
    
    vector<char*> com1 = charstarToVector(str, delim);
    
    checkUserInput(com1);
    
    
    if (hasAnd(firstCommand) || hasOr(firstCommand))
    {
        vector<int> ops = readCmd(firstCommand);
        cout << ops.at(0) << endl;
        ops.push_back(firstCommand.size());
        
        for (unsigned int i = 0, k = 0; i < firstCommand.size(); ++i, ++k)
        {
            string command = "";
            for (int j = i; j < ops.at(k); j++)
            {
                command = command + firstCommand.at(j);
            }
            char * strConnector = new char[command.length() + 1];
            vector<char*> com = charstarToVector(strConnector, delim);
            strcpy(strConnector, command.c_str());
            
            if(firstCommand.at(ops.at(k)) == '&')
            {
                i = ops.at(k) + 1;
            }
            else if (firstCommand.at(ops.at(k)) == '|')
            {
                
                i = ops.at(k+1) + 1;
                k++;
            }
            else
            {
                i = firstCommand.size();
            }
            delete[] strConnector;
            
        }
    }
    
    delete[] str;
    
    newInput = secondCommand;
    
    return true;
}

vector<char*> stringToCharVector(string s){
    vector<char*> temp;
    char* cstr = new char[1000];
    strcpy(cstr, s.c_str());
    char* pch;
    pch = strtok(cstr, " ");
    while(pch != NULL){
        temp.push_back(pch);
        pch = strtok(NULL, " ");
    }
    return temp;
}

bool executeAnd(string input, string &newInput, string delim){
    unsigned i;
    for (i = 0; i < input.size(); ++i){
        if(input.at(i) == '&' && input.at(i + 1) == '&'){
            
            break;
        }
    }
    string firstCommand = input.substr(0, i - 1);
    string secondCommand = input.substr(i + 2);
    
    char * str = new char[firstCommand.length() + 1];
    char * str2 = new char[secondCommand.length() + 1];
    strcpy(str, firstCommand.c_str());
    strcpy(str2, secondCommand.c_str());
    
    
    vector<char*> com1 = charstarToVector(str, delim);
    vector<char*> com2 = charstarToVector(str2, delim);
    
    checkUserInput(com1);
    andOp* command = new andOp(com1, com2);
    if(!(command->executeStatement()))
    {
        return false;
    }
    
    delete[] str;
    return true;
    
}

vector<char*> charstarToVector(char* str, string delim){
    vector<char*> result;
    char * pch = strtok (str, delim.c_str());
    while(pch != NULL){
        result.push_back(pch);
        pch = strtok (NULL, delim.c_str());
    }
    return result;
}

void displayShell(){
    string comment = "#";        // character used to indicate comment
    string delim = " ";          // delimiters for strtok 
    string userInput;  
    string revisedInput;
    
    while(1){
        printInfo();
        execute te;
        vector<char*> toExecute;      // reset toExecute to empty
        getline(cin, userInput);
        
        while(hasConnector(userInput)){
            if(hasSemicolon(userInput)){
                executeSemicolon(userInput, revisedInput, delim);
                userInput = revisedInput;
            }
            else
            {
                 if (hasAnd(userInput) || hasOr(userInput))
                {
                    vector<int> ops = readCmd(userInput);
                    cout << ops.at(0) << endl;
                    ops.push_back(userInput.size() - 1);
                    
                    for (unsigned int i = 0, k = 0; i < userInput.size(); ++i, ++k)
                    {
                        string command = "";
                        for (int j = i; j < ops.at(k); j++)
                        {
                            command = command + userInput.at(j);
                        }
                        char * strConnector = new char[command.length() + 1];
                        vector<char*> com = charstarToVector(strConnector, delim);
                        strcpy(strConnector, command.c_str());
                        
                        if(userInput.at(ops.at(k)) == '&')
                        {
                            i = ops.at(k) + 1;
                        }
                        else if (userInput.at(ops.at(k)) == '|')
                        {
                            
                            i = ops.at(k+1) + 1;
                            k++;
                        }
                        else
                        {
                            i = userInput.size();
                        }
                        delete[] strConnector;
                        
                    }
                }
            }
        }
        
        char * str = new char[userInput.length() + 1];  // char array named str
        strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
        str = strtok (str, comment.c_str());     // break at every comment 
        
        toExecute = charstarToVector(str, delim);

        checkUserInput(toExecute);
        
        te.executeStatement(toExecute);
        
        delete [] str;
    }    
}

void displayShell2(){
    string comment = "#";        // character used to indicate comment
    unsigned int commentIndex;
    string userInput;  
    string revisedInput;
    vector<string> v;
    
    while(1){
        vector<char*> toExecute;
        printInfo();
        getline(cin, userInput);
        commentIndex = userInput.find(comment);
        if(commentIndex != string::npos){   //rid of comments
            userInput.substr(0,commentIndex);
        }
        
        if(hasConnector(userInput)){
            v = parseUI(userInput);
            checkUserInput2(v);
            executeVector(v);
        }
        else{
            char * str = new char[userInput.length() + 1];  // char array named str
            strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
            toExecute = charstarToVector(str, " ");
            checkUserInput(toExecute);
            execute te;
            te.executeStatement(toExecute);
        }
    }
}

int main(){
    displayShell2();     
                            /* Since we cannot call main(), I needed another 
                            function to call itself, so I made main just call
                            that function.*/
    // cout << "enter string\n";
    // string str;
    // getline(cin, str);
    // vector<char*>ayy = stringToCharVector(str);
    // // cout << "you entered: \n";
    // // for(unsigned i = 0; i < ayy.size(); i++){
    // //     cout << ayy.at(i) << "\n";
    // // }
    // test temp;
    // temp.executeStatement(ayy);
}