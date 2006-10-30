#ifndef XCLIENT_CLIENTNETWORK_H
#define XCLIENT_CLIENTNETWORK_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "xcore/Network.h"
#include "xcore/Event.h"
#include "xcore/MapLoadEvent.h"
#include "xcore/ChatEvent.h"

class ClientNetwork : public Network, public Singleton<ClientNetwork>
{
   friend class Singleton<ClientNetwork>;
   public:
      virtual ~ClientNetwork();

      bool connectToServer(const char* hostname, int port);
      void closeConnection();

      void processSDLEvent(SDL_Event& event);

      template <class T>
      void send(const T &e);

      void disconnect();

   protected:
      ClientNetwork();

      int _socket;
};


// template functions have to be defined in the header =(
template <class T>
void ClientNetwork::send(const T& e)
{
   char buf[MAX_MSG_SIZE];

   serialize(NetworkSendMessage(1, &e), buf, sizeof(buf));

   NET2_TCPSend(_socket, buf, sizeof(buf));
}

#endif
