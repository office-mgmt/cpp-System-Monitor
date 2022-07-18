#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  //using vector for container, populated by vector<int> Pids from LinuxParser
  vector<int> pidSet = LinuxParser::Pids();
  //clear system processes
  processes_.clear();
  //iterate through all pids
  for (int pid : pidSet){
   Process currentProcess(pid);
    //push process to processes
   processes_.push_back(currentProcess);
  }
  
  //FEEDBACK - processes display to console, now need to display by greatest CPU usage
  //Overloads operator< defined in process.cpp to compare CPUutilization
  std::sort(processes_.begin(), processes_.end(), std::less<Process>());
  
  return processes_; 

}

//Return the system's kernel identifier (string)
std::string System::Kernel() { 
    //returning Kernel from LinuxParser
  return LinuxParser::Kernel(); 
}

//Return the system's memory utilization
float System::MemoryUtilization() { 
    //returning MemoryUtilization from LinuxParser
  return LinuxParser::MemoryUtilization(); 
}

//Return the operating system name
std::string System::OperatingSystem() { 
  //returning Operating System count from LinuxParser
  return LinuxParser::OperatingSystem(); 
}

//Return the number of processes actively running on the system
int System::RunningProcesses() { 
  //returns RunningProcesses count from LinuxParser
  return LinuxParser::RunningProcesses();
}

//Return the total number of processes on the system
int System::TotalProcesses() { 
  //returns TotalProcesses count from LinuxParser
  return LinuxParser::TotalProcesses();
}

//Return the number of seconds since the system started running
long int System::UpTime() { 
  //returns System UpTime from LinuxParser
  return LinuxParser::UpTime(); 
}
