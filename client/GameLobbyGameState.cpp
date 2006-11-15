#include "includes.h"
#include "xcore/EventManager.h"
#include "xcore/ClientConnectEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "xcore/PlayerLeaveEvent.h"
#include "xcore/GameHostEvent.h"
#include "xcore/GameJoinEvent.h"
#include "GameLobbyGameState.h"
#include "FrontEndGameState.h"
#include "ClientNetwork.h"
#include "ConfigOptions.h"
#include "Display.h"
#include "InventoryState.h"


GameLobbyGameState::GameLobbyGameState(Game* game) :
   GameState(game),
   _cancel(false),
   input(NULL),
   graphics(NULL),
   imageLoader(NULL),
   gui(NULL),
   top(NULL),
   font(NULL),
   _refreshButton(NULL),
   _backButton(NULL)
{
}

GameLobbyGameState::~GameLobbyGameState()
{
   EventManager::instance().removeListener(this); 
}


void GameLobbyGameState::deinit()
{
   SAFE_DELETE(input);
   SAFE_DELETE(graphics);
   SAFE_DELETE(imageLoader);
   SAFE_DELETE(gui);
   SAFE_DELETE(top);
   SAFE_DELETE(font);
   SAFE_DELETE(_refreshButton);
   SAFE_DELETE(_backButton);
}

void GameLobbyGameState::init()
{
   //ClientNetwork& cn = ClientNetwork::instance();
   //cn.connectToServer(o.get<string>(HOSTNAME).c_str(), o.get<int>(PORT));

   imageLoader = new gcn::SDLImageLoader();
   gcn::Image::setImageLoader(imageLoader);

   graphics = new gcn::SDLGraphics();
   graphics->setTarget(Display::instance().getScreen());

   input = new gcn::SDLInput();

   top = new gcn::Container();    
   top->setDimension(gcn::Rectangle(0, 0, 800, 600));

   gui = new gcn::Gui();
   gui->setGraphics(graphics);
   gui->setInput(input); 
   gui->setTop(top);

   font = new gcn::ImageFont("resources/fonts/guichanfont.png", 
      " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
   gcn::Widget::setGlobalFont(font);

   top->setBaseColor(gcn::Color(42,42,42));

   buildConnectWindow();
   //buildGUI();
   EventManager::instance().addListener<ClientConnectEvent>(this);
   EventManager::instance().addListener<PlayerJoinEvent>(this);
   EventManager::instance().addListener<PlayerLeaveEvent>(this);
   EventManager::instance().addListener<ChatEvent>(this);
   EventManager::instance().addListener<MapListEvent>(this);
   EventManager::instance().addListener<GameListEvent>(this);
   EventManager::instance().addListener<GameJoinEvent>(this);
}

void GameLobbyGameState::clean()
{
   
}

void GameLobbyGameState::markCancelled()
{
   _cancel = true;
}

void GameLobbyGameState::cancel()
{
   ClientNetwork::instance().closeConnection();
   GameState* gs = new FrontEndGameState(_game);
   gs->init();
   _game->changeState(gs);
}

void GameLobbyGameState::connect(string ipAddress)
{
   ConfigOptions& o = ConfigOptions::instance();
   ClientNetwork& cn = ClientNetwork::instance();
   cn.connectToServer(ipAddress.c_str(), o.get<int>(PORT));
}

void GameLobbyGameState::buildConnectWindow()
{
   _connect = spConnectWindow(new ConnectWindow(this));
   _connect->build();
   _connect->registerCancelAction(&GameLobbyGameState::markCancelled);
   _connect->registerConnectAction(&GameLobbyGameState::connect);
   top->add(_connect->getGUI(), 250, 200);
}

void GameLobbyGameState::buildGUI()
{
   _gameList = spGameList(new GameList());
 
   _chat = new ChatBox();

   _playerList = new gcn::ListBox();
   //_playerList->setSize(150, 200);
   _playerListModel = new PlayerListModel();
   _playerList->setListModel(_playerListModel);
   _playerList->setBackgroundColor(gcn::Color(0,0,0));
   _playerList->setSize(150, 20);
   
   _playerScroll = new gcn::ScrollArea(_playerList);
   _playerScroll->setSize(150, 200);
   _playerScroll->setBorderSize(2);
   _playerScroll->setBackgroundColor(gcn::Color(0,0,0));

   _refreshButton = new gcn::Button("Refresh");
   _refreshButton->addActionListener(this);
   _backButton = new gcn::Button("Back");
   _backButton->addActionListener(this);

   top->add(_gameList->getGUI().get(), 0, 0);
   top->add(_playerScroll, 500, 340);
   top->add(_chat, 20, 340);
   top->add(_refreshButton, 695, 350);
   top->add(_backButton, 705, 380);
}

void GameLobbyGameState::action(const string& eventId, gcn::Widget* widget)
{
   if (widget == _backButton)
   {
      _cancel = true;
   }
   if (widget == _refreshButton)
   {
      ClientNetwork::instance().send(GameListEvent());
   }
}

void GameLobbyGameState::update(uint32 deltaTime)
{
   gui->logic();
   gui->draw();
   if (_cancel)
   {
      cancel();
   }
}

string GameLobbyGameState::getName()
{
   return "GameLobby";
}

void GameLobbyGameState::processSDLEvent(SDL_Event& evnt)
{
   input->pushInput(evnt);
}

void GameLobbyGameState::handleEvent(ClientConnectEvent& e)
{
   _localPlayer = spPlayer(new Player(e.getPlayerID()));
   ClientNetwork::instance().send(PlayerJoinEvent(_connect->getPlayerName()));
   buildGUI();
   _connect.reset();
}

void GameLobbyGameState::handleEvent(PlayerJoinEvent& e)
{
   if (_localPlayer->getName() == "")
   {
      _localPlayer->setName(e.getPlayerName());
      ClientNetwork::instance().send(GameListEvent());
   }
   _playerListModel->addPlayer(e.getPlayerID(), e.getPlayerName());
}

void GameLobbyGameState::handleEvent(PlayerLeaveEvent& e)
{
   _playerListModel->removePlayer(e.getPlayerName());
}

void GameLobbyGameState::handleEvent(ChatEvent& e)
{
   _chat->addMessage(e);
}

void GameLobbyGameState::handleEvent(MapListEvent &e)
{
   _gameList->setMaps(e.getMaps());
}

void GameLobbyGameState::handleEvent(GameListEvent& e)
{
   cout << "got game list" << endl;
   top->remove(_gameList->getGUI().get());
   _gameList->update(e.getGames());
   top->add(_gameList->getGUI().get(), 0, 0);
   ClientNetwork::instance().send(MapListEvent());
}

void GameLobbyGameState::handleEvent(GameJoinEvent& e)
{
   if (e.hasJoined())
   {
      _localPlayer->setPlayerNumber(e.getPlayerNumber());
      InventoryState* gs = new InventoryState(_game, _localPlayer, e.getMaxPoints());
      gs->init();
      _game->changeState(gs);
   }
}
