#include "includes.h"
#include "ClientWeapon.h"
#include "Display.h"
#include "xcore/Weapon.h"
#include "ResourceManager.h"

#define SPRITE_EXPLOSION "explosion"

ClientPistol::ClientPistol(uint32 entityID) :
   ClientItem(entityID, spItem(new Pistol(0)))
{
   //_image = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   //_invImage = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   _image = Display::instance().loadImage("resources/images/gui/pistol_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/pistol_gui_inv.png");
}
ClientRPG::ClientRPG(uint32 entityID) :
   ClientItem(entityID, spItem(new RPG(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/rpg_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/rpg_gui_inv.png");
}

int ClientRPG::use( spMapTile tile )
{
   ClientItem::use(tile);
   Sprite::abandon(ResourceManager::instance().getSprite(SPRITE_EXPLOSION), tile->getCenterX() , tile->getCenterY());
   return 0;
}

ClientRocket::ClientRocket(uint32 entityID) :
   ClientItem(entityID, spItem(new Rocket(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/rocket_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/rocket_gui_inv.png");
}

ClientStimpack::ClientStimpack(uint32 entityID) :
   ClientItem(entityID, spItem(new Stimpack(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/stim_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/stim_gui_inv.png");
}

ClientPistolClip::ClientPistolClip(uint32 entityID) :
   ClientItem(entityID, spItem(new PistolClip(0)))
{
   //_image = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   //_invImage = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   _image = Display::instance().loadImage( "resources/images/gui/pistol_clip_gui.png" );
   _invImage = Display::instance().loadImage( "resources/images/gui/pistol_clip_gui_inv.png" );
}


ClientRifle::ClientRifle(uint32 entityID) :
   ClientItem(entityID, spItem(new Rifle(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/rifle_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/rifle_gui_inv.png");
}
ClientRifleClip::ClientRifleClip(uint32 entityID) :
   ClientItem(entityID, spItem(new RifleClip(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/rifleclip_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/rifleclip_gui_inv.png");
}

ClientGrenade::ClientGrenade(uint32 entityID) :
   ClientItem(entityID, spItem(new Grenade(0)))
{
   //_image = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   //_invImage = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   _image = Display::instance().loadImage("resources/images/gui/grenade_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/grenade_gui_inv.png");
}

int ClientGrenade::use( spMapTile tile )
{
   ClientItem::use(tile);
   Sprite::abandon(ResourceManager::instance().getSprite(SPRITE_EXPLOSION), tile->getCenterX(), tile->getCenterY());
   return 0;
}


ClientMedKit::ClientMedKit(uint32 entityID) :
   ClientItem(entityID, spItem(new MedKit(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/medkit_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/medkit_gui_inv.png");
   //_image = NULL;
   //_invImage = NULL;
}

