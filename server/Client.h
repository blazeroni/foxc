#ifndef XSERVER_CLIENT_H
#define XSERVER_CLIENT_H

#include "includes.h"
#include "ServerNetwork.h"

class Client;
typedef boost::shared_ptr<Client> spClient;

class Client
{
   public:
      Client(uint32 id);
      ~Client();

      void setPlayerName(string name);
      string getPlayerName();

      uint32 getPlayerID() const;

      uint16 getPlayerNumber() const;
      void setPlayerNumber(uint16 num);

      void setGameID(uint32 gameID);
      uint32 getGameID() const;

      template <class T>
      void send(const T &e);
      
   private:
      string _playerName;
      uint32 _gameID;
      uint16 _playerNum;

      // _socket is used as the clients playerID
      uint32 _socket;
};


// template functions have to be defined in the header =(
template <class T>
void Client::send(const T& e)
{
   ServerNetwork::instance().send(e, _socket);
}

#endif
