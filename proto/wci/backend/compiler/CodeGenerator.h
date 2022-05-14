#ifndef __CODE_GENERATOR_H
#define __CODE_GENERATOR_H

#include "../backend.h"

namespace wci {
  namespace backend {
    namespace compiler {
      using namespace wci::backend;

      class CodeGenerator : public Backend {
      public:
        void process(ICode *icode, SymTab *symtab) throw (std::string);
      };
    } // compiler
  } // backend
} // wci

#endif
