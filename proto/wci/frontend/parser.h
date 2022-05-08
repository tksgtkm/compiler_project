#ifndef __PARSER_H
#define __PARSER_H

#include "scanner.h"
#include "../intermediate/symtab.h"
#include "../intermediate/icode.h"
#include "../message/message.h"
#include "../message/message_handler.h"
#include "../message/message_producer.h"

namespace wci {
  namespace frontend {
    namespace wci::intermediate;
    namespace wci::message;

    class Parser : MessageProducer {
    public:
      Parser(Scanner *scanner) : scanner(scanner), symtab(nullptr), icode(nullptr) {}

      virtual ~Parser() {}

      Scanner *get_scanner() const {
        return scanner;
      }

      SymTab *get_symtab() const {
        return symtab;
      }

      ICode *get_icode() const {
        return icode;
      }

      MessageHandler& get_message_handler() const {
        return message_handler;
      }

      Token *next_token(Token *prev_token) throw (std::string) {
        return scanner->next_token(prev_token);
      }

      virtual void parse() throw (std::string) = 0;

      virtual int get_error_count() const = 0;

      void add_message_listener(MessageListener *listener) {
        message_handler.add_listener(listener);
      }

      void send_message(Message& message) const {
        message_handler.send_message(message);
      }

    protected:
      static MessageHandler message_handler;

      Scanner *scanner;
      SymTab *symtab;
      ICode *icode;
    };
  } // frontend
} // wci

#endif
