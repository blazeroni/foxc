#include "includes.h"
#include "Input.h"
#include "xcore/Unit.h"
#include "xcore/Game.h"
#include "XClient.h"
#include "MainGameState.h"
#include "Chat.h"
#include "Display.h"


Input::Input()
{
    _mode = (InputMode)0;
    _str = "";
    SDL_EnableUNICODE( SDL_ENABLE );
}

void Input::processSDLEvent(SDL_Event& event) 
{
    GameState *gs = XClient::instance().getCurrentGameState();

    if (gs->getName() != "Main")
    {
       XClient::instance().processSDLEvent(event);
       return;
    }

    if ( GUIInput(event) )
        return;

    // fire mode
    if ( _mode == USE )
    {
        spUnit unit = ((MainGameState*)gs)->getActiveUnit();
        if ( !(unit->getHand(_hand).get()) || unit->getActionPoints() < unit->getHand(_hand)->getTurnCost() )
        {
            _mode = COMMAND;
            ((MainGameState*)gs)->setDisplayMouseOverHighlight(true);
            Display::instance().setCursor();
            return;
        }
        if ( event.type == SDL_KEYDOWN ) 
        {
            switch ( event.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                case SDLK_f:
                case SDLK_u:
                    _mode = COMMAND;
                    ((MainGameState*)gs)->setDisplayMouseOverHighlight(true);
                    Display::instance().setCursor();
                    break;
            }
        }
        else if ( event.type == SDL_MOUSEBUTTONUP )
        {
            if ( event.button.button == SDL_BUTTON_RIGHT )
            {
                _mode = COMMAND;
                ((MainGameState*)gs)->setDisplayMouseOverHighlight(true);
                Display::instance().setCursor();
                return;
            }
            else if ( event.button.button == SDL_BUTTON_LEFT )
            {
                if ( unit->getActionPoints() >= unit->getHand(_hand)->getTurnCost() )
                {
                    if (unit->getHand(_hand)->getType() == GRENADE)
                    {
                        //unit->use( ((MainGameState*)gs)->getMap()->getMouseOverTile(), _hand );
                        ((MainGameState*)gs)->fire(((MainGameState*)gs)->getActiveUnit(),
                                          ((MainGameState*)gs)->getMap()->getMouseOverTile(), _hand);
                        return;
                    }
                    else
                    {
                        MainGameState *mgs = (MainGameState*)gs;
                        Point t = mgs->existsLOS( mgs->getMap()->getTile(unit->getX(),unit->getY()), mgs->getMap()->getMouseOverTile() );
                        mgs->fire( mgs->getActiveUnit(), mgs->getMap()->getTile(t.x,t.y), _hand);
                    }
                }
                else
                {
                    _mode = COMMAND;
                    ((MainGameState*)gs)->setDisplayMouseOverHighlight(true);
                    Display::instance().setCursor();
                    return;
                }
            }
        }
        else if ( event.type == SDL_MOUSEMOTION )
        {
           ((MainGameState*)gs)->processSDLEvent(event);
        }
    }
    /*
    if ( _mode == FIRE )
    {
        if ( event.type == SDL_KEYDOWN ) 
        {
            switch ( event.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                case SDLK_f:
                    _mode = COMMAND;
                    SDL_ShowCursor( SDL_ENABLE );
                    break;
            }
        }
        else if ( event.type == SDL_MOUSEBUTTONUP )
        {
            if ( event.button.button == SDL_BUTTON_RIGHT )
            {
                    _mode = COMMAND;
                    SDL_ShowCursor( SDL_ENABLE );
            }
            else if ( event.button.button == SDL_BUTTON_LEFT )
            {
               //if ( ((MainGameState*)gs)->getActiveUnit()->getActionPoints()
               //     > COST_FIRE )
               //{
               ((MainGameState*)gs)->fire(((MainGameState*)gs)->getActiveUnit(),
                                          ((MainGameState*)gs)->getMap()->getMouseOverTile(), 1);
               //}
               _mode = COMMAND;
               SDL_ShowCursor( SDL_ENABLE );
                //((MainGameState*)gs)->getActiveUnit()->fire(
                //    ((MainGameState*)gs)->getMap()->getMouseOverTile() );
                //if ( ((MainGameState*)gs)->getActiveUnit()->getActionPoints()
                //    < COST_FIRE )
                //{
                //    _mode = COMMAND;
                //    SDL_ShowCursor( SDL_ENABLE );
                //}
            }
        }
    }
    */

    else if ( event.type == SDL_KEYDOWN )
    {

        // command mode
       if ( _mode == COMMAND )
       {
          switch ( event.key.keysym.sym )
          {
             case SDLK_ESCAPE:
                XClient::instance().quit();
                break;
             case SDLK_RETURN:
                _mode = CHAT;
                break;
             case SDLK_f:
                if ( !((MainGameState*)gs)->isMyTurn() )
                    return;
                _mode = USE;
                _hand = 1;
                ((MainGameState*)gs)->setDisplayMouseOverHighlight(false);
                Display::instance().setCursor( Display::instance()._crosshair );
                break;
             case SDLK_u:
                if ( !((MainGameState*)gs)->isMyTurn() )
                    return;
                _hand = 0;
                Display::instance().setCursor( Display::instance()._crosshair );
                break;
             default:
                XClient::instance().processSDLEvent(event);
                break;
          }
       }


       // in game chat
       else if (_mode == CHAT)
       {
          if( _str.length() <= 64 )
          {
             //if( event.key.keysym.unicode == (Uint16)' ' )
             //    _str += (char)event.key.keysym.unicode;    
             if( event.key.keysym.unicode == SDLK_SPACE )
                _str += " ";
             if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
                _str += (char)event.key.keysym.unicode;
             else if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
                _str += (char)event.key.keysym.unicode;
             else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
                _str += (char)event.key.keysym.unicode;
          }
          if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( _str.length() != 0 ) )
             _str.erase( _str.length() - 1 );
          if( ( event.key.keysym.sym == SDLK_ESCAPE ) )
          {
             _str = "";
             _mode = COMMAND;
             return;
          }
          if( ( event.key.keysym.sym == SDLK_RETURN ) && ( _str.length() != 0 ) )
          {
             Chat::instance().sendMessage( _str );
             _mode = COMMAND;
             _str = "";
          }
          Chat::instance().inputMessage( _str );
          return;
       }
    }

    // process in maingamestate
    else
       XClient::instance().processSDLEvent(event);
}

