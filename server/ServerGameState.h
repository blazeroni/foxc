#ifndef XSERVER_SERVERGAMESTATE_H
#define XSERVER_SERVETGAMESTATE_H

#include "includes.h"
#include "xcore/GameState.h"

class ServerGameState : public GameState
{
   public:
      ServerGameState(Game* app);
      virtual ~ServerGameState();

      virtual void init();
      virtual void deinit();
      virtual void update(unsigned int deltaTime);

      virtual void processSDLEvent(const SDL_Event& event);

   private:
      Game* _game;
};

#endif
