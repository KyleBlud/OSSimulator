//
//  Interface.hpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 8/26/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef Interface_hpp
#define Interface_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <dirent.h>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

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
    HELP
};

class Interface
{
private:
    vector<command> commands;
    vector<string> commandHistory;
    void addCommands();
    void commandPrompt();
    int executeCommand(string input);
    void printVersion();
    void printDate();
    void printFilesInDirectory();
    void renameCommand();
    void printCommandHistory();
    void printHelp();
    void runBatchFile(string fileName);
    int findCommand(string command);
    
public:
    Interface();
};

#endif /* Interface_hpp */