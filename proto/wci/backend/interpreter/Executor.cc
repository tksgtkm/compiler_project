#include <chrono>
#include "Executor.h"
#include "../Backend.h"
#include "../../message/Message.h"

namespace wci {
  namespace backend {
    namespace interpreter {
      using namespace wci::backend;
      using namespace wci::message;

      void Executor::process(ICode *icode, SymTab *symtab) throw (std::string) {
        std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
        int execution_count = 0;
        int runtime_errors = 0;
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
        Message message(INTERPRETER_SUMMARY);
        message.content.interpreter_summary.execution_count = execution_count;
        message.content.interpreter_summary.runtime_errors = runtime_errors;
        message.content.interpreter_summary.elapsed_time = elapsed_time;
        send_message(message);
      }
    } // namespace interpreter
  } // backend
} // wci
