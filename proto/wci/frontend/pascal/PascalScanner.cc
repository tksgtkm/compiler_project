#include <iostream>
#include "pascal_scanner.h"
#include "../source.h"

namespace wci {
  namespace frontend {
    namespace pascal {
      using namespace wci::frontend;

      Token *PascalScanner::extract_token() throw (std::string) {
        Token *token;
        char current_ch = current_char();

        if (current_ch == Source::END_OF_FILE) {
          token = nullptr;
        } else {
          token = new Token(source);
        }
        return token;
      }
    } // pascal
  } // frontend
} // wci
