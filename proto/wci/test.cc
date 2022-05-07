#include <iostream>
#include <string>
#include <boost/any.hpp>

int main() {
  boost::any x = 1;
  x = std::string("Hello");

  if (x.type() == typeid(std::string)) {
    std::cout << boost::any_cast<std::string>(x) << std::endl;
  } else {
    std::cout << "not string" << std::endl;
  }
}
