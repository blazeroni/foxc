#include "includes.h"
#include "PlayerListModel.h"

PlayerListModel::PlayerListModel()
{
}

PlayerListModel::~PlayerListModel()
{
}

int PlayerListModel::getNumberOfElements()
{
   return _players.size();
}

string PlayerListModel::getElementAt (int i)
{
   return _players[i];
}

void PlayerListModel::addPlayer(uint32 id, string name)
{
   _players.push_back(name);
   sort(_players.begin(), _players.end());

   _playerIDs[name] = id;
}

void PlayerListModel::removePlayer(string name)
{
   _players.erase(find(_players.begin(), _players.end(), name));
   //_players.erase(_players.find(name));
   _playerIDs.erase(name);
   sort(_players.begin(), _players.end());
}

//string PlayerListModel::getPlayerByID(uint32 id)
//{
//   return _playerIDs.find(id);
//}
