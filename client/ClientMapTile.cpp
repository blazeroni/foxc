#include "includes.h"
#include "ClientMapTile.h"
#include "ClientTerrain.h"
#include "Display.h"
#include "XClient.h"
#include "MainGameState.h"

int ClientMapTile::_width = 80;
int ClientMapTile::_height = 42;

SDL_Surface* ClientMapTile::_highlight = NULL;
SDL_Surface* ClientMapTile::_highlightMove = NULL;
SDL_Surface* ClientMapTile::_highlightFog = NULL;
SDL_Surface* ClientMapTile::_debris = NULL;

#define TILE_HIGHLIGHT "resources/images/tile_highlight.png"
#define TILE_MOVEABLE "resources/images/tile_moveable.png"
#define TILE_FOG "resources/images/tile_fog.png"
#define TILE_DEBRIS "resources/images/tile_debris.png"

ClientMapTile::ClientMapTile(TerrainType type, int x, int y) :
   MapTile(type, x, y)
{
   static int halfHeight = ClientMapTile::_height / 2;
   _screenX = y*_width/2 - (x)*_width/2;
   _screenY = (y+x) * _height/2;
   _centerX = y*_width/2 - (x-1)*_width/2;
   _centerY = (y+x+1) * _height/2;
   _hasDebris = false;

   if (!_highlight)
   {
      _highlight = Display::instance().loadImage(TILE_HIGHLIGHT);
      _highlightMove = Display::instance().loadImage(TILE_MOVEABLE);
      _highlightFog = Display::instance().loadImage(TILE_FOG);
	  _debris = Display::instance().loadImage(TILE_DEBRIS);
   }

   setTerrain(makeTerrain(type));
}

ClientMapTile::~ClientMapTile()
{
}

spClientMapTile ClientMapTile::makeMapTile(TerrainType type, int x, int y) 
{
   return makeMapTileHelper<ClientMapTile>(type, x, y);  
}

spTerrain ClientMapTile::makeTerrain(TerrainType type)
{
   return spTerrain(new ClientTerrain(type));
}

spClientUnit ClientMapTile::getUnit() const
{
   return boost::static_pointer_cast<ClientUnit>(MapTile::getUnit());
}

int ClientMapTile::getScreenX() const
{
   return _screenX;
}
int ClientMapTile::getScreenY() const
{
   return _screenY;
}
int ClientMapTile::getCenterX() const
{
   return _centerX;
}
int ClientMapTile::getCenterY() const
{
   return _centerY;
}

int ClientMapTile::getWidth()
{
   return _width;
}

int ClientMapTile::getHeight()
{
   return _height;
}

void ClientMapTile::drawTerrain(Point offset) const
{
    bool isShroud = ((MainGameState*)XClient::instance().getCurrentGameState())->getShroud(getX(),getY());
    if ( isShroud )
        Display::instance().draw(_screenX - offset.x, _screenY - offset.y, _highlightFog);
    else
	{
        getTerrain()->draw(_screenX - offset.x, _screenY - offset.y);
		if ( _hasDebris && getTerrain()->getType() != WATER )
			Display::instance().draw(_screenX - offset.x, _screenY - offset.y, _debris);
	}
}

void ClientMapTile::drawObjects(Point offset) const
{
   Point dim(_width, _height);
   Point position(_screenX - offset.x, _screenY - offset.y);
   bool isFog = ((MainGameState*)XClient::instance().getCurrentGameState())->getFog(getX(),getY());
   bool isShroud = ((MainGameState*)XClient::instance().getCurrentGameState())->getShroud(getX(),getY());
   if ( isFog )
        Display::instance().draw( position.x, position.y, _highlightFog ); 
   if ( isShroud ) return;
   if (!_objects.empty())
   {
      //_objects.front()->draw(position, dim);
        list<spMapObject>::const_iterator iter;

        for (iter = _objects.begin(); iter != _objects.end(); ++iter)
        {
            (*iter)->draw(position, dim);
        }
   }
   if ( isFog ) return;
   if (_unit.get())
   {
      _unit->draw(position, dim);
   }
}

void ClientMapTile::highlight(Point offset)
{
   Display::instance().draw(_screenX - offset.x, _screenY - offset.y, _highlight);
}

void ClientMapTile::highlightMoveable(Point offset)
{
   Display::instance().draw(_screenX - offset.x, _screenY - offset.y, _highlightMove);
}

