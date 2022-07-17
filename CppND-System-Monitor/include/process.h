#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);							//Added constructor
  int Pid() ;                               //  See src/process.cpp
  std::string User() ;                      // See src/process.cpp
  std::string Command() ;                   // See src/process.cpp
  float CpuUtilization() const;                  // See src/process.cpp
  std::string Ram() ;                       // See src/process.cpp
  long int UpTime() ;                       // See src/process.cpp
  bool operator<(Process const& a) const;  //See src/process.cpp

  // Declare any necessary private members
 private:
  int processPid;								  //added private member pid
  long activeJiffies;						  //added private member activeJiffies to localize activeJiffies (per pid)
  long pidUpTime;						  //added private member activeJiffies to localize upTime (per pid)
};

#endif