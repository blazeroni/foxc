#include "includes.h"
#include "ServerGame.h"
#include "xcore/UnitMoveEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "xcore/GameOverEvent.h"
#include "xcore/UnitWaitEvent.h"
#include "xcore/UnitActiveEvent.h"
#include "xcore/StartGameEvent.h"
#include "xcore/UnitFireEvent.h"
#include "xcore/UnitInvSwapEvent.h"

ServerGame::ServerGame(uint32 gameID, string gameName) :
   _gameID(gameID),
   _gameName(gameName)
{
}

ServerGame::~ServerGame()
{
}

void ServerGame::init()
{
   _factory = spEntityFactory(new EntityFactory(_gameID));
   _map = Map::makeMap(_factory);
   _map->load("maps/test.xcm");
}

void ServerGame::update()
{

}

void ServerGame::deinit()
{

}

void ServerGame::processSDLEvent(SDL_Event& event)
{

}

void ServerGame::join(spClient client)
{
   _clients[client->getPlayerID()] = client;
   client->setGameID(_gameID);
   client->send(MapLoadEvent(_map->getName()));

   // notify clients of other clients
   map<uint32, spClient>::iterator iter;

   for (iter = _clients.begin(); iter != _clients.end(); ++iter)
   {
      if (client != iter->second)
      {
         iter->second->send(PlayerJoinEvent(client->getPlayerName(), client->getPlayerID()));
         client->send(PlayerJoinEvent(iter->second->getPlayerName(), iter->second->getPlayerID()));
      }
   }

   if(firstPlayer.get() == NULL)
   {
      firstPlayer = client;
   }
   else
   {
      secondPlayer = client;
      createInitialUnits();
      activateNextUnit(true);
      start();
   }
}

void ServerGame::createInitialUnits()
{
   map<uint32, spClient>::iterator iter;

   static bool first = true;
   uint32 x = (first) ? 0 : _map->getWidth() - 1;
   uint32 y = (first) ? 0 : _map->getHeight() - 1;

   for (iter = _clients.begin(); iter != _clients.end(); ++iter)
   {
      uint32 x = (first) ? 0 : _map->getWidth() - 1;
      uint32 y = (first) ? 0 : _map->getHeight() - 1;
      first = false;

      spUnit u = _factory->makeUnit(iter->second->getPlayerID(), _map->getTile(x, y));
	  u->addItem(_factory->makePistol());
	  u->addItem(_factory->makeGrenade());
	  //u->addItem(_factory->makePistolClip());
      addUnit(u);
      send(UnitCreateEvent(iter->second->getPlayerID(), x, y));
   }
}

void ServerGame::leave(spClient client)
{
   _clients.erase((client->getPlayerID()));
   client->setGameID(0);
   if (_clients.empty()) 
   {
      spEvent e = spEvent(new GameOverEvent(_gameID));
      send(*e);
      e->fire();
   }
}

void ServerGame::addUnit(spUnit u)
{
   _units[u->getID()] = u;
   _unitQueue.push_back(u);
}

void ServerGame::activateNextUnit(bool firstTurn)
{
   spUnit u = getActiveUnit();

   if (!firstTurn && u.get())
   {
      //if (u->hasMaxActionPoints())
      //{
      //   send(UnitWaitEvent(u->getID()));
      //   u->wait();
      //}
      _unitQueue.pop_front();
      _unitQueue.push_back(u);
   }
   
   _unitQueue.sort(boost::mem_fn(&Unit::hasTurnBefore));
   uint32 regens = 0;
   while(!_unitQueue.front()->hasMaxActionPoints())
   {
      list<spUnit>::iterator iter;
      for (iter = _unitQueue.begin(); iter != _unitQueue.end(); ++iter)
      {
         (*iter)->regenActionPoints();
      }
      regens++;
   }

   u = getActiveUnit();
   if (u.get())
   {
      //cout << "next active unit ID: " << u->getID() << endl;
      u->markActive();
      send(UnitActiveEvent(u->getID(), regens));
   }
}

spUnit ServerGame::getActiveUnit()
{
    // returns the unit to act next
   return (!_unitQueue.empty()) ? _unitQueue.front() : spUnit();
}

void ServerGame::start()
{
   send(StartGameEvent());
}

uint32 ServerGame::getGameID() const
{
   return _gameID;
}

string ServerGame::getGameName() const
{
   return _gameName;
}

bool ServerGame::isGameOver()
{
   uint32 player1 = 0;
   bool gameOver = true;

   map<uint32, spUnit>::iterator iter;
   for (iter = _units.begin(); iter != _units.end(); ++iter)
   {
      if (iter->second->getHealth() > 0)
      {
         if (!player1)
         {
            player1 = iter->second->getPlayerID();
         }
         else
         {
            if (player1 != iter->second->getPlayerID())
            {
               gameOver = false;
               break;
            }
         }
      }
   }
   return gameOver;
}

void ServerGame::handleEvent(UnitCreateEvent& e)
{
   spUnit u;
   uint32 x = 0;
   uint32 y = 0;

   // for now, allow any unit creation
   if (firstPlayer->getPlayerID() != e.getPlayerID())
   {
      x = _map->getWidth() - 1;
      y = _map->getHeight() - 1;
   }

   cout << "new unit at" << x << "," << y << endl;
   u = _factory->makeUnit(e.getPlayerID(), _map->getTile(x, y));
   addUnit(u);
   send(UnitCreateEvent(e.getPlayerID(), x, y));
}

void ServerGame::handleEvent(UnitMoveEvent& e)
{
   uint32 unitID = e.getUnitID();
   uint32 x = e.getX();
   uint32 y = e.getY();

   spUnit u =_units[e.getUnitID()];

   if (getActiveUnit() = u)
   {
      u->updatePossibleMoves();
      map<spMapTile, int> tiles = u->getPossibleMoves();
      spMapTile mt = _map->getTile(x, y);
      if (tiles.find(_map->getTile(x, y)) != tiles.end())
      {
         u->move(mt);
         u->useActionPoints(tiles[mt]);
         send(UnitMoveEvent(unitID, x, y));
      }
   }
}

void ServerGame::handleEvent(UnitActiveEvent& e)
{
   activateNextUnit();
}

void ServerGame::handleEvent(UnitWaitEvent& e)
{
   spUnit u = _units[e.getUnitID()];
   if (getActiveUnit() == u)
   {
      u->wait();
      send(UnitWaitEvent(e.getUnitID()));
   }
}

void ServerGame::handleEvent(UnitFireEvent& e)
{
   cout << "fire event" << endl;
   spUnit shooter = _units[e.getUnitID()];
   spMapTile tile = _map->getTile(e.getX(), e.getY());
   int hand = e.getHand();
   if (tile.get() && shooter.get())
   {
      spUnit target = tile->getUnit();
      if (shooter == getActiveUnit())
      {
         // damage isn't random, so doing this is ok
         shooter->use(tile, hand);

         send(UnitFireEvent(e.getUnitID(), e.getX(), e.getY(), e.getHand()));         
      }
   }
   
   if (isGameOver())
   {
      spEvent ev = spEvent(new GameOverEvent(_gameID));
      send(*ev);
      ev->fire();
   }
   
}

void ServerGame::handleEvent(UnitInvSwapEvent& e)
{
   spUnit swapper = _units[e.getUnitID()];
   int slot1 = e.getSlot1();
   int slot2 = e.getSlot2();
   if (swapper.get() && swapper == getActiveUnit())
   {
      cout << "inv swap event" << endl;
      swapper->swapEq(slot1, slot2);

      send(UnitInvSwapEvent(e.getUnitID(), slot1, slot2));         
   }
}
