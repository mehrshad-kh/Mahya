#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

#include <QString>

class Utility
{
public:
  /**
   * @throws std::runtime_error
   */
  static std::string nowInUtc();
  static bool isBlank(const QString& str);

private:
};

#endif // UTILITY_HPP
