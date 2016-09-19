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

using std::deque;
using std::cout;
using std::endl;

struct PCB
{
    int PID;
    int CPUUsageTerm;
    int IORequestTerm;
    int waitingTerm;
    int memoryNeeded;
};

class PCBHandler
{
private:
    deque<PCB> readyQ;
    deque<PCB> blockedQ;
    deque<PCB> IOEventQ;
    PCB CPU;
    
public:
    int createPCB(int PID, int memoryNeeded);
    int deletePCB(int PID);
    int blockPCB(int PID);
    int unblockPCB(int PID);
    int showPCBs(int PID);
    void showAllPCBs();
    void showReady();
    void showBlocked();
    void generatePCBs(int amount);
    int find(int PID, deque<PCB> q);
};

#endif /* PCBHandler_hpp */
