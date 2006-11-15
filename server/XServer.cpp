#include "includes.h"
#include "xcore/EventManager.h"
#include "xcore/GameListEvent.h"
#include "xcore/PlayerJoinEvent.h"
#include "xcore/ClientConnectEvent.h"
#include "xcore/ClientDisconnectEvent.h"
#include "xcore/GameJoinEvent.h"
#include "xcore/GameHostEvent.h"
#include "xcore/UnitMoveEvent.h"
#include "xcore/GameOverEvent.h"
#include "xcore/UnitActiveEvent.h"
#include "xcore/UnitWaitEvent.h"
#include "xcore/UnitFireEvent.h"
#include "xcore/UnitInvSwapEvent.h"
#include "xcore/MapListEvent.h"
#include "xcore/PlayerLeaveEvent.h"
#include "xcore/UseMapObjectEvent.h"
#include "xcore/StartGameEvent.h"
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

   loadMapInfo();

   EventManager::instance().addListener<ChatEvent>(this);
   EventManager::instance().addListener<ClientConnectEvent>(this);
   EventManager::instance().addListener<ClientDisconnectEvent>(this);
   EventManager::instance().addListener<PlayerJoinEvent>(this);
   EventManager::instance().addListener<GameListEvent>(this);
   EventManager::instance().addListener<GameJoinEvent>(this);
   EventManager::instance().addListener<GameHostEvent>(this);
   EventManager::instance().addListener<UnitCreateEvent>(this);
   EventManager::instance().addListener<UnitMoveEvent>(this);
   EventManager::instance().addListener<UnitWaitEvent>(this);
   EventManager::instance().addListener<GameOverEvent>(this);
   EventManager::instance().addListener<UnitActiveEvent>(this);
   EventManager::instance().addListener<UnitFireEvent>(this);
   EventManager::instance().addListener<UnitInvSwapEvent>(this);
   EventManager::instance().addListener<MapListEvent>(this);
   EventManager::instance().addListener<UseMapObjectEvent>(this);
   EventManager::instance().addListener<StartGameEvent>(this);
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

void XServer::loadMapInfo()
{
   fs::path dir = fs::initial_path();
   // normal case
   if (fs::exists(dir / "maps"))
   {
      dir /= "maps";
   }
   // Visual Studio setup
   else if (fs::exists(dir / "../debug/maps"))
   {
      dir /= "../debug/maps";
   }
   else
   {
      cout << "Could not locate map directory" << endl;
      return;
   }
   _mapDir = dir;
   fs::directory_iterator end_itr;
   for ( fs::directory_iterator itr( dir ); itr != end_itr; ++itr )
   {
      if ( fs::is_directory( *itr ) )
      {
         continue;
      }
      else 
      {
         try
         {
            ticpp::Document doc(itr->string());
            doc.LoadFile();

            MapListItem mpi;

            ticpp::Element* root = doc.FirstChildElement("map");
            root->GetAttribute("name", &mpi.mapName);
            root->GetAttribute("width", &mpi.width);
            root->GetAttribute("height", &mpi.height);
            root->GetAttribute("players", &mpi.maxPlayers);
            root->GetAttribute("points", &mpi.points);

            _maps.push_back(mpi);
            _nameInfoMap[mpi.mapName] = mpi;
            _nameFileMap[mpi.mapName] = itr->string();
         }
         catch (ticpp::Exception& e)
         {
            cout << e.m_details;
         }
      }
   }
}

bool XServer::isInLobby(uint32 id)
{
   return (_lobbyClients.find(id) != _lobbyClients.end());
}

void XServer::handleEvent(ChatEvent& e)
{
   string name = _clients[e.getSource()]->getPlayerName();
   cout << "Chat Message from " << name << ": " << e.getMessage() << endl;

   sendToPeers(e.getSource(), ChatEvent(e.getMessage(), name));
}

void XServer::handleEvent(ClientConnectEvent& e)
{
   spClient c = spClient(new Client(e.getSource()));
   _clients[e.getSource()] = c;
   _lobbyClients[e.getSource()] = c;
   c->send(ClientConnectEvent(c->getPlayerID()));
}

void XServer::handleEvent(ClientDisconnectEvent& e)
{
   uint32 source = e.getSource();
   if (_clients.find(source) != _clients.end())
   {
      spClient c = _clients[source];
      if (_games.find(_clients[source]->getGameID()) != _games.end())
      {
         _games[c->getGameID()]->leave(c);
      }
      else if (_lobbyClients.find(source) != _lobbyClients.end())
      {
         _lobbyClients.erase(source);
         sendToLobby(PlayerLeaveEvent(_clients[source]->getPlayerName(), source));
      }
      _clients.erase(source);
   }
}

