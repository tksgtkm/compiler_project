#ifndef __MESSAGE_PRODUCER_H
#define __MESSAGE_PRODUCER_H

#include "message_listener.h"

namespace wci {
  namespace message {
    class MessageProducer {
    public:
      virtual ~MessageProducer() {}

      virtual void add_message_listener(MessageListener *listener) = 0;

      virtual void send_message(Message &message) const = 0;
    };
  }
}

#endif
