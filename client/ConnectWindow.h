#ifndef XCLIENT_CONNECTWINDOW_H
#define XCLIENT_CONNECTWINDOW_H

#include "includes.h"

class ConnectWindow;
typedef boost::shared_ptr<ConnectWindow> spConnectWindow;

#include "GameLobbyGameState.h"

// This class is used in conjunction w/ the game lobby

class GameLobbyGameState;

class ConnectWindow : public gcn::ActionListener
{
   public:
      ConnectWindow(GameLobbyGameState* glgs);
      virtual ~ConnectWindow();

      void registerConnectAction(boost::function<void (GameLobbyGameState*, string)> f);
      void registerCancelAction(boost::function<void (GameLobbyGameState*)> f);

      virtual void action(const string& eventId, gcn::Widget* widget);

      string getPlayerName() const;
      string getIPAddress() const;
      gcn::Widget* getGUI();

      void build();

   private:
      void clean();

      boost::function<void (GameLobbyGameState*, string)> _connectAction;
      boost::function<void (GameLobbyGameState*)> _cancelAction;

      GameLobbyGameState* _glgs;
      //void (GameLobbyGameState::*_connectAction)(string);
      //void (GameLobbyGameState::*_cancelAction)();

      gcn::Window* _win;
      gcn::Button* _connect;
      gcn::Button* _cancel;
      gcn::TextField* _ipAddress;
      gcn::TextField* _playerName;
      gcn::Label* _playerLabel;
      gcn::Label* _ipLabel;
};

#endif
