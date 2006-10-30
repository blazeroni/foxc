#ifndef XCLIENT_RESOURCEMANAGER_H
#define XCLIENT_RESOURCEMANAGER_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "Resource.h"

class ResourceManager : public Singleton<ResourceManager>
{
   friend class Singleton<ResourceManager>;

   public:
      ~ResourceManager();
      void load(string filename);
      void unload(string filename);
      void unloadAll();

      template <class T>
      T* get(string resource);

   private: 
      ResourceManager();
      Resource* createResource(ticpp::Element* element);


};

#endif
