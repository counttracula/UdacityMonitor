#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include "linux_parser.h"
#include "process.h"
#include "format.h"

using std::string;
using std::to_string;
using std::vector;
using std::pair;

int Process::Pid() { 
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  pair<long, long> jiffies = LinuxParser::ActiveJiffies(pid_);
  long totalTime = jiffies.first;
  long startTime = jiffies.second;
  long systemUpTime = LinuxParser::UpTime();
  long int hertz = LinuxParser::Hertz();
  long seconds = systemUpTime - (startTime / hertz);
  float cpuUsage = 100 * ((totalTime / hertz) / seconds);
  return cpuUsage;
} // CpuUtilization()

string Process::Command() { 
  return LinuxParser::Command(pid_); 
}

string Process::Ram() { 
  return LinuxParser::Ram(pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
	pair<long, long> jiffies = LinuxParser::ActiveJiffies(pid_);
    long startTime = jiffies.second;
    long systemUpTime = LinuxParser::UpTime();
    long int hertz = LinuxParser::Hertz();
    long seconds = systemUpTime - (startTime / hertz);
	return seconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {                                   
  return (uptime_ > a.uptime_); 
}