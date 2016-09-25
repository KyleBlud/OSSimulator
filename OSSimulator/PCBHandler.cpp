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
    if (memoryNeeded > MAX_MEMORY)
        return 0;
    
    PCB newPCB;
    newPCB.PID = PID;
    newPCB.CPUUsageTerm = 0;
    newPCB.IORequestTerm = 0;
    newPCB.waitingTerm = 0;
    newPCB.memoryNeeded = memoryNeeded;
    newPCB.timeEnteredBlockedQ = 0;
    newPCB.eventTypeRequested = 0;
    
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

int PCBHandler::showPCB(int PID)
{
    int index1 = find(PID, readyQ);
    int index2 = find(PID, blockedQ);
    PCB pcb;
    
    if (index1 != -1)
        pcb = readyQ[index1];
    else if (index2 != -1)
        pcb = blockedQ[index2];
    else if (PID == CPU.PID)
        pcb = CPU;

    cout <<
    pcb.PID << "\t" <<
    pcb.CPUUsageTerm << "\t" <<
    pcb.IORequestTerm << "\t" <<
    pcb.waitingTerm << "\t" <<
    pcb.memoryNeeded << "\t" <<
    pcb.eventTypeRequested << "\t" <<
    pcb.timeEnteredBlockedQ << "\t"
    << endl;
    
    return 0;
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
    cout << qName << " Queue" << endl;
    for (int i = 0; i < q.size(); i++)
    {
        cout <<
        q[i].PID << "\t" <<
        q[i].CPUUsageTerm << "\t" <<
        q[i].IORequestTerm << "\t" <<
        q[i].waitingTerm << "\t" <<
        q[i].memoryNeeded << "\t" <<
        q[i].eventTypeRequested << "\t" <<
        q[i].timeEnteredBlockedQ << "\t"
        << endl;
    }
}

void PCBHandler::generatePCBs(int amount)
{
    srand(time(NULL));
    
    int count = 0;
    int PCBadded = 0;
    
    while (count != amount)
    {
        PCBadded = createPCB(rand() % 999 + 1, rand() % MAX_MEMORY + 1);
        if (PCBadded)
            count++;
    }
}

void PCBHandler::execute()
{
    srand(time(NULL));
    
    int processingTime = 0;
    int timecycle = 0;
    int randomEvent = 0;
    int performAction = 0;
    bool CPUOccupied;
    
    while (!readyQ.empty() || !blockedQ.empty())
    {
        processingTime += rand() % 10001; // 0 to 10,000
        clearCPU();
        CPUOccupied = false;
        if (!readyQ.empty())
        {
            CPU = readyQ.front();
            CPUOccupied = true;
            readyQ.pop_front();
            CPU.CPUUsageTerm = processingTime;
            updateAllWaitingTerms(processingTime);
        }
        while (timecycle != processingTime)
        {
            if (timecycle % 10 == 0)
            {
                randomEvent = rand() % 11; // 0 to 10
                if (randomEvent == USER)
                    addEventToQ(USER, timecycle);
                else if (randomEvent == HARD_DRIVE)
                    addEventToQ(HARD_DRIVE, timecycle);
            }
            timecycle++;
        }
        performAction = rand() % 4; // 0 to 3
        if (performAction == 0)
        {
            if (CPUOccupied)
                showPCB(CPU.PID);
        }
        else if (performAction == 1)
        {
            if (CPUOccupied)
                readyQ.push_back(CPU);
        }
        else if (performAction == 2)
        {
            CPU.timeEnteredBlockedQ = timecycle;
            CPU.eventTypeRequested = USER;
            if (CPUOccupied)
                blockedQ.push_back(CPU);
        }
        else if (performAction == 3)
        {
            CPU.timeEnteredBlockedQ = timecycle;
            CPU.eventTypeRequested = HARD_DRIVE;
            if (CPUOccupied)
                blockedQ.push_back(CPU);
        }
        if (!blockedQ.empty())
        {
            for (int i = 0; i < IOEventQ.size(); i++)
            {
                if (blockedQ[0].timeEnteredBlockedQ > IOEventQ[i].timeCycleStamp)
                {
                    IOEventQ.pop_front();
                    i--;
                }
            }
        }
        for (int i = 0; i < IOEventQ.size(); i++)
        {
            for (int j = 0; j < blockedQ.size(); j++)
            {
                if (blockedQ[j].timeEnteredBlockedQ <= IOEventQ[i].timeCycleStamp)
                {
                    if (blockedQ[j].eventTypeRequested == IOEventQ[i].eventType)
                        unblockPCB(blockedQ[j].PID);
                        break;
                }
            }
        }
    }
}

void PCBHandler::addEventToQ(int eventType, int timecycle)
{
    IOEvent event;
    event.eventType = eventType;
    event.timeCycleStamp = timecycle;
    IOEventQ.push_back(event);
}

void PCBHandler::clearCPU()
{
    CPU.PID = 0;
    CPU.CPUUsageTerm = 0;
    CPU.IORequestTerm = 0;
    CPU.waitingTerm = 0;
    CPU.memoryNeeded = 0;
    CPU.eventTypeRequested = 0;
    CPU.timeEnteredBlockedQ = 0;
}

void PCBHandler::updateAllWaitingTerms(int waitingTerm)
{
    for (int i = 0; i < readyQ.size(); i++)
        readyQ[i].waitingTerm = waitingTerm;
    for (int i = 0; i < blockedQ.size(); i++)
        blockedQ[i].waitingTerm = waitingTerm;
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
