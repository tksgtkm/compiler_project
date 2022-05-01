#include <string>
#include <map>
#include <ctype>

#include "../Object.h"
#include "token.h"

namespace frontend {
  std::map<std::string, TokenType> Token::reservedWords;

  void Token::initialize() {
    reservedWords["PROGRAM"] = PROGRAM;
    reservedWords["BEGIN"] = BEGIN;
    reservedWords["END"] = END;
    reservedWords["REPEAT"] = REPEAT;
    reservedWords["UNTIL"] = UNTIL;
    reservedWords["WRITE"] = WRITE;
    reservedWords["WRITELN"] = WRITELN;
  }

  Token *Token::Word(char firstChar, Source *source) {
    Token *token = new Token(firstChar);
    token->lineNumber = source->lineNumber();

    for (char ch = source->nextChar(); isalnum(ch); ch = source->nextChar()) {
      token->text += ch;
    }

    std::string upper = toUpperCase(token->text);

    if (Token::reservedWords.find(upper) != Token::reservedWords.end()) {
      token->type = Token::reservedWords[upper];
    } else {
      token->type = TokenType::IDENTIFIER;
    }

    return token;
  }

  Token *Token::Number(char firstChar, Source *source) {
    Token *token = new Token(firstChar);
    int pointCount = 0;

    for (char ch = source->nextChar(); std::isdigit(ch) || (ch == '.'); ch = source->nextChar()) {
      if (ch == '.')
        pointCount++;
      token->next += ch;
    }

    if (pointCount == 0) {
      token->type = TokenType::INTEGER;
      token->value.L = std::stol(token->text);
      token->value.D = token->value.L;
    } else if (pointCount == 1) {
      token->type = TokenType::REAL;
      token->value.D = std::stod(token->text);
    } else {
      tokenErrpr(token, "Invalid number");
    }

    return token;
  }

  Token *Token::String(char firstChar, Source *source) {
    Token *token = new Token(firstChar);

    for (char ch = source->nextChar(); ch != '\''; ch = source->nextChar()) {
      token->text += ch;
    }

    token->text += '\'';
    source->nextChar();

    token->type = TokenType::STRING;

    token->value.S = tooken->text.substr(1, token->text.length() - 2);

    return token;
  }

  Token *Token::SpecialSymbol(char firstChar, Source *source) {
    Token *token = new Token(firstChar);

    switch(firstChar) {
      case '.' : token->type = TokenType::PERIOD;    break;
      case ';' : token->type = TokenType::SEMICOLON; break;
      case '+' : token->type = TokenType::PLUS;      break;
      case '-' : token->type = TokenType::MINUS;     break;
      case '*' : token->type = TokenType::STAR;      break;
      case '/' : token->type = TokenType::SLASH;     break;
      case '=' : token->type = TokenType::EQUALS;    break;
      case '<' : token->type = TokenType::LESS_THAN; break;
      case '(' : token->type = TokenType::LPAREN;    break;
      case ')' : token->type = TokenType::RPAREN;    break;

      case ':' : {
        char nextChar = source->nextChar();
        token->text += nextChar;

        if (nextChar == '=') {
          token->type = TokenType::COLON_EQUALS;
        } else {
          token->type = TokenType::COLON;
          return token;
        }

        break;
      }

      case EOF : token->type = END_OF_FILE; break;

      default: token->type = TokenType::ERROR;
    }

    source->nextChar();
    return token;
  }

  void Token::tokenError(Token *token, std::string message) {
    printf("TOKEN ERROR at line %d: %s at '%s'\n", token->lineNumber, message.c_str(), token->text.c_str());
  }
} // namespace frontend
