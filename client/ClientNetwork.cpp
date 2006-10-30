#include "includes.h"
#include "ClientNetwork.h"

ClientNetwork::ClientNetwork() :
   _socket(-1)
{

}

ClientNetwork::~ClientNetwork()
{

}

void ClientNetwork::processSDLEvent(SDL_Event& evnt)
{
   char buf[MAX_MSG_SIZE];

   switch (NET2_GetEventType(&evnt))
   {
      case NET2_TCPRECEIVEEVENT:
         while (0 != (NET2_TCPRead(NET2_GetSocket(&evnt), buf, sizeof(buf))))
         {
            NetworkReceiveMessage nrm;
            //EVENT_TYPE type;
            
            unserialize(nrm, buf, sizeof(buf));

            //if (nrm._eventType >= MAX_EVENT_TYPES) 
            //{
            //   cerr << "Network: Unexpected network message received." << endl;
            //   continue;
            //}
            
            //type = EVENT_TYPE(nrm._eventType);
            nrm._evnt->setSource(NET2_GetSocket(&evnt));
            spEvent e = spEvent(nrm._evnt);
            e->fire();
   	   }
         break;
      
      case NET2_ERROREVENT:
         cerr << "Network error event received." << endl;
         break;

      default:
         break;
   }
}

void ClientNetwork::disconnect()
{
   NET2_TCPClose(_socket);
}

//Opens a connection to the specified host on the specified port.
//Returns whether it connected or not
bool ClientNetwork::connectToServer(const char* hostname, int port)
{
   _socket = NET2_TCPConnectTo(const_cast<char*>(hostname), port);
   if (_socket == -1)
   {
      cout << "Could not connect to " << hostname << " on port " << port << ".";
      return false;
   }
   else 
   {
      cout << "Connected to " << hostname << " on port " << port << ".";
      return true; 
   }
}

//Closes the socket with the specified integer index.
void ClientNetwork::closeConnection()
{
   NET2_TCPClose(_socket);
   _socket = -1;
}

//void ClientNetwork::process(const Event& e)
//{
//   cerr << "Unkown event.";
//}
//
//void ClientNetwork::process(const MapLoadEvent& e)
//{
//   stringstream ss;
//   ar::text_oarchive oa(ss);
//   oa << e;
//
//   ar::text_iarchive ia(ss);
//   MapLoadEvent* e2 = new MapLoadEvent("blah");
//   ia >> *e2;
//   cout << ((MapLoadEvent*)e2)->getMapName();
//
//}
