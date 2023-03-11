# FCFS-Scheduler

This is a First Come First Serve CPU scheduler simulation. It runs very quickly and outputs some data relevent to the CPU utilization,
process throughput, time taken, and average turnaround time for each process.

After cloning, use g++ in a terminal to compile:

g++ main.cpp -o sim

Then run using:

./sim Arrival_Rate Service_Rate

The parameters:
  Arrival_Rate: processes/s. Recommend using values 10-30
  Service_Rate: Avg service time of processes. Must be between 0 and 1 seconds. Recommend using .04
  
Example Call:

./sim 10 .04
