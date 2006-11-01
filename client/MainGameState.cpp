#include "includes.h"
#include "xcore/Unit.h"
#include "xcore/EventManager.h"
#include "xcore/MapLoadEvent.h"
#include "xcore/ClientConnectEvent.h"
#include "xcore/GameJoinEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "xcore/GameListEvent.h"
#include "xcore/UnitCreateEvent.h"
#include "xcore/UnitMoveEvent.h"
#include "xcore/MapLoadEvent.h"
#include "xcore/StartGameEvent.h"
#include "xcore/UnitWaitEvent.h"
#include "xcore/UnitActiveEvent.h"
#include "xcore/UnitFireEvent.h"
#include "xcore/UnitInvSwapEvent.h"
#include "xcore/GameOverEvent.h"
#include "MainGameState.h"
#include "ConfigOptions.h"
#include "Display.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Chat.h"
#include "ClientEntityFactory.h"
#include "ClientNetwork.h"
#include "FrontEndGameState.h"



#define GAME_THEME "resources/sounds/GameTheme.wav"

MainGameState::MainGameState(Game* game) :
  GameState(game),
  _map(),
  _readyToLoad(false), // change to false when event stuff is ready
  _localPlayer(spPlayer())
{
}


MainGameState::~MainGameState()
{
    delete [] _fog;
}


void MainGameState::deinit()
{
   Input::instance().setMode( NONE );
}

void MainGameState::init()
{
   Display::instance().loadFont("resources/fonts/FreeMono.ttf");

   // event stuff... not done yet
   EventManager::instance().addListener<GameListEvent>(this);
   EventManager::instance().addListener<ClientConnectEvent>(this);
   EventManager::instance().addListener<PlayerJoinEvent>(this);
   EventManager::instance().addListener<MapLoadEvent>(this);
   EventManager::instance().addListener<UnitCreateEvent>(this);
   EventManager::instance().addListener<UnitMoveEvent>(this);
   EventManager::instance().addListener<UnitActiveEvent>(this);
   EventManager::instance().addListener<StartGameEvent>(this);
   EventManager::instance().addListener<UnitWaitEvent>(this);
   EventManager::instance().addListener<UnitFireEvent>(this);
   EventManager::instance().addListener<UnitInvSwapEvent>(this);
   EventManager::instance().addListener<GameOverEvent>(this);

   ConfigOptions& o = ConfigOptions::instance();
   ClientNetwork& cn = ClientNetwork::instance();
   cn.connectToServer(o.get<string>(HOSTNAME).c_str(), o.get<int>(PORT));

   //ConfigOptions& o = ConfigOptions::instance();
   //ClientNetwork& cn = ClientNetwork::instance();
   //cn.connectToServer(o.get<char*>(HOSTNAME), o.get<int>(PORT));

   Display::instance().loadFont("resources/fonts/FreeMono.ttf");
   _map = ClientMap::makeMap();
   //loadMap(ConfigOptions::instance().get<string>(MAP));
   //ResourceManager::instance().load("resources/main.xcr");
   //loadMap("maps/test.xcm");
   //_camera.setBorders( 
   //    _map->getTile(_map->getHeight() - 1, 0)->getScreenX(),
   //    _map->getTile(0, _map->getWidth() - 1)->getScreenX() + ClientMapTile::getWidth(),
   //    _map->getTile(0, 0)->getScreenY(),
   //    _map->getTile(_map->getHeight() - 1, _map->getWidth() - 1)->getScreenY() + ClientMapTile::getHeight() );
   ////_localPlayer = spPlayer(new Player(o.get<char*>(PLAYER_NAME)));
   //spUnit u = createUnit(1, 0, 0);
   //u->markActive();
   //u->markSelected();
   //u->updatePossibleMoves();
   //focusOnUnit(u);

   //MusicTheme = Audio::instance().loadMusic(GAME_THEME);
   //Audio::instance().playMusic(-1, MusicTheme);
   Input::instance().setMode( COMMAND );
}

