#ifndef __TOKEN_H
#define __TOKEN_H

#include <string>
#include <map>

#include "../object.h"
#include "source.h"

namespace frontend {
  enum class TokenType {
    PROGRAM, BEGIN, END, REPEAT, UNTIL, WRITE, WRITELN,
    PERIOD, COLON, COLON_EQUALS, SEMICOLON,
    PLUS, MINUS, STAR, SLASH, LPAREN, RPAREN,
    EQUALS, LESS_THAN,
    IDENTIFIER, INTEGER, REAL, STRING, END_OF_FILE, ERROR
  };

  static const std::string TOKEN_TYPE_STRINGS[] = {
    "PROGRAM", "BEGIN", "END", "REPEAT", "UNTIL", "WRITE", "WRITELN",
    "PERIOD", "COLON", "COLON_EQUALS", "SEMICOLON",
    "PLUS", "MINUS", "STAR", "SLASH", "LPAREN", "RPAREN",
    "EQUALS", "LESS_THAN",
    "IDENTIFIER", "INTEGER", "REAL", "STRING", "END_OF_FILE", "ERROR"
  };

  constexpr TokenType PROGRAM = TokenType::PROGRAM;
  constexpr TokenType BEGIN = TokenType::BEGIN;
  constexpr TokenType END = TokenType::END;
  constexpr TokenType REPEAT = TokenType::REPEAT;
  constexpr TokenType UNTIL = TokenType::UNTIL;
  constexpr TokenType WRITE = TokenType::WRITE;
  constexpr TokenType WRITELN = TokenType::WRITELN;
  constexpr TokenType PERIOD = TokenType::PERIOD;
  constexpr TokenType COLON = TokenType::COLON;
  constexpr TokenType COLON_EQUALS = TokenType::COLON_EQUALS;
  constexpr TokenType SEMICOLON = TokenType::SEMICOLON;
  constexpr TokenType PLUS = TokenType::PLUS;
  constexpr TokenType MINUS = TokenType::MINUS;
  constexpr TokenType STAR = TokenType::STAR;
  constexpr TokenType SLASH = TokenType::SLASH;
  constexpr TokenType LPAREN = TokenType::LPAREN;
  constexpr TokenType RPAREN = TokenType::RPAREN;
  constexpr TokenType EQUALS = TokenType::EQUALS;
  constexpr TokenType LESS_THAN = TokenType::LESS_THAN;
  constexpr TokenType IDENTIFIER = TokenType::IDENTIFIER;
  constexpr TokenType INTEGER = TokenType::INTEGER;
  constexpr TokenType REAL = TokenType::REAL;
  constexpr TokenType STRING = TokenType::STRING;
  constexpr TokenType END_OF_FILE = TokenType::END_OF_FILE;
  constexpr TokenType ERROR = TokenType::ERROR;

  class Token {
  private:
    static std::map<std::string, TokenType> reservedWords;

  public:
    static void initialize();

    TokenType type;
    int lineNumber;
    std::string text,
    Object value;

    Token(char firstChar) : type(ERROR), lineNumber(0), text("") {
      text += firstChar;
    }

    static Token *Word(char firstChar, Source *source);

    static Token *Number(char firstChar, Source *source);

    static Token *String(char firstChar, Source *source);

    static Token *SpecialSymbol(char firstChar, Source *source);

    static void tokenError(Token *token, std::string message);
  };
} // namespace frontend

#endif
