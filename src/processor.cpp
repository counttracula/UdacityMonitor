#include "processor.h"
#include <unistd.h>
#include <vector>
#include <string>
#include "linux_parser.h"

using std::vector;
using std::string;

class CpuData {
  public:
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
  	CpuData(vector<string> data) {
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
};

float Processor::Utilization() { 
	vector<string> uPrev = LinuxParser::CpuUtilization();
  	CpuData prev = CpuData(uPrev);
	unsigned int sleepFor = 100000; // 100 ms 
	usleep(sleepFor);	// sleeps for 100ms
 	vector<string> uCurr= LinuxParser::CpuUtilization(); 
  	CpuData curr = CpuData(uCurr);
	prev.idle = prev.idle + prev.iowait;
	curr.idle = curr.idle + curr.iowait;
  	int prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
	int nonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;
  	int prevTotal = prev.idle + prevNonIdle;
	int total = curr.idle + nonIdle;
  	int dTotal = total - prevTotal;
	int dIdle = curr.idle - prev.idle;
  	int cpuUtil = (dTotal - dIdle) / dTotal;
	return cpuUtil;
} // Utilization()