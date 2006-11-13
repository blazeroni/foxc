#ifndef XCLIENT_INCLUDE_H
#define XCLIENT_INCLUDE_H

#include "xcore/includes.h"

using namespace xcore;

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "guichan/guichan.hpp"
#include "guichan/sdl.hpp"

typedef boost::shared_ptr<gcn::Widget> spWidget;
typedef boost::shared_ptr<gcn::Container> spContainer;
typedef boost::shared_ptr<gcn::Window> spWindow;
typedef boost::shared_ptr<gcn::Label> spLabel;
typedef boost::shared_ptr<gcn::Button> spButton;
typedef boost::shared_ptr<gcn::DropDown> spDropDown;
typedef boost::shared_ptr<gcn::TextField> spTextField;
typedef boost::shared_ptr<gcn::ScrollArea> spScrollArea;
typedef boost::shared_ptr<gcn::ListBox> spListBox;

// all clients only have a single game
#define GAME_ID 1

#endif
