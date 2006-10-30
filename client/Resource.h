#ifndef XCLIENT_RESOURCE_H
#define XCLIENT_RESOURCE_H

#include "includes.h"

class Resource
{
   public:
      Resource();
      virtual ~Resource();

      virtual void load(ticpp::Element* xml);

      void incRef();
      void decRef();

      int getRefCount();

   private:
      int ref;
};

#endif