void XServer::handleEvent(GameListEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end())
   {
      vector<GameListItem> games;
      map<uint32, spServerGame>::iterator iter;
      for (iter = _games.begin(); iter != _games.end(); ++iter)
      {
         GameListItem gli;
         gli.gameID = iter->second->getGameID();
         gli.gameName = iter->second->getGameName();
         gli.mapName = iter->second->getMapName();
         gli.currentPlayers = iter->second->getNumberOfPlayers();
         gli.maxPlayers = _nameInfoMap[gli.mapName].maxPlayers;
         gli.points = _nameInfoMap[gli.mapName].points;
         gli.width = _nameInfoMap[gli.mapName].width;
         gli.height = _nameInfoMap[gli.mapName].height;
         games.push_back(gli);
      }
      cout << "sending game list" << endl;
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

      map<uint32, spClient>::iterator iter;
      for (iter = _lobbyClients.begin(); iter != _lobbyClients.end(); ++iter)
      {
         iter->second->send(PlayerJoinEvent(c->getPlayerName(), c->getPlayerID()));
         if (c != iter->second)
         {
            c->send(PlayerJoinEvent(iter->second->getPlayerName(), iter->second->getPlayerID()));
         }
      }
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
         name << "-" << source;
         break;
      }
   }
   return name.str();
}

void XServer::handleEvent(GameJoinEvent& e)
{
   GameJoinEvent response = GameJoinEvent(e.getGameName());
   bool found = false;
   spServerGame sg;
   map<uint32, spServerGame>::iterator iter;
   for (iter = _games.begin(); iter != _games.end(); ++iter)
   {
      if ( e.getGameName() == iter->second->getGameName())
      {
         found = true;
         sg = iter->second;
         cout << _clients[e.getSource()]->getPlayerName() << " joined game "
              << iter->second->getGameName() << endl;
         
         bool joined = iter->second->join(_clients[e.getSource()]);

         _lobbyClients.erase(e.getSource());

         response.setJoined(joined);
         response.setMaxPoints(iter->second->getMaxPoints());
         response.setPlayerNumber(_clients[e.getSource()]->getPlayerNumber());
         break;
      }
   }
   //if (!found)
   //{
   //   if (e.isHost())
   //   {
   //      spServerGame sg = spServerGame(new ServerGame(e.getSource(), e.getGameName()));
   //      sg->init();
   //      sg->join(_clients[e.getSource()]);
   //      _games[e.getSource()] = sg;
   //      response.setJoined(true);
   //      cout << "New game created: " << e.getGameName() << endl;
   //      cout << "Game host: " << _clients[e.getSource()]->getPlayerName() << endl;
   //   }
   //   else
   //   {
   //      response.setJoined(false);
   //   }
   //}
   sg->send(response);
   //_clients[e.getSource()]->send(response);
   if (found)
   {
      sg->tryStart();
   }

}

void XServer::handleEvent(GameHostEvent& e)
{
   spServerGame sg = spServerGame(new ServerGame(e.getSource(), e.getGameName(), 
      _nameFileMap[e.getMapName()], _nameInfoMap[e.getMapName()].maxPlayers, _nameInfoMap[e.getMapName()].points));

   sg->init();
   sg->join(_clients[e.getSource()]);
   _games[e.getSource()] = sg;

   GameJoinEvent response = GameJoinEvent(e.getGameName(), sg->getMaxPoints(), _clients[e.getSource()]->getPlayerNumber());

   _lobbyClients.erase(e.getSource());
   
   response.setJoined(true);
   _clients[e.getSource()]->send(response);
   
   sg->tryStart();
   
   cout << "New game created: " << e.getGameName() << endl;
   cout << "Game host: " << _clients[e.getSource()]->getPlayerName() << endl;
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

void XServer::handleEvent(UseMapObjectEvent& e)
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

void XServer::handleEvent(MapListEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end())
   {
      _clients[e.getSource()]->send(MapListEvent(_maps));
   }
}

void XServer::handleEvent(StartGameEvent& e)
{
   if (_clients.find(e.getSource()) != _clients.end() &&
       _games.find(_clients[e.getSource()]->getGameID()) != _games.end())
   {
      _games[_clients[e.getSource()]->getGameID()]->handleEvent(e);
   }
}
