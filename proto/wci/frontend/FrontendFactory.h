#ifndef __FRONTEND_FACTORY_H
#define __FRONTEND_FACTORY_H

#include <string>
#include "Parser.h"
#include "Scanner.h"
#include "Source.h"
#include "pascal/PascalParserTd.h"
#include "pascal/PascalScanner.h"

namespace wci {
  namespace frontend {
    using namespace wci::frontend::pascal;

    class FrontendFactory {
    public:
      static Parser *create_parser(std::string language, std::string type, Source *source) throw (std::string) {
        if ((language == "Pascal") && (type == "top-down")) {
          Scanner *scanner = new PascalScanner(source);
          return new PascalParserTD(scanner);
        } else if (language != "Pascal") {
          throw new std::string("Parser factory: Invalid language '" + language + "'");
        } else {
          throw new std::string("Parser factory: Invalid type '" + type + "'");
        }
      }
    };
  } // frontend
} // wci

#endif
