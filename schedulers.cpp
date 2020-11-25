#include "schedulers.h"

//Round Robin scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the first process on that list, if available (i.e., if the list has members)
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // if the process isn't done, add it to the back of the ready queue
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}

//Shortest Process Next scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the shortest process next in the queue. This algorithm in non-preemptive.
int ShortestProcessNext(const int& curTime, const vector<Process>& procList)
{
    static deque<int> ready; //queue of process that are ready to be scheduled
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
            //cout<<"size: "<<ready.size()<<"\n";
        }
        
    }

    //If current process is done remove from queue, and select next process with shortest time needed
    if(procList[ready[0]].isDone)
    {
        ready.pop_front();
        for(int i=1, i_end = ready.size(); i < i_end; ++i)
        {
            if(ready.size() == 0)
                break;
            //re-organize queue in order of shortest time needed
            if(procList[ready[0]].totalTimeNeeded > procList[ready[i]].totalTimeNeeded)
            {
                int temp2 = ready[0];
                ready.pop_front();
                ready.push_back(temp2);
                i = 0;
            }
            //If two processes have the same processing time, then the process with 
            //the earlier startTime should be ordered first in the queue
            if(procList[ready[0]].totalTimeNeeded == procList[ready[i]].totalTimeNeeded)
            {
                if(procList[ready[0]].startTime > procList[ready[i]].startTime)
                {
                    int temp2 = ready[0];
                    ready.pop_front();
                    ready.push_back(temp2);
                    i = 0;
                }
            }
            /*for(int j=0; j< ready.size(); ++j)
                cout<<procList[ready[j]].id<<" ";
            cout<<"\n";*/
        }
    }
        
    if(ready.size() > 0)
        idx = ready[0];         //set idx to next process

    // if the ready queue has no processes on it send back invalid index to represent empty queue
    else
        idx = -1;   
    
    //cout<<"idx: "<<idx<<"\n";

    return idx;
}

//Shortest Remaining Time scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the process with the shortest remaining execution time left. This algorithm is preemptive.
int ShortestRemainingTime(const int& curTime,const vector<Process>& procList)
{
    static deque<int> ready; //queue of process that are ready to be scheduled
    int idx = -1;

    if(ready.size() > 0)
    {
        if(procList[ready[0]].isDone)
            ready.pop_front();
    }

    //add process to queue if the startTime of that process is equal to the current time
    //if the process that is ready has a shorter processing time than the current process
    //push it to the front of the queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            if(ready.size() == 0)
                ready.push_back(i);
            else if(procList[i].totalTimeNeeded < (procList[ready[0]].totalTimeNeeded - procList[ready[0]].timeScheduled))
                ready.push_front(i);
            else if(procList[i].totalTimeNeeded >= (procList[ready[0]].totalTimeNeeded - procList[ready[0]].timeScheduled))
                ready.push_back(i);

            //cout<<"size: "<<ready.size()<<"\n";

        }
        
    }
                    /*for(int j=0; j< ready.size(); ++j)
                cout<<procList[ready[j]].id<<" ";
            cout<<"\n";*/

    //This loop re-organizes the queue. The loop above accounts for pre-emptive functionality, but does
    //not ensure correct queue order.
    for(int i=1, i_end = ready.size(); i < i_end; ++i)
    {
        if(ready.size() == 0)
            break;
        //re-organize queue in order of shortest time needed/ remaining
        if((procList[ready[0]].totalTimeNeeded - procList[ready[0]].timeScheduled) > procList[ready[i]].totalTimeNeeded)
        {
            //cout<<"HERE\n";
            int temp = ready[0];
            ready.pop_front();
            ready.push_back(temp);
            i = 0;
        }
        //If two processes have the same processing time remaining, then the process with 
        //the earlier startTime should be ordered first in the queue
        if((procList[ready[0]].totalTimeNeeded - procList[ready[0]].timeScheduled) == procList[ready[i]].totalTimeNeeded)
        {
            if(procList[ready[0]].startTime > procList[ready[i]].startTime)
            {
                int temp2 = ready[0];
                ready.pop_front();
                ready.push_back(temp2);
                i = 0;
            }
        }
    }
    if(ready.size() > 0)
        idx = ready[0];         //set idx to next process

    // if the ready queue has no processes on it send back invalid index to represent empty queue
    else
        idx = -1;   

    return idx;
}

//Highest Response Ratio Next scheduling algorithm. The process with the highest response ratio is ran first.
//represented as (W+S)/S, where W is waiting time and S is burst time (total CPU time needed).
// W = waiting time = curTime - process.startTime
// S = burst time = process.totalTimeNeeded

int HighestResponseRatioNext(const int& curTime,const vector<Process>& procList)
{
    static deque<int> ready; //queue of process that are ready to be scheduled
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
            //cout<<"size: "<<ready.size()<<"\n";
        }
        
    }

    //organize processes by response ratio in the queue
    if(procList[ready[0]].isDone)
    {
        ready.pop_front();
        for(int i=1, i_end = ready.size(); i < i_end; ++i)
        {
            if(ready.size() == 0) //prevent core dump
                break;

            //cout<<"ID: "<<procList[ready[i]].id<<"  curTime: "<<curTime;
            //cout<<"  RR: "<<responseRatio(curTime,procList[ready[i]])<<"      ";
            //sort by highest response ratio
            if(responseRatio(curTime,procList[ready[0]]) < responseRatio(curTime,procList[ready[i]]))
            {
                //cout<<"HERE\n";
                int temp = ready[0];
                ready.pop_front();
                ready.push_back(temp);
                i = 0;
            }
            //If two processes have the same response ratio
            //the earlier startTime should be ordered first in the queue
            if(responseRatio(curTime,procList[ready[0]]) == responseRatio(curTime,procList[ready[i]]))
            {
                if(procList[ready[0]].startTime > procList[ready[i]].startTime)
                {
                    int temp2 = ready[0];
                    ready.pop_front();
                    ready.push_back(temp2);
                    i = 0;
                }
            }
                                /*for(int j=0; j< ready.size(); ++j)
                cout<<procList[ready[j]].id<<" ";
            cout<<"\n";*/
        }
    }


    if(ready.size() > 0)
        idx = ready[0];         //set idx to next process

    // if the ready queue has no processes on it send back invalid index to represent empty queue
    else
        idx = -1;   

    return idx;
}

