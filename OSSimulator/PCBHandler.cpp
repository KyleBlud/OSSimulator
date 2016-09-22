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

void PCBHandler::showAllPCBs()
{
    showQueue(readyQ, "Ready");
    showQueue(blockedQ, "Blocked");
}

void PCBHandler::showReady()
{
    showQueue(readyQ, "Ready");
}

void PCBHandler::showBlocked()
{
    showQueue(blockedQ, "Blocked");
}

void PCBHandler::showQueue(deque<PCB> q, string qName)
{
    std::vector<int> sortedPIDs;
    
    for (int i = 0; i < q.size(); i++)
        sortedPIDs.push_back(q[i].PID);
    
    std::sort(sortedPIDs.begin(), sortedPIDs.end());
    
    cout << qName << " Queue" << endl;
    for (int i = 0; i < sortedPIDs.size(); i++)
    {
        for (int j = 0; j < q.size(); j++)
        {
            if (sortedPIDs[i] == q[j].PID)
            {
                cout <<
                q[j].PID << "\t" <<
                q[j].CPUUsageTerm << "\t" <<
                q[j].IORequestTerm << "\t" <<
                q[j].waitingTerm << "\t" <<
                q[j].memoryNeeded << "\t"
                << endl;
            }
        }
    }
}

void PCBHandler::generatePCBs(int amount)
{
    srand(time(NULL));
    
    int count = 0;
    int PCBadded = 0;
    
    while (count != amount)
    {
        PCBadded = createPCB(rand() % 10 + 1, rand() % 10 + 1);
        if (PCBadded)
            count++;
    }
}

void PCBHandler::execute()
{
    
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
