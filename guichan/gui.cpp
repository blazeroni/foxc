/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004, 2005, 2006 Olof Naess�n and Per Larsson
 *
 *                                                         Js_./
 * Per Larsson a.k.a finalman                          _RqZ{a<^_aa
 * Olof Naess�n a.k.a jansem/yakslem                _asww7!uY`>  )\a//
 *                                                 _Qhm`] _f "'c  1!5m
 * Visit: http://guichan.darkbits.org             )Qk<P ` _: :+' .'  "{[
 *                                               .)j(] .d_/ '-(  P .   S
 * License: (BSD)                                <Td/Z <fP"5(\"??"\a.  .L
 * Redistribution and use in source and          _dV>ws?a-?'      ._/L  #'
 * binary forms, with or without                 )4d[#7r, .   '     )d`)[
 * modification, are permitted provided         _Q-5'5W..j/?'   -?!\)cam'
 * that the following conditions are met:       j<<WP+k/);.        _W=j f
 * 1. Redistributions of source code must       .$%w\/]Q  . ."'  .  mj$
 *    retain the above copyright notice,        ]E.pYY(Q]>.   a     J@\
 *    this list of conditions and the           j(]1u<sE"L,. .   ./^ ]{a
 *    following disclaimer.                     4'_uomm\.  )L);-4     (3=
 * 2. Redistributions in binary form must        )_]X{Z('a_"a7'<a"a,  ]"[
 *    reproduce the above copyright notice,       #}<]m7`Za??4,P-"'7. ).m
 *    this list of conditions and the            ]d2e)Q(<Q(  ?94   b-  LQ/
 *    following disclaimer in the                <B!</]C)d_, '(<' .f. =C+m
 *    documentation and/or other materials      .Z!=J ]e []('-4f _ ) -.)m]'
 *    provided with the distribution.          .w[5]' _[ /.)_-"+?   _/ <W"
 * 3. Neither the name of Guichan nor the      :$we` _! + _/ .        j?
 *    names of its contributors may be used     =3)= _f  (_yQmWW$#(    "
 *    to endorse or promote products derived     -   W,  sQQQQmZQ#Wwa]..
 *    from this software without specific        (js, \[QQW$QWW#?!V"".
 *    prior written permission.                    ]y:.<\..          .
 *                                                 -]n w/ '         [.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT       )/ )/           !
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY         <  (; sac    ,    '
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING,               ]^ .-  %
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF            c <   r
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR            aga<  <La
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE          5%  )P'-3L
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        _bQf` y`..)a
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          ,J?4P'.P"_(\?d'.,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES               _Pa,)!f/<[]/  ?"
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT      _2-..:. .r+_,.. .
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     ?a.<%"'  " -'.a_ _,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     ^
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/gui.hpp"

#include "guichan/exception.hpp"
#include "guichan/focushandler.hpp"
#include "guichan/graphics.hpp"
#include "guichan/input.hpp"
#include "guichan/keyinput.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseinput.hpp"
#include "guichan/widget.hpp"

namespace gcn
{
    Gui::Gui()
    {
        mTop = NULL;
        mInput = NULL;
        mGraphics = NULL;
        mFocusHandler = new FocusHandler();
        mTopHasMouse = false;
        mTabbing = true;
    }

    Gui::~Gui()
    {
        if (Widget::widgetExists(mTop))
        {
            setTop(NULL);
        }

        delete mFocusHandler;
    }

    void Gui::setTop(Widget* top)
    {
        if (mTop)
        {
            mTop->_setFocusHandler(NULL);
        }
        if (top)
        {
            top->_setFocusHandler(mFocusHandler);
        }

        mTop = top;
    }

    Widget* Gui::getTop() const
    {
        return mTop;
    }

    void Gui::setGraphics(Graphics* graphics)
    {
        mGraphics = graphics;
    }

    Graphics* Gui::getGraphics() const
    {
        return mGraphics;
    }

    void Gui::setInput(Input* input)
    {
        mInput = input;
    }

    Input* Gui::getInput() const
    {
        return mInput;
    }

    void Gui::logic()
    {
        if (!mTop)
        {
            throw GCN_EXCEPTION("No top widget set");
        }

        mFocusHandler->applyChanges();

        if(mInput)
        {
            mInput->_pollInput();

            while (!mInput->isMouseQueueEmpty())
            {
                MouseInput mi = mInput->dequeueMouseInput();

                // Send mouse input to every widget that has the mouse.
                if (mi.x > 0 && mi.y > 0
                    && mTop->getDimension().isPointInRect(mi.x, mi.y))
                {
                    if (!mTop->hasMouse())
                    {
                        mTop->_mouseInMessage();
                    }

                    MouseInput mio = mi;
                    mio.x -= mTop->getX();
                    mio.y -= mTop->getY();
                    mTop->_mouseInputMessage(mio);
                }
                else if (mTop->hasMouse())
                {
                    mTop->_mouseOutMessage();
                }

                Widget* f = mFocusHandler->getFocused();
                Widget* d = mFocusHandler->getDragged();

                // If the focused widget doesn't have the mouse,
                // send the mouse input to the focused widget.
                if (f != NULL && !f->hasMouse())
                {
                    int xOffset, yOffset;
                    f->getAbsolutePosition(xOffset, yOffset);

                    MouseInput mio = mi;
                    mio.x -= xOffset;
                    mio.y -= yOffset;

                    f->_mouseInputMessage(mio);
                }

                // If the dragged widget is different from the focused
                // widget, send the mouse input to the dragged widget.
                if (d != NULL && d != f && !d->hasMouse())
                {
                    int xOffset, yOffset;
                    d->getAbsolutePosition(xOffset, yOffset);

                    MouseInput mio = mi;
                    mio.x -= xOffset;
                    mio.y -= yOffset;

                    d->_mouseInputMessage(mio);
                }

                mFocusHandler->applyChanges();

            } // end while

            while (!mInput->isKeyQueueEmpty())
            {
                KeyInput ki = mInput->dequeueKeyInput();

                KeyListenerListIterator it;

                // Propagate key input to global KeyListeners
                switch(ki.getType())
                {
                  case KeyInput::PRESS:
                      for (it = mKeyListeners.begin(); it != mKeyListeners.end(); ++it)
                      {
                          (*it)->keyPress(ki.getKey());
                      }
                      break;

                  case KeyInput::RELEASE:
                      for (it = mKeyListeners.begin(); it != mKeyListeners.end(); ++it)
                      {
                          (*it)->keyRelease(ki.getKey());
                      }
                      break;
                }

                if (mTabbing
                    && ki.getKey().getValue() == Key::TAB
                    && ki.getType() == KeyInput::PRESS)
                {
                    if (ki.getKey().isShiftPressed())
                    {
                        mFocusHandler->tabPrevious();
                    }
                    else
                    {
                        mFocusHandler->tabNext();
                    }
                }
                else
                {
                    // Send key inputs to the focused widgets
                    if (mFocusHandler->getFocused())
                    {
                        if (mFocusHandler->getFocused()->isFocusable())
                        {
                            mFocusHandler->getFocused()->_keyInputMessage(ki);
                        }
                        else
                        {
                            mFocusHandler->focusNone();
                        }
                    }
                }

                mFocusHandler->applyChanges();

            } // end while

            // Apply changes even if no input has been processed.
            // A widget might has asked for focus.
            mFocusHandler->applyChanges();

        } // end if

        mTop->logic();
    }

    void Gui::draw()
    {
        if (!mTop)
        {
            throw GCN_EXCEPTION("No top widget set");
        }
        if (!mGraphics)
        {
            throw GCN_EXCEPTION("No graphics set");
        }

        if (!mTop->isVisible())
        {
            return;
        }

        mGraphics->_beginDraw();

        // If top has a border,
        // draw it before drawing top
        if (mTop->getBorderSize() > 0)
        {
            Rectangle rec = mTop->getDimension();
            rec.x -= mTop->getBorderSize();
            rec.y -= mTop->getBorderSize();
            rec.width += 2 * mTop->getBorderSize();
            rec.height += 2 * mTop->getBorderSize();
            mGraphics->pushClipArea(rec);
            mTop->drawBorder(mGraphics);
            mGraphics->popClipArea();
        }

        mGraphics->pushClipArea(mTop->getDimension());
        mTop->draw(mGraphics);
        mGraphics->popClipArea();


        mGraphics->_endDraw();
    }

    void Gui::focusNone()
    {
        mFocusHandler->focusNone();
    }

    void Gui::setTabbingEnabled(bool tabbing)
    {
        mTabbing = tabbing;
    }

    bool Gui::isTabbingEnabled()
    {
        return mTabbing;
    }

    void Gui::addGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.push_back(keyListener);
    }

    void Gui::removeGlobalKeyListener(KeyListener* keyListener)
    {
        mKeyListeners.remove(keyListener);
    }
}
