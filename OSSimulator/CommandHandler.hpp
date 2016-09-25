//
//  CommandHandler.hpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/19/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef CommandHandler_hpp
#define CommandHandler_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <dirent.h>
#include "PCBHandler.hpp"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::ifstream;

struct command
{
    string command;
    string description;
};

enum Commands
{
    VER = 0,
    DATE,
    LS,
    CMMDHIST,
    RNCMMD,
    EXIT,
    BATCH,
    HELP,
    PCB
};

class CommandHandler
{
private:
    vector<command> validCommands;
    vector<string> userCommands;
    vector<string> commandHistory;
    string currCommand;
    PCBHandler p;
    int currPos;
    
    void prompt();
    int addValidCommands();
    int executeCommand(string command);
    void setCurrCommand(string s);
    string getCurrCommand();
    int nextCommand();
    void resetPosition();
    vector<string> tokenizer(string s, char delim);
    void printVersion();
    void printDate();
    void printFilesInDirectory();
    void renameCommand();
    void printCommandHistory();
    bool executeExit();
    void printHelp();
    void runBatchFile();
    void handlePCB();
    int defineCommandType();
    void invalidCommandMessage(string command);
    bool isInteger(string s);
    
public:
    CommandHandler();
};

#endif /* CommandHandler_hpp */
