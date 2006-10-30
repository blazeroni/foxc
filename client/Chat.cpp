#include "includes.h"
#include "Chat.h"
#include "Display.h"
#include "ClientNetwork.h"
#include "xcore/EventManager.h"
#include "xcore/ChatEvent.h"

Chat::Chat()
{
    for ( int i = 0; i < 5; ++i )
    {
        _message[i] = _player[i] = "";
        _time[i] = 0;
    }
   EventManager::instance().addListener<ChatEvent>(this);
}

Chat::~Chat()
{
}

void Chat::draw()
{
    for ( int i = 0; i < 5; ++i )
    {
        if ( _time[i] > SDL_GetTicks() )
            Display::instance().draw( 20, 20+(i*15), _message[i] );
    }
    Display::instance().draw( 20, 400, _input );
}

void Chat::inputMessage( std::string text )
{
    _input = text;
}

void Chat::sendMessage( std::string text )
{
   _input = "";

   ClientNetwork::instance().send(ChatEvent(text));
}

void Chat::handleEvent(ChatEvent& e)
{
    for ( int i = 4; i > 0; --i )
    {
        _message[i] = _message[i-1];
        _time[i] = _time[i-1];
    }
   _message[0] = e.getPlayerName() + "->" + e.getMessage();
   _time[0] = SDL_GetTicks() + 20000;
}
