#include "includes.h"
#include "Chat.h"
#include "Display.h"
#include "ClientNetwork.h"
#include "xcore/EventManager.h"
#include "xcore/ChatEvent.h"

Chat::Chat()
{
   _message = "";
   EventManager::instance().addListener<ChatEvent>(this);
}

Chat::~Chat()
{
}

void Chat::draw()
{
    Display::instance().draw( 20, 20, _message );
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
   _message = e.getPlayerName() + "> " + e.getMessage();
}
