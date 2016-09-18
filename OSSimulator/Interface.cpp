//
//  Interface.cpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 8/26/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "Interface.hpp"

Interface::Interface()
{
    cout << "Welcome to the O- interface!" << endl;
    addCommands();
    commandPrompt();
}

void Interface::addCommands()
{
    commands.push_back(command());
    commands[0].command = "ver";
    commands[0].description = "displays the software version number";
    commands.push_back(command());
    commands[1].command = "date";
    commands[1].description = "displays the date";
    commands.push_back(command());
    commands[2].command = "ls";
    commands[2].description = "prints a list of all the files in the current directory";
    commands.push_back(command());
    commands[3].command = "cmmdhist";
    commands[3].description = "displays a history of all the commands you have used";
    commands.push_back(command());
    commands[4].command = "rncmmd";
    commands[4].description = "renames a command";
    commands.push_back(command());
    commands[5].command = "exit";
    commands[5].description = "exits the simulation";
    commands.push_back(command());
    commands[6].command = "batch";
    commands[6].description = "run commands from a batch file or script";
    commands.push_back(command());
    commands[7].command = "help";
    commands[7].description = "displays help information for each command";
}

void Interface::commandPrompt()
{
    string input;
    int commandIndex;
    
    while (commandIndex != EXIT)
    {
        commandIndex = -1;
        
        cout << "kyle@blud ~ $ ";
        cin >> input;
        
        commandHistory.push_back(input);
        
        commandIndex = executeCommand(input);
    }
}

int Interface::executeCommand(string input)
{
    int commandIndex = -1;
    string exit;
    string fileName;
    
    commandIndex = findCommand(input);
    
    switch (commandIndex)
    {
        case VER:
            printVersion(); break;
        case DATE:
            printDate(); break;
        case LS:
            printFilesInDirectory(); break;
        case CMMDHIST:
            printCommandHistory(); break;
        case RNCMMD:
            renameCommand(); break;
        case EXIT:
            cout << "Are you sure? (Y/N)" << endl;
            cin >> exit;
            if (exit == "N")
                commandIndex = -2;
            else if (exit == "Y")
                commandIndex = EXIT;
            else
                commandIndex = -1;
            break;
        case BATCH:
            cout << "What file would you like to run?" << endl;
            cin >> fileName;
            runBatchFile(fileName); break;
        case HELP:
            printHelp(); break;
    }
    
    if (commandIndex == -1)
        cout << "Invalid command" << endl;
    
    return commandIndex;
}

void Interface::printVersion()
{
    cout << "O- GNU Version 1.0" << endl;
}

//http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
void Interface::printDate()
{
    time_t t = time(0);
    struct tm * now = localtime(&t);
    cout << (now->tm_mon + 1) << "-"
    << now->tm_mday << "-"
    << (now->tm_year + 1900) << endl;
}

void Interface::printFilesInDirectory()
{
    DIR* dir;
    dirent* pdir;
    
    dir = opendir("/Users/Kyle/Desktop/OSSimulator/OSSimulator");
    
    while ((pdir = readdir(dir)))
        cout << pdir->d_name << endl;
    
    closedir(dir);
}

void Interface::printCommandHistory()
{
    for (int i = 0; i < commandHistory.size(); i++)
    {
        cout << commandHistory[i] << endl;
    }
}

void Interface::renameCommand()
{
    string oldCommand;
    string newCommand;
    int commandIndex = -1;
    
    while (commandIndex == -1)
    {
        cout << "Which command would you like to rename?" << endl;
        cin >> oldCommand;
        
        commandIndex = findCommand(oldCommand);
        
        if (commandIndex == -1)
            cout << "Invalid command." << endl;
    }
    
    cout << "What would you like to rename it as?" << endl;
    cin >> newCommand;
    
    commands[commandIndex].command = newCommand;
    
    cout << "The command \"" << oldCommand << "\" is now called \""
    << newCommand << "\"" << endl;
    
}

void Interface::printHelp()
{
    cout << "Commands" << endl;
    
    for (int i = 0; i < commands.size(); i++)
    {
        cout << commands[i].command << " - " << commands[i].description << endl;
    }
}

void Interface::runBatchFile(string fileName)
{
    string line;
    
    ifstream myFile(fileName);
    
    if (myFile.is_open())
    {
        while (getline(myFile, line))
            executeCommand(line);
        
        myFile.close();
    }
    else
    {
        cout << "File does not exist." << endl;
    }
}

int Interface::findCommand(string command)
{
    int commandIndex = -1;
    
    for (int i = 0; i < commands.size(); i++)
    {
        if (command == commands[i].command)
            commandIndex = i;
    }
    
    return commandIndex;
}