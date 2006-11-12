#include "includes.h"
#include "xcore/EventManager.h"
#include "xcore/GameListEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "xcore/ClientConnectEvent.h"
#include "xcore/ClientDisconnectEvent.h"
#include "xcore/GameJoinEvent.h"
#include "xcore/UnitMoveEvent.h"
#include "xcore/GameOverEvent.h"
#include "xcore/UnitActiveEvent.h"
#include "xcore/UnitWaitEvent.h"
#include "xcore/UnitFireEvent.h"
#include "xcore/UnitEquipEvent.h"
#include "xcore/UnitInvSwapEvent.h"
#include "XServer.h"
#include "ConfigOptions.h"
#include "ServerNetwork.h"

XServer::XServer() :
   _configFile("server.ini"),
   _running(true)
{

}

XServer::~XServer()
{

}

void XServer::init()
{
   ConfigOptions& co = ConfigOptions::instance();
   co.init();
   co.load(_configFile);

   uint32 flags = SDL_INIT_VIDEO | SDL_INIT_TIMER; 

#ifndef WIN32
   flags |= SDL_INIT_EVENTTHREAD;
#endif

   if (SDL_Init(flags) < 0)
   {
      cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
      exit(1);
   }
   atexit(SDL_Quit);

   if (-1 == SDLNet_Init())
   {
      cerr << "Could not initialize SDLNet: " << SDLNet_GetError() << endl; 
   }

   if (-1 == FE_Init())
   {
      cerr << "Could not initialize fastevents: " << FE_GetError() << endl; 
   }

   if (-1 == NET2_Init())
   {
      cerr << "Could not initialize NET2: " << NET2_GetError() << endl; 
   }

   int port = co.get<int>(PORT);
   if (!ServerNetwork::instance().host(port))
   {
      cerr << "Failed to start accepting connections on port " << port << "." << endl << NET2_GetError() << endl;
   }
   else 
   {
      cout << "Now hosting on port " << port << "." << endl;
   }
   //GameState* gs = new MainGameState(this);
   //gs->init();

   //changeState(gs);

   EventManager::instance().addListener<ChatEvent>(this);
   EventManager::instance().addListener<ClientConnectEvent>(this);
   EventManager::instance().addListener<ClientDisconnectEvent>(this);
   EventManager::instance().addListener<PlayerJoinEvent>(this);
   EventManager::instance().addListener<GameListEvent>(this);
   EventManager::instance().addListener<GameJoinEvent>(this);
   EventManager::instance().addListener<UnitCreateEvent>(this);
   EventManager::instance().addListener<UnitMoveEvent>(this);
   EventManager::instance().addListener<UnitWaitEvent>(this);
   EventManager::instance().addListener<GameOverEvent>(this);
   EventManager::instance().addListener<UnitActiveEvent>(this);
   EventManager::instance().addListener<UnitFireEvent>(this);
   EventManager::instance().addListener<UnitEquipEvent>(this);
   EventManager::instance().addListener<UnitInvSwapEvent>(this);
}

void XServer::deinit()
{
   ConfigOptions::instance().deinit();
}

void XServer::quitSDL() 
{
   NET2_Quit();
   FE_Quit();
   SDLNet_Quit();
   SDL_Quit();
}

void XServer::update()
{
   unsigned int currentTime = SDL_GetTicks();
   deltaTime = currentTime - lastUpdateTime;
   lastUpdateTime = currentTime;

   SDL_Event evnt;
   while(FE_WaitEvent(&evnt)) 
   {
      switch (evnt.type)
      {
         case SDL_QUIT:
            quit();
            return;
            break;

         // all user events are network events
         case SDL_USEREVENT:
            ServerNetwork::instance().processSDLEvent(evnt);
            break;

         default:
            break;
      }
   }
}

bool XServer::running()
{
   return _running;
}

void XServer::quit()
{
   _running = false;
}

void XServer::handleEvent(ChatEvent& e)
{
   string name = _clients[e.getSource()]->getPlayerName();
   cout << "Chat Message from " << name << ": " << e.getMessage() << endl;

   ServerNetwork::instance().sendAll(ChatEvent(e.getMessage(), name));
}

void XServer::handleEvent(ClientConnectEvent& e)
{
   spClient c = spClient(new Client(e.getSource()));
   _clients[e.getSource()] = c;
   c->send(ClientConnectEvent(c->getPlayerID()));
}

void XServer::handleEvent(ClientDisconnectEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
         spClient c = _clients[e.getSource()];
      _games[c->getGameID()]->leave(c);
   }
   _clients.erase(e.getSource());
}

void XServer::handleEvent(GameListEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end())
   {
      list<string> games;
      map<uint32, spServerGame>::iterator iter;
      for (iter = _games.begin(); iter != _games.end(); ++iter)
      {
         games.push_back(iter->second->getGameName());
      }
      
      _clients[e.getSource()]->send(GameListEvent(games));
   }
}

void XServer::handleEvent(PlayerJoinEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end())
   {
      spClient c = _clients[e.getSource()];
      c->setPlayerName(createPlayerName(e.getPlayerName(), e.getSource()));
      cout << "Player name: " << _clients[e.getSource()]->getPlayerName() << endl;
      c->send(PlayerJoinEvent(c->getPlayerName(), c->getPlayerID()));
   }
}

string XServer::createPlayerName(string requested, uint32 source)
{
   ostringstream name(requested);
   name.seekp(0, ios::end);
   map<uint32, spClient>::iterator iter;
   for (iter = _clients.begin(); iter != _clients.end(); ++iter)
   {
      if ( name.str() == iter->second->getPlayerName())
      {
         name << "_" << source;
         break;
      }
   }
   return name.str();
}

void XServer::handleEvent(GameJoinEvent& e)
{
   GameJoinEvent response = GameJoinEvent(e.getGameName(), e.isHost());
   bool found = false;
   map<uint32, spServerGame>::iterator iter;
   for (iter = _games.begin(); iter != _games.end(); ++iter)
   {
      if ( e.getGameName() == iter->second->getGameName())
      {
         found = true;
         if (e.isHost()) {
            response.setJoined(false);
         }
         else
         {
            cout << _clients[e.getSource()]->getPlayerName() << " joined game "
                 << iter->second->getGameName() << endl;
            iter->second->join(_clients[e.getSource()]);
            response.setJoined(true);
         }
         break;
      }
   }
   if (!found)
   {
      if (e.isHost())
      {
         spServerGame sg = spServerGame(new ServerGame(e.getSource(), e.getGameName()));
         sg->init();
         sg->join(_clients[e.getSource()]);
         _games[e.getSource()] = sg;
         response.setJoined(true);
         cout << "New game created: " << e.getGameName() << endl;
         cout << "Game host: " << _clients[e.getSource()]->getPlayerName() << endl;
      }
      else
      {
         response.setJoined(false);
      }
   }
   _clients[e.getSource()]->send(response);
}

void XServer::handleEvent(UnitCreateEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}

void XServer::handleEvent(UnitMoveEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}

void XServer::handleEvent(GameOverEvent& e)
{
   cout << "Game Over" << endl;
   _clients.clear();
   //_games.erase(e.getGameID());
   _games.clear();
}

void XServer::handleEvent(UnitActiveEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}

void XServer::handleEvent(UnitWaitEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}

void XServer::handleEvent(UnitFireEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}

void XServer::handleEvent(UnitEquipEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}

void XServer::handleEvent(UnitInvSwapEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}
