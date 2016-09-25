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
    int timeEnteredBlockedQ;
    int eventTypeRequested;
};

struct IOEvent
{
    int eventType;
    int timeCycleStamp;
};

enum IOEventType
{
    USER = 4,
    HARD_DRIVE = 9
};

class PCBHandler
{
private:
    const int MAX_MEMORY = 500;
    deque<PCB> readyQ;
    deque<PCB> blockedQ;
    deque<IOEvent> IOEventQ;
    PCB CPU;
    
public:
    int createPCB(int PID, int memoryNeeded);
    int deletePCB(int PID);
    int blockPCB(int PID);
    int unblockPCB(int PID);
    int showPCB(int PID);
    void showAllPCBs();
    void showReady();
    void showBlocked();
    void showQueue(deque<PCB> q, string qName);
    void generatePCBs(int amount);
    void execute();
    void writeQueue(deque<PCB> q, string qName, ofstream &trace);
    void clearCPU();
    void addEventToQ(int eventType, int timeCycleStamp);
    void updateAllWaitingTerms(int waitingTerm);
    int find(int PID, deque<PCB> q);
};

#endif /* PCBHandler_hpp */
