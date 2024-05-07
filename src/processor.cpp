#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

Processor::Processor(std::vector<long long> init_values): start_cpu_total_util(init_values) {
  prevUptime = LinuxParser::UpTime();
//   std::vector<float> start_cpu_total_util = LinuxParser::TotalCpuUtilization();
  prevuser = start_cpu_total_util[0], prevnice = start_cpu_total_util[1], prevsystem = start_cpu_total_util[2], previdle = start_cpu_total_util[3], previowait = start_cpu_total_util[4], previrq = start_cpu_total_util[5], prevsoftirq = start_cpu_total_util[6], prevsteal = start_cpu_total_util[7], prevguest = start_cpu_total_util[8], prevguest_nice = start_cpu_total_util[9];
  
//   std::cout << prevuser + prevnice + prevsystem << "\n";
//   std::cout << prevuser + prevnice + prevsystem + previdle << "\n";
  long long PrevIdle = previdle + previowait;
  long long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
//   currCPUutil = float(PrevNonIdle)/float(PrevNonIdle + PrevIdle);
  
//   std::cout << previdle << "\n";
  
//   if(prevuser + prevnice + prevsystem + previdle != 0) {
//     currCPUutil = 0;// (prevuser + prevnice + prevsystem) / (prevuser + prevnice + prevsystem + previdle);
//   }
  
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long now = LinuxParser::UpTime();
//   std::cout << now << "\n";
//   std::cout << prevUptime << "\n";
  
  if(now - prevUptime < 3) return currCPUutil;
  prevUptime = now;
  
  start_cpu_total_util = LinuxParser::TotalCpuUtilization();
  
//   vector<std::float> cpu_util_vector = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  long long user = start_cpu_total_util[0], nice = start_cpu_total_util[1], system = start_cpu_total_util[2], idle = start_cpu_total_util[3], iowait = start_cpu_total_util[4], irq = start_cpu_total_util[5], softirq = start_cpu_total_util[6], steal = start_cpu_total_util[7], guest = start_cpu_total_util[8], guest_nice = start_cpu_total_util[9];
  
  long long PrevIdle = previdle + previowait;
  long long Idle = idle + iowait;

  long long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  long long NonIdle = user + nice + system + irq + softirq + steal;

  long long PrevTotal = PrevIdle + PrevNonIdle;
  long long Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  long long totald = Total - PrevTotal;
  long long idled = Idle - PrevIdle;

  float CPU_Percentage = currCPUutil;
  if(totald != 0) { 
    CPU_Percentage = float(totald - idled)/float(totald);
    currCPUutil = CPU_Percentage;
  }
  
  prevuser = user, prevnice = nice, prevsystem = system, previdle = idle, previowait = iowait, previrq = irq, prevsoftirq = softirq, prevsteal = steal, prevguest = guest, prevguest_nice = guest_nice;

  return currCPUutil;
}
