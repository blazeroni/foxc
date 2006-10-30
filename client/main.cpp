#include "includes.h"
#include "XClient.h"

int main(int argc, char** argv)
{
   XClient& game = XClient::instance();

   game.init();

   while (game.running())
   {
      game.update();
   }

   game.deinit();

   return 0;
}
