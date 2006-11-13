#ifndef XCLIENT_PLAYERLISTMODEL_H
#define XCLIENT_PLAYERLISTMODEL_H

#include "includes.h"

class PlayerListModel : public gcn::ListModel
{
   public:
      PlayerListModel();
      virtual ~PlayerListModel();

      virtual int getNumberOfElements();
      virtual string getElementAt (int i);

      string getPlayerByID(uint32 id);
      void addPlayer(uint32 id, string name);
      void removePlayer(string name);
      void removePlayer(uint32 id);

   private:
      vector<string> _players;
      map<string, uint32> _playerIDs;
};

#endif