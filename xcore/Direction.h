#ifndef XCORE_DIRECTION_H
#define XCORE_DIRECTION_H

#include "includes.h"
#include "Point.h"

namespace xcore
{

/**
 * Enum class to represent directions
 *
 * Represents all 8 possible directions which exist on the map:
 * N, NE, E, SE, S, SW, W, NW.  Each one is represented by a separate
 * static object and contains information specific to that direction.
 * Intended to be a replacement for trying to do x,y calculations since
 * it's simpler and less error-prone to think in terms of Directions
 */
class Direction
{
   public:
      ~Direction();

      static const Direction N;
      static const Direction NE;
      static const Direction E;
      static const Direction SE;
      static const Direction S;
      static const Direction SW;
      static const Direction W;
      static const Direction NW;

      /// x,y information 
      const Point& offset() const;
      /// Direction in the opposite direction, i.e. south for a north direction
      const Direction& opposite() const;
      /// Direction that is clockwise on a traditional map legend.
      const Direction& clockwise() const;
      /// Direction that is counter-clockwise on a traditional map legend
      const Direction& counterClockwise() const;
      /// Whether or not this is one of the cardinal directions, i.e. N, S, E, or W
      bool isCardinal() const;

      /// Gets all the existing static objects
      static vector<const Direction*> getAllDirections();

      static vector<const Direction*> getCardinalDirections();
      static vector<const Direction*> getNonCardinalDirections();

      /// Used primarily for sorting in maps and other containers that may need it.  Not normally used otherwise.
      bool operator<(const Direction& dir) const;
      bool operator==(const Direction& dir) const;

   private:
      /// Private constructor since only 8 instances (and copies of them) should ever exist
      Direction(int x, int y, const Direction& opp, 
                const Direction& cw, const Direction& ccw, bool cardinal);

      Point _offset;
      const Direction* const _opposite;
      const Direction* const _clockwise;
      const Direction* const _counterClockwise;
      bool _cardinal;
};

} // namespace

#endif
