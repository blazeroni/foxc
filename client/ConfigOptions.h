#ifndef XCLIENT_CONFIGOPTIONS_H
#define XCLIENT_CONFIGOPTIONS_H

#include "includes.h"
#include "xcore/Singleton.h"

/// List of all the possible configuration options
enum ConfigOptionName { WIDTH, 
                        HEIGHT, 
                        COLOR_DEPTH, 
                        MAP, 
                        FONT, 
                        PORT, 
                        HOSTNAME,
                        PLAYER_NAME ,
                        GAME_NAME };

class ConfigOptions;
typedef boost::shared_ptr<ConfigOptions> spConfigOptions;

/**
 * Loads and stores the initial configuration options
 *
 * These values aren't updated if they change later during
 * program execution, they are only used to lookup the starting
 * values
 */
class ConfigOptions : public Singleton<ConfigOptions>
{
   friend class Singleton<ConfigOptions>;

   public:
      void init();
      void deinit();
      /** 
       * Loads in a config file
       *
       * Format should be one option per line, 
       * set up like 
       * @f[
       * option=value
       * @f]
       */ 
      bool load(string filename);

      /// gets the specified option and returns it as the specified type
      template <typename T> 
      T get(ConfigOptionName option);

   private:
      ConfigOptions();
      void createOptionsMap();

      po::variables_map _vm;
      map<ConfigOptionName, const char*> optionsMap;
};

template <typename T>
T ConfigOptions::get(ConfigOptionName option)
{
   return _vm[optionsMap[option]].as<T>();
}

#endif
