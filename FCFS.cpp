#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <set>
using namespace std;

class Process
{
private:
    int waitingTime, arrivalTime, burstTime,responseTime,turnAroundTime,id;

public:
    Process() : waitingTime{}, arrivalTime{}, burstTime{},responseTime{}
    {
    }
    void set_arrival(int a){
        arrivalTime=a;
    }
    void set_burst(int b)
    {
        burstTime = b;
    }
    void set_id(int i)
    {
        id = i;
    }
    int get_arrival()
    {
        return arrivalTime;
    }
    int get_burst()
    {
        return burstTime;
    }
    int get_id(){
        return id;
    }
    int get_waiting()
    {
        return waitingTime;
    }
    int get_response(){
        return responseTime;
    }
    int get_Turnaround(){
        return turnAroundTime;
    }
    void calcWaitingtime(int currentTime)
    {
        // Calculate the over all-time the process has taken in the ready queue
        waitingTime = currentTime - arrivalTime;
        responseTime = waitingTime;
    }
    void calcTurnAroundTime() {
        turnAroundTime = waitingTime + burstTime;
    }
};

struct FCFS
{
    multiset<tuple<int,int,int>>p,readyQueue;
    vector<Process>original;
    FCFS(vector<Process> a) {
        original = a;
        for (int i = 1; i < a.size(); i++)
            p.insert({a[i].get_arrival(), a[i].get_burst(), a[i].get_id()});
        int currentTime = get<0>(*p.begin());
        vector<int> GantChart;
        while (true) {
            while (!p.empty() && currentTime >= get<0>(*p.begin())) {
                readyQueue.insert(*p.begin());
                p.erase(p.begin());
            }
            auto currentProcess = *readyQueue.begin();
            int id = get<2>(currentProcess), burstTime = get<1>(currentProcess);
            GantChart.push_back(id);
            original[id].calcWaitingtime(currentTime);
            original[id].calcTurnAroundTime();
            readyQueue.erase(readyQueue.begin());
            currentTime += burstTime;
            if (readyQueue.empty() && !p.empty())
                currentTime = max(currentTime, get<0>(*p.begin()));
            if (readyQueue.empty() && p.empty())
                break;
        }
        cout << "\nGantChart\n";
        int n = original.size() - 1, avg;
        for (int i = 0; i < n; i++)
            cout << "p" << GantChart[i] << " ";
        cout << "\n\nWaitingTime\n";
        for (int i = 1; i <= n; i++) {
            cout << "p" << i << ": " << original[i].get_waiting() << '\n';
            avg += original[i].get_waiting();
        }
        cout << "AverageWaitingTime " << avg * 1.0 / n;
        cout << "\n\nResponseTime\n";
        for (int i = 1; i <= n; i++)
            cout << "p" << i << ": " << original[i].get_response() << '\n';
        cout<<"\n\nTurnAroundTime\n";
        for(int i=1;i<=n;i++)
            cout << "p" << i << ": " << original[i].get_Turnaround() << '\n';
    }
};

int main()
{
    int numberOfProcesses;
    cin >> numberOfProcesses;
    vector<Process> p(numberOfProcesses+1);
    /*
        Example:
            5
            p1 5 0
            p2 3 1
            p3 1 2
            p4 2 3
            p5 3 4
    */
    for (int i = 0; i < numberOfProcesses; i++)
    {
        string name;
        int bt,at;
        cin>>name>>bt>>at;
        int id = name[1]-'0';
        p[id].set_arrival(at);
        p[id].set_burst(bt);
        p[id].set_id(id);
    }
    FCFS calc(p);

    return 0;
}
