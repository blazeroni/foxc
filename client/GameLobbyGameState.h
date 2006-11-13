#ifndef XCLIENT_ARMYSETUPGAMESTATE_H
#define XCLIENT_ARMYSETUPGAMESTATE_H

#include "includes.h"
#include "xcore/GameState.h"
#include "xcore/IEventListener.h"
#include "xcore/Player.h"
#include "GameList.h"
#include "ConnectWindow.h"
#include "PlayerListModel.h"
#include "ChatBox.h"

class GameLobbyGameState : public GameState, public IEventListener
{
   public:
      GameLobbyGameState(Game* game);
      virtual ~GameLobbyGameState();

      virtual void deinit();
      virtual void init();
      virtual void update(uint32 deltaTime);

      virtual string getName();

      void processSDLEvent(SDL_Event& evnt);

      void markCancelled();
      void cancel();
      void connect(string ipAddress);

      virtual void handleEvent(ClientConnectEvent& e);
      virtual void handleEvent(GameListEvent& evnt);
      virtual void handleEvent(PlayerJoinEvent& e);
      virtual void handleEvent(PlayerLeaveEvent& e);
      virtual void handleEvent(ChatEvent& e);
      virtual void handleEvent(MapListEvent& e);
      virtual void handleEvent(GameJoinEvent& e);

   private:
      void buildConnectGUI();
      void buildGUI();
      void clean();

      void buildConnectWindow();

      spPlayer _localPlayer;

      bool _cancel;

      spConnectWindow _connect;

      gcn::SDLInput* input;
      gcn::SDLGraphics* graphics;
      gcn::SDLImageLoader* imageLoader;
      gcn::Gui* gui;
      gcn::Container* top;
      gcn::ImageFont* font;

      gcn::ScrollArea* _playerScroll;
      gcn::ListBox* _playerList;
      
      PlayerListModel* _playerListModel;

      ChatBox* _chat;

      spGameList _gameList;
};

#endif