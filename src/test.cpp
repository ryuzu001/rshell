// // STRICTLY FOR TESTING PURPOSES. WILL NOT BE INCLUDED IN THE FINAL PROJECT


// #include <iostream>
// #include <string.h>
// #include <cstdlib>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <vector>

// using namespace std;

// int main(){
    
//     // char* args[3];
//     // string ls = "ls -l";
//     // string dash_l = "";
    
//     // args[0] = (char*) ls.c_str();
//     // args[1] = (char*) dash_l.c_str();
//     // args[2] = NULL;
    
//     // // for(int x = 0; x < 2; x++){
//     // //     cout << args[x] << " ";
//     // // }
    
//     // if( execvp (args [0],args) == -1){
//     //     perror ("exec");
//     // }
    
//     string userInput = "HELLO THERE MY NAME IS RYAN#YOOOOOOOOOO";
//     vector<char*> toExecute;
    
//     char * str = new char[userInput.length() + 1];  // char array named str
    
//     strcpy(str, userInput.c_str());                 // copy string to char* (NOT CONST)
    
//     printf("Before: %s\n", str);
    
//     char * pch2;  
//     string delim = "#";    // delimiters for strtok
  
//     pch2 = strtok (str,delim.c_str());      // break at every delimiter
//     toExecute.push_back(pch2);
//     cout << "After: ";
//     for(unsigned int x = 0; x < toExecute.size(); x++){
//         cout << toExecute.at(x) << " ";
//     }
    
// }





