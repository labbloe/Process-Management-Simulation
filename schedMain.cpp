/*  Process Memory Management Simulation
    Jared Allen & Gabe Gilley
*/

#include<vector>   // process vector
#include<iostream> // cout, cin
#include<chrono>   // sleep duration
#include<thread>   // sleep_for
#include<iomanip>  // setw 
#include<stdio.h>
#include<fstream>
#include "schedulers.h"    

using namespace std::chrono;
using std::cout;
using std::cerr;
using std::cin;

//constant for number of processes to run
const int n = 10;

int main(int argc, char* argv[])
{
    vector<int>timeList;
    string fname;
    chrono::duration<int, std::milli> sleepTime = chrono::milliseconds(500);
    vector<Process> procList;
    int curTime = 0, input, schedChoice, numProc, procIdx, timeQuantum, highQuantum, lowQuantum;
    bool inputGiven = false;
    bool done;
    string tempStr;
    srand(time(NULL));

    //Default to process list simulation. See procList.txt for process example setup.
    if(argc == 1)
    {
        fname = "procList.txt";
    }
    // gave a file name but not sleep time
    else if(argc == 2)
    {
        fname = argv[1];
    }
    else
    {
        fname = argv[1];
        input = stoi(argv[2]);
        inputGiven = true;
    }
    
    //read in the process list and store the total number of processes
   // readInProcList(fname, procList);

    cout << "\nWelcome to the Process Memory Management Schedular Simulator!\n\n";

    cout << "What scheduler would you like to test?\n\n"
        << "1) Round Robin\n"
        << "2) Shortest Process Next\n"
        << "3) Shortest Remaining Time\n"
        << "4) Highest Response Ratio Next\n"
        << "5) Modified Highest Response Ratio next\n"
        << "6) First In First Out\n"
        << "7) Multilvel Queue\n"
        << "8) Multilevel Feedback Queue\n";
    cout << "\n--> ";
    if(inputGiven == false)
        cin >> input;
    if(input > 0 && input <= 8)
    {
        schedChoice = input;
    }
    else
    {
        cout << "INVALID ENTRY\n\n";
    }

    
    //if the scheduler selected needs a time quantum, ask for it
    if((schedChoice == 1) || (schedChoice >= 7))
    {
        cout << "Enter the time quantum you would like to use: ";
        //cin >> timeQuantum;
        timeQuantum = 5;
    }
    if(schedChoice == 8)
    {
        cout << "Enter the high-priority switch time quantum: ";
        cin >> highQuantum;
        cout << "Enter the low-priority switch time quantum: ";
        cin >> lowQuantum;
    }

    //ofstream f;
   // for (int j = 0; j < 5; j++)
    //{
      //  this_thread::sleep_for(sleepTime);
        curTime = 0;
        procIdx = -1;
        /*done = false;
        remove("procList.txt"); //should delete procList.txt
        f.open("procList.txt", fstream::out); //should create a brand new procList.txt
        f << n << endl; //should write a number of n processes and a new line
        int arrivalTime = 0;
        int temp = 0;
        int totalTime = rand() % 10 + 1;
        f << "p" << 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2 << endl;
        for (int i = 1; i < n; i++)
        {
            do
            {
                temp = arrivalTime;
                arrivalTime = rand() % (temp + totalTime);
                if (arrivalTime < temp || arrivalTime > n)
                {
                    arrivalTime = temp;
                }
            } while (arrivalTime == temp && temp != n);
            totalTime = rand() % 10 + 1;
            f << "p" << i + 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2; //will add processName (pi), arrival time, and length
            if (i < n - 1)
                f << endl;
        }
        f.close();*/
        readInProcList(fname, procList);
        numProc = procList.size();

        //output the header for the run table
        cout << "\nStarting simulation\n"
            << "   O: Process scheduled\n"
            << "   X: Process completed\n"
            << "   !: Completed process scheduled more time than needed\n\n"
            << "Time ";
        tempStr = "-----";
        for(int i = 0; i < numProc; ++i)
        {
            cout << "| " << setw(4) << procList[i].id.substr(0,4) << " ";
            tempStr += "-------";
        }
        cout << "| IDLE |\n" << tempStr << "--------\n";

        auto start = high_resolution_clock::now();
        //while not all processes have completed:
       /* for(int i=0; i<numProc; i++)
        {
            cout<<procList[i].id << " " << procList[i].startTime << " " << procList[i].totalTimeNeeded << " " << procList[i].priority << endl;
        }*/
        while(!done)
        {
            //get the process to schedule next using the indicated scheduler
            switch(schedChoice)
            {
                //Round Robin
                case 1:
                    procIdx = RoundRobin(curTime, procList, timeQuantum);
                    break;
                //Shortest Process Next
                case 2:
                    procIdx = ShortestProcessNext(curTime,procList);
                    break;
                //Shortest Remaining Time
                case 3:
                    procIdx = ShortestRemainingTime(curTime,procList);
                    break;
                //Highest Response Ratio Next
                case 4:
                    procIdx = HighestResponseRatioNext(curTime,procList);
                    break;
                //Modified Highest Response Ratio Next
                case 5:
                    procIdx = Modified_HRRN(curTime,procList);
                    break;
                //First in First Out
                case 6:
                    procIdx = FIFO(curTime,procList);
                    break;
                //Multilevel Queue
                case 7:
                    procIdx = MultilevelQueue(curTime,procList,timeQuantum);
                    break;
                //Multilevel Feedback Queue
                case 8:
                    procIdx = MultilevelFeedbackQueue(curTime,procList,timeQuantum,highQuantum,lowQuantum);
                    break;
            }

            //if we were given a valid process index
            if(procIdx >= 0 && procIdx < procList.size())
            {
                //update the details for the scheduled process
                ++procList[procIdx].timeScheduled;
                ++procList[procIdx].quantumTime;
                if(procList[procIdx].totalTimeNeeded == procList[procIdx].timeScheduled)
                {
                    procList[procIdx].isDone = true;
                    procList[procIdx].timeFinished = curTime;
                }
            }
            
            done = true;
            // output the row for the time step, denoting which process was selected
            // also check and see if all processes have finished running yet
            cout << setw(4) << curTime;
            done = true;
            for(int i = 0; i < numProc; ++i)
            {
                done = done && procList[i].isDone;

                if(i == procIdx)
                {
                    if(procList[i].isDone)
                    {
                        if(procList[i].timeScheduled > procList[i].totalTimeNeeded)
                        {
                            cout << " |   ! ";
                        }
                        else
                        {
                            cout << " |   X ";
                        }
                    }                
                    else
                    {
                        cout << " |   O ";
                    }
                }
                else
                {
                    cout << " |     ";
                }
            }
            // output for the IDLE process
            if(procIdx < 0 || procIdx >= numProc)
            {
                cout << " |   O ";
            }
            else
            {
                cout << " |     ";
            }
            cout << " |" << endl;
            
            //if we aren't done yet, sleep for a bit and move on to the next time step
            if(!done)
            {
                //this_thread::sleep_for(sleepTime);
                ++curTime;
            }
        }
        auto stop = high_resolution_clock::now();
        auto t = duration_cast<microseconds>(stop - start);
        timeList.push_back(t.count());
       // procList.clear();
    //}
    unsigned long long sum = 0;
    int average = 0;
    for (unsigned int i = 0; i < timeList.size(); i++)
    {
        sum += timeList[i];
    }
    average = sum/100;

    cout << average << endl;

    //its done! output the run statistics
    cout << "\n\nRun Statistics:\n";

    //TODO output the requested run stats
    double avgTurnAroundTime = 0;
    double avgNormalTurnAroundTime = 0;
    int turnAroundTime = 0;
    double normalTurnAroundTime = 0;
    cout << "Process | Finish Time | Turnaround Time | Normalized Turnaround Time |" << endl
         << "----------------------------------------------------------------------" << endl;
    for(auto & Process: procList)
    {
        cout << setw(7) << Process.id << " |";
        cout << setw(12) << (Process.timeFinished + 1) << " |";

        turnAroundTime = ((Process.timeFinished + 1) - Process.startTime);
        avgTurnAroundTime += turnAroundTime;

        cout << setw(16) << turnAroundTime << " |";

        normalTurnAroundTime = ((static_cast<double>(Process.timeFinished + 1) - Process.startTime)/Process.totalTimeNeeded);
        avgNormalTurnAroundTime += normalTurnAroundTime;

        cout << setprecision(2) << fixed;
        cout << setw(27) << normalTurnAroundTime << " |"; 
        cout << endl;
    }
    avgTurnAroundTime /= numProc;
    avgNormalTurnAroundTime /= numProc;
    cout << "----------------------------------------------------------------------" << endl;
    cout << setprecision(2) << fixed;
    cout << setw(9) << "Mean |" << setw(14) <<" |" << setw(16) << avgTurnAroundTime << " |" << setw(27) << avgNormalTurnAroundTime << " |" << endl;

    ofstream output;
    output.open("output.txt",fstream::app);
    //output >> "Input," >> "Turnaround Time," >> "Normalized Turnaround Time," >> "Runtime,\n"
    output << input << "," << avgTurnAroundTime << "," << avgNormalTurnAroundTime << "," << average << ",\n";
    output.close();

    return 0;
}