#include "processor.h"
#include "linux_parser.h"

//Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  //Utilization is duration of activeJiffies / duration of totalJiffies (active + idle)
  
  //Need to capture a temporary value, done with private variables in processor class
  
  //fetch active jiffies from linux_parser (just current active)
  float activeJiffies = float(LinuxParser::ActiveJiffies());

  //fetch total jiffies (include current total of active & idle jiffies)
  float totalJiffies = float(LinuxParser::Jiffies());
  
  //Utilization = (activeJiffies(current) - activeJiffies(prior)) / (totalJiffies(current) - totalJiffies(prior)
  float utilization = (activeJiffies - temp_activeJiffies) / (totalJiffies - temp_totalJiffies);
  
  //must also capture private variables in processor class
  //must be done AFTER utilization assignment to ensure values don't yield a 0 delta
  temp_activeJiffies = LinuxParser::ActiveJiffies();
  temp_totalJiffies = LinuxParser::Jiffies();
  
  
  //final return of total utilization (as float)
  return utilization;
  
}



