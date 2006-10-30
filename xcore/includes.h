#ifndef XCORE_INCLUDE_H
#define XCORE_INCLUDE_H

#include <limits>
#include <vector>
#include <queue>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/functional/hash.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

typedef boost::uint8_t uint8;
typedef boost::uint16_t uint16;
typedef boost::uint32_t uint32;
typedef boost::int8_t int8;
typedef boost::int16_t int16;
typedef boost::int32_t int32;

#include "tinyxml/ticpp.h"
#include "net2/net2.h"
#include "net2/SDLUtils.h"
#include "net2/fastevents.h"
#include "macros.h"
//#include "utility.h"

namespace xcore {}

using namespace std;

namespace po = boost::program_options;
namespace ar = boost::archive;

#endif
