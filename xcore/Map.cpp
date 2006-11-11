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

/*
spMapTile Map::getTile(Point position, Point offset)
{
   static Point size(MapTile::getWidth(), MapTile::getHeight());
   int x = position.x + offset.x;
   int y = position.y + offset.y;

   vector<spMapTile>::iterator iter;

   for (iter = _mapTiles.begin(); iter != _mapTiles.end(); ++iter)
   {
      Point origin((*iter)->getScreenX(), (*iter)->getScreenY());
      if (x > origin.x && x < origin.x + size.x && y > origin.y && y < origin.y + size.y)
      {
         // difference from pixel to origin of image
         Point diff(x - origin.x, y - origin.y);

         if (SDL_MUSTLOCK(_tileMouseMap))
         {
            SDL_LockSurface(_tileMouseMap);
         }
 
         Uint16* pixels = static_cast<Uint16*>(_tileMouseMap->pixels);
         Uint16 pix = pixels[diff.x + diff.y*_tileMouseMap->w];

         Uint16 red = pix & _tileMouseMap->format->Rmask;
         Uint16 green = pix & _tileMouseMap->format->Gmask;
         Uint16 blue = pix & _tileMouseMap->format->Bmask;

         if (SDL_MUSTLOCK(_tileMouseMap))
         {
            SDL_UnlockSurface(_tileMouseMap);
         }
         
         // offset from current image to actual MapTile
         Point offset;

         if (red && !green && !blue) 
         {
            offset.y = -1;
         }
         else if (!red && green && !blue)
         {
            offset.x = -1;
         }
         else if (!red && !green && blue)
         {
            offset.x = 1;
         }
         else if (red && green && !blue)
         {
            offset.y = 1;
         }

         Point coords((*iter)->getX() + offset.x, (*iter)->getY() + offset.y);
         if (coords.x >= 0 && coords.x < _width && coords.y >= 0 && coords.y < _height)
         {
            return getTile(coords.x, coords.y);
         }
      }
   }

   return spMapTile();
}
*/

bool Map::load(string fileName)
{
   _name = "";
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

      map<string, spMapTile> idTileMap;

      map<spMapTile, bool> walls;

      ticpp::Element* tiles = root->FirstChildElement("tiles");
      ticpp::Iterator< ticpp::Element > child;
      for ( child = tiles->FirstChildElement(); child != child.end(); child++ )
      {
         child->GetAttribute("id", &id);
         child->GetAttribute("x", &x);
         child->GetAttribute("y", &y);
         child->GetAttribute("terrain", &terrain);

         spMapTile tile =  makeMapTile(_terrainMap[terrain], x, y);

         ticpp::Element* object = child->FirstChildElement(false);
         if (object != NULL)
         {
            string type;
            object->GetAttribute("type", &type);

            if (type == "wall")
            {
               walls[tile] = true;
               //tile->addObject(_factory->makeWall());
            }
            else if (type == "door")
            {
               // TODO fill in
            }
         }
         else
         {
            walls[tile] = false;
         }

         idTileMap[id] = tile;
         populateTileNeighbors(tile);
         _mapTiles.push_back(tile);
      }
      loadWalls(walls);
   }
   catch (ticpp::Exception& e)
   {
      cout << e.m_details;
      return false;
   }
   _name = fileName;
   return true;
}

void Map::loadWalls(map<spMapTile, bool> walls)
{
   map<spMapTile, bool>::iterator iter;

   for (iter = walls.begin(); iter != walls.end(); ++iter)
   {
      if (iter->second == true)
      {
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

         WALL_TYPE type = WALL_TYPE(wallType);
         iter->first->addObject(_factory->makeWall(type));
      }
   }
}

bool Map::load(istream& data)
{
/*   string line;
   bool firstLine = true;

   // empty the current map
   _mapTiles.resize(0);

   int x = 0;
   int y = 0;

   while(data >> line)
   {
      if (firstLine)
      {
         string width(line.begin(), line.begin() + line.find(","));
         _width = atoi(width.c_str());

         string height(line.begin() + line.find(",") + 1, line.end());
         _height = atoi(height.c_str());

         // make enough room for the current map size to avoid reallocations
         int num = _width * _height;
         _mapTiles.reserve(_width * _height);

         firstLine = false;
      }
      else
      {
         if (x >= _width)
         {
            x = 0;
            y++;
         }
         string terrain;
         int wall = 0;
         if (line.find(":") != string::npos)
         {
            terrain = string(line.begin(), line.begin() + line.find(":"));
            wall = std::atoi (string(line.begin() + line.find(":") + 1, line.end()).c_str());
         }
         else
         {
            terrain = line;
         }
         //string terrain(line.begin(), line.begin() + line.find(":"));
         // it has to be a wall so the next line isn't needed currently
         //string extra(line.begin() + line.find(":") + 1, line.end();
         spMapTile tile = makeMapTile(_terrainMap[terrain], x, y);

         populateTileNeighbors(tile);

         _mapTiles.push_back(tile);

         if (wall & NE)
            tile->addObject(makeWall(Direction::NE));
         if (wall & NW)
            tile->addObject(makeWall(Direction::NW));
         if (wall & SE)
            tile->addObject(makeWall(Direction::SE));
         if (wall & SW)
            tile->addObject(makeWall(Direction::SW));
         x++;
      }
   }*/
   return true;
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

} // namespace
