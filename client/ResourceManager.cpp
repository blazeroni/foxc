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
         Resource* r = createResource(child.Get());
      }
   }
   catch (ticpp::Exception&) {
      throw;
   }
}

Resource* ResourceManager::createResource(ticpp::Element* element) 
{
   string type = element->Value();
   Resource* res = NULL;

   if (type == "sprite")
   {
      res = new Sprite();
   }

   if (res != NULL) 
   {
      res->load(element);
   }

   return res;
}

