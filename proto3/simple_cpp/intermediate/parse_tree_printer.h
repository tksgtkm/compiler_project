#ifndef __PARSE_TREE_PRINTER_H
#define __PARSE_TREE_PRINTER_H

#include <string>
#include <vector>

#include "Node.h"

namespace intermediate {
  class ParseTreePrinter {
  private:
    static const std::string INDENT_SIZE;
    std::string indentation;
    std::string line;

  public:
    ParseTreePrinter() : indentation(""), line("") {}
    void print(Node *node);

  private:
    void printChildren(std::vector<Node *> children);

    void printLine();
  };
} // namespace intermediate

#endif
