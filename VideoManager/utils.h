#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>

const std::string WHITESPACE = " \n\r\t\f\v";

inline void rtrim(std::string &s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  s = (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

#endif // _UTILS_H_