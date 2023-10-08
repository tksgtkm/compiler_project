#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include <any>
#include <memory>
#include <string>
#include <vector>
#include "lox_callable.h"

class Environment;
class Function;
class LoxInstance;

class LoxFunction: public LoxCallable {
  std::shared_ptr<Function> declaration;
  std::shared_ptr<Environment> closure;

  bool isInitializer;

public:
  LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure, bool isInitializer);

  std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> initstance);
  std::string toString() override;
  int arity() override;
  std::any call(Interpreter& interpreter, std::vector<std::any> arguments) override;
};

#endif