Point Input::getMousePosition() const
{
    Point p;
    SDL_GetMouseState(&p.x, &p.y);
    return p;
}

int Input::getMode() const { return _mode; }
int Input::getHand() const { return _hand; }

bool Input::GUIInput( SDL_Event& event )
{
    switch ( _mode )
    {
    case ( COMMAND ):
    if ( event.type == SDL_MOUSEBUTTONUP )
    {
        Point click;
        SDL_GetMouseState(&click.x, &click.y);
        MainGameState *gs = (MainGameState*)XClient::instance().getCurrentGameState();
        if ( !gs->isMyTurn() )
            return 0;
        spUnit activeUnit = gs->getActiveUnit();
        if ( !activeUnit.get() )
            return false;
        // LEFT WEAPON
        if ( click.x >= 800 - 180 && click.x <= 800 - 110 && click.y >= 600 - 130 && click.y <= 600 - 10 )
        {
            if ( event.button.button == SDL_BUTTON_LEFT && activeUnit->getLeft().get() )
            {
                _hand = 1;
                if ( activeUnit->getLeft()->getRange() == 0 )
                {
                    ((MainGameState*)gs)->fire(((MainGameState*)gs)->getActiveUnit(),
                                          ((MainGameState*)gs)->getMap()->getTile(0,0), _hand);
                    return true;
                }
                _mode = USE;
                ((MainGameState*)gs)->setDisplayMouseOverHighlight(false);
                Display::instance().setCursor( Display::instance()._crosshair );
                return true;
            }
            if ( event.button.button == SDL_BUTTON_RIGHT )
            {
                if ( activeUnit->getLeft().get() )
                {
                    _mode = INVMOVE;
                    Display::instance().setCursor( activeUnit->getLeft()->getInvImage() );
                    _slot1 = 1;
                }
                return true;
            }
        }
        // RIGHT WEAPON
        if ( click.x >= 800 - 80 && click.x <= 800 - 10 && click.y >= 600 - 130 && click.y <= 600 - 10 )
        {
            if ( event.button.button == SDL_BUTTON_LEFT && activeUnit->getRight().get() )
            {
                _hand = 0;
                if ( activeUnit->getRight()->getRange() == 0 )
                {
                    ((MainGameState*)gs)->fire(((MainGameState*)gs)->getActiveUnit(),
                                          ((MainGameState*)gs)->getMap()->getTile(0,0), _hand);
                    return true;
                }
                _mode = USE;
                ((MainGameState*)gs)->setDisplayMouseOverHighlight(false);
                Display::instance().setCursor( Display::instance()._crosshair );
                return true;
            }
            if ( event.button.button == SDL_BUTTON_RIGHT )
            {
                if ( activeUnit->getRight().get() )
                {
                    _mode = INVMOVE;
                    Display::instance().setCursor( activeUnit->getRight()->getInvImage() );
                    _slot1 = 0;
                }
                return true;
            }
        }
        for ( int i = 0; i <= 5; ++i )
        {
            // right clicks on an inventory slot
            if ( click.x >= 800 - 190 - 10 - 55 - 60*i && click.x <= 800 - 190 - 10 - 55 - 60*(i-1) && click.y >= 600 - 65 && click.y <= 600 - 10 && event.button.button == SDL_BUTTON_RIGHT ) 
            {
                if ( !activeUnit->getInv(i) )
                    return true;
                _mode = INVMOVE;
                Display::instance().setCursor( activeUnit->getInv(i)->getInvImage() );
                _slot1 = i+2;
                return true;
            }
        }
        // use button
        MainGameState* mgs = (MainGameState*)XClient::instance().getCurrentGameState();
        if (mgs->isUseObjectActive() && click.x >= 800 - 125 && click.x <= 800-5 && click.y >= 405 && click.y <= 435)
        {
           cout << "clicked" << endl;
           mgs->useObject();
           return true;
        }
    }
    break;
        case ( INVMOVE ):
        {
            Point click;
            SDL_GetMouseState(&click.x, &click.y);
            MainGameState *gs = (MainGameState*)XClient::instance().getCurrentGameState();
            if ( !gs->isMyTurn() )
                return 0;
            spUnit activeUnit = gs->getActiveUnit();
            if ( !activeUnit.get() )
                return false;
            if ( event.type == SDL_MOUSEBUTTONUP )
            {
                if ( event.button.button == SDL_BUTTON_RIGHT )
                {
                    Display::instance().setCursor();
                    _mode = COMMAND;
                    return true;
                }
                if ( event.button.button == SDL_BUTTON_LEFT )
                {
                    if ( click.x >= 800 - 180 && click.x <= 800 - 110 && click.y >= 600 - 130 && click.y <= 600 - 10 )
                    {
                        Display::instance().setCursor();
                        gs->swapEq( activeUnit, _slot1, 1 );
                        _mode = COMMAND;
                        return true;
                    }
                    if ( click.x >= 800 - 80 && click.x <= 800 - 10 && click.y >= 600 - 130 && click.y <= 600 - 10 )
                    {
                        Display::instance().setCursor();
                        gs->swapEq( activeUnit, _slot1, 0 );
                        _mode = COMMAND;
                        return true;
                    }
                    for ( int i = 0; i <= 5; ++i )
                    {
                        // clicks on an inventory slot
                        if ( click.x >= 800 - 190 - 10 - 55 - 60*i && click.x <= 800 - 190 - 10 - 55 - 60*(i-1) && click.y >= 600 - 65 && click.y <= 600 - 10 ) 
                        {
                            Display::instance().setCursor();
                            gs->swapEq( activeUnit, _slot1, i+2 );
                            _mode = COMMAND;
                            return true;
                        }
                    }
                }
            }
            
            break;
        }
    }
    return false;
}

void Input::setMode( InputMode mode )
{
    _mode = mode;
}
