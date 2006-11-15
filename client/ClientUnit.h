#ifndef XCLIENT_CLIENTUNIT_H
#define XCLIENT_CLIENTUNIT_H

#include "includes.h"
#include "xcore/Unit.h"
#include "IDisplayable.h"
#include "Sprite.h"

class ClientUnit;
typedef boost::shared_ptr<ClientUnit> spClientUnit;

class ClientUnit : public Unit
{
   // to allow Unit factory method to create these objects
   friend class Unit;

   public:
      static spClientUnit makeUnit(uint32 entityID, int playerID);
      static spClientUnit makeUnit(uint32 entityID, int playerID, spMapTile tile);
      virtual ~ClientUnit();

      virtual void move( spMapTile tile );

      virtual void markSelected();
      static void clearSelected();

      static spUnit getSelectedUnit();

      virtual void draw(const Point& position, const Point& dimensions) const;
      virtual void drawMovePath(const Point& offset) const;
      static SDL_Surface* _image;

   protected:
      ClientUnit(uint32 entityID, int playerID, spMapTile tile);

   private:
      static spUnit _selectedUnit;

      static spSprite _sprite;

      static SDL_Surface* _corpseImage;
      static SDL_Surface* _pathImage;
      static SDL_Surface* _pathEndImage;
      static SDL_Surface* _selectedImage;
      static SDL_Surface* _activeImage;
};

#endif
