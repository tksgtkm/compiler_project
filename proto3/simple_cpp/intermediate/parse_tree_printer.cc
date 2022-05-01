#include <iostream>

#include "node.h"
#include "parse_tree_printer.h"

namespace intermediate {
  const std::string ParseTreePrinter::INDENT_SIZE = "    ";

  void ParseTreePrinter::print(Node *node) {
    line += indentation;
    line += "<" + NODE_TYPE_STRINGS[(int) node->type];

    if (node->type == PROGRAM)               line += " " + node->text;
    else if (node->type == VARIABLE)         line += " " + node->text;
    else if (node->type == INTEGER_CONSTANT) line += " " + std::to_string(node->value.L);
    else if (node->type == REAL_CONSTANT)    line += " " + std::to_string(node->value.D);
    else if (node->type == STRING_CONSTANT)  line += " '" + node->value.S + "'";
    if (node->lineNumber > 0)                line += " line " + std::to_string(node->lineNumber);

    std::vector<Node *> children = node->children;
    if (children.size() > 0) {
      line += ">";
      printLine();
      printChildren(children);
      line += indentation;
      line += "</" + NODE_TYPE_STRINGS[(int) node->type] + ">";
    } else {
      line += " />";
    }

    printLine();
  }

  void ParseTreePrinter::printChildren(std::vector<Node *> children) {
    std::string saveIndentation = indentation;
    indentation += INDENT_SIZE;
    for (Node *child : children)
      print(child);
    indentation = saveIndentation;
  }

  void ParseTreePrinter::printLine() {
    std::cout << line << std::endl;
    line = "";
  }
} // namespace intermediate
