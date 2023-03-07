//Luka Brown
//CPU FCFS Simulator


#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <iostream>
#include <random>


struct Process {
    double serviceTime;
    double arrivalTime;
    double finishTime;
};

struct Processor {
    double time;
    bool idle;
    int processesCompleted;
};


//constants
static const int MAX_PROCESSES = 10000;

//prototypes
static void   FillList(std::vector<Process> &processes, double arrivalRate, double serviceRate);
static void   FCFS(std::vector<Process> &processes, Processor &CPU);
static void   Results(std::vector<Process> &processes, Processor &CPU, double arrivalRate, double serviceRate);
static double Poisson(double lambda);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "ERROR: Parameters: Arrival_Rate Service_Rate\n";
        return -1;
    }

    double arrivalRate = atof(argv[1]);
    double serviceRate = atof(argv[2]);

    std::vector<Process> processes(MAX_PROCESSES);
    Processor CPU;

    FillList(processes, arrivalRate, serviceRate);
    FCFS(processes, CPU);
    Results(processes, CPU, arrivalRate, serviceRate);
    return 0;
}

//builds random data to fill the table
static void FillList(std::vector<Process> &processes, double arrivalRate, double serviceRate) {

    for(auto i = 0; i < processes.size(); i++) {
        Process temp;
        temp.arrivalTime = Poisson(arrivalRate);
        temp.serviceTime = Poisson(serviceRate);
        temp.finishTime = -1; //-1 = unfinished
        processes[i] = temp;
    }
}

//simulates FCFS CPU scheduler
static void FCFS(std::vector<Process> &processes, Processor &CPU) {
    //initializes processor
    CPU.idle = false;
    CPU.processesCompleted = 0;
    CPU.time = 0;

    std::priority_queue<int, std::vector<int>, std::greater<int>> min_queue;
}

//prints useful data to screen
static void Results(std::vector<Process> &processes, Processor &CPU, double arrivalRate, double serviceRate) {
    std::cout << "Finished FCFS Simulation\n";
    std::cout << "Parameters: Arrival_Rate: " << arrivalRate << '\n';
    std::cout << "Parameters: Arrival_Rate: " << serviceRate << '\n';

    for (int i = 0; i < 10; i++) {
        std::cout << i << ' ' << processes[i].arrivalTime << ' ' << processes[i].serviceTime << '\n';
    }

}

// returns a random number that follows an exponential distribution
static double Poisson(double lambda) {
	double u,x;
	x = 0;
	while (x == 0) {
        u = rand() / RAND_MAX;
        x = (-1/lambda)*log(u);
	}
	return(x);
}