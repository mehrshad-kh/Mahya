#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

#include <QString>

class Utility
{
public:
  static bool isBlank(const QString& s);
  static bool isNumber(const QString& s);

  /**
   * @throws std::runtime_error
   */
  static std::string nowInUtc();

private:
};

#endif // UTILITY_HPP