float responseRatio(const int& curTime,const Process& proc)
{
    float waitTime = curTime - proc.startTime;
    float burstTime = proc.totalTimeNeeded;

    float respRatio = (waitTime + burstTime) / burstTime;
    return respRatio;
}

int Modified_HRRN(const int& curTime,const vector<Process>& procList)
{
    static deque<int> ready; //queue of process that are ready to be scheduled
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
            //cout<<"size: "<<ready.size()<<"\n";
        }
        
    }

    if(ready.size() > 0)
        idx = ready[0];         //set idx to next process

    // if the ready queue has no processes on it send back invalid index to represent empty queue
    else
        idx = -1;   

    return idx;

}

int FIFO(const int& curTime, const vector<Process>& procList)
{
    static deque<int> ready; //queue of process that are ready to be scheduled
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
            //cout<<"size: "<<ready.size()<<"\n";
        }
        
    }

    if(ready.size() > 0)
    {
        if(procList[ready[0]].isDone)
            ready.pop_front();
        idx = ready[0];         //set idx to next process
    }
    // if the ready queue has no processes on it send back invalid index to represent empty queue
    else
        idx = -1;   

    return idx;    
}

//Multilevel Queue scheduling algorithm partitions the reqdy queue into several separate queues based
//on some priority such as memory size, process priority, or process type.
//Each queue will have its own scheduling algorithm

//In this process, each process is given a priority number
//We will call priority number 0 (High Priority) foreground processes (System & Interactive Processes)
//This is due to foreground processes requiring fast response times for good user experience
//Priority numbers 1 (Low Priority) will be called background processes (Batch & Student Processes)

//High priority foreground processes will be scheduled using round robin
//Low priority background processes will be scheduled using FIFO

//All processes with a higher priority number will preempt any running process with 
//a lower priority. Note, this does leave the possibility for Starvation!
int MultilevelQueue(const int& curTime, const vector<Process>& procList,const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready; //queue of process that are ready to be scheduled
    static deque<int> foreground; //queue of foreground processes
    static deque<int> background; //queue of background processes
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    //if the process that is ready has a larger priority (smaller number) than the following process
    //push it to the front of the queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            if(procList[i].priority == 0)
            {
                if(foreground.size() == 0)
                    foreground.push_back(i);
                else if(procList[i].priority < procList[foreground[0]].priority)
                    foreground.push_front(i);
                else if(procList[i].priority >= procList[foreground[0]].priority)
                    foreground.push_back(i);
            }
             else
            {
                if(background.size() == 0)
                    background.push_back(i);
                else if(procList[i].priority < procList[background[0]].priority)
                    background.push_front(i);
                else if(procList[i].priority >= procList[background[0]].priority)
                    background.push_back(i);
            }
        }
    }/*
                                    for(int j=0; j< foreground.size(); ++j)
                cout<<procList[foreground[j]].id<<" ";
            cout<<"\n";
                                for(int j=0; j< background.size(); ++j)
                cout<<procList[background[j]].id<<" ";
            cout<<"\n";*/

    //FOREGROUND PROCESSES (HIGH PRIORITY ROUND ROBIN ALGORITHM)
    if(foreground.size() > 0)
    {
        if((timeToNextSched == 0) || (procList[foreground[0]].isDone))
        {
            if(!procList[foreground[0]].isDone)
            {
                foreground.push_back(foreground[0]);
            }
            foreground.pop_front();
            timeToNextSched = timeQuantum;
        }
        // if the ready queue has any processes on it
        if(foreground.size() > 0)
        {
            // grab the front process and decrement the time to next scheduling
            idx = foreground[0];
            --timeToNextSched;
            return idx;
        }
        // if the ready queue has no processes on it
        else
        {
            // send back an invalid process index and set the time to next scheduling
            // value so that we try again next time step
            idx = -1;
            timeToNextSched = 0;
            if(background.size() > 0)
                idx = background[0];
        }
    }

    //BACKGROUND PROCESSES (LOW PRIORITY FIRST IN FIRST OUT ALGORITHM)
    else
    {
        if(procList[background[0]].isDone)
            background.pop_front();
        idx = background[0];         //set idx to next process       
    }

    if(background.size() == 0 && foreground.size() == 0)
        idx = -1;

    return idx;       
}

int MultilevelFeedbackQueue(const int& curTime, const vector<Process>& procList,const int& timeQuantum)
{
    static deque<int> ready; //queue of process that are ready to be scheduled
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
            //cout<<"size: "<<ready.size()<<"\n";
        }
        
    }

    if(ready.size() > 0)
    {
        if(procList[ready[0]].isDone)
            ready.pop_front();
        idx = ready[0];         //set idx to next process
    }
    // if the ready queue has no processes on it send back invalid index to represent empty queue
    else
        idx = -1;   

    return idx;      
}