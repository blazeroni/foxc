#ifndef XCLONE_InventoryState_H
#define XCLONE_inventoryState_H

#include "includes.h"
#include "xcore/GameState.h"
#include "Display.h"
#include "xcore/Game.h"
#include "Audio.h"
#include "xcore/Weapon.h"
#include "xcore/Item.h"


class InventoryState : public GameState
{
    public:
        InventoryState(Game* app);
        virtual void update(uint32 deltaTime);
        virtual void init();
        virtual void deinit();
        virtual void processSDLEvent(SDL_Event& event);	  

    private:
	Mix_Music* _music;
	SDL_Surface *_unitBox, *_unitBoxHL, *_itemBox, *_itemBoxHL, *_weaponBox, *_weaponBoxHL, *_unitImage;
	SDL_Surface *_pistolImage, *_pistolInvImage, *_pistolClipImage, *_pistolClipInvImage;
	SDL_Surface *_grenadeImage, *_grenadeInvImage;
        int _pointsMax;
        int _pointsSpent;
        int _selectedUnit;
        int _selectedInv;
        int _unit[8];
        itemtype _loadout[8][7];
        string _playerName;

	bool load_files();
};

#endif

