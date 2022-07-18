#include <dirent.h>
#include <unistd.h>
#include <sstream>
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
  string os, kernel, version;
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

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  //floats to notate total Memory & what's free
  float totalMemory, freeMemory;
  // string values to match with when parsing
  string line, value;
  //create new input file stream
  std::ifstream memStream(kProcDirectory + kMeminfoFilename);
  //check is stream is open
  if (memStream.is_open()){
   //loop 
    while (std::getline(memStream, line)) {
     //assign line from linestream to value for matching
      std::istringstream iLineStream(line);
      iLineStream >> value;
     //if read value is the total itself, save to total
      if(value =="MemTotal:"){
        //write as totalMemory
        iLineStream >> totalMemory;
        //else if the read value is MemAvailable
      } else if (value == "MemAvailable:"){
        //write as freeMemory
        iLineStream >> freeMemory;
        //ensures file doesn't continue to read after values for free & total are set
        break;
      }
    }
  }
  
  //return how much memory is being USED (not free)
  return (totalMemory - freeMemory) / totalMemory;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  //long for system uptime
  long uptime;
  //string values for line and time parsed from file
  string line, parsedTime;
  //new input file stream
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  //check if stream is open
  if (fileStream.is_open()){
   //use getLine to pull line from filestream
    std::getline(fileStream, line);
    std::istringstream inputStream(line);
    //write values from kUptiemFilename to parsedTime
    inputStream >> parsedTime;
  }
  //store parsedTime(string) as long into final uptime value
  uptime = stol(parsedTime); 
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  //Jiffies are sum of all Active & Idle jiffies, so return sum of the below functions
    return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  //variables for line, parsedValue, and string vector to hold all parsed values
  string line, parsedValue;
  vector<string> parsedValues;
  //new input file stream (path includes specific pid)
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);
  //check if stream is open
  if (fileStream.is_open()){
   //use getLine to pull line from filestream
    std::getline(fileStream, line);
    std::istringstream inputStream(line);
    //continually push values from stream to vector of strings
    while( inputStream >> parsedValue ){
      parsedValues.push_back(parsedValue);
    };
  }
  //PID Indexes: User (13), Kernel(14), ChildUser(15), ChildKernel(16);
  return stol(parsedValues[13]) + stol(parsedValues[14]) + stol(parsedValues[15]) + stol(parsedValues[16]); 
 
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  //vector of strings to retain return type from CpuUtilization
  vector<string> currentJiffies = CpuUtilization();
  //return active CPU enum states (must be longs to fit return type)
  
  return stol(currentJiffies[CPUStates::kUser_]) + 
    stol(currentJiffies[CPUStates::kNice_]) +
    stol(currentJiffies[CPUStates::kSystem_]) + 
    stol(currentJiffies[CPUStates::kIRQ_]) +
    stol(currentJiffies[CPUStates::kSoftIRQ_]) + 
    stol(currentJiffies[CPUStates::kSteal_]);
  
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  //vector of strings to retain return type from CpuUtilization
  vector<string> currentJiffies = CpuUtilization();
  //return active CPU enum states (must be longs to fit return type)
  return stol(currentJiffies[CPUStates::kIdle_]) + 
    stol(currentJiffies[CPUStates::kIOwait_]);
  
}

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  //string variables for line, parsed value to match with, and parsed value to commit to vector of strings
  string line, parsedValue;
  //vector of strings to hold values read from stream
  vector<string> parsedValues;
  //new input file stream
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  //check if stream is open
  if (fileStream.is_open()){
   //use getLine to pull line from filestream
   // std::getline(fileStream, line);
   //FEEDBACK: looping getline action
    while (std::getline(fileStream, line)){
      std::istringstream inputStream(line);
      //input fileStream to parsedValue
      inputStream >> parsedValue;
      //iteratively push values from stream to vector of strings
    while( inputStream >> parsedValue ){
      parsedValues.push_back(parsedValue);
    };
    };
  }
  //FEEDBACK: to resolve compiler error
  return parsedValues; 
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  //variables for lines, parsed values to match with, and count of total processes (as string, will cast later)
  string line, parsedValue, totalProcesses;
  //new input file stream
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  //check if stream is open
  if (fileStream.is_open()){

   //use getLine to pull line from filestream (looping this with while)
    //std::getline(fileStream, line);
    while (std::getline(fileStream, line)){
      std::istringstream inputStream(line);
      //parse inputStream to parsedValue and totalProcesses
      while (inputStream >> parsedValue >> totalProcesses){
        //if parsedValue matches "processes" from file
        if(parsedValue == "processes"){
          //return the total of all processes as an int to adhere to function
          return stoi(totalProcesses);
          break;
        }
      };
    };
  }
 //FEEDBACK To resolve warning
