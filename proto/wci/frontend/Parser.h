#ifndef __PARSER_H
#define __PARSER_H

#include "Scanner.h"
#include "../intermediate/Symtab.h"
#include "../intermediate/Icode.h"
#include "../message/Message.h"
#include "../message/MessageHandler.h"
#include "../message/MessageProducer.h"

namespace wci {
  namespace frontend {
    using namespace wci::intermediate;
    using namespace wci::message;

    class Parser : MessageProducer {
    public:
      /*
      Constructor
      @param scanner the scanner to be used with this parser.
      */
      Parser(Scanner *scanner) : scanner(scanner), symtab(nullptr), icode(nullptr) {}

      // Destructor
      virtual ~Parser() {}

      /*
      Getter
      @return the scanner used by this parser
      */
      Scanner *get_scanner() const {
        return scanner;
      }

      /*
      Getter
      @return
      */
      SymTab *get_symtab() const {
        return symtab;
      }

      ICode *get_icode() const {
        return icode;
      }

      MessageHandler& get_message_handler() const {
        return message_handler;
      }

      Token *current_token() {
        return scanner->current_token();
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
