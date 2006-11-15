#include "includes.h"
#include "xcore/Unit.h"
#include "xcore/EventManager.h"
#include "xcore/MapLoadEvent.h"
#include "xcore/GameJoinEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "xcore/UnitCreateEvent.h"
#include "xcore/UnitMoveEvent.h"
#include "xcore/MapLoadEvent.h"
#include "xcore/StartGameEvent.h"
#include "xcore/UnitWaitEvent.h"
#include "xcore/UnitActiveEvent.h"
#include "xcore/UnitFireEvent.h"
#include "xcore/UnitInvSwapEvent.h"
#include "xcore/GameOverEvent.h"
#include "xcore/UseMapObjectEvent.h"
#include "MainGameState.h"
#include "ConfigOptions.h"
#include "Display.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Chat.h"
#include "ClientEntityFactory.h"
#include "ClientNetwork.h"
#include "FrontEndGameState.h"

#define RESOURCE_FILE "resources/main.xcr"
#define GAME_THEME "resources/sounds/GameTheme.wav"

MainGameState::MainGameState(Game* game, spPlayer localPlayer) :
  GameState(game),
  _map(),
  _readyToLoad(false), // change to false when event stuff is ready
  _localPlayer(localPlayer),
  _fog(NULL),
  _canUseObject(false)
{
    _players[_localPlayer->getID()] = _localPlayer;
}


MainGameState::~MainGameState()
{
   SAFE_DELETE_ARRAY(_fog);
   EventManager::instance().removeListener(this); 
}


void MainGameState::deinit()
{
   Input::instance().setMode( (InputMode)0 );
   ResourceManager::instance().unloadAll();
}

void MainGameState::init()
{
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
   EventManager::instance().addListener<UseMapObjectEvent>(this);

   ClientEntityFactory::instance().resetEntities();

   ResourceManager::instance().load(RESOURCE_FILE);

   ConfigOptions& o = ConfigOptions::instance();

   Display::instance().loadFont("resources/fonts/FreeMono.ttf");
   _map = ClientMap::makeMap();

   //MusicTheme = Audio::instance().loadMusic(GAME_THEME);
   //Audio::instance().playMusic(-1, MusicTheme);
   Input::instance().setMode( COMMAND );
}

string MainGameState::getName()
{
   return "Main";
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
     {
        Display::instance().highlightUsable( _map->getTile(_activeUnit->getX(),_activeUnit->getY()),
           _activeUnit->getHand(Input::instance().getHand())->getRange(), offset.x, offset.y, _activeUnit->getHand(Input::instance().getHand())->getType() == GRENADE ); 
     }

      if (_displayMouseOverHighlight)
      {
         _map->highlightMouseOverTile(offset);
      }
      _map->drawObjectLayer(offset);
      spUnit u = getActiveUnit();
      u->drawMovePath(offset);

      Sprite::drawOrphans(offset);

      ostringstream stream;
      Point p = Input::instance().getMousePosition();
      stream << "Mouse Position: " << p.x << ", " << p.y;

      Display::instance().draw(5, 5, stream.str());
   //   _map->drawMinimap();
      Chat::instance().draw();
      Display::instance().drawGUI(_canUseObject);
      Display::instance().drawCursor();
   }
   else
   {
      Display::instance().draw(275, 300, "Waiting for players...");
   }
}

void MainGameState::setLocalPlayer(spPlayer player)
{
   _localPlayer = player;
}

bool MainGameState::loadMap(string fileName)
{
   return _map->load(fileName);
}

spUnit MainGameState::createUnit(uint32 playerID, uint32 x, uint32 y, itemtype s0, itemtype s1, itemtype s2, 
                                 itemtype s3, itemtype s4, itemtype s5, itemtype s6)
{
   ClientEntityFactory& cef = ClientEntityFactory::instance();
   spUnit u = cef.makeUnit(playerID, _map->getTile(x, y), _players[playerID]->getPlayerNumber());
   // all items added here must be added in server as well so that itemids match
   u->addItem(cef.makeItem(s0), 0);
   u->addItem(cef.makeItem(s1), 1);
   u->addItem(cef.makeItem(s2), 2);
   u->addItem(cef.makeItem(s3), 3);
   u->addItem(cef.makeItem(s4), 4);
   u->addItem(cef.makeItem(s5), 5);
   u->addItem(cef.makeItem(s6), 6);
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
   updateCanUseObject();
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
            spMapTile mot = _map->getMouseOverTile();
            if (mot.get() && !getFog(mot->getX(), mot->getY()))
            {
               u->computeMovePath(_map->getMouseOverTile());
            }
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
}

