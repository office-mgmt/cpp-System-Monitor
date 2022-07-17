#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  //See src/processor.cpp
  // Declare any necessary private members
  //Need to account for count of active,idle, & total system ticks, both current & previous 
  //will calculate duration based on delta of prior & present values
  
  
 private:
  //'snapshot' variables
  long temp_activeJiffies;
  long temp_totalJiffies;
};

#endif