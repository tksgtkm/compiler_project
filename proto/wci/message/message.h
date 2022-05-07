#ifndef __MESSAGE_H
#define __MESSAGE_H

#include <string>
#include <map>

#include "../object.h"

namespace wci {
  namespace message {

    enum class MessageType {
      SOURCE_LINE, SYNTAX_ERROR, PARSER_SUMMARY, INTERPRETER_SUMMARY, COMPILER_SUMMARY
    };

    constexpr MessageType SOURCE_LINE = MessageType::SOURCE_LINE;
    constexpr MessageType SYNTAX_ERROR = MessageType::SYNTAX_ERROR;
    constexpr MessageType PARSER_SUMMARY = MessageType::PARSER_SUMMARY;
    constexpr MessageType INTERPRETER_SUMMARY = MessageType::INTERPRETER_SUMMARY;
    constexpr MessageType COMPILER_SUMMARY = MessageType::COMPILER_SUMMARY;

    typedef union {
      struct {
        int line_number;
        const char *line_text;
      } source_line;

      struct {
        int position;
        const char *token_text;
        const char *error_message;
      } syntax_error;

      struct {
        int line_count;
        int error_count;
        double elapsed_time;
      } parser_summary;

      struct {
        int execution_count;
        int runtime_errors;
        double elapsed_time;
      } interpreter_summary;

      struct {
        int instruction_count;
        double elapsed_time;
      } compiler_summary;
    } MessageContent;

    class Message {
    public:
      MessageType type;
      int line_number;
      MessageContent content;

      Message(const MessageType type) : type(type), line_number(0) {}
    };
  }
}

#endif
