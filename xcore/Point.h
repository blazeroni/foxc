#ifndef XCORE_UTILITY_H
#define XCORE_UTILITY_H

namespace xcore
{

struct Point
{
   Point() : x(0), y(0) {}
   Point(int xPos, int yPos) : x(xPos), y(yPos) {}
   int x;
   int y;

   bool operator == (const Point& p) const
   {
      return (x == p.x && y == p.y);
   }

   Point operator - () const
   {
      return Point(-x, -y);
   }
};

}

#endif
