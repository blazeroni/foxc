#ifndef XSERVER_XSERVER_H
#define XSERVER_XSERVER_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "xcore/IEventListener.h"
#include "xcore/Game.h"
#include "Client.h"
#include "ServerGame.h"

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
      virtual void handleEvent(UnitCreateEvent& e);
      virtual void handleEvent(UnitMoveEvent& e);
      virtual void handleEvent(GameOverEvent& e);
      virtual void handleEvent(UnitActiveEvent& e);
      virtual void handleEvent(UnitWaitEvent& e);
      virtual void handleEvent(UnitFireEvent& e);
      virtual void handleEvent(UnitEquipEvent& e);
      virtual void handleEvent(UnitInvSwapEvent& e);

   private:
      XServer();

      string createPlayerName(string requested, uint32 source);

      void quitSDL();

      map<uint32, spClient> _clients;

      map<uint32, spServerGame> _games;

//      vector<spServerGame> games;
      const string _configFile;
      bool _running;
      unsigned int lastUpdateTime;
      unsigned int deltaTime;
};

#endif
