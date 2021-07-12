#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stoi;
using std::pair;

// Adopted from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
vector<string> LinuxParser::split(const string& line, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(line);
   while (getline(tokenStream, token, delimiter)) {
      tokens.emplace_back(token);
   }
   return tokens;
} // split()

// Check if the string is a floating point number
bool LinuxParser::isFloat(const string& str)
{
    return str.find_first_not_of("0123456789.") == str.npos;
}

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
        pids.emplace_back(pid);
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


long int LinuxParser::Hertz() {
  return sysconf(_SC_CLK_TCK);
}

pair<long, long> LinuxParser::ActiveJiffies(int pid) { 
	const string kPid{'/' + to_string(pid)};
  	string line;
  	long int utime, stime, cutime, cstime, startTime, totalTime;
  	std::ifstream stream(kProcDirectory + kPid + kStatFilename);
    if (stream.is_open()) {
	    std::getline(stream, line);
		std::vector<std::string> tokens = split(line, ' ');
      	utime = stol(tokens[13]);
      	stime = stol(tokens[14]);
      	cutime = stol(tokens[15]);
      	cstime = stol(tokens[16]);
      	startTime = stol(tokens[21]);
    } // if
	totalTime = utime + stime + cutime + cstime;
  	return std::make_pair(totalTime, startTime);
}


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
        if (key.compare("cpu") == 0) {
          while (linestream >> value) {
          	cpuUtil.emplace_back(value);
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


string LinuxParser::Command(int pid) { 
  string cmd;
  string line;
  const string kPid{'/' + to_string(pid)};
  std::ifstream stream(kProcDirectory + kPid + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
  }
  
  return cmd;
}

string LinuxParser::Ram(int pid) { 
    string rStr;
  	string line;
  	string mem;
  	int ram;
    const string kPid{to_string(pid)};
    std::ifstream stream(kProcDirectory + kPid + kStatusFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> mem) {
	      if (mem.compare("VmRSS:") == 0) {
    	    linestream >> rStr; 
            break;
          }
        } // while
	  } // while
    } // if

  	// sanitize input
  	int firstNonFloat = rStr.find_first_not_of("0123456789.");
    rStr = rStr.substr(0, firstNonFloat);
  	ram = stof(rStr);
  	ram /= 1024;
    
  	return to_string(ram); 
}

string LinuxParser::Uid(int pid) { 	
  string uid, line, field1;
  const string kPid{'/' + to_string(pid)};
  std::ifstream stream(kProcDirectory + kPid + kStatusFilename);
  if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> field1) {
	      if (field1.compare("Uid:") == 0) {
    	    linestream >> uid; 
			return uid;
          }
        } // while
	  } // while
  } // if
  return string();
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
    string uid, line, password, uuid;
  	string user("Quagmire"); // should never print
	uid = Uid(pid);
    // we now have the uid
    std::ifstream userstream(kPasswordPath);
    if (userstream.is_open()) { 
      while (std::getline(userstream, line)) {
      	std::vector<std::string> tokens = split(line, ':');
        if (tokens[2].compare(uid) == 0) {
          user = tokens[0];
          break;
        }
      } // while
    }  // if
    return user;
}
