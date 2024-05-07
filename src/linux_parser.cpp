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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key, num, kb;
  float MemTotal, MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> std::ws >> num >> std::ws >> kb) {
        if (key == "MemTotal:") {
          MemTotal = float(stol(num));
        }
        else if (key == "MemFree:") {
          MemFree = float(stol(num));
        }
      }
      
    }
  }
    
  // (MemTotal - MemFree) / MemTotal
  float mem_util_percentage = ((MemTotal) - (MemFree)) / (MemTotal);
  return mem_util_percentage;
}
  

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;
  string idle_time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
//       std::cout << line << std::endl;
      
      std::istringstream linestream(line);
      while (linestream >> uptime >> std::ws >> idle_time) {
//           std::cout << stol(uptime) << std::endl;
          return stol(uptime);
      }
    }
  }
  return stol(uptime);
};

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization for each process
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return total CPU utilization
std::vector<long long> LinuxParser::TotalCpuUtilization() { 
  string line;
  string key, user_s, nice_s, system_s, idle_s, iowait_s, irq_s, softirq_s, steal_s, guest_s, guest_nice_s;
  long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> std::ws >> user_s >> std::ws >> nice_s >> std::ws >> system_s >> std::ws >> idle_s >> std::ws >> iowait_s >> std::ws >> irq_s >> std::ws >> softirq_s >> std::ws >> steal_s >> std::ws >> guest_s >> std::ws >> guest_nice_s >> std::ws) {
        if (key == "cpu") {
//           std::cout << line << "\n";
          user = stoll(user_s), nice = stoll(nice_s), system = stoll(system_s), idle = stoll(idle_s), iowait = stoll(iowait_s), irq = stoll(irq_s), softirq = stoll(softirq_s), steal = stoll(steal_s), guest = stoll(guest_s), guest_nice = stoll(guest_nice_s);
        }
      }
      
    }
  }
    
  vector<long long> cpu_util_vector = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  return cpu_util_vector;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> std::ws >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
};

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> std::ws >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return stoi(value);
};

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key, RAM_in_kb, units;
  long long RAM_in_mb;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> std::ws >> RAM_in_kb >> std::ws >> units) {
        if (key == "VmSize:") {
          RAM_in_mb = stoll(RAM_in_kb) / 1000;
          return std::to_string(RAM_in_mb);
        }
      }
    }
  }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
