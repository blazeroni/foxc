#include "includes.h"
#include "ResourceManager.h"
#include "Sprite.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::load(string filename) 
{
   try
   {
      ticpp::Document doc(filename);
      doc.LoadFile();

      ticpp::Element* root = doc.FirstChildElement("resources");

      ticpp::Iterator< ticpp::Element > child;
      for ( child = root->FirstChildElement(); child != child.end(); child++ )
      {
         createResource(child.Get());
      }
   }
   catch (ticpp::Exception&) {
      throw;
   }
}

void ResourceManager::unloadAll()
{
   _sprites.clear();
}

void ResourceManager::createResource(ticpp::Element* element) 
{
   string type = element->Value();
   spResource res;

   string id;

   element->GetAttribute("id", &id);

   if (type == "sprite")
   {
      _sprites[id] = spSprite(new Sprite(id));
      res = _sprites[id];
   }

   if (res != NULL) 
   {
      res->load(element);
   }
}

spLightweightSprite ResourceManager::getSprite(string id)
{
   spLightweightSprite lite = spLightweightSprite(new LightweightSprite(_sprites[id]));
   return lite;
}

