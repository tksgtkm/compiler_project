#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include "environment.h"
#include "error.h"
#include "expr.h"
#include "lox_callable.h"
#include "runtime_error.h"
#include "stmt.h"



class Interpreter: public ExprVisitor, StmtVisitor{
  
public: std::shared_ptr<Environment> globals{new Environment};
private: std::shared_ptr<Environment> environment = globals;

public:

  Interpreter() {
    globals->defines("clock", std::shared_ptr<NativeClock>{});
  }
  
  void interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    try {
      for (const std::shared_ptr<Stmt>& statement : statements) {
        execute(statement);
      }
    } catch (RuntimeError error) {
      runtimeError(error);
    }
  }

private:
  std::any evaluate(std::shared_ptr<Expr> expr) {
    return expr->accept(*this);
  }

  void execute(std::shared_ptr<Stmt> stmt) {
    stmt->accept(*this);
  }

  void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment) {
    std::shared_ptr<Environment> previous = this->environment;
    try {
      this->environment = environment;

      for (const std::shared_ptr<Stmt>& statement : statements) {
        execute(statement);
      }
    } catch (...) {
      this->environment = previous;
      throw;
    }

    this->environment = previous;
  }

public:

  std::any visitBlockStmt(std::shared_ptr<Block> stmt) override {
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
    return {};
  }

  std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override {
    evaluate(stmt->expression);
    return {};
  }
  
  std::any visitIfStmt(std::shared_ptr<If> stmt) override {
    if (isTruthy(evaluate(stmt->condition))) {
      execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
      execute(stmt->elseBranch);
    }
    return {};
  }

  std::any visitPrintStmt(std::shared_ptr<Print> stmt) override {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << "\n";
    return {};
  }

  std::any visitVarStmt(std::shared_ptr<Var> stmt) override {
    std::any value = nullptr;
    if (stmt->initializer != nullptr) {
      value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, std::move(value));
    return {};
  }

  std::any visitWhileStmt(std::shared_ptr<While> stmt) override {
    while (isTruthy(evaluate(stmt->condition))) {
      execute(stmt->body);
    }

    return {};
  }

  std::any visitAssignExpr(std::shared_ptr<Assign> expr) override {
    std::any value = evaluate(expr->value);
    environment->assign(expr->name, value);
    return value;
  }
  
  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
      case BANG_EQUAL:
        return !isEqual(left, right);
      case EQUAL_EQUAL:
        return !isEqual(left, right);
      case GREATER:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
      case GREATER_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
      case LESS:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
      case LESS_EQUAL:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
      case MINUS:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
      case PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
          return std::any_cast<double>(left) + std::any_cast<double>(right);
        }

        if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
          return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }

        throw RuntimeError{expr->op, "Operands must be two numbers or two strings"};
      case SLASH:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right);
      case STAR:
        checkNumberOperands(expr->op, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    }

    return {};
  }

  // 関数呼び出し
  std::any visitCallExpr(std::shared_ptr<Call> expr) override {
    std::any callee = evaluate(expr->callee);

    std::vector<std::any> arguments;
    for (const std::shared_ptr<Expr>& argument : expr->arguments) {
      arguments.push_back(evaluate(argument));
    }

    std::shared_ptr<LoxCallable> function;

    if (callee.type() == typeid(std::shared_ptr<LoxFunction>)) {
      function = std::any_cast<std::shared_ptr<LoxFunction>>(callee);
    } else {
      throw RuntimeError{expr->paren, "Can only functions and classes"};
    }
    
  } 

  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override {
    return evaluate(expr->expression);
  }

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override {
    return expr->value;
  }

  std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override {
    std::any left = evaluate(expr->left);

    if (expr->op.type == OR) {
      if (isTruthy(left))
        return left;
    } else {
      if (!isTruthy(left))
        return left;
    }

    return evaluate(expr->right);
  }

  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override {
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
      case BANG:
        return !isTruthy(right);
      case MINUS:
        checkNumberOperand(expr->op, right);
        return -std::any_cast<double>(right);
    }

    return {};
  }

  std::any visitVariableExpr(std::shared_ptr<Variable> expr) override {
    return environment->get(expr->name);
  }

private:

  void checkNumberOperand(const Token& op, const std::any& operand) {
    if (operand.type() == typeid(double))
      return;

    throw RuntimeError{op, "Operand must be a number."};
  }

  void checkNumberOperands(const Token& op, const std::any& left, const std::any& right) {
    if (left.type() == typeid(double) && right.type() == typeid(double)) {
      return;
    }

    throw RuntimeError{op, "Operands must be numbers"};
  }

  bool isTruthy(const std::any& object) {
    if (object.type() == typeid(nullptr))
      return false;

    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object);
    }

    return true;
  }

  bool isEqual(const std::any& a, const std::any& b) {
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
      return true;
    }

    if (a.type() == typeid(nullptr))
      return false;

    if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) {
      return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }

    if (a.type() == typeid(double) && b.type() == typeid(double)) {
      return std::any_cast<double>(a) == std::any_cast<double>(b);
    }

    if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
      return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
  }

  std::string stringify(const std::any& object) {
    if (object.type() == typeid(nullptr))
      return "nil";

    if (object.type() == typeid(double)) {
      std::string text = std::to_string(std::any_cast<double>(object));
      if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0') {
        text = text.substr(0, text.length() - 2);
      }
      return text;
    }

    if (object.type() == typeid(std::string)) {
      return std::any_cast<std::string>(object);
    }

    if (object.type() == typeid(bool)) {
      return std::any_cast<bool>(object) ? "true" : "false";
    }

    return "Error in stringify: object type not recognized.";
  }
};

#endif