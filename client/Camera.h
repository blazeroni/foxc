#ifndef XCLIENT_CAMERA_H
#define XCLIENT_CAMERA_H

#include "includes.h"
#include "xcore/Point.h"

class Camera
{
   public:
      Camera();
      Camera(int x, int y);
      ~Camera();

      void update(uint32 deltaTime);
      const Point& getPosition() const;
      void setPosition(int x, int y);
      void setBorders( int left, int right, int top, int bottom );

   private:
      int _leftBorder, _rightBorder, _topBorder, _bottomBorder;
      Point _position;
      static const int _edgeDistance;
      static const float _scrollRatio;
      static const float _marginRatio;
};

#endif
