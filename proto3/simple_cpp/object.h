#ifndef __OBJECT_H
#define __OBJECT_H

#include <string>
#include <algorithm>

class Object {
public:
  long        L;
  double      D;
  std::string S;
  bool        B;

  Object()                  : L(0),     D(0.0),   S(""),    B(false) {}
  Object(long value)        : L(value), D(0.0),   S(""),    B(false) {}
  Object(double value)      : L(0),     D(value), S(""),    B(false) {}
  Object(std::string value) : L(0),     D(0.0),   S(value), B(false) {}
  Object(bool value)        : L(0),     D(0.0),   S(""),    B(value) {}
};

inline std::string toUpperCase(std::string str) {
  std::string upper_case(str);
  transform(upper_case.begin(), upper_case.end(),
            upper_case.begin(), ::toupper);
  return upper_case;
}

inline std::string toLowerCase(std::string str) {
  std::string lower_case(str);
  transform(lower_case.begin(), lower_case.end(),
            lower_case.begin(), ::tolower);
  return lower_case;
}

#endif