void MainGameState::update(uint32 deltaTime)
{
   if (_readyToLoad)
   {
      _camera.update(deltaTime);
      Point offset = _camera.getPosition();

      _map->drawTerrainLayer(offset);

      map<spMapTile, int> moveTiles = ClientUnit::getSelectedUnit()->getPossibleMoves();
      map<spMapTile, int>::iterator iter2;
      for (iter2 = moveTiles.begin(); iter2 != moveTiles.end(); ++iter2)
      {
         (*iter2).first->highlightMoveable(offset);
         ostringstream stream;
         stream << (*iter2).second;
         Display::instance().draw((*iter2).first->getCenterX()-offset.x, (*iter2).first->getCenterY()-offset.y, stream.str());
      }

	  if ( Input::instance().getMode() == USE && _activeUnit->getHand(Input::instance().getHand()).get() )
       Display::instance().highlightUsable( _map->getTile(_activeUnit->getX(),_activeUnit->getY()), _activeUnit->getHand(Input::instance().getHand())->getRange(), offset.x, offset.y ); 

      _map->highlightMouseOverTile(offset);
      _map->drawObjectLayer(offset);
      spUnit u = getActiveUnit();
      u->drawMovePath(offset);
     
      //ostringstream stream;
      //stream << "Mouse Position: " << p.x << ", " << p.y;

      //Display::instance().draw(5, 5, stream.str());
   //   _map->drawMinimap();
      Chat::instance().draw();
      Display::instance().drawGUI();
      Display::instance().drawCursor();
   }
   else
   {
      Display::instance().draw(275, 300, "Waiting for players...");
   }
}

bool MainGameState::loadMap(string fileName)
{
   return _map->load(fileName);
}

spUnit MainGameState::createUnit(int playerID, int x, int y)
{
   ClientEntityFactory& cef = ClientEntityFactory::instance();
   spUnit u = cef.makeUnit(playerID, _map->getTile(x, y));
   // all items added here must be added in server as well so that itemids match
   u->addItem(cef.makePistol());
   u->addItem(cef.makeGrenade());
   //u->addItem(cef.makePistolClip());
   vector<Player>::iterator iter;

   //for (iter = _players.begin(); iter != _players.end(); ++iter)
   //{
   //   if ((*iter).getID() == playerID)  
   //   {
   //      (*iter).addUnit(u);
   //      break;
   //   }
   //}

   _units[u->getID()] = u;

   //_unitQueue.push_back(u);
   if (getActiveUnit().get() && getActiveUnit()->getPlayerID() == _localPlayer->getID())
   {
      getActiveUnit()->updatePossibleMoves();
   }
   return u;
}

spUnit MainGameState::getActiveUnit()
{
   return _activeUnit;
    // returns the unit to act next
   //return (!_unitQueue.empty()) ? _unitQueue.front() : spUnit();
}

void MainGameState::processSDLEvent(SDL_Event& event) 
{
   if (!_readyToLoad)
   {
      return;
   }
   if ( event.type == SDL_KEYDOWN )
   {
      switch ( event.key.keysym.sym )
      {
         case SDLK_n:
            if (_map->getTile(0, 0)->isPassable())
            {
               // for now, the position will be determined by the server
               ClientNetwork::instance().send(UnitCreateEvent(_localPlayer->getID(), 0,0));
               //spUnit u = createUnit(1,0,0);
            }
            break;
         case SDLK_SPACE:
            endActiveUnitTurn();
            break;
         default:
            break;
      }
      spUnit u = getActiveUnit();
      if (u == ClientUnit::getSelectedUnit())
      {
         switch ( event.key.keysym.sym )
         {
            //case SDLK_KP1:
            //  u->move(Direction::SW);
            //   break;
            //case SDLK_KP2:
            //   u->move(Direction::S);
            //   break;
            //case SDLK_KP3:
            //   u->move(Direction::SE);
            //   break;
            //case SDLK_KP4:
            //   u->move(Direction::W);
            //   break;
            //case SDLK_KP6:
            //   u->move(Direction::E);
            //   break;
            //case SDLK_KP7:
            //   u->move(Direction::NW);
            //   break;
            //case SDLK_KP8:
            //   u->move(Direction::N);
            //   break;
            //case SDLK_KP9:
            //   u->move(Direction::NE);
            //   break;
            case SDLK_m:
               if (u->hasMovePath())
               {
                  u->moveAlongPath();
               }
               break;
            default:
               break;
         }
      }
   }
   else if ( event.type == SDL_MOUSEMOTION )
   {
      _map->updateMouseOverTile(Input::instance().getMousePosition(), _camera.getPosition());
   }
   else if ( event.type == SDL_MOUSEBUTTONDOWN )
   {
      spUnit u = getActiveUnit();
      spMapTile t = _map->getMouseOverTile();
      if (t.get() && !_fog[t->getX()*_map->getWidth()+t->getY()] && t->hasUnit())
      {
         selectUnit(t->getUnit());
      }
      else if (u->hasMovePath() && t == u->getDestination())
      {
         spMapTile t = u->getDestination();
         ClientNetwork::instance().send(UnitMoveEvent(u->getID(), t->getX(), t->getY()));
         //u->moveAlongPath(true);
      }
      else
      {
         if (u == ClientUnit::getSelectedUnit())
         {
            u->computeMovePath(_map->getMouseOverTile());
         }
      }
   }
}

