#ifndef STMT_H
#define STMT_H

#include <any>
#include <memory>
#include <utility>
#include <vector>
#include "token.h"
#include "expr.h"

struct Block;
struct Expression;
struct Print;
struct Var;

struct StmtVisitor {
  virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
  virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
  virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
  virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
  virtual ~StmtVisitor() = default;
};

struct Stmt {
  virtual std::any accept(StmtVisitor& visitor) = 0;
};

#endif