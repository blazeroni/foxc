#include "includes.h"
#include "Weapon.h"
//#include "Display.h"
//#include "Audio.h"
#include "MapObject.h"

namespace xcore {

Weapon::Weapon(uint32 entityID) : Item(entityID)
{
    _type = (itemtype)0;
}

int Weapon::calcDamage( int range ) const
{
    int damage = 0;
    // chance to miss
    if ( _name == "Grenade" || rand()%100 < _baseAccuracy )
    // compute initial damage with variation
        damage = _baseDamage + rand()%_damageRange;
    // adjust damage for range
    // damage *= (_effectiveRange - range) / _effectiveRange;
    return damage;
}

int Weapon::use( spMapTile tile )
{
    if ( _ammo )
    {
        _ammo--;
        //Audio::instance().playEffect(Audio::_blaster, 1);
        tile->takeDamage( calcDamage() );
        return 1;
    }
    return -1;
}

void Weapon::reload()
{
    _ammo = _clipSize;
}

int Weapon::hasAmmo() const
{
    return _ammo;
}

Clip::Clip(uint32 entityID) : Item(entityID)
{
    _name = "Clip";
}

Pistol::Pistol(uint32 entityID) : Weapon(entityID)
{
    _name = "Pistol";
    _type = PISTOL;
    _baseDamage = 50;
    _damageRange = 10;
    _effectiveRange = 20;
    _baseAccuracy = 65;
    _clipSize = 12;
    _turnCost = 20;
    //_image = Display::instance().loadImage("images/pistol.gif", false);
    //_image = Display::instance().loadImage("images/gui/pistol_gui.png");
    //_invImage = Display::instance().loadImage("images/gui/pistol_gui_inv.png");
    _ammo = _clipSize;
}
PistolClip::PistolClip(uint32 entityID) :
   Clip(entityID)
{
    _type = PISTOLCLIP;
    //_image = Display::instance().loadImage( "images/gui/pistol_clip_gui.png" );
    //_invImage = Display::instance().loadImage( "images/gui/pistol_clip_gui_inv.png" );
}

int Pistol::use( spMapTile tile ) { Weapon::use( tile ); return 0; }

Rifle::Rifle(uint32 entityID) : Weapon(entityID)
{
    _name = "Rifle";
    _baseDamage = 100;
    _damageRange = 0;
    _effectiveRange = 50;
    _baseAccuracy = 90;
    _clipSize = 3;
    _turnCost = 30;
    //_image = Display::instance().loadImage("images/rifle.png", false);
    _ammo = _clipSize;
}

int Rifle::use( spMapTile tile ) { Weapon::use( tile ); return 0; }

Bazooka::Bazooka(uint32 entityID) : Weapon(entityID)
{
    _name = "Bazooka";
    _baseDamage = 200;
    _damageRange = 20;
    _effectiveRange = 20;
    _baseAccuracy = 45;
    _clipSize = 12;
    _turnCost = 30;
    //_image = Display::instance().loadImage("images/bazooka.png", false);
    _ammo = _clipSize;
}

int Bazooka::use( spMapTile tile ) { Weapon::use( tile ); return 0; }

Grenade::Grenade(uint32 entityID) : Weapon(entityID)
{
    _name = "Grenade";
    _type = GRENADE;
    _baseDamage = 200;
    _damageRange = 20;
    _effectiveRange = 8;
    _baseAccuracy = 30;
    _clipSize = -1;
    _turnCost = 30;
    //_image = Display::instance().loadImage("images/gui/grenade_gui.png");
    //_invImage = Display::instance().loadImage("images/gui/grenade_gui_inv.png");
    _ammo = _clipSize;
}

int Grenade::use( spMapTile tile )
{
    explosion( tile, calcDamage() );
    return 0;
}

MedKit::MedKit(uint32 entityID) : Weapon(entityID)
{
    _name = "MedKit";
    _baseDamage = -30;
    _damageRange = 10;
    _effectiveRange = 1;
    _baseAccuracy = 100;
    _clipSize = 10;
    _turnCost = 20;
//    _image = NULL;
    _ammo = _clipSize;
    _type = MEDKIT;
}

// should be changed to only heal units once other objects can take damage
int MedKit::use( spMapTile tile ) { Weapon::use( tile ); return 0; }

void Weapon::explosion( spMapTile tileInit, int damage )
{
    // this method takes a creates an explosion on a tile by assigning that tile a damage and then
    // giving tile out to range 2 decreasing damage
    //Audio::instance().playEffect(Audio::_explosion, 1);
    spMapTile tile, tile2;
    tileInit->takeDamage( damage, true);
	tileInit->addDebris();
    //tileInit->addObject( spMapObject(new Debris()) );
    tile = tileInit->getTileInDirection( Direction::NW );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::W );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::NW );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::N );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::NE );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::E );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::NE );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::N );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::SW );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::W );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::SW );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::S );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::SE );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::E );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::SE );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
        tile2 = tile->getTileInDirection( Direction::S );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::N );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::N );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::S );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::S );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::E );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::E );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
    tile = tileInit->getTileInDirection( Direction::W );
    if ( tile.get() )
    {
        tile->takeDamage( damage / 2, true );
        //tile->addObject( spMapObject(new Debris()) );
		tile->addDebris();
        tile2 = tile->getTileInDirection( Direction::W );
        if ( tile2.get() )
            tile2->takeDamage( damage / 4, true );
    }
}

} // namespace
