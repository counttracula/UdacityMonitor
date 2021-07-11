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
using std::sort;

int Process::Pid() { 
  return pid_; 
}

float Process::CpuUtilization() { 
  pair<long, long> jiffies = LinuxParser::ActiveJiffies(pid_);
  long totalTime = jiffies.first;
  long startTime = jiffies.second;
  long systemUpTime = LinuxParser::UpTime();
  long int hertz = LinuxParser::Hertz();
  float seconds = systemUpTime - (startTime / hertz);
  float cpuUsage = static_cast<float>(totalTime / hertz) / seconds;
  return cpuUsage;
} 

string Process::Command() { 
  command_ = LinuxParser::Command(pid_);
  return command_;
}

string Process::Ram() { 
  return LinuxParser::Ram(pid_);
}

string Process::User() { 
  user_ = LinuxParser::User(pid_); 
  return user_;
}

long int Process::UpTime() { 
	pair<long, long> jiffies = LinuxParser::ActiveJiffies(pid_);
    long startTime = jiffies.second;
    long systemUpTime = LinuxParser::UpTime();
    long int hertz = LinuxParser::Hertz();
    long seconds = systemUpTime - (startTime / hertz);
	uptime_ = seconds;
  	return uptime_;
}

bool Process::operator<(Process const& a) const { 
  return (uptime_ < a.uptime_);
}