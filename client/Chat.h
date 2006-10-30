#ifndef XCLIENT_CHAT_H
#define XCLIENT_CHAT_H

#include "includes.h"
#include "xcore/IEventListener.h"
#include "xcore/Singleton.h"

class Chat : public IEventListener, public Singleton<Chat>
{
    friend class Singleton<Chat>;

    private:
      string _message[5];
      uint32 _time[5];
      string _player[5];
      string _input;
    
    public:
      Chat();
      ~Chat();
      void draw();
      void inputMessage( std::string text );
      void sendMessage( std::string text );

      void handleEvent(ChatEvent& e);
};

#endif /* XCLONE_CHAT_H */

