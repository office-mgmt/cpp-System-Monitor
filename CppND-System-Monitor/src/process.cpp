#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//This class needs a mechanism to accept a pid, so a constructor will be needed
//When creating a Process object, process object will require a pid, so the value must accepted and committed to a private variable
Process::Process(int givenPid){
  //when creating Process object, given pid will be stored in processPid private member
 processPid = givenPid;
  //This constructor can also include Jiffies values required for CPUutilization
 activeJiffies = LinuxParser::ActiveJiffies(givenPid);
 pidUpTime = float(LinuxParser::UpTime(processPid)); //must be float, so casted to one
}

//Return this process's ID
int Process::Pid() { 
  //returns private member variable (this process's PID)
  return processPid; 
}

//Return this process's CPU utilization
//must also be const for operator below to compile
float Process::CpuUtilization() const{ 
  //total utilization is the pid of ActiveProcesses(jiffies) / pidUpTime
  float processUtilization = activeJiffies / pidUpTime;
  return processUtilization; 
}

//Following convention similar to system.cpp, each below method leverages the LinuxParser functions
//Return the command that generated this process
string Process::Command() { 
  //returning Command from LinuxParser based on private processPid
  return LinuxParser::Command(processPid); 
}

//Return this process's memory utilization
string Process::Ram() { 
  //returning Command from LinuxParser based on private processPid
  return LinuxParser::Ram(processPid); 
}

//Return the user (name) that generated this process
string Process::User() { 
  //returning User from LinuxParser based on private processPid
  return LinuxParser::User(processPid); 
}

//Return the age of this process (in seconds)
long int Process::UpTime() { 
  //returning UpTime from LinuxParser based on private processPid
  return LinuxParser::UpTime(processPid); 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  //accepts reference to memory address of parameter a
  //operator to compare two Process objects based on resource usage
  //will use CpuUtilization since local computation occurs there
  return CpuUtilization() < a.CpuUtilization(); 
}