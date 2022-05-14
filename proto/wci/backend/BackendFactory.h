#ifndef __BACKEND_FACTORY_H
#define __BACKEND_FACTORY_H

#include <string>
#include "backend.h"
#include "compiler/code_generator.h"
#include "interpreter/executor.h"
#include "../object.h"

namespace wci {
  namespace backend {
    using namespace wci::backend::compiler;
    using namespace wci::beckend::interpreter;

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
