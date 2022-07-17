#include <string>

#include "format.h"

using std::string;

// Formatting Helper Function
// INPUT: Long int measuring seconds

string Format::ElapsedTime(long seconds) { 
  //mod 60 to account for remaining seconds not accounted for in minutes
  int seconds_ = seconds % 60;
  //divisor to get exact minute,
  int minutes_ = seconds / 60;
  //divisor to get exact hour
  //based on parameter for direct access (rather than routed through other int variables
  int hours_ = seconds / 3600;
  
  // OUTPUT: HH:MM:SS
  //format values as timestamp string
  /*string timeStamp = std::to_string(hours_) + ":" 
    + std::to_string(minutes_) + ":" 
    + std::to_string(seconds_);
  */
  
  //CHANGING TO STREAM PER FEEDBACK
  std::stringstream timeStamp{};
  timeStamp << std::setw(2) << std::setfill('0') << std::to_string(hours_)
     		<< std::setw(1) << ":"
    		<< std::setw(2) << std::setfill('0') << std::to_string(minutes_)
    		<< std::setw(1) << ":"
    		<< std::setw(2) << std::setfill('0') << std::to_string(seconds_);
  
  return timeStamp.str(); 


}