#include "includes.h"
#include "ClientUnit.h"
#include "Display.h"
#include "ResourceManager.h"

spUnit ClientUnit::_selectedUnit;

SDL_Surface* ClientUnit::_image = NULL;
SDL_Surface* ClientUnit::_corpseImage = NULL;
SDL_Surface* ClientUnit::_pathImage = NULL;
SDL_Surface* ClientUnit::_pathEndImage = NULL;
SDL_Surface* ClientUnit::_selectedImage = NULL;
SDL_Surface* ClientUnit::_activeImage = NULL;

spLightweightSprite ClientUnit::_player1 = spLightweightSprite();
spLightweightSprite ClientUnit::_player2 = spLightweightSprite();

spLightweightSprite ClientUnit::_player1GUI = spLightweightSprite();
spLightweightSprite ClientUnit::_player2GUI = spLightweightSprite();

#define UNIT_SPRITE_ID "unit"

#define PLAYER_1_SPRITES "unit1"
#define PLAYER_2_SPRITES "unit2"
#define PLAYER_3_SPRITES "unit3"
#define PLAYER_4_SPRITES "unit4"

#define PLAYER_1_GUI_SPRITE "unit1gui"
#define PLAYER_2_GUI_SPRITE "unit2gui"

#define UNIT_IMAGE  "resources/images/CharFront.png"
#define UNIT_IMAGEF "resources/images/CharFront.png"
#define UNIT_IMAGEL "resources/images/CharLeft.png"
#define UNIT_IMAGER "resources/images/CharRight.png"
#define UNIT_IMAGEB "resources/images/CharBack.png"
#define UNIT_CORPSE_IMAGE "resources/images/unit_corpse.png"

#define UNIT_PATH_IMAGE "resources/images/unit_path.png"
#define UNIT_PATH_END_IMAGE "resources/images/unit_path_end.png"
#define UNIT_ACTIVE_IMAGE "resources/images/unit_active.png"
#define UNIT_SELECTED_IMAGE "resources/images/unit_selected.png"

ClientUnit::ClientUnit(uint32 entityID, int playerID, spMapTile tile, int playerNum) :
   Unit(entityID, playerID, tile, playerNum)
{
   static bool mapped = false;
   if (!mapped) 
   {
      _image = Display::instance().loadImage(UNIT_IMAGE, false);
      _corpseImage = Display::instance().loadImage(UNIT_CORPSE_IMAGE, false);
      _pathImage = Display::instance().loadImage(UNIT_PATH_IMAGE);
      _pathEndImage = Display::instance().loadImage(UNIT_PATH_END_IMAGE);
      _selectedImage = Display::instance().loadImage(UNIT_SELECTED_IMAGE);
      _activeImage = Display::instance().loadImage(UNIT_ACTIVE_IMAGE);


      _player1 = ResourceManager::instance().getSprite(PLAYER_1_SPRITES);
      _player2 = ResourceManager::instance().getSprite(PLAYER_2_SPRITES);

      _player1GUI = ResourceManager::instance().getSprite(PLAYER_1_GUI_SPRITE);
      _player2GUI = ResourceManager::instance().getSprite(PLAYER_2_GUI_SPRITE);

      mapped = true;
   }

   switch (playerNum)
   {
      case 1:
      default:
         _sprite = spLightweightSprite(new LightweightSprite(_player1));
         _guiSprite = spLightweightSprite(new LightweightSprite(_player1GUI));
         break;
      case 2:
         _sprite = spLightweightSprite(new LightweightSprite(_player2));
         _guiSprite = spLightweightSprite(new LightweightSprite(_player2GUI));
         break;
   }

}

ClientUnit::~ClientUnit()
{
}

spClientUnit ClientUnit::makeUnit(uint32 entityID, int playerID, int playerNum)
{
   return makeUnit(entityID, playerID, spMapTile(), playerNum);
}

spClientUnit ClientUnit::makeUnit(uint32 entityID, int playerID, spMapTile tile, int playerNum)
{
   return makeUnitHelper<ClientUnit>(entityID, playerID, tile, playerNum);
}

// this needs to be changed...
void ClientUnit::move( spMapTile tile ) 
{
   if (tile->isPassable())
   {
      _tile->removeUnit();
      if(tile->getX() <= _tile->getX() && tile->getY() < _tile->getY())
      {
         _image = Display::instance().loadImage(UNIT_IMAGER, false);
      }
      else if(tile->getX() > _tile->getX() && tile->getY() <= _tile->getY())
      {
         _image = Display::instance().loadImage(UNIT_IMAGEF, false);
      }
      else if(tile->getX() >= _tile->getX() && tile->getY() > _tile->getY())
      {
         _image = Display::instance().loadImage(UNIT_IMAGEL, false);
      }
      else if(tile->getX() < _tile->getX() && tile->getY() >= _tile->getY())
      {
         _image = Display::instance().loadImage(UNIT_IMAGEB, false);
      }
      _tile = tile;
      _tile->addUnit(spUnit(_weak));
      updatePossibleMoves();
   }  
}

void ClientUnit::markSelected()
{
   _selectedUnit = spUnit(_weak);
}

void ClientUnit::clearSelected()
{
   _selectedUnit = spClientUnit();
}

spUnit ClientUnit::getSelectedUnit()
{
   return _selectedUnit;
}

void ClientUnit::draw(const Point& position, const Point& dimensions) const
{
   Display& d = Display::instance();
   if (this == _selectedUnit.get())
   {
      d.draw(position.x + dimensions.x/2 -_selectedImage->w/2,
             position.y + dimensions.y/2 - _selectedImage->h/2, 
             _selectedImage);

      ostringstream stream;
      stream << "Action Points: " << getActionPoints();
      d.draw(d.getScreenWidth() - 150, 5, stream.str());

   }
   if (this == _activeUnit.get())
   {
      d.draw(position.x + dimensions.x/2 -_activeImage->w/2,
             position.y + dimensions.y/2 - _activeImage->h/2, 
             _activeImage);
   }

   if ( _health <= 0 )
   {
      d.draw(position.x + dimensions.x/2 - _corpseImage->w/2,
      position.y + dimensions.y/2 - _corpseImage->h + 5,
      _corpseImage);
   }
   else
   {
      //d.draw(position.x + dimensions.x/2 - _image->w/2,
      //      position.y + dimensions.y/2 - _image->h + 5, 
      //      _image);

      _sprite->draw(position.x + dimensions.x/2, position.y + dimensions.y/2);
   }
}

void ClientUnit::drawMovePath(const Point& offset) const
{
   list<pair<spMapTile, int> >::const_iterator iter;
   for (iter = _movementPath.begin(); iter != _movementPath.end(); ++iter)
   {
      SDL_Surface* img = NULL;
      if (*iter == _movementPath.back())
      {
         img = _pathEndImage;
      }
      else 
      {
         img = _pathImage;
      }

      Display::instance().draw((*iter).first->getCenterX() - img->w/2 - offset.x, 
                               (*iter).first->getCenterY() - img->h/2 - offset.y, 
                               img);
   }
}