void MainGameState::endActiveUnitTurn()
{
   spUnit u = getActiveUnit();
   if (u->getPlayerID() == _localPlayer->getID())
   {
      u->clearMovePath();
      if (u->hasMaxActionPoints())
      {
         ClientNetwork::instance().send(UnitWaitEvent(_activeUnit->getID()));
      }
      ClientNetwork::instance().send(UnitActiveEvent(_activeUnit->getID()));
   }
   //spUnit u = getActiveUnit();
   //u->clearMovePath();
   //if (u->hasMaxActionPoints())
   //{
   //   u->wait();
   //}
   //_unitQueue.pop_front();
   //_unitQueue.push_back(u);
   //
   //_unitQueue.sort(boost::mem_fn(&Unit::hasTurnBefore));
   //while(!_unitQueue.front()->hasMaxActionPoints())
   //{
   //   for_each(_unitQueue.begin(), _unitQueue.end(), boost::mem_fn(&Unit::regenActionPoints));
   //}

   //getActiveUnit()->markActive();
   //getActiveUnit()->markSelected();
   //focusOnUnit(getActiveUnit());
}

void MainGameState::selectUnit( spUnit u )
{
   _activeUnit->clearMovePath();
   u->markSelected();
   if (u->getPlayerID() == _localPlayer->getID())
   {
      u->updatePossibleMoves();
   }
   else
   {
      u->clearPossibleMoves();
   }
 
   //spUnit active = getActiveUnit();

   //active->clearMovePath();
   //   
   //if (active->hasMaxActionPoints() && u->hasMaxActionPoints())
   //{
   //   // current unit hasn't moved yet, so allow switching
   //   _unitQueue.remove(u);
   //   _unitQueue.push_front(u);
   //   u->markActive();
   //}
}

void MainGameState::focusOnUnit( spUnit u )
{
   spMapTile tile = _map->getTile(u->getX(), u->getY());
   _camera.setPosition(-Display::instance().getScreenWidth()/2 + tile->getCenterX(), 
                       -Display::instance().getScreenHeight()/2 + tile->getCenterY());
   u->updatePossibleMoves();
   _map->updateMouseOverTile(Input::instance().getMousePosition(), _camera.getPosition());
}

spMap MainGameState::getMap()
{
   return _map;
}

void MainGameState::fire(spUnit target, spMapTile t, int hand)
{
   if (_activeUnit->getPlayerID() == _localPlayer->getID() &&  t.get() )
   {
      ClientNetwork::instance().send(UnitFireEvent(_activeUnit->getID(), t->getX(), t->getY(), hand));
   }
}
void MainGameState::swapEq(spUnit swapper, int slot1, int slot2)
{
    if (swapper->getPlayerID() == _localPlayer->getID())
        ClientNetwork::instance().send(UnitInvSwapEvent(swapper->getID(), slot1, slot2));
    // debug
    else
    {
        cerr << "you have swapped inventory items of a unit that does not belong to you" << endl;
        exit(1);
    }
}

