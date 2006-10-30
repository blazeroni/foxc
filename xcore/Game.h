#ifndef XCORE_GAME_H
#define XCORE_GAME_H

#include "includes.h"

namespace xcore
{
   class Game;
   typedef boost::shared_ptr<Game> spGame;
}

#include "GameState.h"

namespace xcore 
{

class GameState;

/**
 * Represents one instance of a game.
 *
 * Serves as a state manager a game instance.  There will only be one 
 * in clients, but potentially multiple on servers.  Starts up the initial
 * state and tells it to continually update.  States use this class
 * to actually change to a different state.
 */
class Game
{
   public:
     Game();
     virtual ~Game();

     /// Startup code for the application.
     virtual void init() = 0;          

     /**
      * The game loop.
      *
      * Function intended to be called repeatedly to update the game.  
      * Each call would represent a cycle through the game loop.
      */
     virtual void update() = 0;

     /// Shutdown code for the application.
     virtual void deinit() = 0;        

     /**
      * Processes SDL events sent to it by various other classes
      *
      * @todo This method should be removed in favor using our own events,
      *       of which the Application class would only handle a few like input
      */
     virtual void processSDLEvent(SDL_Event& event) = 0;

     /**
      * Changes the application game state.
      *
      * Removes all game states from the stack and sets the game state to the new one
      * Does not run init() or attempt to modify the game state at all.
      */
     void changeState(GameState* gs);

     /**
      * Pushes a game state onto the stack
      *
      * Uses this game state until popped or a new state is 
      * pushed or the state is changed via changeState
      */
     void pushState(GameState* gs);

     /** 
      * Remvoes the current state from the stack
      *
      * The state underneath becomes the current state.
      */
     void popState();

     /// Checks whethere the application should continue running
     bool running() const;

     /// Tells the application that it should quit
     void quit();

     GameState* getCurrentGameState() const;

   protected:
     
     /**
      * Removes all game states
      *
      * Leaves the game state stack empty.  
      * Another stack will need to be pushed on if the game is supposed to continue to run
      */
     void popAllStates();

   private:
     vector<GameState*> _states;
     bool _running;
};

} // namespace

#endif
