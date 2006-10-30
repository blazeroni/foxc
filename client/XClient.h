#ifndef XCLIENT_XCLIENT_H
#define XCLIENT_XCLIENT_H

#include "includes.h"
#include "xcore/Game.h"
#include "xcore/Singleton.h"

class XClient : public Game, public Singleton<XClient>
{
   friend class Singleton<XClient>;
   public:
      virtual ~XClient();

      void deinit();
      void init();
      void update();

      static void quitSDL();

      void processSDLEvent(SDL_Event& event);

   private:
      XClient();
      const string configFile;

      uint32 lastUpdateTime;
      uint32 deltaTime;
};

#endif
