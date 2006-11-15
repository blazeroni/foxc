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
      static spClientUnit makeUnit(uint32 entityID, int playerID, int playerNum, string name);
      static spClientUnit makeUnit(uint32 entityID, int playerID, spMapTile tile, int playerNum, string name);
      virtual ~ClientUnit();

      virtual void move( spMapTile tile );

      virtual void markSelected();
      static void clearSelected();

      static spUnit getSelectedUnit();

      virtual void draw(const Point& position, const Point& dimensions) const;
      virtual void drawMovePath(const Point& offset) const;

      spLightweightSprite _guiSprite;

      static SDL_Surface* _image;

   protected:
      ClientUnit(uint32 entityID, int playerID, spMapTile tile, int playerNum, string name);

   private:
      spLightweightSprite _sprite;

      static spUnit _selectedUnit;

      static spLightweightSprite _player1;
      static spLightweightSprite _player2;

      static spLightweightSprite _player1GUI;
      static spLightweightSprite _player2GUI;

      static SDL_Surface* _corpseImage;
      static SDL_Surface* _pathImage;
      static SDL_Surface* _pathEndImage;
      static SDL_Surface* _selectedImage;
      static SDL_Surface* _activeImage;
};

#endif
