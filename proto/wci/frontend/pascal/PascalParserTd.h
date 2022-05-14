#ifndef __PASCAL_PARSER_TD_H
#define __PASCAL_PARSER_TD_H

#include "../parser.h"
#include "../scanner.h"

namespace wci {
  namespace frontend {
    namespace pascal {
      using namespace wci::frontend;

      class PascalParserTD : public Parser {
      public:
        PascalParserTD(Scanner *scanner) : Parser(scanner) {}

        PascalParserTD(PascalParserTD *parent) : Parser(parent->get_scanner()) {}

        void parse() throw (std::string);

        int get_error_count() const {return 0;}
      };
    } // pascal
  } // frontend
} // wci

#endif
