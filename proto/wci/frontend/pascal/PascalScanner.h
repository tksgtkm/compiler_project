#ifndef __PASCAL_PARSER_H
#define __PASCAL_PARSER_H

#include "../Scanner.h"
#include "../Source.h"
#include "../Token.h"

namespace wci {
  namespace frontend {
    namespace pascal {
      using namespace wci::frontend;

      class PascalScanner : public Scanner {
      public:
        PascalScanner(Source *source) : Scanner(source) {}

      protected:
        Token *extract_token() throw (std::string);
      };
    } // namespace pascal
  } // namespace frontend
} // wci

#endif
