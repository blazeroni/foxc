#include "includes.h"
#include "Camera.h"
#include "Display.h"
#include "Input.h"

const int Camera::_edgeDistance = 25;
const float Camera::_scrollRatio = 0.5f;
const float Camera::_marginRatio = 0.5f;

Camera::Camera() :
   _position(0,0)
{

}

Camera::Camera(int x, int y) :
    _position(-x, -y)
{

}

Camera::~Camera()
{
}

void Camera::setBorders( int left, int right, int top, int bottom )
{
   Display& d = Display::instance();
   Point screenMargin;
   screenMargin.x = static_cast<int>(static_cast<double>(d.getScreenWidth()) * _marginRatio);
   screenMargin.y = static_cast<int>(static_cast<double>(d.getScreenHeight()) * _marginRatio);

    _leftBorder = left - screenMargin.x;
    _rightBorder = max ((right+screenMargin.x), (_leftBorder+d.getScreenWidth()));
    _topBorder = top - screenMargin.y;
    _bottomBorder = max ((bottom+screenMargin.y), (_topBorder+d.getScreenHeight()));
    _position.x = _leftBorder;
    _position.y = _topBorder;
}

void Camera::update(uint32 deltaTime)
{
   int distance = static_cast<int>(deltaTime * _scrollRatio);

   int width = Display::instance().getScreenWidth();
   int height = Display::instance().getScreenHeight();
   Point p = Input::instance().getMousePosition();
   if (p.x > width - _edgeDistance) {
      _position.x += distance;
   }
   else if (p.x < _edgeDistance) {
      _position.x += -distance;
   }
   if (p.y > height - _edgeDistance) {
      _position.y += distance;
   }
   else if (p.y < _edgeDistance) {
      _position.y += -distance;
   }

   // Enforce map boundaries on camera
   if ( _position.x < _leftBorder )
        _position.x = _leftBorder;
   else if ( _position.x + Display::instance().getScreenWidth() > _rightBorder )
        _position.x = _rightBorder - Display::instance().getScreenWidth();
   if ( _position.y < _topBorder )
        _position.y = _topBorder;
   else if ( _position.y + Display::instance().getScreenHeight() > _bottomBorder )
        _position.y = _bottomBorder - Display::instance().getScreenHeight();
}

const Point& Camera::getPosition() const
{
   return _position;
}

void Camera::setPosition(int x, int y)
{
   _position = Point(x,y);
}
