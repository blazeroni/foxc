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
#include "xcore/UseMapObjectEvent.h"

ServerGame::ServerGame(uint32 gameID, string gameName, string mapFile, string clientMapFile, uint16 players, uint32 points) :
   _gameID(gameID),
   _gameName(gameName),
   _mapFile(mapFile),
   _clientMapFile(mapFile),
   _maxPlayers(players),
   _maxPoints(points)
{
    _playersReady = 0;
//    _numPlayers = 0;
}

ServerGame::~ServerGame()
{
}

void ServerGame::init()
{
   _factory = spEntityFactory(new EntityFactory(_gameID));
   _map = Map::makeMap(_factory);
   _map->load(_mapFile);
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

string ServerGame::getMapName() const
{
   return _map->getName();
}
 
uint16 ServerGame::getNumberOfPlayers() const
{
   return _clients.size();
}

uint16 ServerGame::getMaxNumberOfPlayers() const
{
   return _maxPlayers;
}

uint32 ServerGame::getMaxPoints() const
{
   return _maxPoints;
}

void ServerGame::setMaxPoints(uint32 points)
{
   _maxPoints = points;
}

map<uint32, spClient> ServerGame::getClients()
{
   return _clients;
}

bool ServerGame::join(spClient client)
{
   if (_maxPlayers == getNumberOfPlayers())
   {
      return false;
   }

   cout << "join method" << endl;

   _clients[client->getPlayerID()] = client;
   client->setGameID(_gameID);
   client->setPlayerNumber(_clients.size());

   // notify clients of other clients
   map<uint32, spClient>::iterator iter;

   for (iter = _clients.begin(); iter != _clients.end(); ++iter)
   {
      if (client != iter->second)
      {
         iter->second->send(PlayerJoinEvent(client->getPlayerName(), client->getPlayerID(), client->getPlayerNumber()));
         client->send(PlayerJoinEvent(iter->second->getPlayerName(), iter->second->getPlayerID(),  client->getPlayerNumber()));
      }
   }

   //if(firstPlayer.get() == NULL)
   //{
   //   firstPlayer = client;
   //}
   //else
   //{
   //   secondPlayer = client;
   //}

   return true;
}

void ServerGame::tryStart()
{
   if (_maxPlayers == getNumberOfPlayers() && _playersReady == _maxPlayers)
   {
      map<uint32, spClient>::iterator i;
      for (i = _clients.begin(); i != _clients.end(); ++i)
      {
         spClient c = i->second;
         sendToOthers(PlayerJoinEvent(c->getPlayerName(), c->getPlayerID(), c->getPlayerNumber()), c);
      }

      send(MapLoadEvent(_map->getName(), _clientMapFile));
      
      map<uint32, spUnit>::iterator iter;
      for (iter = _units.begin(); iter != _units.end(); ++iter)
	   {
         spUnit u = iter->second;
         itemtype hand0 = (u->getHand(0).get()) ? u->getHand(0)->getType() : itemtype(0);
         itemtype hand1 = (u->getHand(1).get()) ? u->getHand(1)->getType() : itemtype(0);
         itemtype it0 = (u->getInv(0).get()) ? u->getInv(0)->getType() : itemtype(0);
         itemtype it1 = (u->getInv(1).get()) ? u->getInv(1)->getType() : itemtype(0);
         itemtype it2 = (u->getInv(2).get()) ? u->getInv(2)->getType() : itemtype(0);
         itemtype it3 = (u->getInv(3).get()) ? u->getInv(3)->getType() : itemtype(0);
         itemtype it4 = (u->getInv(4).get()) ? u->getInv(4)->getType() : itemtype(0);

         send( UnitCreateEvent( iter->second->getPlayerID(), iter->second->getX(), iter->second->getY(),
            hand0, hand1, it0, it1, it2, it3, it4 ) );
      }

      activateNextUnit(true);
      start();
   }
}
/*
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
*/

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
   cout << "start game" << endl;
   uint32 seed = SDL_GetTicks();
   send(StartGameEvent(SDL_GetTicks()));
   srand(seed);
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
   // special case for 1 player test maps
   if (_units.size() > 0 && _maxPlayers == 1)
   {
      return false;
   }

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

/*
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
*/
void ServerGame::handleEvent(UnitCreateEvent& e)
{
   spMapTile tile = _map->getNextStartPref(_clients[e.getPlayerID()]->getPlayerNumber());
   if (!tile.get())
   {
      cerr << "No empty start positions left!" << endl;
      return;
   }

   spUnit u = _factory->makeUnit(e.getPlayerID(), tile, _clients[e.getPlayerID()]->getPlayerNumber());
   u->addItem(_factory->makeItem(e.getS0()), 0);
   u->addItem(_factory->makeItem(e.getS1()), 1);
   u->addItem(_factory->makeItem(e.getS2()), 2);
   u->addItem(_factory->makeItem(e.getS3()), 3);
   u->addItem(_factory->makeItem(e.getS4()), 4);
   u->addItem(_factory->makeItem(e.getS5()), 5);
   u->addItem(_factory->makeItem(e.getS6()), 6);
   addUnit(u);
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

void ServerGame::handleEvent(StartGameEvent& e)
{
   cout << "player ready" << endl;
   _playersReady++;
   tryStart();
}

void ServerGame::handleEvent(UseMapObjectEvent& e)
{
   spUnit unit = _units[e.getUnitID()];
   if (unit == getActiveUnit() && e.getSource() == unit->getPlayerID() && unit->canUseNearbyObjects())
   {
      unit->useNearbyObjects();
      send(UseMapObjectEvent(e.getUnitID()));
   }
}


