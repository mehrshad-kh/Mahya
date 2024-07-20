#include "utility.hpp"

#include <ctime>
#include <stdexcept>

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

bool Utility::isBlank(const QString& str)
{
  return str.trimmed().isEmpty();
}

