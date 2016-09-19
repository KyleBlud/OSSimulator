//
//  PCB.cpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/18/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "PCBHandler.hpp"

int PCBHandler::createPCB(int PID, int memoryNeeded)
{
    if ((find(PID, readyQ) != -1) || (find(PID, blockedQ) != -1))
        return 0;
    
    PCB newPCB;
    newPCB.PID = PID;
    newPCB.CPUUsageTerm = 0;
    newPCB.IORequestTerm = 0;
    newPCB.waitingTerm = 0;
    newPCB.memoryNeeded = memoryNeeded;
    
    readyQ.push_back(newPCB);
    
    return 1;
}

int PCBHandler::deletePCB(int PID)
{
    int index = find(PID, readyQ);
    
    if (index != -1)
    {
        readyQ.erase(readyQ.begin() + index);
        return 1;
    }
    
    index = find(PID, blockedQ);
    
    if (index != -1)
    {
        blockedQ.erase(blockedQ.begin() + index);
        return 1;
    }
    
    return 0;
}

int PCBHandler::blockPCB(int PID)
{
    int index = find(PID, readyQ);
    
    if (index == -1)
        return 0;
    
    blockedQ.push_back(readyQ[index]);
    readyQ.erase(readyQ.begin() + index);
    
    return 1;
}

int PCBHandler::unblockPCB(int PID)
{
    int index = find(PID, blockedQ);
    
    if (index == -1)
        return 0;
    
    readyQ.push_back(blockedQ[index]);
    blockedQ.erase(blockedQ.begin() + index);
    
    return 1;
}

void PCBHandler::showReady()                                                        // Try to fix formatting problem
{
    cout << "Ready Queue" << endl;
    for (int i = 0; i < readyQ.size(); i++)
    {
        cout <<
        readyQ[i].PID << "\t" <<
        readyQ[i].CPUUsageTerm << "\t" <<
        readyQ[i].IORequestTerm << "\t" <<
        readyQ[i].waitingTerm << "\t" <<
        readyQ[i].memoryNeeded << "\t"
        << endl;
    }
}

void PCBHandler::showBlocked()
{
    cout << "Blocked Queue" << endl;
    for (int i = 0; i < blockedQ.size(); i++)
    {
        cout <<
        blockedQ[i].PID << "\t" <<
        blockedQ[i].CPUUsageTerm << "\t" <<
        blockedQ[i].IORequestTerm << "\t" <<
        blockedQ[i].waitingTerm << "\t" <<
        blockedQ[i].memoryNeeded << "\t"
        << endl;
    }
}

void PCBHandler::showAllPCBs()
{

}

void PCBHandler::generatePCBs(int amount)
{
    srand(time(NULL));
    
    int count = 0;
    int PCBadded = 0;
    
    while (count != amount)
    {
        PCBadded = createPCB(rand() % 10 + 1, rand() % 10 + 1);
        if (PCBadded != 0)
            count++;
    }
}

int PCBHandler::find(int PID, deque<PCB> q)
{
    int index = -1;
    
    for (int i = 0; i < q.size(); i++)
    {
        if (q[i].PID == PID)
        {
            index = i;
            return index;
        }
    }
    return index;
}
