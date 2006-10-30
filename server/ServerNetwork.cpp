#include "includes.h"
#include "ServerNetwork.h"
#include "xcore/ClientConnectEvent.h"
#include "xcore/ClientDisconnectEvent.h"

ServerNetwork::ServerNetwork()
{

}

ServerNetwork::~ServerNetwork()
{

}

bool ServerNetwork::host(int port)
{
   //Comment out below 1 line if there are problems
   initSocketSet(&_set, 10);
   return (NET2_TCPAcceptOn(port) >= 0);
}

void ServerNetwork::processSDLEvent(SDL_Event &evnt)
{
   char buf[MAX_MSG_SIZE];
   int clientSocket;
   spEvent e;

   switch (NET2_GetEventType(&evnt))
   {
      case NET2_TCPACCEPTEVENT:
         cout << "Client connected." << endl;
         //Comment out below 2 lines if there are problems
         clientSocket = NET2_GetSocket(&evnt);
         e = spEvent(new ClientConnectEvent(clientSocket));
         e->setSource(clientSocket);
         e->fire();
         addSocketSet(&_set,&clientSocket);
		   break;

      case NET2_TCPCLOSEEVENT:
         cout << "Client disconnected." << endl;
         //Remove the closed socket from the socket set so
         //that caling NET2_TCPSendSet won't generate errors.
         //Comment out below 2 lines if there are problems.
         clientSocket = NET2_GetSocket(&evnt);
         e = spEvent(new ClientDisconnectEvent());
         e->setSource(clientSocket);
         e->fire();
         delSocketSet(&_set,&clientSocket);
         break;

      case NET2_TCPRECEIVEEVENT:
         while (0 != (NET2_TCPRead(NET2_GetSocket(&evnt), buf, sizeof(buf))))
         {
            NetworkReceiveMessage nrm;
            
            unserialize(nrm, buf, sizeof(buf));

            nrm._evnt->setSource(NET2_GetSocket(&evnt));
            e = spEvent(nrm._evnt);
            e->fire();
   	   }
         break;

      case NET2_ERROREVENT:
         cerr << "Network Error: " << NET2_GetError() << endl;
         break;

      default:
         break;
   }
}

//Sends an event to all clients in the game.
//_set is the SocketSet for the game.
//Comment out below function if there are problems
void ServerNetwork::sendAll(const ChatEvent& e)
{
   char buf[MAX_MSG_SIZE];

   serialize(NetworkSendMessage(1, &e), buf, sizeof(buf));

   NET2_TCPSendSet(&_set, buf, sizeof(buf));
}
