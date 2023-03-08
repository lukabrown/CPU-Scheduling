//Luka Brown
//CPU FCFS Simulator


#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <iostream>
#include <cmath>
#include <cstdlib>


struct Process {
    double serviceTime;
    double arrivalTime;
    double finishTime;
};

struct Processor {
    double time;
    bool idle;
    int processesCompleted;
    double avgQ;
};


//constants
static const int MAX_PROCESSES = 10000;

//prototypes
static void   FCFS(std::vector<Process> &processes, Processor &CPU);
static void   Results(std::vector<Process> &processes, Processor &CPU, double arrivalRate, double serviceRate);
static void   Init(int argc, char *argv[], double &arrivalRate, double &serviceRate, std::vector<Process> &processes, Processor &CPU);
static double Poisson(double lambda);

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
    

    std::priority_queue<int, std::vector<int>, std::greater<int>> min_queue;
}

//prints useful data to screen
static void Results(std::vector<Process> &processes, Processor &CPU, double arrivalRate, double serviceRate) {
    std::cout << "\nFinished FCFS Simulation\n";
    std::cout << "Parameters: Arrival_Rate: " << arrivalRate << '\n';
    std::cout << "Parameters: Service_Rate: " << serviceRate << '\n';

    for (int i = 0; i < 10; i++) {
        std::cout << i << ' ' << processes[i].arrivalTime << ' ' << processes[i].serviceTime << '\n';
    }

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

    //initializes processor
    CPU.idle = false;
    CPU.processesCompleted = 0;
    CPU.time = 0;
    CPU.avgQ = 0;

    //seeds random
    srand ( time(NULL) ); 

    //initializes processes
    for(auto i = 0; i < processes.size(); i++) {
        if (i == 0)
            processes[i].arrivalTime = Poisson(arrivalRate);
        else
            processes[i].arrivalTime = Poisson(arrivalRate) + processes[i-1].arrivalTime;
        processes[i].serviceTime = Poisson(serviceRate);
        processes[i].finishTime = -1; //-1 = unfinished
    }
}

// returns a random number that follows an exponential distribution
static double Poisson(double lambda) {
	double u,x;
	x = 0;
	while (x == 0) {
        u = (double) rand() / RAND_MAX;
        x = (-1/lambda)*log(u);
	}
	return(x);
}