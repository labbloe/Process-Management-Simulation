#include "schedulers.h"

void quickSort(int arr[], int low, int high); 
int partition (int arr[], int low, int high);

void swap(int* a, int* b)  
{  
    int t = *a;  
    *a = *b;  
    *b = t;  
}  

/* The main function that implements QuickSort  
arr[] --> Array to be sorted,  
low --> Starting index,  
high --> Ending index */
void quickSort(int arr[], int low, int high)  
{  
    if (low < high)  
    {  
        /* pi is partitioning index, arr[p] is now  
        at right place */
        int pi = partition(arr, low, high);  
  
        // Separately sort elements before  
        // partition and after partition  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}  

/* This function takes last element as pivot, places  
the pivot element at its correct position in sorted  
array, and places all smaller (smaller than pivot)  
to left of pivot and all greater elements to right  
of pivot */
int partition (int arr[], int low, int high)  
{  
    int pivot = arr[high]; // pivot  
    int i = (low - 1); // Index of smaller element  
  
    for (int j = low; j <= high - 1; j++)  
    {  
        // If current element is smaller than the pivot  
        if (arr[j] < pivot)  
        {  
            i++; // increment index of smaller element  
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  


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
    static deque<int> ready; //keeps track of the processes that are ready to be scheduled
    int idx = -1;
    //add to queue on arrival
    for(unsigned int i = 0; i < procList.size(); i++)
        if(procList[i].startTime == curTime)
            ready.push_back(i);
    //remove done and resort
    if((procList[ready[0]].isDone) || (curTime == 0))
    {
        if(curTime != 0)
            ready.pop_front();
        for(unsigned int i = 0; i < ready.size() - 1; i++) //bubble sort to order queue
            for(unsigned int j = 0; j < ready.size() - i - 1; j++)
                if(procList[ready[j]].totalTimeNeeded > procList[ready[j + 1]].totalTimeNeeded)
                    swap(ready[j], ready[j + 1]);
        if (ready.size() > 1) //if same swap by arrival time
            if (procList[ready[0]].totalTimeNeeded == procList[ready[1]].totalTimeNeeded)
                if (procList[ready[0]].startTime > procList[ready[1]].startTime)
                    swap(ready[0], ready[1]);
    }

        
    if(ready.size() > 0)
        idx = ready[0];

    return idx;
}

//Shortest Remaining Time scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the process with the shortest remaining execution time left. This algorithm is preemptive.
int ShortestRemainingTime(const int& curTime,const vector<Process>& procList)
{
    static deque<int> ready; //keeps track of the processes that are ready to be scheduled
    int idx = -1;

    //add to queue on arrival
    for(unsigned int i = 0; i < procList.size(); i++)
        if(procList[i].startTime == curTime)
            ready.push_back(i);
    //remove done and resort
        if(procList[ready[0]].isDone && ready.size())
            ready.pop_front();
        for(unsigned int i = 0; i < ready.size() - 1; i++) //bubble sort to order processlist
            for(unsigned int j = 0; j < ready.size() - i - 1; j++)
                if(procList[ready[j]].totalTimeNeeded - procList[ready[j]].timeScheduled > procList[ready[j + 1]].totalTimeNeeded - procList[ready[j + 1]].timeScheduled)
                    swap(ready[j], ready[j + 1]);
        if (ready.size() > 1) //if same swap by arrival time
            if (procList[ready[0]].totalTimeNeeded - procList[ready[0]].timeScheduled == procList[ready[1]].totalTimeNeeded - procList[ready[1]].timeScheduled)
                if (procList[ready[0]].startTime > procList[ready[1]].startTime)
                    swap(ready[0], ready[1]);
        
    if(ready.size() > 0)
        idx = ready[0];

    return idx;
}

//Highest Response Ratio Next scheduling algorithm. The process with the highest response ratio is ran first.
//represented as (W+S)/S, where W is waiting time and S is burst time (total CPU time needed).
// W = waiting time = curTime - process.startTime
// S = burst time = process.totalTimeNeeded

int HighestResponseRatioNext(const int& curTime,const vector<Process>& procList)
{
    static deque<int> ready; //keeps track of the processes that are ready to be scheduled
    int idx = -1;
   
    //add to queue on arrival
    for(unsigned int i = 0; i < procList.size(); i++)
        if(procList[i].startTime == curTime)
            ready.push_back(i);

    //remove done processes and resort
    if((procList[ready[0]].isDone) || (curTime == 0))
    {
        if(curTime != 0)
            ready.pop_front();
        for(unsigned int i = 0; i < ready.size() - 1; i++) //simple bubble sort
            for(unsigned int j = 0; j < ready.size() - i - 1; j++)
                if(getResponseRatio(curTime, procList[ready[j]]) < getResponseRatio(curTime, procList[ready[j + 1]]))
                    swap(ready[j], ready[j + 1]);
        if (ready.size() > 1)
            if (getResponseRatio(curTime, procList[ready[0]]) == getResponseRatio(curTime, procList[ready[1]])) //if same swap by arrival time
                if (procList[ready[0]].startTime > procList[ready[1]].startTime)
                    swap(ready[0], ready[1]);
    }
    if(ready.size() > 0)
        idx = ready[0];   
    return idx;
}

double getResponseRatio(const int & curTime, const Process & process)
{
    double waitTime = curTime - process.startTime;
    double burstTime = process.totalTimeNeeded;
    return ((waitTime + burstTime) / burstTime);
}

//Modified Highest Response Ratio Next scheduling algorithm. The process with the highest response ratio and priority is
//ran first. Normal HRRN already accounts for indefinite postponement and starvation, but is not normally suitable for
//a priority based system. This algorithm not only accounts for the response ratio, but also the processes's priority
//when scheduling. High priority has a '0' bit, and low priority has a '1' bit.
int Modified_HRRN(const int& curTime,const vector<Process>& procList)
{
    static deque<int> ready; //keeps track of the processes that are ready to be scheduled
    int idx = -1;
   
    //add to queue on arrival
    for(unsigned int i = 0; i < procList.size(); i++)
        if(procList[i].startTime == curTime)
            ready.push_back(i);

    //remove done processes and resort
    if((procList[ready[0]].isDone) || (curTime == 0))
    {
        if(curTime != 0)
            ready.pop_front();
        for(unsigned int i = 0; i < ready.size() - 1; i++) //simple bubble sort
            for(unsigned int j = 0; j < ready.size() - i - 1; j++)
                if(getModifiedResponseRatio(curTime, procList[ready[j]]) < getModifiedResponseRatio(curTime, procList[ready[j + 1]]))
                    swap(ready[j], ready[j + 1]);
        if (ready.size() > 1)
            if (getModifiedResponseRatio(curTime, procList[ready[0]]) == getModifiedResponseRatio(curTime, procList[ready[1]])) //if same swap by arrival time
                if (procList[ready[0]].startTime > procList[ready[1]].startTime)
                    swap(ready[0], ready[1]);
    }
    if(ready.size() > 0)
        idx = ready[0];   
    return idx;

}

//Priority = 0.5 * Priority + 0.5 * Ratio
double getModifiedResponseRatio(const int & curTime, const Process & process)
{
    double waitTime = curTime - process.startTime;
    double burstTime = process.totalTimeNeeded;
    double ratio = ((waitTime + burstTime) / burstTime);
    int priority;
    if(process.priority == 0)
        priority = 1;
    else
        priority = 0;
    return ((0.5 * priority) + (0.5 * ratio));
}

//First in First out scheduling algorithm. Non-preemptive
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

//Multilevel Feedback Queue allows a process to move between queues. This is based on the CPU burst of the process.
//If a process uses too much CPU time, it will be moved to a lower-priority queue.
//Additionally, if a process waits too long in a lower-priority queus, it can be moved to a high-priority queue. 
//This will prevent the problem of starvation that exists in Mulilevel Queue scheduling. 

//Like before, high priority will be represented by the bit '0' and high priority is represented by a '1'
//This is a simplified version as there are multiple levels of priority in a real system.

//For this simulation, a process's CPU burst time will have a random amount of time added to it (by no more than 20)
//This will represent the realistic possibility that a program takes much longer than its estimated burst time.
//The user can give a "Switch Time" for both the high-priority and low-priority queus. This time is another
//time quantum. The seperate switch times allow the user to specify longer wait times before switching queues
//for both high and low priority processes.

//High priority will retain a Round Robin algorithm and low priority will keep a FIFO algorithm for completion.
int MultilevelFeedbackQueue(const int& curTime, vector<Process>& procList,const int& timeQuantum,const int& highQuantum, const int&lowQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready; //queue of process that are ready to be scheduled
    static deque<int> foreground; //queue of foreground processes
    static deque<int> background; //queue of background processes
    bool queueChange = false;     //Marks the need for a queue change based on priority quantums
    srand(time(NULL));
    int idx = -1;

    //add process to queue if the startTime of that process is equal to the current time
    //if the process that is ready has a larger priority (smaller number) than the following process
    //push it to the front of the queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if((procList[i].startTime == curTime) || (queueChange == true))
        {
            if(procList[i].priority == 0)
            {
                if(foreground.size() == 0)
                    foreground.push_back(i);
                else if(procList[i].priority < procList[foreground[0]].priority)
                    foreground.push_front(i);
                else if(procList[i].priority >= procList[foreground[0]].priority)
                    foreground.push_back(i);
                //procList[foreground[0]].totalTimeNeeded += (rand() % 20);
            }
             else
            {
                if(background.size() == 0)
                    background.push_back(i);
                else if(procList[i].priority < procList[background[0]].priority)
                    background.push_front(i);
                else if(procList[i].priority >= procList[background[0]].priority)
                    background.push_back(i);
                //procList[background[0]].totalTimeNeeded += (rand() % 20);
            }
        }
    }
    if(foreground.size() > 0)
    {
        if(!procList[foreground[0]].isDone)
        {
            if((procList[foreground[0]].quantumTime % highQuantum == 0) && (procList[foreground[0]].quantumTime != 0))
            {
                procList[foreground[0]].quantumTime = 0;
                background.push_back(foreground[0]); //move to lower queue
                foreground.pop_front(); //remove from high-priority queue
            }
        }
    }
    if(background.size() > 0)
    {
        if(!procList[background[0]].isDone)
        {
            if((procList[background[0]].quantumTime % lowQuantum == 0) && (procList[background[0]].quantumTime != 0))
            {
                procList[background[0]].quantumTime = 0;
                foreground.push_back(background[0]); //move to higher queue
                background.pop_front(); //remove from low-priority queue
            }
        }
    }

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