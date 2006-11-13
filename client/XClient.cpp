#include "includes.h"
#include "XClient.h"
#include "FrontEndGameState.h"
#include "ConfigOptions.h"
#include "Display.h"
#include "Input.h"
#include "ClientNetwork.h"

XClient::XClient() :
   Game(),
   configFile("client.ini")
{

}

XClient::~XClient()
{

}

void XClient::init()
{
   ConfigOptions& co = ConfigOptions::instance();
   co.init();
   co.load(configFile);

   uint32 flags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO;

#ifndef WIN32
   flags |= SDL_INIT_EVENTTHREAD;
#endif

   if (SDL_Init(flags) < 0)
   {
      cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
      exit(1);
   }
   atexit(XClient::quitSDL);

   if (TTF_Init() != 0)
   {
      cerr << "Could not initialize SDL TrueTypeFonts: " << TTF_GetError() << endl;
   }

   if (-1 == SDLNet_Init())
   {
      cerr << "Could not initialize SDL Net: " << SDLNet_GetError() << endl; 
   }

   if (-1 == FE_Init())
   {
      cerr << "Could not initialize fastevents: " << FE_GetError() << endl; 
   }

   if (-1 == NET2_Init())
   {
      cerr << "Could not initialize NET2: " << NET2_GetError() << endl; 
   }

   SDL_EnableUNICODE(1);
   SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

   Audio& a = Audio::instance();
   a.init();

   Display& d = Display::instance();
   d.init();
   //d.loadFont(co.get<string>(FONT));
   d.loadFont("resources/fonts/FreeMono.ttf");

   GameState* gs = new FrontEndGameState(this);
   gs->init();

   changeState(gs);
}

void XClient::deinit()
{
   popAllStates();
   ConfigOptions::instance().deinit();
}

void XClient::quitSDL() 
{
   FE_Quit();
   SDLNet_Quit();
   SDL_Quit();
}

void XClient::update()
{
   uint32 currentTime = SDL_GetTicks();
   deltaTime = currentTime - lastUpdateTime;
   lastUpdateTime = currentTime;

   SDL_Event evnt;
   while(SDL_PollEvent(&evnt)) 
   {
      switch (evnt.type)
      {
         case SDL_QUIT:
            quit();
            return;
            break;

         // all user events are network events
         case SDL_USEREVENT:
            ClientNetwork::instance().processSDLEvent(evnt);
            break;

         case SDL_KEYDOWN:
         case SDL_KEYUP:
         case SDL_MOUSEMOTION:
         case SDL_MOUSEBUTTONUP:
         case SDL_MOUSEBUTTONDOWN:
            Input::instance().processSDLEvent(evnt);
            break;

         default:
            break;
      }
   }

   getCurrentGameState()->update(deltaTime);
   Display::instance().render();
}

void XClient::processSDLEvent(SDL_Event& evnt) {
   getCurrentGameState()->processSDLEvent(evnt);
}
