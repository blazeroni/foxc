#include "includes.h"
#include "Client.h"

Client::Client(uint32 id) :
   _socket(id),
   _playerName(),
   _playerNum(0)
{
}

Client::~Client()
{
}

void Client::setPlayerName(string name) 
{
   _playerName = name;
}

string Client::getPlayerName()
{
   return _playerName;
}
uint32 Client::getPlayerID() const
{
   return _socket;
}

void Client::setGameID(uint32 gameID)
{
   _gameID = gameID;
}
      
uint32 Client::getGameID() const 
{
   return _gameID;
}

uint16 Client::getPlayerNumber() const
{
   return _playerNum;
}
 
void Client::setPlayerNumber(uint16 num)
{
   _playerNum = num;
}

