#ifndef XCLIENT_RESOURCEMANAGER_H
#define XCLIENT_RESOURCEMANAGER_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "Resource.h"
#include "Sprite.h"

class ResourceManager : public Singleton<ResourceManager>
{
   friend class Singleton<ResourceManager>;

   public:
      ~ResourceManager();
      void load(string filename);
      void unload(string filename);
      void unloadAll();

      spSprite getSprite(string resource);

   private: 
      ResourceManager();
      void createResource(ticpp::Element* element);

      map<string, spSprite> _sprites;
};

#endif
