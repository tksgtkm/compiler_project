#ifndef __SYMTABENTRY_H
#define __SYMTABENTRY_H

#include <string>

namespace intermediate {
  class SymtabEntry {
  private:
    std::string name;
    double value;

  public:
    SymtabEntry(std::string name) : name(name), value(0.0) {}

    std::string getName() const {return name;}
    double getValue() const {return value;}

    void setValue(const double value) {this->value = value;}
  };
} // namespace intermediate

#endif
