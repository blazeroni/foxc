#ifndef XCORE_NETWORK_H
#define XCORE_NETWORK_H

#include "includes.h"
#include "Event.h"
#include "MapLoadEvent.h"
#include "ChatEvent.h"

#define MAX_MSG_SIZE 1024

namespace xcore
{

class Network
{
   public:
      Network();
      ~Network();

      void processSDLEvent(const SDL_Event& evnt);

   protected:
      template <class T> 
      class NetworkMessage
      {
         friend class boost::serialization::access;

         public:
            NetworkMessage() {}
            NetworkMessage(uint32 _gameID, T* e) : 
               _evnt(e) { }

            template<class U>
            void serialize(U & archive, const unsigned int version)
            {
               archive & _gameID;
               archive & _evnt;
            }

            T* _evnt;
            uint32 _gameID;
      };

      typedef NetworkMessage<const Event> NetworkSendMessage;
      typedef NetworkMessage<Event> NetworkReceiveMessage;

      void serialize(const NetworkSendMessage& nsm, char* buf, size_t s);
      void unserialize(NetworkReceiveMessage& nrm, char* buf, size_t s);
};

} // namespace

#endif
