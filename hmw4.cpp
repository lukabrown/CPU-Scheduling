//Luka Brown
//CPU FCFS Simulator

#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <random>

struct Process {
    double serviceTime;
    double arrivalTime;
    double finishTime;

    bool operator<(const Process &temp) const
    {
        return arrivalTime < temp.arrivalTime;
    }
};

struct Processor {
    double  time;
    double  idleTime;
    bool    idle;
    int     processesCompleted;
    double  avgQ;
    Process currentProcess;
};

//constants
static const int ERROR = 5000; //generates excess processes for scheduling near end of sim
static const int MAX_PROCESSES = 10000 + ERROR;

//prototypes
static void   FCFS(std::vector<Process> &processes, Processor &CPU);
static void   Results(std::vector<Process> &processes, Processor &CPU, double arrivalRate, double serviceRate);
static void   Init(int argc, char *argv[], double &arrivalRate, double &serviceRate, std::vector<Process> &processes, Processor &CPU);
static double Poisson(double lambda);
static double Exponential(double lambda); //needs more work

int main(int argc, char *argv[]) {

    //global variables
    std::vector<Process> processes(MAX_PROCESSES); //list of 10000 processes
    Processor CPU;                                 //holds simulation data
    double arrivalRate, serviceRate;               //input variables

    //functions
    Init(argc, argv, arrivalRate, serviceRate, processes, CPU); //initializes and seeds random
    FCFS(processes, CPU);                                       //computes simulation
    Results(processes, CPU, arrivalRate, serviceRate);          //reports sim results
    return 0;
}

//simulates FCFS CPU scheduler
static void FCFS(std::vector<Process> &processes, Processor &CPU) {
    std::priority_queue<Process> min_queue;

    while (CPU.processesCompleted != MAX_PROCESSES-ERROR) {
        //updates time to next process picked, and picks the process
        if (CPU.idle) { //finds a process

            if (min_queue.size() == 0) { //waits for next process in future
                //finds next future process
                int i;
                for (i = CPU.processesCompleted; i < MAX_PROCESSES; i++) {
                    if (CPU.time < processes[i].arrivalTime) {
                        CPU.idleTime += processes[i].arrivalTime - CPU.time;
                        CPU.time = processes[i].arrivalTime;
                        goto end1;
                    }
                } end1:
                CPU.idle = false;
                CPU.currentProcess = processes[i];

            } else { //finds process from queue
                CPU.currentProcess = min_queue.top();
                min_queue.pop();
                if (CPU.time < CPU.currentProcess.arrivalTime) {
                    CPU.idleTime += CPU.currentProcess.arrivalTime - CPU.time;
                    CPU.time = CPU.currentProcess.arrivalTime;
                }
            }
        } //end cpu idle
        
        //adds all processes that came in while CPU was busy to queue
        double tempTime = CPU.time + CPU.currentProcess.serviceTime;
        
        for (int i = CPU.processesCompleted+1; i < MAX_PROCESSES; i++) {
            if ((processes[i].arrivalTime >= CPU.time) && (processes[i].arrivalTime < tempTime) && (processes[i].finishTime == -1)) {
                min_queue.push(processes[i]);
            } else if (processes[i].arrivalTime >= tempTime) {
                goto end2;
            }
        } end2:

        //process finishes
        CPU.time += CPU.currentProcess.serviceTime;
        CPU.idle = true;
        CPU.avgQ = (CPU.avgQ + min_queue.size()) / 2.0; //updates avg in queue
        processes[CPU.processesCompleted].finishTime = CPU.time; //marks finish time
        CPU.processesCompleted++;

    } // end while < 10000 processes
    return;
}

//prints useful data to screen
static void Results(std::vector<Process> &processes, Processor &CPU, double arrivalRate, double serviceRate) {
    //computes avg turnaround
    double turnaround = processes[0].finishTime - processes[0].arrivalTime;
    for (int i = 1; i < CPU.processesCompleted; i++) {
        turnaround += processes[i].finishTime - processes[i].arrivalTime;
        turnaround /= 2.0;
    }

    //prints all output stats
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nFinished FCFS simulation.\n";
    std::cout << CPU.processesCompleted << " processes completed in " << CPU.time << " s\n";
    std::cout << "Average processes in queue:  " << CPU.avgQ << "\n";
    std::cout << "Average turnaround time:     " << turnaround << " s\n";
    std::cout << "Throughput:                  " << (MAX_PROCESSES-ERROR)/CPU.time << " processes/s\n";
    std::cout << "CPU Utilization:             " << (CPU.time-CPU.idleTime)/CPU.time*100 << " %\n";
    //std::cout << "Idle: " << CPU.idleTime/CPU.time*100 << " %, " << CPU.idleTime << " s\n";

    //debug //for (int i = 10000; i <= 10000; i++) {std::cout << i << ' ' << processes[i].finishTime << ' ' << processes[i].serviceTime << '\n';}
}

//initializes all variables
static void Init(int argc, char *argv[], double &arrivalRate, double &serviceRate, std::vector<Process> &processes, Processor &CPU) {
    //inits input vars
    if (argc != 3) {
        std::cout << "ERROR: Parameters: Arrival_Rate Service_Rate\n";
        return;
    }
    arrivalRate = atof(argv[1]);
    serviceRate = 1 / atof(argv[2]);
    std::cout << "Parameters: Arrival_Rate: " << arrivalRate << " processes/s\n";
    std::cout << "            Service_Rate: " << 1 / serviceRate << " s\n";

    //initializes processor
    CPU.idle = true;
    CPU.processesCompleted = 0;
    CPU.time = 0;
    CPU.avgQ = 0;
    CPU.idleTime = 0;

    //seeds random
    unsigned int seed = time(NULL);
    //seed = 1;
    srand(seed);
    std::cout << "            Seed:         " << seed << '\n';
    
    /*for c++ 11
    //exponential number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> d(serviceRate);*/
    
    //initializes processes
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (i == 0)
            processes[i].arrivalTime = Poisson(arrivalRate);
        else
            processes[i].arrivalTime = Poisson(arrivalRate) + processes[i-1].arrivalTime;

        processes[i].serviceTime = Poisson(serviceRate);
        //processes[i].serviceTime = d(gen); //for c++ 11
        processes[i].finishTime = -1; //-1 = unfinished
    }
}

// returns a random number that follows an exponential distribution
static double Poisson(double lambda) {

	double temp, num = 0;
	while (num == 0 || num == INFINITY) {
        temp = (double) rand() / RAND_MAX; //0 or 1
        num = (-1 / lambda) * log(temp); 
	}
	return(num);
}

//supposed to generate nums on exp dist.
static double Exponential(double lambda) {
    double temp = lambda * exp(-lambda * rand());
    while (temp == INFINITY || temp == 0)
        temp = lambda * exp(-lambda * (double) rand());
    return temp;
}