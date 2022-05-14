#ifndef __BACKEND_FACTORY_H
#define __BACKEND_FACTORY_H

#include <string>
#include "Backend.h"
#include "compiler/CodeGenerator.h"
#include "interpreter/Executor.h"
#include "../Object.h"

namespace wci {
  namespace backend {
    using namespace wci::backend::compiler;
    using namespace wci::backend::interpreter;

    class BackendFactory {
    public:
      static Backend * create_backend(std::string operation) throw (std::string) {
        if (operation == "compile") {
          return new CodeGenerator();
        } else if (operation == "execute") {
          return new Executor();
        } else {
          throw new std::string("Backend factory: Invalid operation '" + operation + "'");
        }
      }
    };
  } // backend
} // wci

#endif
