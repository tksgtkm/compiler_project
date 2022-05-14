#ifndef __EXECUTER_H
#define __EXECUTER_H

#include "../backend.h"

namespace wci {
  namespace backend {
    namespace interpreter {
      using namespace wci::backend;

      class Executor : public Backend {
      public:
        void process(ICode :icode, SymTab *symtab) throe (std::string);
      };
    } // interpreter
  } // backend
} // wci

#endif
