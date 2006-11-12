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
      ServerGame(uint32 gameID, string gameName);
      virtual ~ServerGame();

      virtual void init();
      virtual void update();
      virtual void deinit();

      virtual void processSDLEvent(SDL_Event& evnt);

      void join(spClient client);
      void leave(spClient client);

      // sends to all clients
      template <class T>
      void send(const T &e);

      uint32 getGameID() const;
      string getGameName() const;

      void handleEvent(UnitCreateEvent& e);
      void handleEvent(UnitMoveEvent& e);
      void handleEvent(UnitActiveEvent& e);
      void handleEvent(UnitWaitEvent& e);
      void handleEvent(UnitFireEvent& e);
      void handleEvent(UnitInvSwapEvent& e);
      void handleEvent(StartGameEvent& e);

   private:
      void addUnit(spUnit);
      spUnit getActiveUnit();
      void activateNextUnit(bool firstTurn = false);
      void start();
      bool isGameOver();
      int _playersReady;
      int _numPlayers;

      // temp function to automate initial units
      void createInitialUnits();
      // temp vars for unit creation;

      spClient firstPlayer;
      spClient secondPlayer;

      uint32 _gameID;
      string _gameName;

      string _mapName;

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