void MainGameState::selectUnit( spUnit u )
{
   _activeUnit->clearMovePath();
   u->markSelected();
   _selectedUnit = u;
   if (u->getPlayerID() == _localPlayer->getID())
   {
      u->updatePossibleMoves();
   }
   else
   {
      u->clearPossibleMoves();
   }

   updateCanUseObject();
 
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
   updateCanUseObject();
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

void MainGameState::handleEvent(PlayerJoinEvent& e)
{
   assert(_localPlayer.get());

   cout << "received player join event" << endl;
   if (_localPlayer->getID() == e.getPlayerID()) {
      //_localPlayer->setName(e.getPlayerName());

      //// move this once there's an GUI to get the list
      //ClientNetwork::instance().send(GameListEvent());
      //cout << "asking for game list" << endl;
   }
   else
   {
      spPlayer p = spPlayer(new Player(e.getPlayerID(), e.getPlayerName(), e.getPlayerNumber()));
      _players[e.getPlayerID()] = (p);
   }
}

void MainGameState::handleEvent(MapLoadEvent& e)
{  
   loadMap(e.getFileName());
   _camera.setBorders( 
       _map->getTile(_map->getHeight() - 1, 0)->getScreenX(),
       _map->getTile(0, _map->getWidth() - 1)->getScreenX() + ClientMapTile::getWidth(),
       _map->getTile(0, 0)->getScreenY(),
       _map->getTile(_map->getHeight() - 1, _map->getWidth() - 1)->getScreenY() + ClientMapTile::getHeight() );

   //MusicTheme = Audio::instance().loadMusic(GAME_THEME);
   //Audio::instance().playMusic(-1, MusicTheme);
}

void MainGameState::handleEvent(UnitCreateEvent& e)
{
   createUnit(e.getPlayerID(), e.getX(), e.getY(), e.getS0(), e.getS1(), e.getS2(), 
      e.getS3(), e.getS4(), e.getS5(), e.getS6());
   //cout << "new unit: " << e.getX() << "," << e.getY() << endl;
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

   updateCanUseObject();
   cout << "unit move" << endl;
}

void MainGameState::handleEvent(StartGameEvent& e)
{
   srand(e.getSeed());
   selectUnit(getActiveUnit());
   _readyToLoad = true;
   cout << "start game" << endl;
   _fog = new bool[_map->getWidth()*_map->getHeight()];
   _shroud = new bool[_map->getWidth()*_map->getHeight()];
    for ( int i = 0; i < _map->getWidth()*_map->getHeight(); ++i )
        _fog[i] = _shroud[i] = true;
   updateFog();
   updateCanUseObject();
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
	spUnit u = _units[e.getUnitID()];
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
      selectUnit(_activeUnit);
      focusOnUnit(_activeUnit);
   }

   updateCanUseObject();

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
      
   updateCanUseObject();

   cout << "unit wait" << endl;
}

void MainGameState::handleEvent(UnitFireEvent& e)
{
   //spUnit u = _units[e.getUnitID()];
	spUnit u = getUnitFromID(e.getUnitID());
   u->use(_map->getTile(e.getX(), e.getY()), e.getHand());
   if (_units[e.getUnitID()]->getPlayerID() == _localPlayer->getID())
   {
      u->updatePossibleMoves();
   }

   updateCanUseObject();
   updateFog();
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

   updateCanUseObject();
}

void MainGameState::handleEvent(UseMapObjectEvent& e)
{
   spUnit u = getUnitFromID(e.getUnitID());
   u->useNearbyObjects();
   u->updatePossibleMoves();
   updateCanUseObject();
   updateFog();
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
                            Point los = existsLOS( tile, tile2 );
                            _fog[los.x*width+los.y] = false;
                            _shroud[los.x*width+los.y] = false;
                        }
                    }
                }
            }
        }
    }
}

#define ROUND(d)  (int)floor(d + 0.5)

Point MainGameState::existsLOS( spMapTile start, spMapTile end )
{
    double xi = (double)start->getX(), yi = (double)start->getY();
    double xf = (double)end->getX(), yf = (double)end->getY();
    double x = xi, y = yi;
    int d = start->getDistance(end);
    double dx = (xf-xi)/d, dy = (yf-yi)/d;
    spMapTile tile = spMapTile();
    for ( int i = 0; i < d; ++i )
    {
        x += dx; y += dy;
        tile = _map->getTile(ROUND(x),ROUND(y));
        if ( tile->getTerrainType() != WATER && !tile->isPassable() )
        {
            Point obstruction; obstruction.x = ROUND(x); obstruction.y = ROUND(y);
            return obstruction;
        }
    }
    Point destination; destination.x = ROUND(xf); destination.y = ROUND(yf);
    return destination;
}

bool MainGameState::getFog( int x, int y ) const
{
    if ( !_map.get() ) return true;
    return _fog[x*_map->getWidth()+y];
}
bool MainGameState::getShroud( int x, int y ) const
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

void MainGameState::updateCanUseObject()
{
   spUnit u = getActiveUnit();
   if (u.get() && u->getPlayerID() == _localPlayer->getID() && 
       u.get() == _selectedUnit.get() && 
       u->getActionPoints() >= COST_USE)
   {
      _canUseObject = u->canUseNearbyObjects();
   }
   else
   {
      _canUseObject = false;
   }
}

bool MainGameState::isUseObjectActive() const
{
   return _canUseObject;
}

void MainGameState::useObject()
{
   ClientNetwork::instance().send(UseMapObjectEvent(_activeUnit->getID()));
}

void MainGameState::setDisplayMouseOverHighlight(bool on)
{
   _displayMouseOverHighlight = on;
}

