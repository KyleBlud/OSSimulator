//
//  PCB.cpp
//  OSSimulator
//
//  Created by Kyle Bludworth on 9/18/16.
//  Copyright © 2016 Kyle Bludworth. All rights reserved.
//

#include "PCBHandler.hpp"

PCBHandler::PCBHandler()
{
    srand(time(NULL));
}

int PCBHandler::createPCB(int PID, int memoryNeeded)
{
    if ((find(PID, readyQ) != -1) || (find(PID, blockedQ) != -1))
        return 0;
    if (memoryNeeded > MAX_MEMORY)
        return 0;
    
    int typeOfPCB = (rand() % 3) + 1;
    
    PCB newPCB;
    newPCB.PID = PID;
    newPCB.CPUUsageTerm = 0;
    newPCB.IORequestTerm = 0;
    newPCB.waitingTerm = 0;
    newPCB.memoryNeeded = memoryNeeded;
    newPCB.blockedTimeStamp = 0;
    newPCB.IORequestType = 0;
    newPCB.interactivity = typeOfPCB;
    newPCB.reference = 0;
    newPCB.priority = 0;
    
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
    
    readyQ[index].IORequestType = USER;
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
    
    printPCBContent(pcb);
    
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
    printPCBTableHeader();
    for (int i = 0; i < q.size(); i++)
    {
        printPCBContent(q[i]);
    }
    std::setw(0);
}

void PCBHandler::printPCBContent(PCB pcb)
{
    string PCBType;
    if (pcb.interactivity == INTERACTIVE)
        PCBType = "Inter.";
    if (pcb.interactivity == CPU_BOUND)
        PCBType = "Bound";
    if (pcb.interactivity == NORMAL)
        PCBType = "Normal";
    
    cout <<
    pcb.PID << std::setw(SPACE_PADDING) <<
    pcb.CPUUsageTerm << std::setw(SPACE_PADDING) <<
    pcb.IORequestTerm << std::setw(SPACE_PADDING) <<
    pcb.waitingTerm << std::setw(SPACE_PADDING) <<
    pcb.memoryNeeded << std::setw(SPACE_PADDING) <<
    PCBType
    << endl;
}

void PCBHandler::printPCBTableHeader()
{
    cout <<
    "PID" << std::setw(SPACE_PADDING) <<
    "CPU_T" << std::setw(SPACE_PADDING) <<
    "IO_R_T" << std::setw(SPACE_PADDING) <<
    "W_T" << std::setw(SPACE_PADDING) <<
    "M_N" << std::setw(SPACE_PADDING) <<
    "PCB_Type"
    << endl;
}

void PCBHandler::generatePCBs(int amount)
{
    int count = 0;
    int PCBadded = 0;
    
    while (count != amount)
    {
        PCBadded = createPCB((rand() % 899) + 100, rand() % MAX_MEMORY + 1);
        if (PCBadded)
            count++;
    }
}

void PCBHandler::execute(bool MLFQ, int highestPriority,
                         bool roundRobin, int timeSlice)
{
    unsigned long int totalPCBs = readyQ.size() + blockedQ.size();
    int turnaroundTime = 0;
    int responseTime = 0;
    double av_turnaround = 0;
    double av_response = 0;
    int currTime = 0;
    int totalTime = 0;
    int CPUUsageTerm = 0;
    int boostTime = 1000;
    bool OSControlOfCPU;
    bool processCompleted;
    if (MLFQ)
    {
        this->highestPriority = highestPriority;
        boostPriorityLevels();
    }
    
    printPCBTableHeader();
    while (!readyQ.empty() || !blockedQ.empty())
    {
        totalTime += ((roundRobin) ? timeSlice : (rand() % 10001) + 1);
        OSControlOfCPU = true;
        if (MLFQ && totalTime >= boostTime)
            boostPriorityLevels();
        if (!readyQ.empty())
        {
            CPUUsageTerm = (totalTime - currTime) + CONTEXT_SWITCH_COST;
            contextSwitch(MLFQ, CPUUsageTerm);
            totalTime += CONTEXT_SWITCH_COST;
            if (CPU.reference == 0)
            {
                responseTime += totalTime;
                CPU.reference = 1;
            }
            OSControlOfCPU = false;
        }
        while (currTime != totalTime)
        {
            detectIOEvents(currTime);
            currTime++;
        }
        if (!OSControlOfCPU)
        {
            processCompleted = scheduler(MLFQ, totalTime);
            if (processCompleted)
                turnaroundTime += totalTime;
        }
        if (!blockedQ.empty())
        {
            cleanIOEventQ();
            findIOEventForBlockedPCB(totalTime);
        }
        clearCPU();
    }
    
    av_turnaround = turnaroundTime / totalPCBs;
    av_response = responseTime / totalPCBs;
    
    cout << "Average turnaround time: " << av_turnaround << endl;
    cout << "Average response time: " << av_response << endl;
}

void PCBHandler::executeAllPolicies(int timeSlice, int priorityLevels)
{
    deque<PCB> temp_ready = readyQ;
    deque<PCB> temp_blocked = blockedQ;
    
    cout << "Normal:" << endl;
    execute(false, 0, false, 0);
    
    readyQ = temp_ready;
    blockedQ = temp_blocked;
    IOEventQ.clear();
    
    cout << "\nRound Robin:" << endl;
    execute(false, 0, true, timeSlice);
    
    readyQ = temp_ready;
    blockedQ = temp_blocked;
    IOEventQ.clear();
    
    cout << "\nMLFQ:" << endl;
    execute(true, priorityLevels, true, 100);
}

