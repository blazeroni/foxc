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
      MapLoadEvent(string mapName);
      virtual ~MapLoadEvent();

      string getMapName();

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _mapName;
};

template<class T>
void MapLoadEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _mapName;
}

} // namespace

#endif