void MainGameState::handleEvent(GameListEvent& evnt)
{
cout << "game list event" << endl;
   bool host = false;
   if (evnt.getGames().empty()) 
   {
      host = true;
   }
   ClientNetwork::instance().send(GameJoinEvent(ConfigOptions::instance().get<string>(GAME_NAME), host));
}

// this will only ever be sent to the client for itself, not other players
void MainGameState::handleEvent(ClientConnectEvent& e)
{
   if (!_localPlayer.get())
   {
      _localPlayer = spPlayer(new Player(e.getPlayerID()));
      _players.push_back(_localPlayer);
      ClientNetwork::instance().send(PlayerJoinEvent(ConfigOptions::instance().get<string>(PLAYER_NAME)));
   }
   else {
      cerr << "Received ClientConnectEvent after player already created" << endl;
   }
}

void MainGameState::handleEvent(PlayerJoinEvent& e)
{
   assert(_localPlayer.get());

   cout << "received player join event" << endl;
   if (_localPlayer->getID() == e.getPlayerID()) {
      _localPlayer->setName(e.getPlayerName());

      // move this once there's an GUI to get the list
      ClientNetwork::instance().send(GameListEvent());
      cout << "asking for game list" << endl;
   }
   else
   {
      spPlayer p = spPlayer(new Player(e.getPlayerID(), e.getPlayerName()));
      _players.push_back(p);
   }
}

void MainGameState::handleEvent(MapLoadEvent& e)
{  
   loadMap(e.getMapName());
   _camera.setBorders( 
       _map->getTile(_map->getHeight() - 1, 0)->getScreenX(),
       _map->getTile(0, _map->getWidth() - 1)->getScreenX() + ClientMapTile::getWidth(),
       _map->getTile(0, 0)->getScreenY(),
       _map->getTile(_map->getHeight() - 1, _map->getWidth() - 1)->getScreenY() + ClientMapTile::getHeight() );
   //_localPlayer = spPlayer(new Player(o.get<char*>(PLAYER_NAME)));
   //spUnit u = createUnit(1, 0, 0);
   //u->markActive();
   //u->markSelected();
   //u->updatePossibleMoves();
   //focusOnUnit(u);

   //MusicTheme = Audio::instance().loadMusic(GAME_THEME);
   //Audio::instance().playMusic(-1, MusicTheme);
}

void MainGameState::handleEvent(UnitCreateEvent& e)
{
   createUnit(e.getPlayerID(), e.getX(), e.getY());
   cout << "new unit: " << e.getX() << "," << e.getY() << endl;
   // why does this crash?  are units created before the map?
   //if ( e.getPlayerID() == _localPlayer->getID() )
       //updateFog();
}

void MainGameState::handleEvent(UnitMoveEvent& e)
{
   //spUnit u = _units[e.getUnitID()];
	spUnit u = getUnitFromID(e.getUnitID());
   spMapTile t = _map->getTile(e.getX(), e.getY());
   //u->updatePossibleMoves();
   u->updatePossibleMoves();
   u->computeMovePath(t);
   u->moveAlongPath(true);
   if (u->getPlayerID() == _localPlayer->getID())
   {
      u->updatePossibleMoves();
      updateFog();
   }
   else {
      u->clearPossibleMoves();
   }
   //u->move(_map->getTile(e.getX(), e.getY()));
   cout << "unit move" << endl;
}

void MainGameState::handleEvent(StartGameEvent& e)
{
   getActiveUnit()->markSelected();
   _readyToLoad = true;
   cout << "start game" << endl;
   _fog = new bool[_map->getWidth()*_map->getHeight()];
   _shroud = new bool[_map->getWidth()*_map->getHeight()];
    for ( int i = 0; i < _map->getWidth()*_map->getHeight(); ++i )
        _fog[i] = _shroud[i] = true;
   updateFog();
}

void MainGameState::handleEvent(GameOverEvent& e)
{
   SDL_ShowCursor( SDL_ENABLE );
   ClientNetwork::instance().disconnect();
   GameState* fegs = new FrontEndGameState(_game);
   fegs->init();
   _game->changeState(fegs);
   cout << "game over" << endl;
}

