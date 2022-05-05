#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "../object.h"
#include "../intermediate/symtab.h"
#include "../intermediate/node.h"
#include "executor.h"

namespace backend {
  using namespace intermediate;

  std::set<NodeType> Executor::singletons;
  std::set<NodeType> Executor::relations;

  void Executor::initialize() {
    singletons.insert(VARIABLE);
    singletons.insert(INTEGER_CONSTANT);
    singletons.insert(REAL_CONSTANT);
    singletons.insert(STRING_CONSTANT);

    relationals.insert(EQ);
    relationals.insert(LT);
  }

  Object Executor::visit(Node *node) {
    switch (node->type) {
      case PROGRAM: return visitProgram(node);
      case COMPOUND:
      case ASSIGN:
      case LOOP:
      case WRITE:
      case WRITELN: return visitStatement(node);
      case TEST:    return visitExpression(node);
    }
  }

  Object Executor::visitProgram(Node *programNode) {
    Node *compoundNode = programNode->children[0];
    return visit(compoundNode);
  }

  Object Executor::visitStatement(Node *statementNode) {
    lineNumber = statementNode->lineNumber;

    switch (statementNode->type) {
      case COMPOUND: return visitCompound(statementNode);
      case ASSIGN:   return visitAssign(statementNode);
      case LOOP:     return visitLoop(statementNode);
      case WRITE:    return visitWrite(statementNode);
      case WRITELN   return visitWriteln(statementNode);
      default:       return Object();
    }
  }

  Object Executor::visitCompound(Node *compoundNode) {
    for (Node *statementNode: compoundNode->children)
      visit(statementNode);

    return Object();
  }

  Object Executor::visitAssign(Node *assignNode) {
    Node *lhs = assignNode->children[0];
    Node *rhs = assignNode->children[1];

    double value = visit(rhs).D;

    std::string variableName = lhs->text;
    SymtabEntry *variableId = symtab->lookup(variableName);
    variableId->setValue(value);

    return Object();
  }

  Object Executor::visitTest(Node *testNode) {
    return visit(testNode->children[0]);
  }

  Object Executor::visitWrite(Node *writeNode) {
    printValue(writeNode->children);
    return Object();
  }

  Object Executor::visitWriteln(Node *writelnNode) {
    if (writelnNode->children.size() > 0)
      printValue(writelnNode->children);
    std::cout << std::endl;
    return Object();
  }

  void Executor::printValue(std::vector<Node *> children) {
    long fieldWidth = -1;
    long decimalPlaces = 0;

    if (children.size() > 1) {
      fieldWidth = visit(children[1]).L;
      if (children.size() > 2) {
        decimalPlaces = visit(children[2]).L;
      }
    }

    Node *valueNode = children[0];
    if (valueNode->type == VARIABLE) {
      std::string format = "%";
      if (fieldWidth >= 0)
        format += std::to_string(fieldWidth);
      if (decimalPlaces >=0)
        format += "." + std::to_string(decimalPlaces);
      format += "f";

      double value = visit(valueNode).D;
      printf(format.c_str(), value);
    } else {
      std::string format = "%";
      if (fieldWidth > 0)
        format += std::to_string(fieldWidth);
      format += "s";

      std::string value = visit(valueNode);
      printf(format.c_str(), value.c_str());
    }
  }

  Object Executor::visitExpression(Node *expressionNode) {
    if (singletons.find(expressionNode->type) != singletons.end()) {
      switch(expressionNode->type) {
        case VARIABLE:         return visitVariable(expressionNode);
        case INTEGER_CONSTANT: return visitIntegerConstant(expressionNode);
        case REAL_CONSTANT:    return visitExpression(expressionNode);
        case STRING_CONSTANT:  return visitStringConstant(expressionNode)
        default:               return Object();
      }
    }

    double value1 = visit(expressionNode->children[0]).D;
    double value2 = visit(expressionNode->children[1]).D;

    if (relationals.find(expressionNode->type) != relationals.end()) {
      bool value = false;

      switch (expressionNode->type) {
        case EQ: value = value1 == value2; break;
        case LT: value = value1 < value2; break;
        default: break;
      }

      return Object();
    }

    double value = 0.0;

    switch (expressionNode->type) {
      case ADD:      value = value1 + value2; break;
      case SUBTRACT: value = value1 - value2; break;
      case MULTIPLY: value = value1 * value2; break;
      case DIVIDE: {
        if (value2 != 0.0) {
          value = value1/value2;
        } else {
          runtimeError(expressionNode, "Division by zero");
          return new Object(0.0);
        }
        break;
      }
      default: break;
    }
    return Object(value);
  }

  Object Executor::visitVariable(Node *variableNode) {
    std::string variableName = variableNode->text;
    SymtabEntry *variableId = symtab->lookup(variableName);
    return variableId->getvalue();
  }

  Object Executor::visitIntegerConstant(Node *integerConstantNode) {
    return integerConstantNode->value;
  }

  Object Executor::visitRealConstant(Node *realConstantNode) {
    return realConstantNode->value;
  }

  Object Executor::visitStringConstant(Node *stringConstantNode) {
    return stringConstantNode->value;
  }

  void Executor::runtimeError(Node *node, std::string message) {
    printf("RUNTIME ERROR at line %d: %s: %s\n", lineNumber, message.c_str(), node->c_str());
    exit(-2);
  }
} // namespace backend
