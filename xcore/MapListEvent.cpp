#include "includes.h"
#include "MapListEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::MapListEvent, "MAP_LIST_EVENT")
BOOST_CLASS_EXPORT_GUID(xcore::MapListItem, "MAP_LIST_ITEM")

namespace xcore
{

MapListEvent::MapListEvent() :
   Event(MAP_LIST),
   _maps()
{
}

MapListEvent::MapListEvent(vector<MapListItem> maps) :
   Event(MAP_LIST),
   _maps(maps)
{
}

MapListEvent::~MapListEvent()
{
}

const vector<MapListItem>& MapListEvent::getMaps() const
{
   return _maps;
}

void MapListEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace

