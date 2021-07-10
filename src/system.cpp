#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  for (int pid: LinuxParser::Pids()) {
    Process proc = Process(pid);
    processes_.emplace_back(proc);
  }
  return processes_; 
}


std::string System::Kernel() { 
  return LinuxParser::Kernel();
} // Kernel()


float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
} // MemoryUtilization()

std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
} // OperatingSystem()


int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();; 
}

int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
} // TotalProcesses()

long System::UpTime() {
  return LinuxParser::UpTime();
}