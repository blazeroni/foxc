#include "includes.h"
#include "Unit.h"
#include "Weapon.h"
#include "UnsupportedOperationException.h"
//#include "Display.h"

//#define UNIT_IMAGE "images/unit.png"
//#define UNIT_PATH_IMAGE "images/unit_path.png"
//#define UNIT_PATH_END_IMAGE "images/unit_path_end.png"
//#define UNIT_ACTIVE_IMAGE "images/unit_active.png"
//#define UNIT_SELECTED_IMAGE "images/unit_selected.png"
//
//SDL_Surface* Unit::_image = NULL;
//SDL_Surface* Unit::_pathImage = NULL;
//SDL_Surface* Unit::_pathEndImage = NULL;
//SDL_Surface* Unit::_selectedImage = NULL;
//SDL_Surface* Unit::_activeImage = NULL;

#define UNIT_RESOURCE_ID "unit"
#define UNIT_PATH_RESOURCE_ID "unit_path"
#define UNIT_PATH_END_RESOURCE_ID "unit_path_end"
#define UNIT_ACTIVE_RESOURCE_ID "unit_active"
#define UNIT_SELECTED_RESOURCE_ID "unit_selected"

namespace xcore
{

int Unit::_maxActionPoints = 50;
int Unit::_maxHealth = 100;
int Unit::_maxMorale = 100;

spUnit Unit::_activeUnit;

//Unit::Unit(int playerID)
//{
//   Unit(playerID);
//}

spUnit Unit::makeUnit(uint32 entityID, int playerID)
{
   return makeUnit(entityID, playerID, spMapTile());
}

spUnit Unit::makeUnit(uint32 entityID, int playerID, spMapTile tile)
{
   return makeUnitHelper<Unit>(entityID, playerID, tile);
   //if (tile->isPassable())
   //{
   //   spUnit unit = makeUnitHelper<Unit>(playerID, tile);
   //   unit->_weak = unit;
   //   unit->_tile->addUnit(unit);
   //   return unit;
   //}
   //return spUnit();
}

Unit::Unit(uint32 entityID, int playerID, spMapTile tile) :
   GameEntity(entityID),
   _playerID(playerID),
   _actionPoints(_maxActionPoints),
   _health(_maxHealth),
   _morale(_maxMorale),
   _regenRate(10),
   _accuracy(50),
   _rank(1),
   _name("UnitName")
{
   static bool mapped = false;
   //if (!mapped) 
   //{
   //   _image = Display::instance().loadImage(UNIT_IMAGE);
   //   _pathImage = Display::instance().loadImage(UNIT_PATH_IMAGE);
   //   _pathEndImage = Display::instance().loadImage(UNIT_PATH_END_IMAGE);
   //   _selectedImage = Display::instance().loadImage(UNIT_SELECTED_IMAGE);
   //   _activeImage = Display::instance().loadImage(UNIT_ACTIVE_IMAGE);

   //   mapped = true;
   //}
   _tile = tile;

   // for weapons test
/*   _hand[1] = spItem(new Pistol());
   _hand[0] = spItem(new Grenade());
   _inventory[0] = spItem(new PistolClip());
   _inventory[1] = spItem(new Grenade());*/
   _hand[1] = spItem();
   _hand[0] = spItem();
   _inventory[0] = spItem();
   _inventory[1] = spItem();
   _inventory[2] = spItem();
   _inventory[3] = spItem();
   _inventory[4] = spItem();
}

Unit::~Unit()
{
}

int Unit::getPlayerID() const
{
   return _playerID;
}

int Unit::getX() const
{
   return _tile->getX();
}

int Unit::getY() const
{
   return _tile->getY();
}

void Unit::draw(const Point& position, const Point& dimensions) const
{
   throw UnsupportedOperationException();
}

bool Unit::isPassable() const
{
   return false;
}

void Unit::wait()
{
   _actionPoints = max(_actionPoints - 50, 0);
}

void Unit::move(const Direction& dir)
{
   if (canMoveInDirection(dir))
   {
      move(_tile->getTileInDirection(dir));
   }
}

void Unit::move(spMapTile tile)
{
   if (tile->isPassable())
   {
      _tile->removeUnit();
      _tile = tile;
      _tile->addUnit(spUnit(_weak));
      //updatePossibleMoves();
   }  
}

bool Unit::canMoveInDirection( const Direction& dir )
{
   return canMoveInDirection(_tile, dir, _actionPoints);
}

bool Unit::canMoveInDirection( spMapTile tile, const Direction& dir, int points )
{
   int x = getX() + dir.offset().x;
   int y = getY() + dir.offset().y;

   spMapTile t = tile->getTileInDirection(dir);
   bool ignoreObjects = false;

   // true if it's the current tile
   // the actual map tile will say it's impassable
   // used for pathfinding
   if (_tile == t)
   {
      ignoreObjects = true;
   }

   bool ok = false;
   if (t.get() && t->isPassable() && points >= tile->getMoveCost(dir))
   {
      if (dir.isCardinal()) 
      {
         // this assumes that maps are square - so there will always be adjacent tiles
         /*if (tile->isPassable(dir.clockwise(), ignoreObjects) &&
             tile->isPassable(dir.counterClockwise(), ignoreObjects) &&
             t->isPassable(dir.opposite().clockwise(), ignoreObjects) && 
             t->isPassable(dir.opposite().counterClockwise(), ignoreObjects))
         {*/
            ok = true;
         //}
      }
      else
      {
         ok = true;
      }
   }
   return ok;
}

map<spMapTile, int> Unit::getPossibleMoves()
{
   return _possibleMoves;
}

void Unit::updatePossibleMoves()
{
   //map<spMapTile, int> final;
   //_possibleMoves.clear();
   map<spMapTile, int> temp;
   queue<spMapTile> waiting;

   waiting.push(_tile);
   temp[_tile] = 0;

   while (!waiting.empty())
   {
      spMapTile tile = waiting.front();
      int cost = temp[tile];
      int remaining = _actionPoints - cost;
      
      //_possibleMoves[tile] = cost;
      waiting.pop();

      vector<const Direction*> dirs = Direction::getAllDirections();
      vector<const Direction*>::const_iterator iter;
      for(iter = dirs.begin(); iter != dirs.end(); ++iter)
      {
         if (canMoveInDirection(tile, **iter, remaining))
         {
            spMapTile nextTile = tile->getTileInDirection(**iter);
            int nextCost = tile->getMoveCost(**iter) + cost;
            if (temp.count(nextTile) == 0)
            {
               waiting.push(nextTile);
               temp[nextTile] = nextCost;
            }
            else if (temp[nextTile] > nextCost)
            {
               temp[nextTile] = nextCost;
            }
            
            //tiles.push_back(_tile->getTileInDirection(**iter));
         }
      }
   }

   _possibleMoves = temp;
}

void Unit::clearPossibleMoves()
{
   _possibleMoves.clear();
}

void Unit::computeMovePath(spMapTile dest)
{
   _movementPath.clear();
   map<spMapTile, int>::iterator iter = _possibleMoves.find(dest);
   if (iter != _possibleMoves.end())
   {
      _movementPath.push_front(*iter);
      pair<spMapTile, int> next = *iter;
      while (true)
      {
         next = computeMovePathHelper(next.first);
         if (next.first == _tile)
         {
            break;
         }
         else
         {
            _movementPath.push_front(next);
         }
      }
   }
}

pair<spMapTile, int> Unit::computeMovePathHelper(spMapTile current)
{
   pair<spMapTile, int> lowest(spMapTile(), numeric_limits<int>::max());
   vector<const Direction*> dirs = Direction::getAllDirections();
   vector<const Direction*>::const_iterator iter;
   for(iter = dirs.begin(); iter != dirs.end(); ++iter)
   {
      if (canMoveInDirection(current, **iter, numeric_limits<int>::max()))
      {
         spMapTile adjacent = current->getTileInDirection(**iter);
         if (adjacent.get())
         {
            map<spMapTile, int>::iterator iter = _possibleMoves.find(adjacent);
            if (iter != _possibleMoves.end() && (*iter).second < lowest.second)
            {
               lowest = *iter;
            }
         }
      }
   }
   return lowest;
}

bool Unit::hasMovePath() const
{
   return !_movementPath.empty();
}

void Unit::moveAlongPath( bool teleport )
{
   if (!hasMovePath())
   {
      return;
   }
   
   if (teleport)
   {
      _actionPoints -= _movementPath.back().second;
      move(_movementPath.back().first);
      clearMovePath();
   }
   else
   {
      _actionPoints = _maxActionPoints - _movementPath.front().second;
      move(_movementPath.front().first);
      _movementPath.pop_front();
   }
}

void Unit::drawMovePath(const Point& offset) const
{
   throw UnsupportedOperationException();
}

void Unit::clearMovePath()
{
   _movementPath.clear();
}

const spMapTile Unit::getDestination() const
{
   if (hasMovePath())
   {
      return _movementPath.back().first;
   }
   else
   {
      return spMapTile();
   }
}

bool Unit::hasMaxActionPoints() const
{
   return _actionPoints == _maxActionPoints;
}

void Unit::regenActionPoints(int times)
{
   _actionPoints = min(_actionPoints + (_regenRate * times), _maxActionPoints);
}

bool Unit::hasTurnBefore(spUnit u) const
{
   if ( _regenRate == 0 )
      return false;
   if ( u->_regenRate == 0 )
      return true;

   // "_regenRate - 1" is there because this is integer math and it's flooring everything
   int thisSteps = (_maxActionPoints - _actionPoints + _regenRate - 1) / _regenRate; 
   int thatSteps = (_maxActionPoints - u->_actionPoints + u->_regenRate - 1) / u->_regenRate;

   return thisSteps < thatSteps;
}

void Unit::markActive()
{
   _activeUnit = spUnit(_weak);
}

void Unit::clearActive()
{
   _activeUnit = spUnit();
}

void Unit::markSelected() 
{
   throw UnsupportedOperationException();
}

vector<string> Unit::getResourceIDs()
{
   static vector<string> resIDs;
   static bool initialized = false;
   if (!initialized)
   {
      resIDs.push_back(UNIT_RESOURCE_ID);
      resIDs.push_back(UNIT_PATH_RESOURCE_ID);
      resIDs.push_back(UNIT_PATH_END_RESOURCE_ID);
      resIDs.push_back(UNIT_ACTIVE_RESOURCE_ID);
      resIDs.push_back(UNIT_SELECTED_RESOURCE_ID);
   }
   return resIDs;
}

int Unit::getActionPoints() const
{
   return _actionPoints;
}

void Unit::useActionPoints(int points)
{
   _actionPoints = max(0, _actionPoints - points);
}
/*
void Unit::fire( spMapTile tile )
{
   if ( !tile || tile == _tile )
      return;
   int damage = _rightHand->use();
   if ( damage >= 0 )
   {
      //Audio::instance().playEffect(Audio::_blaster, 1);
      _actionPoints -= _rightHand->getTurnCost();
      spUnit target = tile->getUnit();
      if ( target )
      {
         target->takeDamage( damage );
      }
   }
}
*/

void Unit::takeDamage( int damage )
{
   _health -= damage;
   if ( _health > _maxHealth ) _health = _maxHealth;
   if ( _health <= 0 )
      die();
}

void Unit::die()
{
   //Audio::instance().playEffect(Audio::_scream, 2);
   _regenRate = 0;
   _actionPoints = 0;
}

void Unit::heal()
{
    // Medkit has been use, heal unit
    _health = _maxHealth;
}// end of heal

/*
bool Unit::equip( spItem item )
{
   // no item
   if ( !item )
         return false;
   if ( _rightHand )
   {
      // hands full
      if ( _leftHand )
         return false;
      _leftHand = item;
   }
   _rightHand = item;
   return true;
}

bool Unit::drop( spItem item )
{
   // for now dropping destroys the item
   if ( _rightHand == item )
   {
      _rightHand = spItem();
      return true;
   }
   if ( _leftHand == item )
   {
      _leftHand = spItem();
      return true;
   }
   for ( int i = 0; i < 5; ++i )
   {
      if ( _inventory[i] == item )
      {
         _inventory[i] = spItem();
         return true;
      }
   }
   return false;
}

bool Unit::unequipRight()
{
   for ( int i = 0; i < 5; ++i )
   {
      if ( !_inventory[i] )
      {
         _inventory[i] = _rightHand;
         _rightHand = spItem();
         return true;
      }
   }
   return false;
}

bool Unit::unequipLeft()
{
   for ( int i = 0; i < 5; ++i )
   {
      if ( !_inventory[i] )
      {
         _inventory[i] = _leftHand;
         _leftHand = spItem();
         return true;
      }
   }
   return false;
}
*/

int Unit::getHealth() const
{
   return _health;
}

int Unit::getAccuracy() const
{
  return _accuracy;
}// end of getAccuracy

int Unit::getRank() const
{
  return _rank;
} // end of getRank

bool Unit::addItem(spItem item, int slot)
{
	if ( slot == 0 || slot == 1 )
		_hand[slot] = item;
	else
		_inventory[slot-2] = item;
	return true;
	/*
   for ( int i = 0; i < 5; ++i )
   {
      if ( !_inventory[i] )
      {
         _inventory[i] = item;
         return true;
      }
   }
   return false;
   */
} // end of addItem


spItem* Unit::getInventory()
{
	return _inventory;
}// end of getInventory

spItem Unit::getRight()
{
    return _hand[RIGHT];
}
spItem Unit::getLeft()
{
    return _hand[LEFT];
}
spItem Unit::getHand( int h )
{
    return _hand[h];
}
spItem Unit::getInv( int slot )
{
    return _inventory[slot];
}
/*
void Unit::reload()
{
    for ( int j = 0; j < 2; ++j )
    {
    if ( _hand[j] && _hand[j]->getType() == "Weapon" )
    {
        for ( int i = 0; i < 5; ++i )
        {
            if ( _inventory[i].get() && _inventory[i]->getName() == "Clip" )
            {
                if ( _inventory[i]->getType() == _hand[(j+1)%2]->getName() )
                {
                    _hand[(j+1)%2]->reload();
                    _inventory[i] = spItem();
                    }
            }
        }
    }
    }
}
*/
void Unit::use( spMapTile tile, int wHand )
{
    if ( !_hand[wHand].get() )
        return;
    if ( _hand[wHand]->getName() == "Clip" ) 
    {
        if ( _hand[(wHand+1)%2].get() && (_hand[wHand])->getType() == PISTOL && _hand[(wHand+1)%2]->getType() == PISTOLCLIP )
        {
            (_hand[(wHand+1)%2])->reload();
            _actionPoints -= _hand[wHand]->getTurnCost();
            _hand[wHand] = spItem();
            //updatePossibleMoves();
        }
        return;
    }
	if ( _tile->getDistance(tile) > _hand[wHand]->getRange() )
        return;
    if ( _hand[wHand]->getName() == "Grenade" )
    {
        _hand[wHand]->use( tile );
        _actionPoints -= _hand[wHand]->getTurnCost();
        //updatePossibleMoves();
        _hand[wHand] = spItem();
        return;
    }
    if ( !tile || !_hand[wHand]->hasAmmo() )
    //if ( !tile || tile == _tile || !_hand[wHand]->hasAmmo() )
        return;
    int used = _hand[wHand]->use( tile );
    if ( used >= 0 )
    {
        _actionPoints -= _hand[wHand]->getTurnCost();
        //updatePossibleMoves();
    }
}
void Unit::useLeft( spMapTile tile )
{
    use( tile, LEFT );
}
void Unit::useRight( spMapTile tile )
{
    use( tile, RIGHT );
}
void Unit::swapEq( int slot1, int slot2 )
{
    // switch hands
    if ( slot1 <= 1 && slot2 <= 1 )
    {
        spItem temp = _hand[slot1];
        _hand[slot1] = _hand[slot2];
        _hand[slot2] = temp;
    }
    // inventory swap
    else if ( slot1 >= 2 && slot2 >= 2 )
    {
        spItem temp = _inventory[slot1-2];
        _inventory[slot1-2] = _inventory[slot2-2];
        _inventory[slot2-2] = temp;
    }
    // get item from inventory and/or put item in inventory
    else if ( _actionPoints >= 15 )
    {
        int s1 = max ( slot1, slot2 ), s2 = min ( slot1, slot2 );
        spItem temp = _hand[s1];
        _hand[s1] = _inventory[s2-2];
        _inventory[s2-2] = temp;
        _actionPoints -= 15;
        //updatePossibleMoves();
    }
}

SDL_Surface* Unit::getImage()
{
    //return _image;
   return NULL;
}

string Unit::getName() const
{
    return _name;
}

bool Unit::canUseNearbyObjects() const
{
   vector<const Direction*> dirs = Direction::getNonCardinalDirections();
   vector<const Direction*>::const_iterator iter;
   for(iter = dirs.begin(); iter != dirs.end(); ++iter)
   {
      spMapTile mt = _tile->getTileInDirection(**iter);
      if (mt.get() && mt->hasUseableObjects())
      {
         return true;
      }
   }
   return false;
}

void Unit::useNearbyObjects()
{
   bool usedSomething = false;
   vector<const Direction*> dirs = Direction::getNonCardinalDirections();
   vector<const Direction*>::const_iterator iter;
   for(iter = dirs.begin(); iter != dirs.end(); ++iter)
   {
      spMapTile mt = _tile->getTileInDirection(**iter);
      if (mt.get() && mt->hasUseableObjects())
      {
         mt->useObjects();
         usedSomething = true;
      }
   }
   if (usedSomething)
   {
      useActionPoints(COST_USE);
   }
}


} // namespace

