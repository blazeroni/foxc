#ifndef XCORE_MAPLISTEVENT_H
#define XCORE_MAPLISTEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

struct MapListItem
{
   friend class boost::serialization::access;

   string mapName;
   uint32 width;
   uint32 height;
   uint16 maxPlayers;
   uint32 points;

   template<class T>
   void serialize(T & archive, const unsigned int version);
};


class MapListEvent : public Event
{
   public:
      MapListEvent();
      MapListEvent(vector<MapListItem> maps);
      virtual ~MapListEvent();

      const vector<MapListItem>& getMaps() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      vector<MapListItem> _maps;
};


template<class T>
void MapListItem::serialize(T & archive, const unsigned int version)
{
   archive & mapName;
   archive & width;
   archive & height;
   archive & maxPlayers;
   archive & points;
}

template<class T>
void MapListEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _maps;
}

} // namespace

#endif
