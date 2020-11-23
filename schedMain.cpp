/*  Process Memory Management Simulation
    Jared Allen & Gabe Gilley
*/

#include<vector>   // process vector
#include<iostream> // cout, cin
#include<chrono>   // sleep duration
#include<thread>   // sleep_for
#include<iomanip>  // setw 

#include "schedulers.h"    

using namespace std;

int main(int argc, char* argv[])
{
    string fname;
    chrono::duration<int, std::milli> sleepTime = chrono::milliseconds(500);
    vector<Process> procList;
    int curTime = 0, input, schedChoice, numProc, procIdx, timeQuantum;
    bool done;
    string tempStr;

    //Default to process list simulation. See procList.txt for process example setup.
    if(argc == 1)
    {
        fname = "./procList.txt";
    }
    // gave a file name but not sleep time
    else if(argc == 2)
    {
        fname = argv[1];
    }
    else
    {
        fname = argv[1];
        sleepTime = chrono::milliseconds(stoul(argv[2]));
    }
    
    //read in the process list and store the total number of processes
    readInProcList(fname, procList);
    numProc = procList.size();

    cout << "\nWelcome to the Process Memory Management Schedular Simulator!\n\n";

    schedChoice = -1;
    do
    {
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
        cin >> input;

        if(input > 0 && input <= 4)
        {
            schedChoice = input;
        }
        else
        {
            cout << "INVALID ENTRY\n\n";
        }
    } while (schedChoice == -1);
    
    //if the scheduler selected needs a time quantum, ask for it
    if(schedChoice == 1)
    {
        cout << "Enter the time quantum you would like to use: ";
        cin >> timeQuantum;
    }
    
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

    //pre-run init
    done = false;
    curTime = 0;
    procIdx = -1;

    //while not all processes have completed:
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
                procIdx = Modified_HRRN(curTime,procList);
                break;
        }

        //if we were given a valid process index
        if(procIdx >= 0 && procIdx < procList.size())
        {
            //update the details for the scheduled process
            ++procList[procIdx].timeScheduled;
            if(procList[procIdx].totalTimeNeeded == procList[procIdx].timeScheduled)
            {
                procList[procIdx].isDone = true;
                procList[procIdx].timeFinished = curTime;
            }
        }

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
            this_thread::sleep_for(sleepTime);
            curTime = curTime + 1;
        }
    }
    cout << "\n\nRun Statistics:\n";



    //Simulation Stats
    cout << "Process | Finish Time | Turnaround Time | Normalized Turnaround Time |" << "\n"
         << "----------------------------------------------------------------------" << endl;
    float mean_TurnTime = 0;
    float mean_normTurnTime = 0;
    int turnTime;
    float normTurnTime;
    for(auto & Proc : procList)
    {
        cout << setw(7) << Proc.id << " |";
        cout << setw(12) << (Proc.timeFinished+1)<< " |";
        turnTime = ((Proc.timeFinished +1)-Proc.startTime);
        mean_TurnTime += turnTime;
        cout << setw(16) <<turnTime<<" |";
        normTurnTime = ((static_cast<float>(Proc.timeFinished+1)-Proc.startTime)/Proc.totalTimeNeeded);
        mean_normTurnTime += normTurnTime;
        cout << std::setprecision(2)<<fixed;
        cout << setw(27) <<normTurnTime<<" |"; 
        //cout << setw(2) << state << " |";
        cout<<"\n";
    }
    mean_TurnTime /= numProc;
    mean_normTurnTime /= numProc;
    cout<< "----------------------------------------------------------------------" << endl;
    cout<<std::setprecision(2)<<fixed;
    cout << setw(9) << "Mean |" <<setw(14)<<" |"<<setw(16)<<mean_TurnTime<<" |"<<setw(27)<<mean_normTurnTime<<" |\n";



    return 0;
}