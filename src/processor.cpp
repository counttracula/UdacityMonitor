#include "processor.h"
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include "linux_parser.h"
#include <stdio.h>

using std::vector;
using std::string;
using std::cout;

double Processor::Utilization() { 
	vector<string> uPrev = LinuxParser::CpuUtilization();
  	Processor prev(uPrev);
	unsigned int sleepFor = 1000000; // 500ms 
	usleep(sleepFor);	// sleep
 	vector<string> uCurr= LinuxParser::CpuUtilization(); 
  	Processor curr(uCurr);
	prev.idle = prev.idle + prev.iowait;
	curr.idle = curr.idle + curr.iowait;
  	int prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
	int currNonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;
  	int prevTotal = prev.idle + prevNonIdle;
	int currTotal = curr.idle + currNonIdle;
  	int dTotal = currTotal - prevTotal;
	int dIdle = curr.idle - prev.idle;
  	float cpuUtil = static_cast<float>(dTotal - dIdle) / static_cast<float>(dTotal);
	return cpuUtil;
} // Utilization()