#include "includes.h"
#include "MapLoadEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::MapLoadEvent, "MAP_LOAD_EVENT")

namespace xcore
{

MapLoadEvent::MapLoadEvent() :
   Event(MAP_LOAD),
   _mapName(""),
   _fileName("")
{
}

MapLoadEvent::MapLoadEvent(string mapName, string fileName) :
   Event(MAP_LOAD),
   _mapName(mapName),
   _fileName(fileName)
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

string MapLoadEvent::getFileName()
{
   return _fileName;
}

} // namespace

