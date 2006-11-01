#include "includes.h"
#include "ClientWeapon.h"
#include "Display.h"
#include "xcore/Weapon.h"

ClientPistol::ClientPistol(uint32 entityID) :
   ClientItem(entityID, spItem(new Pistol(0)))
{
   //_image = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   //_invImage = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   _image = Display::instance().loadImage("resources/images/gui/pistol_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/pistol_gui_inv.png");
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
   _image = Display::instance().loadImage("resources/resources/images/gui/cartridge_gui.png");
   _invImage = Display::instance().loadImage("resources/resources/images/gui/cartridge_gui.png");
   //_image = Display::instance().loadImage("images/rifle.png", false);
}

ClientGrenade::ClientGrenade(uint32 entityID) :
   ClientItem(entityID, spItem(new Grenade(0)))
{
   //_image = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   //_invImage = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   _image = Display::instance().loadImage("resources/images/gui/grenade_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/grenade_gui_inv.png");
}

ClientMedKit::ClientMedKit(uint32 entityID) :
   ClientItem(entityID, spItem(new MedKit(0)))
{
   _image = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   _invImage = Display::instance().loadImage("resources/images/gui/cartridge_gui.png");
   //_image = NULL;
   //_invImage = NULL;
}

