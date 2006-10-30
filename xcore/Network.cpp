#include "includes.h"
#include "Network.h"

namespace xcore
{

Network::Network()
{
}

Network::~Network()
{
}

void Network::serialize(const NetworkSendMessage& nsm, char* buf, size_t s)
{
   stringstream ss;
   ar::text_oarchive oa(ss);
   oa << nsm;

   ss.read(buf, s);
}

void Network::unserialize(NetworkReceiveMessage& nrm, char* buf, size_t s)
{
   stringstream ss;
   ss.write(buf, s);
   ar::text_iarchive ia(ss);
   ia >> nrm;
}

//spEvent Network::reconstruct(EVENT_TYPE type, char* data)
//{
//   spEvent e;
//   //stringstream ss;
//   //ar::text_iarchive ia(ss);
//   //ia << data;
//
//   switch (type)
//   {
//      case MAP_LOAD:
//         cout << "Map load" << endl;
//         e = reconstructHelper(new MapLoadEvent(), data);
//         break;
//
//      case CHAT:
//         e = reconstructHelper(new ChatEvent(), data);
//         break;
//
//      default:
//         cout << "Unhandled network event." << endl;
//         break;
//   }
//
//   return spEvent(e);
//}
//
//// make a new helper function for each type of event that needs to be reconstructed
//spEvent Network::reconstructHelper(MapLoadEvent* e, char* data) 
//{
//   istringstream ss (data);
//
//   ar::binary_iarchive ia(ss);
//   ia >> *e;
//
//   return spEvent(e);
//}
//
//spEvent Network::reconstructHelper(ChatEvent* e, char* data) 
//{
//   char data1[MAX_MSG_SIZE];
//   memmove(data1, data, MAX_MSG_SIZE - sizeof(uint16));
//   istringstream ss(data1);
//   ar::text_iarchive ia(ss);
//   ia >> *e;
//   return spEvent(e);
//}

} // namespace
