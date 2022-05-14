#include <chrono>
#include "pascal_parser_td.h"

#include "../scanner.h"
#include "../source.h"
#include "../../message/message.h"

namespace wci {
  namespace frontend {
    namespace pascal {
      using namespace wci::frontend;
      using namespace wci::message;

      void PascalParserTD::parse() throw (std::string) {
        std::steady_clock::time_point start_time = std::steady_clock::now();
        int last_line_number;
        Token *token = nullptr;

        while ((tilen = next_token(token)) != nullptr) {
          last_line_number = token->get_line_number();
        }

        std::steady_clock::time_point end_time = std::steady_clock::now();
        double elapsed_time = std::duration_cast<std::duration<double>>(end_time - start_time).count();
        Message message(PARSER_SUMMARY);
        message.content.parser_summary.line_count = last_line_number;
        message.content.parser_summary.error_count = get_error_count();
        message.content.parser_summary.elapsed_time = elapsed_time;
        send_message(message);
      }
    } // pascal
  } // frontend
} // wci