return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
//variables for lines, parsed values to match with, and count of all running processes (as string, will cast later)
  string line, parsedValue, runningProcesses;
  //new input file stream
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  //check if stream is open
  if (fileStream.is_open()){

   //use getLine to pull line from filestream (looping this with while)
    //std::getline(fileStream, line);
    while (std::getline(fileStream, line)){
      std::istringstream inputStream(line);
      //parse inputStream to parsedValue and totalProcesses
      while (inputStream >> parsedValue >> runningProcesses){
        //if parsedValue matches "processes" from file
        if(parsedValue == "procs_running"){
          //return the total of all processes as an int to adhere to function
          return stoi(runningProcesses);
          break;
        }
      };
    };
  }
 //FEEDBACK To resolve warning
return 0;
}


// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  //variable for line 
  string line;
  //new input file stream to path
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kCmdlineFilename);
   if (fileStream.is_open()){
     //get command from line
     std::getline(fileStream, line);
     //return value of pid at line
     return line;
   }
  //Else case, return default
  return line;

}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  //variable for line, value to match, and parsedValue
  string line, parsedValue;
  //long to hold ram in GB
  long ramGB;
  //new input file stream to path
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);
   if (fileStream.is_open()){
     while (fileStream >> parsedValue){
        //FEEDBACK implementation: check for "VmRSS" & if active write
       //Was told by reviewer that VmSize will show memory usage larger than Physical RAM size
       if (parsedValue == "VmRSS" && fileStream >> parsedValue){
       // take parsedValue of available Ram, divide by 1024 for amount in GBs
         ramGB = stol(parsedValue) / 1024;
         //return ramGB as string to adhere to return type
         return to_string(ramGB);
         break;
       }
     }
     
   }
  //FEEDBACK: Removes warning and returns valid value if 0
  return "0";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  //variable for line and parsedValue
  string line, parsedValue;
  //new input file stream to path
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatusFilename);
   if (fileStream.is_open()){
     while (fileStream >> parsedValue){
       //check for Uid & if active write
       if (parsedValue == "Uid:" && fileStream >> parsedValue){
       // take parseValue of Uid and return as string
         return parsedValue;
         break;
       }
     }
     
   }
 return "";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  //Will need to compare uid of file with uid 
  //get Uid
  string uId = Uid(pid);
  //strings for line, parsed value
  string line, fileUid, parsedValue;
  //Create default Username case
  string userName{"GUEST"};
  //new input file stream to path
  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()){
   while (std::getline(fileStream, line)){
     //string replace to remove : from parsedLine
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream inputStream(line);
	//assign value from stream
    inputStream>>parsedValue>>fileUid;
     //compare this uid and Uid from file
     if (fileUid == uId){
       //update
      userName = parsedValue;
       //exit loop
      break;
     }
   }
  }
  //else case, return default username
  return userName;
}

//Read and return the uptime of a process (when receiving pid)
long LinuxParser::UpTime(int pid) { 

  //string values for line and time 
  string line, parsedTime;
  //long to hold upTime
  long upTime = 0.0f;
  //new input file stream
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  //check if stream is open
  if (fileStream.is_open()){
   //use getLine to pull line from filestream
    std::getline(fileStream, line);
    std::istringstream inputStream(line);
    //loop while adhering to range
    for (int i = 0; i <= 21; ++i){
     inputStream >> parsedTime; 
    }
    //FEEDBACK implementation - assign upTime to parsedPID /
    //upTime = UpTime() - stol(parsedTime) / sysconf(_SC_CLK_TCK);
    if (upTime <= 0.0f){
      upTime = 0.0f;
    } else{
    	upTime = UpTime() - stol(parsedTime) / sysconf(_SC_CLK_TCK);
    }
    
  }
 return upTime;
}