void PCBHandler::clearCPU()
{
    CPU.PID = 0;
    CPU.CPUUsageTerm = 0;
    CPU.IORequestTerm = 0;
    CPU.waitingTerm = 0;
    CPU.memoryNeeded = 0;
    CPU.blockedTimeStamp = 0;
    CPU.IORequestType = 0;
    CPU.interactivity = 0;
    CPU.reference = 0;
    CPU.priority = 0;
}

void PCBHandler::boostPriorityLevels()
{
    for (int i = 0; i < readyQ.size(); i++)
        readyQ[i].priority = highestPriority;
    for (int i = 0; i < blockedQ.size(); i++)
        blockedQ[i].priority = highestPriority;
}

void PCBHandler::contextSwitch(bool MLFQ, int timeInCPU)
{
    if (MLFQ)
        CPU = findPCBWithHighestPriority();
    else
    {
        CPU = readyQ.front();
        readyQ.pop_front();
    }
    CPU.CPUUsageTerm += timeInCPU;
    updateAllWaitingTerms(timeInCPU);
}

PCB PCBHandler::findPCBWithHighestPriority()
{
    int index = 0;
    int currHighestPriority = 0;
    PCB winner;
    for (int i = 0; i < readyQ.size(); i++)
    {
        if (currHighestPriority < readyQ[i].priority)
        {
            currHighestPriority = readyQ[i].priority;
            index = i;
        }
    }
    winner = readyQ.at(index);
    readyQ.erase(readyQ.begin() + index);
    
    return winner;
}

void PCBHandler::updateAllWaitingTerms(int waitingTerm)
{
    for (int i = 0; i < readyQ.size(); i++)
        readyQ[i].waitingTerm += waitingTerm;
    for (int i = 0; i < blockedQ.size(); i++)
        blockedQ[i].waitingTerm += waitingTerm;
}

void PCBHandler::addEventToQ(int eventType, int timecycle)
{
    IOEvent event;
    event.eventType = eventType;
    event.timeStamp = timecycle;
    IOEventQ.push_back(event);
}

void PCBHandler::detectIOEvents(int currTime)
{
    int randEvent;
    if (currTime % 10 == 0)
    {
        randEvent = rand() % 11; // 0 to 10
        if (randEvent == USER)
            addEventToQ(USER, currTime);
        else if (randEvent == HARD_DRIVE)
            addEventToQ(HARD_DRIVE, currTime);
    }
}

bool PCBHandler::scheduler(bool MLFQ, int totalTime)
{
    int randAction = (rand() % 100) + 1;
    int firstBound = 0;
    int secondBound = 0;
    int thirdBound = 0;
    
    if (CPU.interactivity == INTERACTIVE)
    {
        firstBound = 8;
        secondBound = 16;
        thirdBound = 91;
    }
    else if (CPU.interactivity == CPU_BOUND)
    {
        firstBound = 8;
        secondBound = 83;
        thirdBound = 91;
    }
    else if (CPU.interactivity == NORMAL)
    {
        firstBound = 25;
        secondBound = 50;
        thirdBound = 75;
    }
    
    if (randAction > 0 && randAction <= firstBound)
    {
        showPCB(CPU.PID);
        return true;
    }
    else if (randAction > firstBound && randAction <= secondBound)
    {
        if (MLFQ)
        {
            if (CPU.priority != 0)
                CPU.priority -= 1;
        }
        readyQ.push_back(CPU);
    }
    else if (randAction > secondBound && randAction <= thirdBound)
    {
        CPU.blockedTimeStamp = totalTime;
        CPU.IORequestType = USER;
        if (MLFQ)
        {
            if (CPU.priority != 0)
                CPU.priority -= 1;
        }
        blockedQ.push_back(CPU);
    }
    else if (randAction > thirdBound && randAction <= 100)
    {
        CPU.blockedTimeStamp = totalTime;
        CPU.IORequestType = HARD_DRIVE;
        if (MLFQ)
        {
            if (CPU.priority != 0)
                CPU.priority -= 1;
        }
        blockedQ.push_back(CPU);
    }
    return false;
}

void PCBHandler::cleanIOEventQ()
{
    for (int i = 0; i < IOEventQ.size(); i++)
    {
        if (IOEventQ[i].timeStamp < blockedQ[0].blockedTimeStamp)
        {
            IOEventQ.pop_front();
            i--;
        }
        else break;
    }
}

void PCBHandler::findIOEventForBlockedPCB(int totalTime)
{
    for (int i = 0; i < blockedQ.size(); i++)
    {
        for (int j = 0; j < IOEventQ.size(); j++)
        {
            if (blockedQ[i].blockedTimeStamp <= IOEventQ[j].timeStamp)
            {
                if (blockedQ[i].IORequestType == IOEventQ[j].eventType)
                {
                    blockedQ[i].IORequestTerm =
                    totalTime - blockedQ[i].blockedTimeStamp;
                    unblockPCB(blockedQ[i].PID);
                    i--;
                    break;
                }
            }
        }
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
