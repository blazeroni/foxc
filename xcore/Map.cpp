#include "includes.h"
#include "Map.h"
#include "Wall.h"
#include "MapTile.h"
#include "Terrain.h"
#include "Event.h"
#include "MapLoadEvent.h"
#include "UnsupportedOperationException.h"

namespace xcore 
{

map<string, TerrainType> Map::_terrainMap;

spMap Map::makeMap(spEntityFactory factory)
{
   return makeMapHelper<Map>(factory);
}

Map::Map(spEntityFactory factory) :
   _factory(factory),
   _width(0),
   _height(0),
   _name("")
{
   static bool mapped = false;
   if (!mapped) 
   {
      _terrainMap["grass"] = GRASS;
      _terrainMap["water"] = WATER;

      mapped = true;
   }
}

Map::~Map()
{
}

spMapTile Map::getTile(int x, int y) const
{
   unsigned int index = y + (x * _height);
   if (index < _mapTiles.size() && x >= 0 && x < _width && y >= 0 && y < _height)
   {
      return _mapTiles[index];
   }
   else
   {
      return spMapTile();
   }
}

//vector<spMapTile> Map::getMapTiles()
//{
//   return _mapTiles;
//}

//spMapTile Map::getMouseOverTile()
//{
//   return _mouseOverTile;
//}

//void Map::updateMouseOverTile(Point offset)
//{
//   Point mouse = Input::instance().getMousePosition();
//   _mouseOverTile = getTile(mouse, offset);
//}


bool Map::load(string fileName)
{
   _fileName = "";
   try
   {
      ticpp::Document doc(fileName);
      doc.LoadFile();

      ticpp::Element* root = doc.FirstChildElement("map");
      root->GetAttribute("name", &_name);
      root->GetAttribute("width", &_width);
      root->GetAttribute("height", &_height);

      // empty the current map
      _mapTiles.resize(0);

      // make enough room for the current map size to avoid reallocations
      _mapTiles.reserve(_width * _height);

      int x = 0;
      int y = 0;
      string id;
      string terrain;

      uint16 startPlayer;
      uint16 startPref;

      map<string, spMapTile> idTileMap;

      map<spMapTile, bool> walls;
      map<spMapTile, bool> doors;

      ticpp::Element* tiles = root->FirstChildElement("tiles");
      ticpp::Iterator< ticpp::Element > child;
      for ( child = tiles->FirstChildElement(); child != child.end(); child++ )
      {
         child->GetAttribute("id", &id);
         child->GetAttribute("x", &x);
         child->GetAttribute("y", &y);
         child->GetAttribute("terrain", &terrain);

         spMapTile tile =  makeMapTile(_terrainMap[terrain], x, y);

         child->GetAttributeOrDefault("startPlayer", &startPlayer, 0);
         if (startPlayer != 0)
         {
            child->GetAttribute("startPref", &startPref);
            _playerStartPrefs[startPlayer][startPref] = tile;
         }

         ticpp::Element* object = child->FirstChildElement(false);
         if (object != NULL)
         {
            string type;
            object->GetAttribute("type", &type);

            if (type == "wall")
            {
               walls[tile] = true;
               doors[tile] = false;
               //tile->addObject(_factory->makeWall());
            }
            else if (type == "door")
            {
               walls[tile] = true;
               doors[tile] = true;
               // TODO replace w/ a real door
               //tile->addObject(_factory->makeWall(WT_DOOR));
            }
         }
         else
         {
            walls[tile] = false;
            doors[tile] = false;
         }

         idTileMap[id] = tile;
         populateTileNeighbors(tile);
         _mapTiles.push_back(tile);
      }
      loadWalls(walls, doors);
   }
   catch (ticpp::Exception& e)
   {
      cout << e.m_details;
      return false;
   }
   _fileName = fileName;
   return true;
}

void Map::loadWalls(map<spMapTile, bool> walls, map<spMapTile, bool> doors)
{
   map<spMapTile, bool>::iterator iter;

   for (iter = walls.begin(); iter != walls.end(); ++iter)
   {
      if (iter->second == true)
      {
         WALL_TYPE type;

         uint8 wallType = 0;
   
         spMapTile temp = iter->first->getTileInDirection(Direction::NE);
         if (temp.get() && walls[temp])
         {
            wallType |= WD_NE;
         }
         temp = iter->first->getTileInDirection(Direction::NW);
         if (temp.get() && walls[temp])
         {
            wallType |= WD_NW;
         }
         temp = iter->first->getTileInDirection(Direction::SE);
         if (temp.get() && walls[temp])
         {
            wallType |= WD_SE;
         }
         temp = iter->first->getTileInDirection(Direction::SW);
         if (temp.get() && walls[temp])
         {
            wallType |= WD_SW;
         }
         type = WALL_TYPE(wallType);

         if (doors[iter->first] == true)
         {
            iter->first->addObject(_factory->makeDoor(type));
         }
         else
         {
            iter->first->addObject(_factory->makeWall(type));
         }
      }
   }
}

spMapTile Map::getPlayerStartPref(uint16 playerNum, uint16 startPref)
{
   return _playerStartPrefs[playerNum][startPref];
}

spMapTile Map::getNextStartPref(uint16 playerNum)
{
   map<int, spMapTile> tiles = _playerStartPrefs[playerNum];
   spMapTile empty = spMapTile();
   map<int, spMapTile>::iterator iter;
   for (iter = tiles.begin(); iter != tiles.end(); ++iter)
   {
      if (iter->second->isPassable())
      {
         empty = iter->second;
         break;
      }
   }
   return empty;
}

spMapTile Map::makeMapTile(TerrainType type, int x, int y)
{
   return MapTile::makeMapTile(type, x, y);
}

/*spWall Map::makeWall() 
{
   return spWall(new Wall());
}*/

void Map::populateTileNeighbors(spMapTile tile)
{
   int x = tile->getX();
   int y = tile->getY();

   vector<const Direction*> directions = Direction::getAllDirections();

   vector<const Direction*>::const_iterator iter;
   for (iter = directions.begin(); iter != directions.end(); ++iter)
   {
      spMapTile t = getTile(x + (*iter)->offset().x, y + (*iter)->offset().y);
      if (t.get())
      {
         tile->addTileInDirection( t, *(*iter) );
      }
   }
}

void Map::drawTerrainLayer(const Point& offset)
{
   throw UnsupportedOperationException();
}

void Map::drawObjectLayer(const Point& offset)
{
   throw UnsupportedOperationException();
}

/*
void Map::drawMinimap()
{
    int minimapTileSize = 4;
    int minimapScreenMargin = 10;
    Display& d = Display::instance();
    SDL_Rect border = { minimapScreenMargin, d.getScreenHeight()-minimapTileSize*_height-2 - minimapScreenMargin, _width*minimapTileSize+2, _height*minimapTileSize+2 };
    SDL_Color white = { 0xFF, 0xFF, 0xFF };
    SDL_Color green = { 0x00, 0x79, 0x1B };
    SDL_Color blue = { 0x00, 0x71, 0xFE };
    SDL_Color red = { 0xFF, 0x00, 0x00 };
    SDL_Color gray = { 0x74, 0x74, 0x74 };
    Display::instance().drawRect( &border, &white );
    for ( int i = 0; i < _height; ++i )
    {
        for ( int j = 0; j < _width; ++j )
        {
            SDL_Rect tile = { border.x + 1 + minimapTileSize*j, border.y + 1 + minimapTileSize*i, minimapTileSize, minimapTileSize };
            if ( getTile(i,j)->hasUnit() )
                d.drawRect( &tile, &red );
            else if ( getTile(i,j)->getTerrainType() == GRASS )
            {
                if ( getTile(i, j)->isPassable() )
                    d.drawRect( &tile, &green );
                else
                    d.drawRect( &tile, &gray );
            }
            else if ( getTile(i,j)->getTerrainType() == WATER )
                d.drawRect( &tile, &blue );
        }
    }
}
*/

void Map::highlightMouseOverTile(const Point& offset)
{
   throw UnsupportedOperationException();
}

int Map::getHeight() const
{
   return _height;
}

int Map::getWidth() const
{
   return _width;
}

spMapTile Map::getMouseOverTile() const
{
   throw UnsupportedOperationException();
}

void Map::updateMouseOverTile(const Point& mouse, const Point& offset)
{
   throw UnsupportedOperationException();
}

string Map::getName() const
{
   return _name;
}

string Map::getFileName() const
{
   return _fileName;
}

} // namespace
