#ifndef __BACKEND_H
#define __BACKEND_H

#include "../message/MessageProducer.h"
#include "../message/MessageHandler.h"
#include "../intermediate/Symtab.h"
#include "../intermediate/Icode.h"

namespace wci {
  namespace backend {
    using namespace wci::message;
    using namespace wci::intermediate;

    class Backend : public MessageHandler {
    public:
      SymTab *get_symtab() const;

      ICode *get_icode() const;

      MessageHandler& get_message_handler() const;

      virtual void process(ICode *icode, SymTab *symtab) throw (std::string) = 0;

      void add_message_listener(MessageListener *listener);

      void send_message(Message& message) const;

    protected:
      SymTab *symtab;
      ICode *icode;

      static MessageHandler message_handler;
    };
  }// backend
} // wci

#endif
