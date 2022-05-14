#include <chrono>
#include "PascalParserTd.h"

#include "../Scanner.h"
#include "../Source.h"
#include "../../message/Message.h"

namespace wci {
  namespace frontend {
    namespace pascal {
      using namespace wci::frontend;
      using namespace wci::message;

      void PascalParserTD::parse() throw (std::string) {
        std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
        int last_line_number;
        Token *token = nullptr;

        while ((token = next_token(token)) != nullptr) {
          last_line_number = token->get_line_number();
        }

        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
        Message message(PARSER_SUMMARY);
        message.content.parser_summary.line_count = last_line_number;
        message.content.parser_summary.error_count = get_error_count();
        message.content.parser_summary.elapsed_time = elapsed_time;
        send_message(message);
      }
    } // pascal
  } // frontend
} // wci
