#ifndef XCLIENT_RESOURCE_H
#define XCLIENT_RESOURCE_H

#include "includes.h"

class Resource;
typedef boost::shared_ptr<Resource> spResource;

class Resource
{
   public:
      Resource(string id);
      virtual ~Resource();

      virtual void load(ticpp::Element* xml) = 0;

      string getID();

   private:
      string _id;
};

#endif
