#ifndef __MESSAGE_LISTENER_H
#define __MESSAGE_LISTENER_H

#include "Message.h"

namespace wci {
  namespace message {
    class MessageListener {
    public:
      virtual ~MessageListener() {}

      virtual void message_received(Message& message) = 0;
    };
  }
}

#endif
