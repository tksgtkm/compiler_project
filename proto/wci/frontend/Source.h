#ifndef __SOURCE_H
#define __SOURCE_H

#include <iostream>
#include <fstream>
#include <string>

#include "../message/MessageHandler.h"
#include "../message/MessageProducer.h"
#include "../message/MessageListener.h"

namespace wci {
  namespace frontend {

    using namespace wci::message;

    class Source : public MessageProducer {
    public:

      Source(std::istream &reader) : reader(reader), line_number(0), current_pos(-2) {}

      int get_line_number() const {return line_number;}

      int get_position() const {return current_pos;}

      char current_char() throw (std::string);

      char next_char() throw (std::string);

      char peek_char() throw (std::string);

      bool at_eol() throw (std::string) {
        return current_pos == line_text.length();
      }

      bool at_eof() throw (std::string) {
        if (current_pos == -2)
          read_line();

        return reader.eof() && (current_pos >= line_text.length());
      }

      void skip_to_next_line() throw (std::string) {
        current_pos = line_text.length() + 1;
      }

      void close() throw (std::string) {

      }

      void send_message(Message& message) const {
        message_handler.send_message(message);
      }

      static const char END_OF_LINE;
      static const char END_OF_FILE;

    private:
      static Message SOURCE_LINE_MESSAGE;

      std::istream& reader;
      std::string line_text;
      int line_number;
      int current_pos;
      MessageHandler message_handler;

      void read_line() throw (std::string);
    };
  }
}

#endif
