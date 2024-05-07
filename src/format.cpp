#include <string>
#include <chrono> 

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
	std::chrono::seconds total_uptime (seconds);
    std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours> (total_uptime);
    total_uptime -= hours;
      
    std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes> (total_uptime);
    total_uptime -= minutes;
    
    string result = "";
    result = to_string(hours.count()) + ":";
    result += to_string(minutes.count()) + ":";
    result += to_string(total_uptime.count());
  
    return result;
}
