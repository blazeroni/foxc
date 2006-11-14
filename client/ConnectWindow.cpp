#include "includes.h"
#include "ConnectWindow.h"

#define WINDOW_WIDTH 300
#define WINDOW_LENGTH 125

ConnectWindow::ConnectWindow(GameLobbyGameState* glgs) :
   _glgs(glgs),
   _win(NULL),
   _connect(NULL),
   _cancel(NULL),
   _ipAddress(NULL)
{
   build();
}

ConnectWindow::~ConnectWindow()
{
   clean();
}

void ConnectWindow::clean()
{
   SAFE_DELETE(_win);
   SAFE_DELETE(_playerLabel);
   SAFE_DELETE(_playerName);
   SAFE_DELETE(_ipLabel);
   SAFE_DELETE(_connect);
   SAFE_DELETE(_cancel);
   SAFE_DELETE(_ipAddress);
}

void ConnectWindow::build()
{
   _win = new gcn::Window("Connect To Server");
   _win->setSize(WINDOW_WIDTH,WINDOW_LENGTH);
   _win->setBaseColor(gcn::Color(119,119,119));
   
   _connect = new gcn::Button("Connect");
   _connect->addActionListener(this);
   
   _cancel = new gcn::Button("Cancel");
   _cancel->addActionListener(this);

   _playerLabel = new gcn::Label("Player:");
   
   _ipLabel = new gcn::Label("Server:");

   _playerName = new gcn::TextField("Player Name");
   _playerName->setSize(205, 18);
   _playerName->setBorderSize(2);
   _playerName->setBackgroundColor(gcn::Color(42,42,42));
   _playerName->setForegroundColor(gcn::Color(255,255,255));
   _playerName->setBaseColor(gcn::Color(42,42,42));
   
   _ipAddress = new gcn::TextField("localhost");
   _ipAddress->setSize(205, 18);
   _ipAddress->setBorderSize(2);
   _ipAddress->setBackgroundColor(gcn::Color(42,42,42));
   _ipAddress->setForegroundColor(gcn::Color(255,255,255));
   _ipAddress->setBaseColor(gcn::Color(42,42,42));

   _win->add(_playerLabel, 10, 15);
   _win->add(_playerName, 70, 15);
   _win->add(_ipLabel, 10, 40);
   _win->add(_ipAddress, 70, 40);

   _win->add(_cancel, 145, 70);
   _win->add(_connect, 210, 70);
}

gcn::Widget* ConnectWindow::getGUI()
{
   return _win;
}

string ConnectWindow::getPlayerName() const
{
   return _playerName->getText();
}

void ConnectWindow::registerConnectAction(boost::function<void (GameLobbyGameState*, string)> f)
{
   _connectAction = f;
}

void ConnectWindow::registerCancelAction(boost::function<void (GameLobbyGameState*)> f)
{
   _cancelAction = f;
}

void ConnectWindow::action(const string& eventId, gcn::Widget* widget)
{
   if (widget == _connect)
   {
      _connectAction(_glgs, _ipAddress->getText());
   }
   else if (widget == _cancel)
   {
      _cancelAction(_glgs);
   }
}
