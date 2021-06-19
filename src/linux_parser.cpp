#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() { 
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  float memTotal;
  float memFree;
  float memBuffers;
  float memCached;
  float totalUsed;
  string key;
  float value;
  string junk;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> junk) {
        if (key == "MemTotal:") {
          memTotal = value;
        } else if (key == "MemFree:") {
          memFree = value;
        } else if (key == "Buffers:") {
          memBuffers = value;
        } else if (key == "Cached:") {
          memCached = value;
        }
      } // while
    } // while
  } // if
  totalUsed = (memTotal - memFree - (memBuffers + memCached)) / memTotal;
  
  return totalUsed; 
}

long LinuxParser::UpTime() { 
  long upTime = 0;
  long idleTime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return upTime;
} // UpTime()

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtil;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string key;
  string value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> value) {
          	cpuUtil.push_back(value);
          } // while
        } // if 
      } // while
    } // while
  } // if
  return cpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  string key;
  int value = 0;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      } // while
    } // while
  } // if

  return value; 
}


int LinuxParser::RunningProcesses() { 
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  string key;
  int value = 0;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      } // while
    } // while
  } // if

  return value; 

} // RunningProcesses()

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Command(int pid) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) { return 0; }
