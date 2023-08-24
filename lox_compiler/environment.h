#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "error.h"
#include "token.h"

class Environment: public std::enable_shared_from_this<Environment> {
  std::shared_ptr<Environment> enclosing;
  std::map<std::string, std::any> values;

public:
  Environment(): enclosing{nullptr} {}

  Environment(std::shared_ptr<Environment> enclosing): enclosing{std::move(enclosing)} {}

  std::any get(const Token& name) {
    auto elem = values.find(name.lexeme);
    if (elem != values.end()) {
      return elem->second;
    }

    if (enclosing != nullptr)
      return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
  }

  void assign(const Token& name, std::any value) {
    auto elem = values.find(name.lexeme);
    if (elem != values.end()) {
      elem->second = std::move(value);
      return;
    }

    if (enclosing != nullptr) {
      enclosing->assign(name, std::move(value));
      return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
  }

  void define(const std::string& name, std::any value) {
    values[name] = std::move(value);
  }
};

#endif