#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include "Object.h"

bool Options::intermediate = false;
bool Options::xref         = false;

std::string to_upper(const std::string str) {
  std::string upper_case(str);
  std::transform(upper_case.begin(), upper_case.end(), upper_case.begin(), std::toupper);
  return upper_case;
}

std::string to_lower(const std::string str) {
  std::string lower_case(str);
  std::transform(lower_case.begin(), lower_case.end(), lower_case.begin(), std::tolower);
  return lower_case;
}

std::string stringify(const Object& obj) {
  auto& objtype = obj.type();
  if (objtype == typeid(int))
    return std::to_string(boost::any_cast<int>(obj));
  else if (objtype == typeid(float))
    return std::to_string(boost::any_cast<float>(obj));
  else if (objtype == typeid(std::string))
    return boost::any_cast<std::string>(obj);
  else if (objtype == typeid(char))
    return std::to_string(boost::any_cast<char>(obj));
  else
    return "";
}
