#ifndef XCLIENT_MAINGAMESTATE_H
#define XCLIENT_MAINGAMESTATE_H

#include "includes.h"
#include "xcore/GameState.h"
#include "xcore/IEventListener.h"
#include "xcore/Player.h"
#include "ClientMap.h"
#include "ClientUnit.h"
#include "Camera.h"
#include "Audio.h"

/**
 * State which represents actual gameplay
 *
 * @see GameState for more information regarding game states
 */
class MainGameState : public GameState, public IEventListener
{
   public:
      MainGameState(Game* app, spPlayer localPlayer);
      virtual ~MainGameState();

      virtual void deinit();
      virtual void init();
      virtual void update(uint32 deltaTime);

      virtual string getName();

      void processSDLEvent(SDL_Event& event);

      void setLocalPlayer(spPlayer player);

      /// Loads a new map from the file
      bool loadMap(string fileName);

      /// Creates a unit using the specified player and location
      spUnit createUnit(uint32 playerID, uint32 x, uint32 y, itemtype s0 = (itemtype)0, 
         itemtype s1 = (itemtype)0, itemtype s2 = (itemtype)0, itemtype s3 = (itemtype)0,
         itemtype s4 = (itemtype)0, itemtype s5 = (itemtype)0, itemtype s6 = (itemtype)0);

      /// Returns the unit which can currently perform actions
      spUnit getActiveUnit();

      /// Returns the map
      spMap getMap();

      /// Returns the unit which is seleted by the player
      spUnit getSelectedUnit();

      /// Ends the active unit turn, regens action points if necessary, and changes the active unit
      void endActiveUnitTurn();

      /// Moves the camera so the specified unit is centered on the screen
      void focusOnUnit( spUnit u );

      /// Changes the player's selected unit
      void selectUnit( spUnit u );

      // hack due to firing code being in Input
      void fire(spUnit u, spMapTile t, int hand);
      // hack due to swap code being in Input
      void swapEq(spUnit u, int slot1, int slot2);

        // is it my turn?
      bool isMyTurn() const;

      void setDisplayMouseOverHighlight(bool on);

      //virtual void handleEvent(ClientConnectEvent& e);
      //virtual void handleEvent(GameListEvent& evnt);
      virtual void handleEvent(PlayerJoinEvent& e);
      virtual void handleEvent(MapLoadEvent& e);
      virtual void handleEvent(UnitCreateEvent& e);
      virtual void handleEvent(UnitMoveEvent& e);
      virtual void handleEvent(StartGameEvent& e);
      virtual void handleEvent(GameOverEvent& e);
      virtual void handleEvent(UnitActiveEvent& e);
      virtual void handleEvent(UnitWaitEvent& e);
      virtual void handleEvent(UnitFireEvent& e);
      virtual void handleEvent(UnitInvSwapEvent& e);
      virtual void handleEvent(UseMapObjectEvent& e);

      bool getFog( int x, int y ) const;
      bool getShroud( int x, int y ) const;
      spPlayer getLocalPlayer();
	  // gets a unit from _units given an id
	  spUnit getUnitFromID(uint32 id);

     bool isUseObjectActive() const;
     void useObject();
     Point existsLOS( spMapTile start, spMapTile end );
      void updateFog();

   private:
      spPlayer getPlayer() const;
      void updateCanUseObject();

      Mix_Music* MusicTheme;

      spClientMap _map;
      bool *_fog, *_shroud;

      spPlayer _localPlayer;

      // _players includes _localPlayer to ease cycling through all players;
      map<int, spPlayer> _players;

      // waiting to load should really be somewhere before this or represented by the state pattern
      bool _readyToLoad;

      map<uint32, spUnit> _units;
      spUnit _activeUnit;

      bool _canUseObject;

      bool _displayMouseOverHighlight;

      /**
       * Unit action order
       *
       * Holds a reference to every unit. 
       * The unit at the front of the queue is the current unit.
       */
      //list<spUnit> _unitQueue;

      map<uint32, spGameEntity> _entities;

      spUnit _selectedUnit;
      Camera _camera;
};

#endif
