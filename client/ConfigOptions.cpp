#include "includes.h"
#include "ConfigOptions.h"

ConfigOptions::ConfigOptions()
{

}

void ConfigOptions::init()
{
   createOptionsMap();
}

void ConfigOptions::deinit()
{

}

void ConfigOptions::createOptionsMap() 
{
   optionsMap[WIDTH] = "width";
   optionsMap[HEIGHT] = "height";
   optionsMap[COLOR_DEPTH] = "colorDepth";
   optionsMap[FONT] = "font";
   optionsMap[MAP] = "map";
   optionsMap[PORT] = "port";
   optionsMap[HOSTNAME] = "hostname";
   optionsMap[PLAYER_NAME] = "playerName";
   optionsMap[GAME_NAME] = "gameName";
}

bool ConfigOptions::load(string filename)
{
   ifstream file(filename.c_str());
   po::options_description desc("");
   desc.add_options()
      (optionsMap[WIDTH], po::value<int>()->default_value(800))
      (optionsMap[HEIGHT], po::value<int>()->default_value(600))
      (optionsMap[COLOR_DEPTH], po::value<int>()->default_value(16))
      (optionsMap[MAP], po::value<string>())
      (optionsMap[FONT], po::value<string>())
      (optionsMap[PORT], po::value<int>()->default_value(57575))
      (optionsMap[HOSTNAME], po::value<string>()->default_value("localhost"))
      (optionsMap[PLAYER_NAME], po::value<string>()->default_value("Player"))
      (optionsMap[GAME_NAME], po::value<string>()->default_value("Default Game"));

   po::store(po::parse_config_file(file, desc), _vm);
   notify(_vm);

   return true;
}

