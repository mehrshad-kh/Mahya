#include "utility.hpp"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <stdexcept>

bool Utility::isBlank(const QString& s)
{
  return s.trimmed().isEmpty();
}

bool Utility::isNumber(const QString& s)
{
  std::string std_s = s.toStdString();
  return !std_s.empty() 
    && std::find_if(
        std_s.begin(), 
        std_s.end(), 
        [](const unsigned char& c) { 
        return !std::isdigit(c); }) == std_s.end();
}

std::string Utility::nowInUtc()
{
  char buf[21] = {0};
  std::time_t now = std::time(0);
  std::tm *now_in_utc = std::gmtime(&now);
  if (std::strftime(buf, 42, "%FT%TZ", now_in_utc) == 0) {
    throw std::runtime_error("Cannot get current date time in UTC.");
  }
  return std::string(buf);
}

