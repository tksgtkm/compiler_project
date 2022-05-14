#ifndef __FRONTEND_FACTORY_H
#define __FRONTEND_FACTORY_H

#include <string>
#include "parser.h"
#include "scanner.h"
#include "source.h"
#include "pascal/pascal_parser_td.h"
#include "pascal/pascal_scanner.h"

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
