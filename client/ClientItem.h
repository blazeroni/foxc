#ifndef XCLIENT_CLIENTITEM_H
#define XCLIENT_CLIENTITEM_H

#include "includes.h"
#include "xcore/Item.h"

// like ClientWeapon, this is meant to be used as an adapter for an Item object
class ClientItem : public Item
{
   public:
      ClientItem(uint32 entityID, spItem item);
      virtual ~ClientItem();

      // define non-image virtual functions to be forwarded to _item
      virtual int use( spMapTile tile )      { return _item->use(tile); }
      virtual int hasAmmo() const            { return _item->hasAmmo(); }
      virtual itemtype getType() const         { return _item->getType(); }
      virtual void reload()                  { _item->reload(); }
      virtual string getName() const         { return _item->getName(); }
      virtual int getRange() const           { return _item->getRange(); }

      virtual SDL_Surface* getImage();
      virtual SDL_Surface* getInvImage();

   protected:
      /// weapons graphic
      SDL_Surface *_image, *_invImage;
      spItem _item;
};

#endif

