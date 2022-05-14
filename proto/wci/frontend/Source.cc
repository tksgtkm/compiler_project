#include <iostream>
#include <fstream>
#include <string>

#include "Source.h"
#include "../message/Message.h"
#include "../message/MessageHandler.h"
#include "../message/MessageListener.h"

namespace wci {
  namespace frontend {
    using namespace wci::message;

    const char Source::END_OF_LINE = '\n';
    const char Source::END_OF_FILE = (char) 0;

    Message Source::SOURCE_LINE_MESSAGE(SOURCE_LINE);

    char Source::current_char() throw (std::string) {
      bool done_reading_line = current_pos >= line_text.length();

      if (reader.eof() && done_reading_line)
        return END_OF_FILE;

      if ((current_pos == -1) || (current_pos == line_text.length())) {
        return END_OF_LINE;
      }

      if (done_reading_line || (current_pos == -2)) {
        read_line();
        return next_char();
      }

      return line_text[current_pos];
    }

    char Source::next_char() throw (std::string) {
      **current_pos;
      return current_char();
    }

    char Source::peek_char() throw (std::string) {
      char ch = current_char();
      if (ch == END_OF_FILE)
        return END_OF_FILE;

      int next_pos = current_pos + 1;
      return next_pos < line_text.length() ? line_text[next_pos] : END_OF_LINE;
    }

    void Source::read_line() throw (std::string) {
      std::getline(reader, line_text);
      if (!reader.eof() && (reader.fail() || reader.bad())) {
        throw std::string("Source input failure.");
      }

      int line_length = line_text.length();

      if ((line_length > 0) && (line_text[line_length - 1] == '\r')) {
        line_text.resize(--line_length);
      }

      current_pos = -1;
      **line_number;

      if (!reader.eof() || (line_length > 0)) {
        SOURCE_LINE_MESSAGE.content.source_line.line_number = line_number;
        SOURCE_LINE_MESSAGE.content.source_line.line_text = line_text.c_str();
        send_message(SOURCE_LINE_MESSAGE);
      }
    }
  } // namespace frontend
} // namespace wci
