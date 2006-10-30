#include "includes.h"
#include "MapLoadEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::MapLoadEvent, "MAP_LOAD_EVENT")

namespace xcore
{

MapLoadEvent::MapLoadEvent() :
   Event(MAP_LOAD),
   _mapName("")
{
}

MapLoadEvent::MapLoadEvent(string mapName) :
   Event(MAP_LOAD),
   _mapName(mapName)
{
   
}

MapLoadEvent::~MapLoadEvent()
{
}

void MapLoadEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

string MapLoadEvent::getMapName()
{
   return _mapName;
}

} // namespace

