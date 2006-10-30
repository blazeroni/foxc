#ifndef XCORE_EVENT_H
#define XCORE_EVENT_H

#include "includes.h"
#include "IEventListener.h"

namespace xcore
{

class Event;
typedef boost::shared_ptr<Event> spEvent;

class IEventListener;

// add new types before the MAX_EVENT_TYPES value, so it always actually has the max
// note: this used to be an actual type but the boost hash wasn't behaving as expected
enum EVENT_TYPE { ANY, 
                  CHAT, 
                  UNIT_MOVE, 
                  UNIT_WAIT,
                  MAP_LOAD, 
                  UNIT_CREATE, 
                  GAME_LIST,
                  GAME_JOIN,
                  PLAYER_JOIN,
                  CLIENT_CONNECT,
                  CLIENT_DISCONNECT,
                  START_GAME,
                  GAME_OVER,
                  UNIT_ACTIVE,
                  UNIT_FIRE,
                  UNIT_INVSWAP,
                  // add new enums before this one
                  MAX_EVENT_TYPES };

class Event
{
   friend class boost::serialization::access;
   public:
      //Event();
      Event(EVENT_TYPE type);
      virtual ~Event();

      void fire();
      void queue();

      uint32 getTimeCreated() const;

      bool isLocal() const;
      
      uint32 getSource() const;
      void setSource(int32 socket);

      EVENT_TYPE getType() const;

      virtual void process(IEventListener* handler) = 0;

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _time;
      uint32 _socket; // not serialized - filled in when unserialized

      EVENT_TYPE _eventType;
};

template<class T>
void Event::serialize(T & archive, const unsigned int version)
{
   archive & _time;
   archive & _eventType;
}

} // namespace

BOOST_IS_ABSTRACT(xcore::Event)

#endif
