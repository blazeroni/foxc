#include "includes.h"
#include "Direction.h"

namespace xcore
{

const Direction Direction::N(-1, -1, Direction::S, Direction::NE, Direction::NW, true);
const Direction Direction::NE(-1, 0, Direction::SW, Direction::E, Direction::N, false);
const Direction Direction::E(-1, 1, Direction::W, Direction::SE, Direction::NE, true);
const Direction Direction::SE(0, 1, Direction::NW, Direction::S, Direction::E, false);
const Direction Direction::S(1, 1, Direction::N, Direction::SW, Direction::SE, true);
const Direction Direction::SW(1, 0, Direction::NE, Direction::W, Direction::S, false);
const Direction Direction::W(1, -1, Direction::E, Direction::NW, Direction::SW, true);
const Direction Direction::NW(0, -1, Direction::SE, Direction::N, Direction::W, false);

Direction::Direction(int x, 
                     int y, 
                     const Direction& opposite, 
                     const Direction& clockwise, 
                     const Direction& counterClockwise, 
                     bool cardinal) :
   _offset(x, y),
   _opposite(&opposite),
   _clockwise(&clockwise),
   _counterClockwise(&counterClockwise),
   _cardinal(cardinal)
{

}


Direction::~Direction()
{
}

vector<const Direction*> Direction::getAllDirections()
{
   static vector<const Direction*> directions;
   if (directions.empty())
   {
      directions.push_back(&N);
      directions.push_back(&NE);
      directions.push_back(&E);
      directions.push_back(&SE);
      directions.push_back(&S);
      directions.push_back(&SW);
      directions.push_back(&W);
      directions.push_back(&NW);
   }

   return directions;
}

const Point& Direction::offset() const
{
   return _offset;
}

const Direction& Direction::opposite() const
{
   return *_opposite;
}

const Direction& Direction::clockwise() const
{
   return *_clockwise;
}

const Direction& Direction::counterClockwise() const
{
   return *_counterClockwise;
}

bool Direction::isCardinal() const
{
   return _cardinal;
}

bool Direction::operator < (const Direction& dir) const
{
   if (offset().x < dir.offset().x)
   {
      return true;
   }
   else if (offset().x > dir.offset().x)
   {
      return false;
   }
   else
   {
      return (offset().y < dir.offset().y);
   }
}

bool Direction::operator == (const Direction& dir) const
{
   return (_offset == dir._offset);
}

} // namespace
