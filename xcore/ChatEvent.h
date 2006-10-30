#ifndef XCORE_CHATEVENT_H
#define XCORE_CHATEVENT_H

#include "includes.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class ChatEvent : public Event
{
   friend class boost::serialization::access;
   public:
      ChatEvent(string message = "", string playerName = "");
      virtual ~ChatEvent();

      string getMessage();
      string getPlayerName();
      
      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _message;
      string _playerName;
};

template<class T>
void ChatEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _message;
   archive & _playerName;
}

} // namespace

#endif
