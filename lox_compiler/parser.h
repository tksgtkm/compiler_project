#ifndef PARSER_H
#define PARSER_H

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "error.h"
#include "expr.h"
#include "token.h"
#include "token_type.h"

class Parser {
  struct ParseError: public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  const std::vector<Token> &tokens;
  int current = 0;

public:
  Parser(const std::vector<Token> &tokens): tokens{tokens} {}

  std::shared_ptr<Expr> parse() {
    try {
      return expression();
    } catch (ParseError error) {
      return nullptr;
    }
  }

private:
  std::shared_ptr<Expr> expression() {
    return;
  }

  template<class... T>
  bool match(T... type) {
    assert((... && std::is_same_v<T, TokenType>));

    if ((... || check(type))) {
      advance();
      return true;
    }

    return false;
  }

  Token consume(TokenType type, std::string_view message) {
    if (check(type))
      return advance();
    throw error(peek(), message);
  }

  bool check(TokenType type) {
    if (isAtEnd())
      return false;
    return peek().type == type;
  }

  Token advance() {
    if (!isAtEnd())
      current++;
    return previous();
  }

  Token advance() {
    if (!isAtEnd())
      ++current;
    return previous();
  }

  bool isAtEnd() {
    return peek().type == END_OF_FILE;
  }

  Token peek() {
    return tokens.at(current);
  }

  Token previous() {
    return tokens.at(current - 1);
  }

  ParseError error(const Token& token, std::string_view message) {
    ::error(token, message);
    return ParseError{""};
  }
};

#endif