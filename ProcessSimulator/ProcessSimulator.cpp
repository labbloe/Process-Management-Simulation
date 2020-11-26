/*
    Jared Allen & Gabe Gilley
*/

#include <iostream>
#include<fstream>
#include<string>

using namespace std;

const int numProcLists = 100;
const int n = 10;

int main()
{
    ofstream f;
    string command = "../program ";
    string procList = "procList";
    string curList;
    string end = ".txt 1";
    char c;
    for(int i=0; i<numProcLists; i++)
    {
        command = "../program ";
        curList = procList + to_string(i);
        command = command + curList + end;
        cout<<command<<endl;
        system(command.c_str());
    }
    return 0;   
}