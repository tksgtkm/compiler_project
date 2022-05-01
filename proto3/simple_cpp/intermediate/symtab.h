#ifndef __SYMTAB_H
#define __SYMTAB_H

#include <string>
#include <map>

#include "SymtabEntry.h"

namespace intermediate {
  class Symtab {
  private:
    std::map<std::string SymtabEntry *> contents;

  public:
    SymtabEntry *enter(std::string name) {
      SymtabEntry *entry = new SymtabEntry(name);
      contents[name] = entry;
      return entry;
    }

    SymtabEntry *lookup(std::string name) {
      return contents.find(name) != contents.end() ? contents[name] : nullptr;
    }
  };
} // namespace intermediate

#endif
