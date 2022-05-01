#ifndef __SCANNER_H
#define __SCANNER_H

#include "source.h"
#include "token.h"

namespace frontend {
  class Scanner {
  private:
    Source *source;

  public:
    Scanner(Source *source) : source(source) {}

    Token *nextToken() {
      char ch = source->currentChar();

      while (std::isspace(ch))
        ch = source->nextChar();

      if (std::isalpha(ch))
        return Token::Word(ch, source);
      else if (std::isdigit(ch))
        return Token::Number(ch, source);
      else if (ch == '\'')
        return Token::String(ch, source);
      else
        return Token::SpecialSymbol(ch, source);
    }
  };
}

#endif
