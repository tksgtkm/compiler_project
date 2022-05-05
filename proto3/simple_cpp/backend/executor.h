#ifndef __EXECUTOR_H
#define __EXECUTOR_H

#include <string>
#include <vector>
#include <set>

#include "../Object.h"
#include "../intermediate/symtab.h"
#include "../intermediate/node.h"

namespace backend {
  using namespace intermediate;

  class Executor {
  private:
    int lineNumber;
    Symtab *symtab;

  public:
    static void initialize();

    Executor(Symtab *symtab) : lineNumber(0), symtab(symtab) {}

    Object visit(Node *node);

  private:
    static std::set<NodeType> singletons;
    static std::set<NodeType> relationals;

    Object visitProgram(Node *programNode);
    Object visitStatement(Node *statementNode);
    Object visitCompound(Node *compoundNode);
    Object visitAssign(Node *assignNode);
    Object visitLoop(Node *loopNode);
    Object visitTest(Node *testNode);
    Object visitWrite(Node *writeNode);
    Object visitWriteln(Node *writelnNode);
    Object visitExpression(Node *expressionNode);
    Object visitVariable(Node *variableNode);
    Object visitIntegerConstant(Node *integerConstantNode);
    Object visitRealConstant(Node *realConstantNode);
    Object visitStringConstant(Node *stringConstantNode);

    void printValue(std::vector<Node *> children);
    void runtimeError(Node *node, std::string message);
  };
} // namespace backend

#endif
