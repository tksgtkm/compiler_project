#ifndef __TOKEN_H
#define __TOKEN_H

#include <string>

#include "Source.h"
#include "../Object.h"

namespace wci {
  namespace frontend {
    using namespace wci;

    enum class TokenType {

    };

    class Token {
    public:
      Token(Source *source) throw (std::string) : type((TokenType) 0), text(""), source(source),
            line_number(source->get_line_number()), position(source->get_position()) {
        value.clear();
        extract();
      }

      Token(const Token& orig) {*this = orig;}

      virtual ~Token() {}

      TokenType get_type() const {return type;}

      std::string get_text() const {return text;}

      Object get_value() {return value;}

      int get_line_number() const {return line_number;}

      int get_position() const {return position;}

    protected:
      TokenType type;
      std::string text;
      Object value;
      Source *source;
      int line_number;
      int position;

      void extract() throw (std::string) {
        text = std::to_string(current_char());
        next_char();
      }

      char current_char() const throw (std::string) {
        return source->current_char();
      }

      char next_char() const throw (std::string) {
        return source->next_char();
      }

      char peek_char() const throw (std::string) {
        return source->peek_char();
      }
    };
  } // namespace frontend
} // wci

#endif
