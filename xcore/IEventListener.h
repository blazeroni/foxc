#ifndef XCORE_IEVENTLISTENER_H
#define XCORE_IEVENTLISTENER_H

#include "includes.h"

namespace xcore
{

class Event;
class MapLoadEvent;
class ChatEvent;
class GameListEvent;
class GameJoinEvent;
class PlayerJoinEvent;
class ClientConnectEvent;
class ClientDisconnectEvent;
class UnitCreateEvent;
class UnitMoveEvent;
class StartGameEvent;
class GameOverEvent;
class UnitActiveEvent;
class UnitWaitEvent;
class UnitFireEvent;
class UnitInvSwapEvent;

// currently this class knows about every possible event type
// in the future, this should probably be split up depending on the 
// types of events, i.e. grouped into categories
class IEventListener
{
   public:
      IEventListener() {};
      virtual ~IEventListener() {};

      virtual void handleEvent(Event& e) {}
      virtual void handleEvent(MapLoadEvent& e) {};
      virtual void handleEvent(ChatEvent& e) {};
      virtual void handleEvent(GameListEvent& e) {};
      virtual void handleEvent(GameJoinEvent& e) {};
      virtual void handleEvent(PlayerJoinEvent& e) {};
      virtual void handleEvent(ClientConnectEvent& e) {};
      virtual void handleEvent(ClientDisconnectEvent& e) {};
      virtual void handleEvent(UnitCreateEvent& e) {};
      virtual void handleEvent(UnitMoveEvent& e) {};
      virtual void handleEvent(StartGameEvent& e) {};
      virtual void handleEvent(GameOverEvent& e) {};
      virtual void handleEvent(UnitActiveEvent& e) {};
      virtual void handleEvent(UnitWaitEvent& e) {};
      virtual void handleEvent(UnitFireEvent& e) {};
      virtual void handleEvent(UnitInvSwapEvent& e) {};
};


} // namespace

#endif
