#ifndef XCLONE_INPUT_H
#define XCLONE_INPUT_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "xcore/Point.h"

enum InputMode { NONE, COMMAND, CHAT, USE, INVMOVE };

class Input : public Singleton<Input>
{
   friend class Singleton<Input>;

   public:
      void processSDLEvent(SDL_Event& event);
      Point getMousePosition() const;
      ///  Return the current InputMode
      int getMode() const;
      int getHand() const;
      void setMode( InputMode mode );

   private:
      Input();
      InputMode _mode;
      std::string _str;
      bool GUIInput( SDL_Event& event );
      int _slot1, _hand;
};

#endif
