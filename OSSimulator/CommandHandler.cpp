//
//  CommandHandler.cpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/19/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "CommandHandler.hpp"

CommandHandler::CommandHandler()
{
    cout << "Welcome to the O- interface!" << endl;
    currPos = 0;
    if(!addValidCommands())
        cout << "Error loading command file." << endl;
    else
        prompt();
}

void CommandHandler::prompt()
{
    string input;
    int commandType = 0;
    while (commandType != EXIT)
    {
        cout << "~ $ ";
        getline(cin, input);
        userCommands = tokenizer(input, ' ');
        commandType = executeCommand(userCommands.front());
        
        if (commandType != -1)
            commandHistory.push_back(input);
        
        userCommands.clear();
        resetPosition();
    }
}

int CommandHandler::addValidCommands()
{
    ifstream commandFile("cmmdlist.txt");
    vector<string> lineTokens;
    string line;
    int i = 0;
    
    if (commandFile.is_open())
    {
        while(getline(commandFile, line))
        {
            lineTokens = tokenizer(line, ',');
            
            if (lineTokens.size() > 2)
                return NULL;
            
            validCommands.push_back(command());
            validCommands[i].command = lineTokens.front();
            validCommands[i].description = lineTokens.back();
            
            i++;
            lineTokens.clear();
        }
        commandFile.close();
        return 1;
    }
    else
        return NULL;
}

int CommandHandler::executeCommand(string command)
{
    setCurrCommand(command);
    int commandType = defineCommandType();
    
    switch (commandType)
    {
        case VER:       printVersion(); break;
        case DATE:      printDate(); break;
        case LS:        printFilesInDirectory(); break;
        case CMMDHIST:  printCommandHistory(); break;
        case RNCMMD:    renameCommand(); break;
        case EXIT:      if (executeExit()) return EXIT; break;
        case BATCH:     runBatchFile(); break;
        case HELP:      printHelp(); break;
        case PCB:       handlePCB(); break;
        default:
            errorMessage(INVALID, getCurrCommand());
            return -1;
    }
    return 0;
}

void CommandHandler::printVersion()
{
    cout << "O- GNU Version 1.0" << endl;
}

void CommandHandler::printDate()
{
    time_t t = time(0);
    struct tm * now = localtime(&t);
    cout << (now->tm_mon + 1) << "-"
    << now->tm_mday << "-"
    << (now->tm_year + 1900) << endl;
}

void CommandHandler::printFilesInDirectory()
{
    DIR* dir;
    dirent* pdir;
    
    dir = opendir("/Users/Kyle/Desktop/OSSimulator/OSSimulator");
    
    while ((pdir = readdir(dir)))
        cout << pdir->d_name << endl;
    
    closedir(dir);
}

/*
 ~ $ <rename command> <old command name> <new command name>
*/
void CommandHandler::renameCommand()
{
    string oldCommand;
    string newCommand;
    int commandType;

    if (!nextCommand()) { errorMessage(INCOMPLETE, ""); return; }
    
    oldCommand = getCurrCommand();
    commandType = defineCommandType();
    
    if (commandType == -1)
    {
        errorMessage(INVALID, getCurrCommand());
        return;
    }
    else if (!nextCommand()) { errorMessage(INCOMPLETE, ""); return; }
    else
    {
        newCommand = getCurrCommand();
        validCommands[commandType].command = newCommand;
        cout << "Successfully changed \"" << oldCommand
        << "\" to \"" << newCommand << "\"" << endl;
    }
}

void CommandHandler::printCommandHistory()
{
    for (int i = 0; i < commandHistory.size(); i++)
    {
        cout << commandHistory[i] << endl;
    }
}

bool CommandHandler::executeExit()
{
    string input;
    cout << "Are you sure? (Y/N)" << endl;
    cout << "~ $ ";
    getline(cin, input);
    
    if (input =="Y")
        return true;
    if (input == "N")
        return false;
    else
        errorMessage(INVALID, input);
    return false;
}

void CommandHandler::printHelp()
{
    cout << "Commands" << endl;
    for (int i = 0; i < validCommands.size(); i++)
    {
        cout <<
        validCommands[i].command
        << " - " <<
        validCommands[i].description
        << endl;
    }
}
void CommandHandler::runBatchFile()
{
    if (!nextCommand()) { errorMessage(INCOMPLETE, ""); return; }
    
    string fileName = getCurrCommand();
    string line;
    ifstream myFile(fileName);
    
    if (myFile.is_open())
    {
        while (getline(myFile, line))
            executeCommand(line);
        
        myFile.close();
    }
    else
        cout << "File does not exist." << endl;
}

