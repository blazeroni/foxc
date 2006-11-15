#include "includes.h"
#include "Resource.h"

Resource::Resource(string id) :
   _id(id)
{
}

Resource::~Resource()
{
}

string Resource::getID()
{
   return _id;
}

