#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include<vector>  //process vector
#include<deque>   //for ready double ended queue
#include<fstream>  // file i/o
#include<iostream> // cerr

using namespace std;

struct Process
{
    Process() : isDone(false), totalTimeNeeded(-1), timeScheduled(0), startTime(-1), timeFinished(-1) {}

    // Given data
    string id;            //The process id
    int startTime;        //The time at which the process becomes available for scheduling
    int totalTimeNeeded;  //The total amount of time needed by the process
    int priority;         //0-9: 0-4 (high priority foreground process) 5-9 (lower priority background process)

    // Process details
    bool isDone;          //Indicates if the process is complete
    int timeScheduled;    //The amount of time the process has been scheduled so far
    int timeFinished;     //The time that the process completed
};


inline void readInProcList(const string& fname, vector<Process>& procList)
{
    ifstream in(fname.c_str());
    int numProcs;

    if(in.fail())
    {
        cerr << "Unable to open file \"" << fname << "\", terminating" << endl;
        exit(-1);
    }

    in >> numProcs;
    procList.resize(numProcs);
    for(auto& p:procList)
    {
        in >> p.id >> p.startTime >> p.totalTimeNeeded >> p.priority;
    }
    in.close();
}

//given
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum);

//shortest job first algorithm
//non-preemptive: allows a running process to continue until the process terminates or blocks on a resource
int ShortestProcessNext(const int& curTime, const vector<Process>& procList); 

//Shortest remaining time algorithm
//Preemptive: allows a running process to be interupted if a new process becomes 'ready' with a shorter processing time
int ShortestRemainingTime(const int& curTime,const vector<Process>& procList);

//Highest response ratio next algorithm
//non-preemptive: allows a running process to continue until the process terminates or blocks on a resource
int HighestResponseRatioNext(const int& curTime,const vector<Process>& procList);

//returns float representing the response ratio of the given process
float responseRatio(const int& curTime,const Process& proc);

//Modified Highest response ratio next algorithm
//preemptive
int Modified_HRRN(const int& curTime,const vector<Process>& procList);

//First in First Out scheduling algorithm
//non-preemptive
int FIFO(const int& curTime, const vector<Process>& procList);

//Multilevel Queue scheduling algorithm
int MultilevelQueue(const int& curTime, const vector<Process>& procList, const int& timeQuantum);

//Multilevel Feedback Queue scheduling algorithm
int MultilevelFeedbackQueue(const int& curTime, const vector<Process>& procList, const int& timeQuantum);

#endif