void MainGameState::handleEvent(UnitActiveEvent& e)
{
   //_activeUnit = _units[e.getUnitID()];
	spUnit u = getUnitFromID(e.getUnitID());
	u->markActive();
	_activeUnit = u;
   // this could probably be replaced by a single function call for each unit that regens a certain number of times
   //      instead of looping a bunch
   map<uint32, spUnit>::iterator iter;
   for (iter = _units.begin(); iter != _units.end(); ++iter)
   {
		if (iter->second.get())
			iter->second->regenActionPoints(e.getNumberOfRegens());
   }

   if (_activeUnit->getPlayerID() == _localPlayer->getID()) {
      _activeUnit->updatePossibleMoves();
      _activeUnit->markSelected();
      focusOnUnit(_activeUnit);
   }
   cout << "unit active" << endl;
}

void MainGameState::handleEvent(UnitWaitEvent& e)
{
   //spUnit u = _units[e.getUnitID()];
	spUnit u = getUnitFromID(e.getUnitID());
   u->wait();
   if (_units[e.getUnitID()]->getPlayerID() == _localPlayer->getID())
   {
      u->updatePossibleMoves();
   }
      
   cout << "unit wait" << endl;
}

void MainGameState::handleEvent(UnitFireEvent& e)
{
   //spUnit u = _units[e.getUnitID()];
	spUnit u = getUnitFromID(e.getUnitID());
   u->use(_map->getTile(e.getX(), e.getY()), e.getHand());
   if (_units[e.getUnitID()]->getPlayerID() == _localPlayer->getID())
      u->updatePossibleMoves();
}

void MainGameState::handleEvent(UnitInvSwapEvent& e)
{
   //spUnit u = _units[e.getUnitID()];
   spUnit u = getUnitFromID(e.getUnitID());
   u->swapEq(e.getSlot1(), e.getSlot2());
   // debug
   if (_units[e.getUnitID()]->getPlayerID() == _localPlayer->getID())
		u->updatePossibleMoves();
   else
        cerr << "somebody has changed the inventory of your unit!" << endl;
}

bool MainGameState::isMyTurn() const
{
    if ( !_activeUnit.get() )
        return false;
    return ( _activeUnit->getPlayerID() == _localPlayer->getID() );
}

void MainGameState::updateFog()
{
    if ( !_map.get() || !_activeUnit.get() ) return;
    for ( int i = 0; i < _map->getWidth()*_map->getHeight(); ++i )
        _fog[i] = true;
    int width = _map->getWidth(), height = _map->getHeight(), sightRadius = 10;
    spUnit unit;
    spMapTile tile, tile2;
    for ( int i = 0; i < width; ++i )
    {
        for ( int j = 0; j < height; ++j )
        {
            tile = _map->getTile(i,j);
            unit = tile->getUnit();
            if ( unit.get() && unit->getPlayerID() == _localPlayer->getID() )
            {
                for ( int k = 0; k < width; ++k )
                {
                    for ( int l = 0; l < height; ++l )
                    {
                        tile2 = _map->getTile(k,l);
                        if ( tile->getDistance(tile2) <= sightRadius )  
                        {
                            _fog[k*width+l] = false;
                            _shroud[k*width+l] = false;
                        }
                    }
                }
            }
        }
    }
}

int MainGameState::getFog( int x, int y ) const
{
    if ( !_map.get() ) return true;
    return _fog[x*_map->getWidth()+y];
}
int MainGameState::getShroud( int x, int y ) const
{
    if ( !_map.get() ) return true;
    return _shroud[x*_map->getWidth()+y];
}

spPlayer MainGameState::getLocalPlayer() { return _localPlayer; }

spUnit MainGameState::getUnitFromID(uint32 id)
{
	map<uint32, spUnit>::iterator iter;
   for (iter = _units.begin(); iter != _units.end(); ++iter)
   {
      if ( iter->second->getID() == id )
		  return iter->second;
   }
   // this should never happen
	return _units[0];
}
