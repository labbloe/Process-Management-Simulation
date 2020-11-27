#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main()
{
    string base = "procList";
    srand(time(0));
    string end = ".txt";
    string fileName = "";
    ofstream f;
    int n = 500;
    
        for (int i = 0; i < 100; i++)
        {
            int arrivalTime = 0;
            int temp = 0;
            int totalTime = rand() % 10 + 1;

            fileName = (base + to_string(i + 1) + end);
            f.open(fileName, fstream::out);
            f << n << endl;
            f << "p" << 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2 << endl;
            for (int k = 0; k < n; k++)
            {
                /*do
                {
                    temp = arrivalTime;
                    arrivalTime = rand() % (temp + totalTime);
                    if (arrivalTime < temp || arrivalTime > n)
                    {
                        arrivalTime = temp;
                    }
                } while (arrivalTime == temp && temp != n);*/
                arrivalTime = rand() % 11;
                totalTime = rand() % 10 + 1;
                f << "p" << k + 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2; //will add processName (pi), arrival time, and length
                if (k < n - 1)
                    f << endl;
            }
            f.close();
            cout << "Creating " << fileName << endl;
        }
    
    fileName = "";
    n = 50;
        for (int i = 0; i < 100; i++)
        {
            int arrivalTime = 0;
            int temp = 0;
            int totalTime = rand() % 10 + 1;

            fileName = (base + to_string(i + 1) + end);
            f.open(fileName, fstream::out);
            f << n << endl;
            f << "p" << 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2 << endl;
            for (int k = 0; k < n; k++)
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
                f << "p" << k + 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2; //will add processName (pi), arrival time, and length
                if (k < n - 1)
                    f << endl;
            }
            f.close();
            cout << "Creating " << fileName << endl;
        }
    fileName = "";
    n = 100;
        for (int i = 0; i < 100; i++)
        {
            int arrivalTime = 0;
            int temp = 0;
            int totalTime = rand() % 10 + 1;

            fileName = (base + to_string(i + 1) + end);
            f.open(fileName, fstream::out);
            f << n << endl;
            f << "p" << 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2 << endl;
            for (int k = 0; k < n; k++)
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
                f << "p" << k + 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2; //will add processName (pi), arrival time, and length
                if (k < n - 1)
                    f << endl;
            }
            f.close();
            cout << "Creating " << fileName << endl;
        }
    fileName = "";
    n = 500;
        for (int i = 0; i < 100; i++)
        {
            int arrivalTime = 0;
            int temp = 0;
            int totalTime = rand() % 10 + 1;

            fileName = (base + to_string(i + 1) + end);
            f.open(fileName, fstream::out);
            f << n << endl;
            f << "p" << 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2 << endl;
            for (int k = 0; k < n; k++)
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
                f << "p" << k + 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2; //will add processName (pi), arrival time, and length
                if (k < n - 1)
                    f << endl;
            }
            f.close();
            cout << "Creating " << fileName << endl;
        }
        /*
    fileName = "";
    n = 1000;
        for (int i = 0; i < 100; i++)
        {
            int arrivalTime = 0;
            int temp = 0;
            int totalTime = rand() % 10 + 1;

            fileName = (base + to_string(i + 1) + end);
            f.open(fileName, fstream::out);
            f << n << endl;
            f << "p" << 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2 << endl;
            for (int k = 0; k < n; k++)
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
                f << "p" << k + 1 << " " << arrivalTime << " " << totalTime << " " << rand() % 2; //will add processName (pi), arrival time, and length
                if (k < n - 1)
                    f << endl;
            }
            f.close();
            cout << "Creating " << fileName << endl;
        }
        */
       return 0;
}