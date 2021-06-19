#include <string>

#include "format.h"

using namespace std;

string pad0(string s, unsigned int width) {
  if (s.length() < width)
    return '0' + s;
  else
    return s;
}

string Format::ElapsedTime(long seconds) { 
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = (seconds % 3600) % 60;
  
  string hourStr = std::to_string(hours);
  string minuteStr = std::to_string(minutes);
  string secondStr = std::to_string(secs);
  
  string s = pad0(hourStr, 2) + ":" + pad0(minuteStr, 2) + ":" + pad0(secondStr, 2);
  return s;
} // Format::ElapsedTime()