/* currently working on reducing this function */
void CommandHandler::handlePCB()
{
    if (!nextCommand()) { errorMessage(INCOMPLETE, ""); return; }
    
    int PID;
    int success = -1;
    
    if (getCurrCommand() == "show")
    {
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (getCurrCommand() == "ready")
            p.showReady();
        else if (getCurrCommand() == "blocked")
            p.showBlocked();
        else if (getCurrCommand() == "all")
            p.showAllPCBs();
        else if (isInteger(getCurrCommand()))
            p.showPCB(std::stoi(getCurrCommand()));
        else
            errorMessage(INVALID, getCurrCommand());
    }
    else if (getCurrCommand() == "create")
    {
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (!isInteger(getCurrCommand()))
        { errorMessage(EXPECTED_INT, ""); return; }
        PID = std::stoi(getCurrCommand());
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (!isInteger(getCurrCommand()))
        { errorMessage(EXPECTED_INT, ""); return; }
        int memory = std::stoi(getCurrCommand());
        success = p.createPCB(PID, memory);
    }
    else if (getCurrCommand() == "delete")
    {
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (!isInteger(getCurrCommand()))
        { errorMessage(EXPECTED_INT, ""); return; }
        PID = std::stoi(getCurrCommand());
        success = p.deletePCB(PID);
    }
    else if (getCurrCommand() == "block")
    {
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (!isInteger(getCurrCommand()))
        { errorMessage(EXPECTED_INT, ""); return; }
        PID = std::stoi(getCurrCommand());
        success = p.blockPCB(PID);
    }
    else if(getCurrCommand() == "unblock")
    {
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (!isInteger(getCurrCommand()))
        { errorMessage(EXPECTED_INT, ""); return; }
        PID = std::stoi(getCurrCommand());
        success = p.unblockPCB(PID);
    }
    else if (getCurrCommand() == "generate")
    {
        if (!nextCommand())
        { errorMessage(INCOMPLETE, ""); return; }
        if (!isInteger(getCurrCommand()))
        { errorMessage(EXPECTED_INT, ""); return; }
        int amount = std::stoi(getCurrCommand());
        p.generatePCBs(amount);
    }
    else if (getCurrCommand() == "execute")
    {
        if (!nextCommand())
        { p.execute(false, 0, false, 0); return; }
        
        if (getCurrCommand() == "rr")
        {
            if (!nextCommand())
            { errorMessage(INCOMPLETE, ""); return; }
            if (!isInteger(getCurrCommand()))
            { errorMessage(EXPECTED_INT, ""); return; }
            else
                p.execute(false, 0, true, std::stoi(getCurrCommand()));
        }
        else if (getCurrCommand() == "mlfq")
        {
            if (!nextCommand())
            { errorMessage(INCOMPLETE, ""); return; }
            if (!isInteger(getCurrCommand()))
            { errorMessage(EXPECTED_INT, ""); return; }
            else
                p.execute(true, std::stoi(getCurrCommand()), true, 100);
        }
        else if (getCurrCommand() == "all")
        {
            if (!nextCommand())
            { errorMessage(INCOMPLETE, ""); return; }
            if (!isInteger(getCurrCommand()))
            { errorMessage(EXPECTED_INT, ""); return; }
            int timeSlice = std::stoi(getCurrCommand());
            if (!nextCommand())
            { errorMessage(INCOMPLETE, ""); return; }
            if (!isInteger(getCurrCommand()))
            { errorMessage(EXPECTED_INT, ""); return; }
            int priorityLevels = std::stoi(getCurrCommand());
            p.executeAllPolicies(timeSlice, priorityLevels);
        }
    }
    else
        errorMessage(INVALID, getCurrCommand());
    
    if (success == 1)
        cout << "Carried out command successfully" << endl;
    else if (success == 0)
        cout << "Failed to carry out command." << endl;
}

void CommandHandler::setCurrCommand(string s)
{
    currCommand = s;
}

string CommandHandler::getCurrCommand()
{
    return currCommand;
}

int CommandHandler::nextCommand()
{
    currPos++;
    if (currPos >= userCommands.size())
        return 0;
    else
    {
        setCurrCommand(userCommands.at(currPos));
        return 1;
    }
}

void CommandHandler::resetPosition()
{
    currPos = 0;
}

vector<string> CommandHandler::tokenizer(string s, char delim)
{
    vector<string> tokens;
    string token;
    std::stringstream ss;
    ss.str(s);
    
    while (getline(ss, token, delim))
        tokens.push_back(token);
    
    return tokens;
}

int CommandHandler::defineCommandType()
{
    for (int i = 0; i < validCommands.size(); i++)
    {
        if (getCurrCommand() == validCommands[i].command)
            return i;
    }
    return -1;
}

void CommandHandler::errorMessage(int errorType, string command)
{
    if (errorType == INVALID)
        cout << "Invalid. Could not recognize \"" << command << "\"" << endl;
    else if (errorType == INCOMPLETE)
        cout << "Incomplete command." << endl;
    else if (errorType == EXPECTED_INT)
    {
        cout << "Invalid command. Expected an integer." << endl;
    }
}

bool CommandHandler::isInteger(string s)
{
    return (s.find_first_not_of("0123456789") == string::npos);
}
