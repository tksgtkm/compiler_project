#include <chrono>
#include "CodeGenerator.h"
#include "../Backend.h"
#include "../../message/Message.h"

namespace wci {
  namespace backend {
    namespace compiler {
      using namespace wci::backend;
      using namespace wci::message;

      void CodeGenerator::process(ICode *icode, SymTab *symtab) throw (std::string) {
        std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
        int instruction_count = 0;

        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
        Message message(COMPILER_SUMMARY);
        message.content.compiler_summary.instruction_count = instruction_count;
        message.content.compiler_summary.elapsed_time = elapsed_time;
        send_message(message);
      }
    } // compiler
  } // backend
} // wci
