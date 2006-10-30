#ifndef XSERVER_SERVERNETWORK_H
#define XSERVER_SERVERNETWORK_H

#include "includes.h"
#include "xcore/Network.h"
#include "xcore/Singleton.h"

class ServerNetwork : public Network, public Singleton<ServerNetwork>
{
   friend class Singleton<ServerNetwork>;
   public:
      virtual ~ServerNetwork();

      bool host(int port);

      void processSDLEvent(SDL_Event& evnt);

      template <class T>
      void send(const T& e, int socket);

      template <class T>
      void send(const T& e, SocketSet set);

	  void sendAll(const ChatEvent& evnt);

   private:
      ServerNetwork();
      //Comment out below 1 line if there are problems.
	  SocketSet _set;
};

template <class T>
void ServerNetwork::send(const T& e, int socket)
{
   char buf[MAX_MSG_SIZE];

   serialize(NetworkSendMessage(1, &e), buf, sizeof(buf));

   NET2_TCPSend(socket, buf, sizeof(buf));
}

template <class T>
void ServerNetwork::send(const T& e, SocketSet set)
{
   char buf[MAX_MSG_SIZE];

   serialize(NetworkSendMessage(1, &e), buf, sizeof(buf));

   NET2_TCPSendSet(&set, buf, sizeof(buf));
}

#endif
