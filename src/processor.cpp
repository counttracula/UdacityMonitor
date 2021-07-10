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

float Processor::Utilization() { 
	vector<string> uPrev = LinuxParser::CpuUtilization();
  	Processor prev(uPrev);
	unsigned int sleepFor = 100000; // 100ms 
	usleep(sleepFor);	// sleep
 	vector<string> uCurr= LinuxParser::CpuUtilization(); 
  	Processor curr(uCurr);
	prev.idle = prev.idle + prev.iowait;
	curr.idle = curr.idle + curr.iowait;
  	int prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
	int nonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;
  	int prevTotal = prev.idle + prevNonIdle;
	int total = curr.idle + nonIdle;
  	int dTotal = total - prevTotal;
	int dIdle = curr.idle - prev.idle;
  	float cpuUtil = (dTotal - dIdle) / dTotal;
	return cpuUtil;
} // Utilization()