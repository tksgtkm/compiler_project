#ifndef __EXECUTER_H
#define __EXECUTER_H

#include "../Backend.h"

namespace wci {
  namespace backend {
    namespace interpreter {
      using namespace wci::backend;

      class Executor : public Backend {
      public:
        void process(ICode *icode, SymTab *symtab) throw (std::string);
      };
    } // interpreter
  } // backend
} // wci

#endif
