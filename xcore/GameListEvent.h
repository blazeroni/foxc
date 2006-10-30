#ifndef XCORE_GAMELIST_EVENT
#define XCORE_GAMELIST_EVENT

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class GameListEvent : public Event
{
   friend class boost::serialization::access;
   public:
      GameListEvent();
      GameListEvent(list<string> games);
      virtual ~GameListEvent();

      const list<string>& getGames();

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      list<string> _games;
};

template<class T>
void GameListEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _games;
}

} // namespace

#endif
