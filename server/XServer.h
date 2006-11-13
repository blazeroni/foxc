#ifndef XSERVER_XSERVER_H
#define XSERVER_XSERVER_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "xcore/IEventListener.h"
#include "xcore/Game.h"
#include "Client.h"
#include "ServerGame.h"
#include "xcore/MapListEvent.h"

class XServer : public IEventListener, public Singleton<XServer>
{
   friend class Singleton<XServer>;
   public:
      virtual ~XServer();

      virtual void deinit();
      virtual void init();
      virtual void update();

      bool running();
      void quit();

      virtual void handleEvent(ChatEvent& e);
      virtual void handleEvent(GameListEvent& e);
      virtual void handleEvent(PlayerJoinEvent& e);
      virtual void handleEvent(ClientConnectEvent& e);
      virtual void handleEvent(ClientDisconnectEvent& e);
      virtual void handleEvent(GameJoinEvent& e);
      virtual void handleEvent(GameHostEvent& e);
      virtual void handleEvent(UnitCreateEvent& e);
      virtual void handleEvent(UnitMoveEvent& e);
      virtual void handleEvent(GameOverEvent& e);
      virtual void handleEvent(UnitActiveEvent& e);
      virtual void handleEvent(UnitWaitEvent& e);
      virtual void handleEvent(UnitFireEvent& e);
      virtual void handleEvent(UnitInvSwapEvent& e);
      virtual void handleEvent(MapListEvent& e);
      virtual void handleEvent(UseMapObjectEvent& e);
      virtual void handleEvent(StartGameEvent& e);

   private:
      XServer();

      void loadMapInfo();

      template <class T>
      void sendToLobby(const T &e);

      template <class T>
      void sendToPeers(uint32 source, const T &e);

      bool isInLobby(uint32 id);

      string createPlayerName(string requested, uint32 source);

      void quitSDL();

      map<uint32, spClient> _clients;

      map<uint32, spClient> _lobbyClients;

      map<uint32, spServerGame> _games;

//      vector<spServerGame> games;
      const string _configFile;
      bool _running;
      unsigned int lastUpdateTime;
      unsigned int deltaTime;

      vector<MapListItem> _maps;

      map<string, MapListItem> _nameInfoMap;

      map<string, string> _nameFileMap;

      fs::path _mapDir;
};

template <class T>
void XServer::sendToLobby(const T& e)
{
   map<uint32, spClient>::iterator iter;
   for (iter = _lobbyClients.begin(); iter != _lobbyClients.end(); ++iter)
   {
      iter->second->send(e);
   }
}

template <class T>
void XServer::sendToPeers(uint32 source, const T& e)
{
   if (_clients.find(source) != _clients.end())
   {
      spClient c = _clients[source];
      if (_games.find(_clients[source]->getGameID()) != _games.end())
      {
         _games[c->getGameID()]->send(e);
      }
      else if (_lobbyClients.find(source) != _lobbyClients.end())
      {
         sendToLobby(e);
      }
   }
}


#endif
