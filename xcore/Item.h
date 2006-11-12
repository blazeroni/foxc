#ifndef XCLONE_ITEM_H
#define XCLONE_ITEM_H

#include "includes.h"
namespace xcore
{
    class Item;
    typedef boost::shared_ptr<Item> spItem;

    enum itemtype { NONE, PISTOL, PISTOLCLIP, GRENADE };
}

#include "MapTile.h"

namespace xcore
{

class Item : public GameEntity
{

    protected:
        /// weapons name
        string _name;
        /// effective range for the weapon
        int _effectiveRange;
	/// turn cost
	int _turnCost;
        /// type for some items
        string _type;

    public:
        Item(uint32 entityID);
        /// returns weapon name
        string getName() const;
	/// returns turn cost to use item
	int getTurnCost() const;
        /// returns effective range
        int getRange() const;
        /// use item
        virtual int use( spMapTile tile );
	     /// returns image
	     virtual SDL_Surface* getImage();
	     virtual SDL_Surface* getInvImage();
        /// returns true for items, used for weapons
        virtual int hasAmmo() const;
        /// returns type
        virtual string getType() const;
        /// reloads for weapons
        virtual void reload();

};
}
#endif /* XCLONE_ITEM_H */
