#include "includes.h"
#include "Game.h"

namespace xcore {

Game::Game() :
   _running(true)
{
   // empty
}

Game::~Game()
{

}

void Game::changeState(GameState* gs)
{
   popAllStates();

   _states.push_back(gs);
}

void Game::pushState(GameState* gs)
{
   _states.push_back(gs);
}


void Game::popState()
{
   GameState* gs = _states.back();
   _states.pop_back();
   gs->deinit();
   SAFE_DELETE(gs);
}

bool Game::running() const
{
   return _running;
}

void Game::quit()
{
   _running = false;
}

GameState* Game::getCurrentGameState() const
{
   return _states.back();
}

void Game::popAllStates()
{
   int size = static_cast<int>(_states.size());
   for (int i = 0; i < size; i++)
   {
      popState();
   }
}

} // namespace
