#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::vector;
using std::string;

class Processor {
	int user;
  	int nice;
  	int system;
  	int idle;
  	int iowait;
  	int irq;
  	int softirq;
  	int steal;
  	int guest;
  	int guest_nice;
public:
  	Processor() {
    	user = 0;
        nice = 0;
        system = 0;
        idle = 0;
        iowait = 0;
        irq = 0;
        softirq = 0;
        steal = 0;
        guest = 0;
        guest_nice = 0;
    }
	Processor(vector<string> data) {
        user = stoi(data[0]);
        nice = stoi(data[1]);
        system = stoi(data[2]);
        idle = stoi(data[3]);
        iowait = stoi(data[4]);
        irq = stoi(data[5]);
        softirq = stoi(data[6]);
        steal = stoi(data[7]);
        guest = stoi(data[8]);
        guest_nice = stoi(data[9]);
   	} // ctor
 	double Utilization();  
};

#endif