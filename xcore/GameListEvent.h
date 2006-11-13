#ifndef XCORE_GAMELIST_EVENT
#define XCORE_GAMELIST_EVENT

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

struct GameListItem
{
   friend class boost::serialization::access;
   
   uint32 gameID;
   string gameName;
   string mapName;
   uint16 currentPlayers;
   uint16 maxPlayers;
   uint32 points;
   uint32 width;
   uint32 height;

   template<class T>
   void serialize(T & archive, const unsigned int version);
};

class GameListEvent : public Event
{
   friend class boost::serialization::access;
   public:
      GameListEvent();
      GameListEvent(vector<GameListItem> games);
      virtual ~GameListEvent();

      const vector<GameListItem>& getGames();

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      vector<GameListItem> _games;
};

template<class T>
void GameListItem::serialize(T & archive, const unsigned int version)
{
   archive & gameID;
   archive & gameName;
   archive & mapName;
   archive & currentPlayers;
   archive & maxPlayers;
   archive & points;
   archive & width;
   archive & height;
}

template<class T>
void GameListEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _games;
}



} // namespace

#endif
