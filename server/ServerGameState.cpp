#include "includes.h"
#include "ServerGameState.h"

ServerGameState::ServerGameState(Game* game) :
   GameState(game)
{
}

ServerGameState::~ServerGameState()
{
}

void ServerGameState::init() {}
void ServerGameState::deinit() {}
void ServerGameState::update(unsigned int deltaTime) {}

void ServerGameState::processSDLEvent(const SDL_Event& evnt) {}
