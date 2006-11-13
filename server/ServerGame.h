#ifndef XSERVER_SERVERGAME_H
#define XSERVER_SERVERGAME_H

#include "includes.h"
#include "xcore/Game.h"
#include "xcore/Map.h"
#include "xcore/UnitCreateEvent.h"
#include "Client.h"

class ServerGame;
typedef boost::shared_ptr<ServerGame> spServerGame;

class ServerGame : public Game
{
   public:
      ServerGame(uint32 gameID, string gameName, string mapFile, uint16 players, uint32 points);
      virtual ~ServerGame();

      virtual void init();
      virtual void update();
      virtual void deinit();

      virtual void processSDLEvent(SDL_Event& evnt);

      bool join(spClient client);
      void leave(spClient client);

      // sends to all clients
      template <class T>
      void send(const T &e);

      uint32 getGameID() const;
      string getGameName() const;
      string getMapName() const;
      uint16 getNumberOfPlayers() const;
      uint16 getMaxNumberOfPlayers() const;

      uint32 getMaxPoints() const;
      void setMaxPoints(uint32 points);

      void tryStart();

      void handleEvent(UnitCreateEvent& e);
      void handleEvent(UnitMoveEvent& e);
      void handleEvent(UnitActiveEvent& e);
      void handleEvent(UnitWaitEvent& e);
      void handleEvent(UnitFireEvent& e);
      void handleEvent(UnitInvSwapEvent& e);
      void handleEvent(UseMapObjectEvent& e);
      void handleEvent(StartGameEvent& e);

   private:
      void addUnit(spUnit);
      spUnit getActiveUnit();
      void activateNextUnit(bool firstTurn = false);
      void start();
      bool isGameOver();
      int _playersReady;

      // temp function to automate initial units
      void createInitialUnits();
      // temp vars for unit creation;

      //spClient firstPlayer;
      //spClient secondPlayer;

      uint32 _gameID;
      string _gameName;

      uint16 _maxPlayers;

      uint32 _maxPoints;

      string _mapFile;

      map<uint32, spClient> _clients;

      map<uint32, spUnit> _units;
      list<spUnit> _unitQueue;

      spMap _map;

      spEntityFactory _factory;
};

template <class T>
void ServerGame::send(const T& e)
{
   map<uint32, spClient>::iterator iter;
   for (iter = _clients.begin(); iter != _clients.end(); ++iter)
   {
      iter->second->send(e);
   }
}

#endif
