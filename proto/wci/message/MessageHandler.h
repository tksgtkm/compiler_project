#ifndef __MESSAGE_HANDLER_H
#define __MESSAGE_HANDLER_H

#include <vector>

#include "MessageListener.h"

namespace wci {
  namespace message {

    class MessageHandler {
    public:
      void add_listener(MessageListener *listener);

      void send_message(Message& message) const;

    private:
      std::vector<MessageListener *> listeners;

      void notify_listeners(Message& message) const;
    };
  }
}

#endif
