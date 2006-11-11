#include "includes.h"
#include "ClientWall.h"
#include "Display.h"

SDL_Surface* ClientWall::_ne = NULL;
SDL_Surface* ClientWall::_nw = NULL;
SDL_Surface* ClientWall::_se = NULL;
SDL_Surface* ClientWall::_sw = NULL;
SDL_Surface* ClientWall::_nesw = NULL;
SDL_Surface* ClientWall::_nwse = NULL;
SDL_Surface* ClientWall::_tsw = NULL;
SDL_Surface* ClientWall::_tse = NULL;
SDL_Surface* ClientWall::_tne = NULL;
SDL_Surface* ClientWall::_tnw = NULL;
SDL_Surface* ClientWall::_le = NULL;
SDL_Surface* ClientWall::_lw = NULL;
SDL_Surface* ClientWall::_ln = NULL;
SDL_Surface* ClientWall::_ls = NULL;
SDL_Surface* ClientWall::_all = NULL;

#define WALL_IMAGE_NE    "resources/images/wall_ne.png"
#define WALL_IMAGE_NW    "resources/images/wall_nw.png"
#define WALL_IMAGE_SE    "resources/images/wall_se.png"
#define WALL_IMAGE_SW    "resources/images/wall_sw.png"
#define WALL_IMAGE_NE_SW "resources/images/wall_ne_sw.png"
#define WALL_IMAGE_NW_SE "resources/images/wall_nw_se.png"
#define WALL_IMAGE_T_SW  "resources/images/wall_t_sw.png"
#define WALL_IMAGE_T_SE  "resources/images/wall_t_se.png"
#define WALL_IMAGE_T_NE  "resources/images/wall_t_ne.png"
#define WALL_IMAGE_T_NW  "resources/images/wall_t_nw.png"
#define WALL_IMAGE_L_E   "resources/images/wall_l_e.png"
#define WALL_IMAGE_L_N   "resources/images/wall_l_n.png"
#define WALL_IMAGE_L_W   "resources/images/wall_l_w.png"
#define WALL_IMAGE_L_S   "resources/images/wall_l_s.png"
#define WALL_IMAGE_ALL   "resources/images/wall_all.png"

ClientWall::ClientWall(uint32 entityID, WALL_TYPE type) :
   Wall(entityID, type),
   _image(NULL)
{
   if (!_all)
   {
      _ne = Display::instance().loadImage(WALL_IMAGE_NE);
      _nw = Display::instance().loadImage(WALL_IMAGE_NW);
      _se = Display::instance().loadImage(WALL_IMAGE_SE);
      _sw = Display::instance().loadImage(WALL_IMAGE_SW);
      _nesw = Display::instance().loadImage(WALL_IMAGE_NE_SW);
      _nwse = Display::instance().loadImage(WALL_IMAGE_NW_SE);
      _tsw = Display::instance().loadImage(WALL_IMAGE_T_SW);
      _tse = Display::instance().loadImage(WALL_IMAGE_T_SE);
      _tne = Display::instance().loadImage(WALL_IMAGE_T_NE);
      _tnw = Display::instance().loadImage(WALL_IMAGE_T_NW);
      _le = Display::instance().loadImage(WALL_IMAGE_L_E);
      _lw = Display::instance().loadImage(WALL_IMAGE_L_W);
      _ln = Display::instance().loadImage(WALL_IMAGE_L_N);
      _ls = Display::instance().loadImage(WALL_IMAGE_L_S);
      _all = Display::instance().loadImage(WALL_IMAGE_ALL);
   }

   switch (type)
   {
      case WT_NE:
         _image = _ne;
         break;
      case WT_NW:
         _image = _nw;
         break;
      case WT_SE:
         _image = _se;
         break;
      case WT_SW:
         _image = _sw;
         break;
      case WT_NE_SW:
         _image = _nesw;
         break;
      case WT_NW_SE:
         _image = _nwse;
         break;
      case WT_T_NE:
         _image = _tne;
         break;
      case WT_T_NW:
         _image = _tnw;
         break;
      case WT_T_SE:
         _image = _tse;
         break;
      case WT_T_SW:
         _image = _tsw;
         break;
      case WT_L_E:
         _image = _le;
         break;
      case WT_L_W:
         _image = _lw;
         break;
      case WT_L_N:
         _image = _ln;
         break;
      case WT_L_S:
         _image = _ls;
         break;

      case WT_ALL:
         _image = _all;
         break;

      case WT_NONE:
      default:
         _image = _all;
         break;
   }
}

ClientWall::~ClientWall()
{

}

void ClientWall::draw(Point position, Point dimensions) const
{
   Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _image);
}
