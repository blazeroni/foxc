#include "includes.h"
#include "XServer.h"

int main(int argc, char** argv)
{
   XServer& server = XServer::instance();

   server.init();

   while (server.running())
   {
      server.update();
   }

   server.deinit();

   return 0;
}
