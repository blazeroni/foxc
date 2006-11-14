#ifndef XCLONE_INVENTORYSTATE_H
#define XCLONE_INVENTORYSTATE_H

#include "includes.h"
#include "xcore/GameState.h"
#include "Display.h"
#include "xcore/Game.h"
#include "Audio.h"
#include "xcore/Weapon.h"
#include "xcore/Item.h"
#include "xcore/StartGameEvent.h"
#include "xcore/EventManager.h"
#include "xcore/IEventListener.h"
#include "xcore/Player.h"

class InventoryState : public GameState, public IEventListener
{
   public:
      InventoryState(Game* app, spPlayer player, uint32 maxPoints);
      virtual void update(uint32 deltaTime);
      virtual void init();
      virtual void deinit();
      virtual void processSDLEvent(SDL_Event& event);	  
      virtual string getName();
      void setLocalPlayer(spPlayer player);

   private:
      Mix_Music* _music;
      SDL_Surface *_unitBox, *_unitBoxHL, *_itemBox, *_itemBoxHL, *_weaponBox, *_weaponBoxHL, *_unitImage;
      SDL_Surface *_pistolImage, *_pistolInvImage, *_pistolClipImage, *_pistolClipInvImage;
      SDL_Surface *_rpgImage, *_rpgInvImage, *_rocketImage, *_rocketInvImage;
      SDL_Surface *_grenadeImage, *_grenadeInvImage, *_medkitImage, *_medkitInvImage;
      SDL_Surface *_readyImage;
      int _pointsMax;
      int _pointsSpent;
      int _selectedUnit;
      int _selectedInv;
      int _unit[8];
      itemtype _loadout[8][7];
      spPlayer _localPlayer;
      //bool _ready;

      bool load_files();
      void commit();
};

#endif

