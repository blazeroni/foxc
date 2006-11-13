#ifndef XCORE_MAPLOADEVENT_H
#define XCORE_MAPLOADEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class MapLoadEvent : public Event
{
   friend class boost::serialization::access;
   public:
      MapLoadEvent();
      MapLoadEvent(string mapName, string fileName);
      virtual ~MapLoadEvent();

      string getMapName();
      string getFileName();

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _mapName;
      string _fileName;
};

template<class T>
void MapLoadEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _mapName;
   archive & _fileName;
}

} // namespace

#endif
