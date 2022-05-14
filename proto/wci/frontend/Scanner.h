#ifndef __SCANNER_H
#define __SCANNER_H

#include "Source.h"
#include "Token.h"

namespace wci {
  namespace frontend {

    class Scanner {
    public:
      Scanner(Source *source) : source(source), token(nullptr) {}

      virtual ~Scanner() {}

      Token *current_token() {
        return token;
      }

      Token *next_token(Token *prev_token) throw (std::string) {
        if (prev_token != nullptr)
          delete prev_token;
        token = extract_token();
        return token;
      }

      char current_char() throw (std::string) {
        return source->current_char();
      }

      char next_char() throw (std::string) {
        return source->next_char();
      }

      bool at_eol() throw (std::string) {
        return source->at_eol();
      }

      bool at_eof() throw (std::string) {
        return source->at_eof();
      }

      void skip_to_next_line() throw (std::string) {
        source->skip_to_next_line();
      }

    protected:
      Source *source;
      virtual Token *extract_token() throw (std::string) = 0;

    private:
      Token *token;
    };
  } // namespace frontend
} // wci

#endif