Base* makeTree(vector<char*> fixedCommandList){
    vector<Base*> commandsParsed;
    vector<char*> connectorsParsed;
    vector<Base*> commandTree;
    while(!fixedCommandList.empty()){
        bool commandWithSemi = false; 
        char* tempToken = fixedCommandList.front();
        fixedCommandList.pop_front();
        if(checkForPrecedence(tempToken)){
            string tempStr = string(tempToken);
            tempStr = tempStr.substr(1, tempStr.size());
            strcpy(tempToken, tempStr.c_str());
            deque<char*> precedenceTree;
            precedenceTree.push_back(tempToken);
            while(!fixedCommandList.empty() && !(strchr(fixedCommandList.front(), ')') != NULL)){
                char* tempPrecedenceToken = fixedCommandList.front();
                fixedCommandList.pop_front();
                precedenceTree.push_back(tempPrecedenceToken);
            }
            bool precedenceSemi = false;
            if(!fixedCommandList.empty() && strchr(fixedCommandList.front(), ')') != NULL){
                char* tempPrecedenceToken = fixedCommandList.front();
                fixedCommandList.pop_front();
                if(strchr(tempPrecedenceToken, ';') != NULL){
                    
                    string fixToken = string(tempPrecedenceToken);
                    fixToken = fixToken.substr(0, fixToken.size() - 1);
                    strcpy(tempPrecedenceToken, fixToken.c_str());
                    
                    precedenceSemi = true;
                }
                string tempPrecedenceString = string(tempPrecedenceToken);
                
                tempPrecedenceString = tempPrecedenceString.substr(0, tempPrecedenceString.size() - 1);
                strcpy(tempPrecedenceToken, tempPrecedenceString.c_str()); 
                
                precedenceTree.push_back(tempPrecedenceToken);
            }
            /*for(int i = 0; i < precedenceTree.size(); ++i){
                cout << precedenceTree.at(i) << endl;
            }*/
            Base* tempConnector = makeTree(precedenceTree);
            commandsParsed.push_back(tempConnector);
            if(precedenceSemi){
                string colonChar = ";";
                char* c_colonChar = new char[2];
                strcpy(c_colonChar, colonChar.c_str());
                connectorsParsed.push_back(c_colonChar);
                precedenceSemi = false;
            }
            /*for(int i = 0; i < commandsParsed.size(); ++i){
                commandsParsed.at(i)->printCommand();
            }*/
            /*cout << commandsParsed.size() << endl;
            for(int i =0; i < fixedCommandList.size(); ++i){
                cout << fixedCommandList.at(i) << endl;
            }*/

        }            
        else if(checkForSemi(tempToken)){
            string tempStr = string(tempToken);
            tempStr = tempStr.substr(0, tempStr.size() - 1);
            strcpy(tempToken, tempStr.c_str());
            string colonChar = ";";
            char* c_colonChar = new char[2];
            strcpy(c_colonChar, colonChar.c_str());
            connectorsParsed.push_back(c_colonChar);
            commandWithSemi = true;
        }
        else if(strcmp(tempToken, "exit") == 0){
            Exit* newExit = new Exit();
            commandsParsed.push_back(newExit);
        }
        else{ 
            if(strcmp(tempToken, "test") == 0 || strcmp(tempToken, "[") == 0){
                TEST* newTest = new TEST();
                if(strcmp(tempToken, "[") == 0){
                    while(!fixedCommandList.empty() && !(strcmp(fixedCommandList.front(), "]") == 0)){ 
                        newTest->addFlag(fixedCommandList.front());
                        fixedCommandList.pop_front();
                    }
                    if(!fixedCommandList.empty()){
                        fixedCommandList.pop_front();
                    }
                }
                else{
                    while(!fixedCommandList.empty() && !checkForConnector(fixedCommandList.front())){
                        if(commandWithSemi){
                            break;
                        }
                        tempToken = fixedCommandList.front();
                        if(checkForSemi(tempToken)){
                            string tempStr = string(tempToken);
                            tempStr = tempStr.substr(0, tempStr.size() - 1);
                            strcpy(tempToken, tempStr.c_str());
                            newTest->addFlag(fixedCommandList.front());
                            fixedCommandList.pop_front();
                            string colonChar = ";";
                            char* c_colonChar = new char[2];
                            strcpy(c_colonChar, colonChar.c_str());
                            connectorsParsed.push_back(c_colonChar);
                            break;
                        }
                        newTest->addFlag(fixedCommandList.front());
                        fixedCommandList.pop_front();
                    }
                } 
                commandsParsed.push_back(newTest);
            }
            else if(strcmp(tempToken, "cd") == 0){
                CD* newCD = new CD();
                while(!fixedCommandList.empty() && !checkForConnector(fixedCommandList.front())){
                    if(commandWithSemi){
                        break;
                    }
                    tempToken = fixedCommandList.front();
                    if(checkForSemi(tempToken)){
                        string tempStr = string(tempToken);
                        tempStr = tempStr.substr(0, tempStr.size() - 1);
                        strcpy(tempToken, tempStr.c_str());
                        newCD->addFlag(fixedCommandList.front());
                        fixedCommandList.pop_front();
                        string colonChar = ";";
                        char* c_colonChar = new char[2];
                        strcpy(c_colonChar, colonChar.c_str());
                        connectorsParsed.push_back(c_colonChar);
                        break;
                    }
                    newCD->addFlag(fixedCommandList.front());
                    fixedCommandList.pop_front();
                }
                commandsParsed.push_back(newCD);
            }    
            else{   


                if(checkForConnector(tempToken)){
                    connectorsParsed.push_back(tempToken);


                }
                else{
 
                    Command* newCommand = new Command(tempToken); 
                    while(!fixedCommandList.empty() && !checkForConnector(fixedCommandList.front())){
                        if(commandWithSemi){
                            break;
                        }
                        tempToken = fixedCommandList.front();
                        if(checkForSemi(tempToken)){
                            string tempStr = string(tempToken);
                            tempStr = tempStr.substr(0, tempStr.size() - 1);
                            strcpy(tempToken, tempStr.c_str());
                            newCommand->addFlag(fixedCommandList.front());
                            fixedCommandList.pop_front();
                            string colonChar = ";";
                            char* c_colonChar = new char[2];
                            strcpy(c_colonChar, colonChar.c_str());
                            connectorsParsed.push_back(c_colonChar);
                            break;
                        }
                        newCommand->addFlag(fixedCommandList.front());
                        fixedCommandList.pop_front();   
                    }
                    /*cout << "COMMAND IS " << endl;
                    newCommand->printCommand();
                    cout << endl;*/
                    commandsParsed.push_back(newCommand);
                } 
            }
        }
        if(!fixedCommandList.empty() && checkForConnector(fixedCommandList.front())){
            tempToken = fixedCommandList.front();
            fixedCommandList.pop_front();
            connectorsParsed.push_back(tempToken);
        }
    }
  
    if(connectorsParsed.size() > 0){
        if(commandTree.size() == 0){
           
          
         
            Base* leftBase = commandsParsed.at(0);
            commandsParsed.pop_front();
            Base* rightBase = commandsParsed.at(0);
            commandsParsed.pop_front();
         
            char* connectorChar = connectorsParsed.at(0);
            connectorsParsed.pop_front();
            if(strchr(connectorChar, ';') != NULL){
                semiColon* newSemicolon = new semiColon(leftBase, rightBase); 
                commandTree.push_back(newSemicolon);
            }
            else if(strstr(connectorChar, "&&") != NULL){
                AND* newAnd = new AND(leftBase, rightBase);      ///////////////////////
                commandTree.push_back(newAnd);
            }
            else if(strstr(connectorChar, "||") != NULL){
                OR* newOr = new OR(leftBase, rightBase);
                commandTree.push_back(newOr);
            } 
        }      
        /*for(int i = 0; i < commandsParsed.size(); ++i){
            commandsParsed.at(i)->printCommand();
            cout << endl;
        } */       
        while(!connectorsParsed.empty() && !commandsParsed.empty()){
            Base* rBase = commandsParsed.front();
            commandsParsed.pop_front();
            Base* lBase = commandTree.at(0);
            commandTree.pop_front();
            char* connectChar = connectorsParsed.at(0);
            connectorsParsed.pop_front();
            if(strchr(connectChar, ';') != NULL){ 
                semiColon* newSemiColon = new semiColon(lBase, rBase); 
                commandTree.push_back(newSemiColon);
            }
            else if(strstr(connectChar, "&&") != NULL){ 
                AND* newAND = new AND(lBase, rBase);               ///////////
                commandTree.push_back(newAND);
            }
            else if(strstr(connectChar, "||") != NULL){ 
                OR* newOR = new OR(lBase, rBase);
                commandTree.push_back(newOR);
            }
        }
        while(!connectorsParsed.empty() && commandTree.size() > 1){
            
            Base* lBase = commandTree.at(0);
            commandTree.pop_front();
            Base* rBase = commandTree.at(0);
            commandTree.pop_front();
            char* connectChar = connectorsParsed.at(0);
            connectorsParsed.pop_front();
            if(strchr(connectChar, ';') != NULL){
                semiColon* newSemiColon = new semiColon(lBase, rBase);
                commandTree.push_back(newSemiColon);
            }
            else if(strstr(connectChar, "&&") != NULL){
                AND* newAND = new AND(lBase, rBase);                      //////////////
                commandTree.push_back(newAND);
            }
            else if(strstr(connectChar, "||") != NULL){
                OR* newOR = new OR(lBase, rBase);
                commandTree.push_back(newOR);
            }
        }

    }
    else if(commandTree.size() > 0){
        return commandTree.front();
    }
    else{
        return commandsParsed.front();
    }
    
    if(commandsParsed.size() != 0){
        perror("invalid number of commands and connectors");
    }
    else{

        return commandTree.back();
    }
    return NULL;    
}
