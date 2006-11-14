#ifndef XCLONE_WEAPON_H
#define XCLONE_WEAPON_H

#include "includes.h"
#include "Item.h"

namespace xcore
{
    class Weapon;
    typedef boost::shared_ptr<Weapon> spWeapon;
    class Clip;
    typedef boost::shared_ptr<Clip> spClip;
}

namespace xcore
{

class Weapon : public Item
{

    protected:
        /// average damage
        int _baseDamage;
        /// +/- modifier to damage
        int _damageRange;
        /// base accuracy for the weapon
        int _baseAccuracy;
        /// standard clip size for the weapon
        int _clipSize;
        /// current ammunition
        int _ammo;
        /// creates an explosion on a tile
        void explosion( spMapTile tile, int damage );
        /// calculated damage given
        int calcDamage( int range = 0 ) const;
		

    public:
        Weapon(uint32 entityID);

        /// reload weapon
        virtual void reload();
        /// fire weapon
        virtual int use( spMapTile tile );
        virtual int hasAmmo() const;
};

class Clip : public Item
{
    public:
        Clip(uint32 entityID);
};

class Pistol : public Weapon
{
    public:
        Pistol(uint32 entityID);
        int use( spMapTile tile );
};

class PistolClip : public Clip
{
    public:
        PistolClip(uint32 entityID);
};

class Rifle : public Weapon
{
    public:
        Rifle(uint32 entityID);
        int use( spMapTile tile );
};

class RPG : public Weapon
{
    public:
        RPG(uint32 entityID);
        int use( spMapTile tile );
};
class Rocket : public Clip
{
    public:
        Rocket(uint32 entityID);
};

class Grenade : public Weapon
{
    public:
        Grenade(uint32 entityID);
        int use( spMapTile tile );
};

class MedKit : public Weapon
{
    public:
        MedKit(uint32 entityID);
        int use( spMapTile tile );
};

} // namespace
#endif /* XCLONE_WEAPON_H */
