#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include <any>
#include <memory>
#include <string>
#include <vector>
#include "lox_callable.h"

class Environment;
class Function;

class LoxFunction: public LoxCallable {
  std::shared_ptr<Function> declaration;
  std::shared_ptr<Environment> closure;

public:
  LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure);
  std::string toString() override;
  int arity() override;
  std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
};

#endif