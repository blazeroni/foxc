#ifndef XCORE_GAMESTATE_H
#define XCORE_GAMESTATE_H

#include "Game.h"

namespace xcore {

class Game;

/**
 * Represents the state of the game at any given time.
 *
 * This is the "meat" of the application.  
 * It handles all information regarding the current state.
 * Example states would be: front end, game setup, main game.
 * Essentially, a state should be created any time a major shift in 
 * required game resources (images, objects, etc) is needed (either
 * a lot is going to be unused or a lot is just starting to be used).
 *
 * After the initial game state creation by the application, the
 * concrete game state classes are responsible for switching to new
 * game states as needed.
 */
class GameState
{
   public: 
      GameState(Game* game);
      virtual ~GameState();

      /**
       * Main loop for the game state
       *
       * @param the time (in milliseconds) since the last update)
       */
      virtual void update(uint32 deltaTime) = 0;
      virtual void init() = 0;
      virtual void deinit() = 0;

      virtual void processSDLEvent(SDL_Event& event) = 0;

   protected:
      /// Used to change the game state
      Game* _game;
};

};

#endif
