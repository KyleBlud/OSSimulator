//
//  PCB.hpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/18/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#ifndef PCBHandler_hpp
#define PCBHandler_hpp

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <vector>
#include <algorithm>
#include <fstream>

using std::deque;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

struct PCB
{
    int PID;
    int CPUUsageTerm;
    int IORequestTerm;
    int waitingTerm;
    int memoryNeeded;
    int blockedTimeStamp;
    int IORequestType;
    int interactivity;
    int reference;
    int priority;
};

struct IOEvent
{
    int eventType;
    int timeStamp;
};

enum IOEventType
{
    USER = 4,
    HARD_DRIVE = 9
};

enum PCBType
{
    INTERACTIVE = 1,
    CPU_BOUND,
    NORMAL
};

class PCBHandler
{
private:
    const int MAX_MEMORY = 500;
    const int CONTEXT_SWITCH_COST = 10;
    const int SPACE_PADDING = 10;
    int highestPriority;
    deque<PCB> readyQ;
    deque<PCB> blockedQ;
    deque<IOEvent> IOEventQ;
    PCB CPU;
    
public:
    PCBHandler();
    int createPCB(int PID, int memoryNeeded);
    int deletePCB(int PID);
    int blockPCB(int PID);
    int unblockPCB(int PID);
    int showPCB(int PID);
    void showAllPCBs();
    void showReady();
    void showBlocked();
    void showQueue(deque<PCB> q, string qName);
    void printPCBContent(PCB pcb);
    void printPCBTableHeader();
    void generatePCBs(int amount);
    void newExecute(bool roundRobin, int timeSlice);
    void execute(bool MLFQ, int highestPriority, bool roundRobin,
                 int timeSlice);
    void executeAllPolicies(int timeSlice, int priorityLevels);
    void clearCPU();
    void boostPriorityLevels();
    void contextSwitch(bool MLFQ, int timeInCPU);
    PCB findPCBWithHighestPriority();
    void updateAllWaitingTerms(int waitingTerm);
    void addEventToQ(int eventType, int timeCycleStamp);
    void detectIOEvents(int currTime);
    bool scheduler(bool MLFQ, int totalTime);
    void cleanIOEventQ();
    void findIOEventForBlockedPCB(int totalTime);
    int find(int PID, deque<PCB> q);
};

#endif /* PCBHandler_hpp */
