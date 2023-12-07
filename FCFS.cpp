#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <iomanip>
#include <windows.h>
using namespace std;

class Process
{
private:
    int waitingTime, arrivalTime, burstTime, priority, executionTime;

public:
    Process() : waitingTime{}, arrivalTime{}, burstTime{}, priority{}, executionTime{}
    {
    }
    void set(int arrival = 0, int burst = 0, int p = 0)
    {
        arrivalTime = arrival;
        burstTime = burst;
        priority = p;
    }
    void set_executiontime(int execution)
    {
        executionTime = execution;
    }
    int get_arrival()
    {
        return arrivalTime;
    }
    int get_burst()
    {
        return burstTime;
    }
    int get_priority()
    {
        return priority;
    }
    int get_execution()
    {
        return executionTime;
    }
    int get_waiting()
    {
        return waitingTime;
    }
    void calcWaitingtime()
    {
        // Calculate the over all time the process has taken in the waiting queue
        waitingTime = executionTime - arrivalTime;
    }
};

void detectPriorityErrors(Process p[], int len)
{
    for (int i = 0; i < len; i++)
    {
        int currentPriority = p[i].get_priority();
        if (currentPriority < 0)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            cout << "Invalid Input, the priority of process" << i + 1 << " is smaller than 0\n";
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            cout << "Hint: Check Note Number 1";
            SetConsoleTextAttribute(hConsole, 7);
            exit(0);
        }
    }
}

// Mapping the process's times and type of errors
string index[3] = {"Arrival Time", "Burst Time", "Priority"};

void detectTimeErrors(int x, int i)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    if (fmod(x, 1.0) != 0 && x < 0)
    {
        cout << "Invalid Input, " << index[i] << " has to be an integer and positive number";
        SetConsoleTextAttribute(hConsole, 7);
        exit(0);
    }
    else if (fmod(x, 1.0) != 0)
    {
        cout << "Invalid Input, " << index[i] << " has to be an integer number";
        SetConsoleTextAttribute(hConsole, 7);
        exit(0);
    }
    else if (x < 0)
    {
        cout << "Invalid Input, " << index[i] << " has to be a positive number";
        SetConsoleTextAttribute(hConsole, 7);
        exit(0);
    }
    SetConsoleTextAttribute(hConsole, 7);
}

struct FCFS
{
    bool operator()(tuple<pair<int, int>, int, int> x, tuple<pair<int, int>, int, int> y)
    {
        if (get<0>(x).second > get<0>(y).second)
            return true;
        else if (get<0>(x).second == get<0>(y).second && get<2>(x) > get<2>(y))
            return true;
        else if (get<0>(x).second == get<0>(y).second && get<2>(x) == 0 && get<1>(x) > get<1>(y))
            return true;
        return false;
    }
    void process(Process p[], int len)
    {
        priority_queue<tuple<pair<int, int>, int, int>, vector<tuple<pair<int, int>, int, int>>, FCFS> waitingQueue;
        for (int i = 0; i < len; i++)
        {
            int currentArrival = p[i].get_arrival();
            int currentBurst = p[i].get_burst();
            int currentPriority = p[i].get_priority();
            tuple<pair<int, int>, int, int> currentProcess({i, currentArrival}, currentBurst, currentPriority);
            waitingQueue.push(currentProcess);
        }
        int currentExecutionTime = -1;
        cout << "\n\nThe Order of execution: \n";
        while (waitingQueue.size())
        {
            tuple<pair<int, int>, int, int> c = waitingQueue.top();
            int processNumber = get<0>(c).first;
            int arrivalTime = get<0>(c).second;
            int burstTime = get<1>(c);
            cout << "\tProcess: " << processNumber + 1 << '\n';
            if (currentExecutionTime < arrivalTime)
            {
                currentExecutionTime = arrivalTime;
            }
            p[processNumber].set_executiontime(currentExecutionTime);
            p[processNumber].calcWaitingtime();
            currentExecutionTime += burstTime;
            waitingQueue.pop();
        }
        cout << "\nWaiting Time: \n";
        double avg = 0;
        for (int i = 0; i < len; i++)
        {
            cout << "\tProcess " << i + 1 << ": " << p[i].get_waiting() << '\n';
            avg += p[i].get_waiting();
        }
        cout << "\nAverage Waiting Time: \n";
        cout << fixed << setprecision(2) << (avg / len * 1.0);
    }
};

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Notes and Comments: \n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << "\t 1- The value of process's priority has to be a positive numbers.\n";
    cout << "\t 2- The value of process's arrivaltime and bursttime has to be integer and positive number\n";
    cout << "\t 3- If you put a value of any process's priority, then you've to put a value for the remaining.\n";
    cout << "\t 4- If there is no priority and there is multiple tasks with the same arrival time, then the minimmum in burst time will be taken first.\n";
    cout << "\t 5- If there is no priority and there is multiple tasks with the same arrival time and the same burst time, then the first in position will be taken first.\n";

    SetConsoleTextAttribute(hConsole, 7);
    cout << "\n\nNumber of Processes: ";
    int numberOfProcesses;
    cin >> numberOfProcesses;
    Process p[numberOfProcesses];

    cout << '\n';
    for (int i = 0; i < numberOfProcesses; i++)
    {
        double x, y, z = 0;
        string stateOfPriority;
        cout << "\nProcess " << i + 1 << ": \n";
        cout << "\tArrival Time: ";
        cin >> x;
        detectTimeErrors(x, 0);
        cout << "\tBurst Time: ";
        cin >> y;
        detectTimeErrors(y, 1);
        cout << "\tIs there any priority [Yes,No]: ";
        cin >> stateOfPriority;
        transform(stateOfPriority.begin(), stateOfPriority.end(), stateOfPriority.begin(), ::tolower);
        if (stateOfPriority == "yes")
        {
            cout << "\t\tPriority: ";
            cin >> z;
            detectTimeErrors(z, 2);
        }
        p[i].set((int)x, (int)y, (int)z);
    }
    detectPriorityErrors(p, numberOfProcesses);
    FCFS calc;
    calc.process(p, numberOfProcesses);

    return 0;
}
