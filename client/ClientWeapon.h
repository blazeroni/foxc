#ifndef XCLIENT_CLIENTWEAPON_H
#define XCLINET_CLIENTWEAPON_H

#include "includes.h"
#include "ClientItem.h"

// no need for a specific ClientWeapon or ClientClip class b/c they don't have images

class ClientPistol : public ClientItem
{
   public:
      ClientPistol(uint32 entityID);
};

class ClientPistolClip : public ClientItem
{
   public:
      ClientPistolClip(uint32 entityID);
};

class ClientRifle : public ClientItem
{
   public:
      ClientRifle(uint32 entityID);
};
class ClientRPG : public ClientItem
{
   public:
      ClientRPG(uint32 entityID);
};
class ClientRocket : public ClientItem
{
   public:
      ClientRocket(uint32 entityID);
};

/*
class ClientBazooka : public ClientItem
{
    public:
        ClientBazooka(uint32 entityID);
};
*/

class ClientGrenade : public ClientItem
{
   public:
      ClientGrenade(uint32 entityID);
};

class ClientMedKit : public ClientItem
{
   public:
      ClientMedKit(uint32 entityID);
};

#endif

