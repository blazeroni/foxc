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
   optionsMap[PORT] = "port";
   optionsMap[MAP] = "map";
}

bool ConfigOptions::load(string filename)
{
   ifstream file(filename.c_str());
   po::options_description desc("");
   desc.add_options()
      (optionsMap[PORT], po::value<int>()->default_value(57575))
      (optionsMap[MAP], po::value<string>()->default_value("test.xcm"));

   po::store(po::parse_config_file(file, desc), _vm);
   notify(_vm);

   return true;
}

