#ifndef XCLIENT_GAMELIST_H
#define XCLIENT_GAMELIST_H

#include "includes.h"
#include "xcore/GameListEvent.h"
#include "xcore/MapListEvent.h"

// These set of class simulate a table for the game lobby
// It's not general, but there's only one table in the game

class MapListModel;
typedef boost::shared_ptr<MapListModel> spMapListModel;

class GameListRow
{
   public:
      GameListRow();
      virtual ~GameListRow();

      virtual spWidget getGameNameColumn() const = 0;
      virtual spWidget getMapNameColumn() const = 0;
      virtual spWidget getPlayerColumn() const = 0;
      virtual spWidget getSizeColumn() const = 0;
      virtual spWidget getPointColumn() const = 0;
      virtual spWidget getHostJoinColumn() const = 0;
};

class GameListHeaderRow : public GameListRow
{
   public:
      GameListHeaderRow();
      virtual spWidget getGameNameColumn() const   { return _gameName; }
      virtual spWidget getMapNameColumn() const    { return _mapName; }
      virtual spWidget getPlayerColumn() const     { return _players; }
      virtual spWidget getSizeColumn() const       { return _size; }
      virtual spWidget getPointColumn() const      { return _points; }
      virtual spWidget getHostJoinColumn() const   { return _hostJoin; }

   private:
      // these are buttons b/c guichan doesn't let me change the background color of labels
      spButton _gameName;
      spButton _mapName;
      spButton _players;
      spButton _size;
      spButton _points;
      spButton _hostJoin;
};

class GameListDataRow : public GameListRow, public gcn::ActionListener
{
   public:
      GameListDataRow(string gameName, string mapName, int currentPlayers, 
                      int width, int height, int maxPlayers, int points);

      virtual spWidget getGameNameColumn() const   { return _gameName; }
      virtual spWidget getMapNameColumn() const    { return _mapName; }
      virtual spWidget getPlayerColumn() const     { return _players; }
      virtual spWidget getSizeColumn() const       { return _size; }
      virtual spWidget getPointColumn() const      { return _points; }
      virtual spWidget getHostJoinColumn() const   { return _join; }

      virtual void action(const string& eventId, gcn::Widget* widget);

   private:
      spLabel _gameName;
      spLabel _mapName;
      spLabel _players;
      spLabel _size;
      spLabel _points;
      spButton _join;
};


class GameListEmptyRow : public GameListRow, public gcn::ActionListener
{
   public:
      GameListEmptyRow(spMapListModel mlm);
      virtual spWidget getGameNameColumn() const   { return _gameName; }
      virtual spWidget getMapNameColumn() const    { return _mapList; }
      virtual spWidget getPlayerColumn() const     { return _players; }
      virtual spWidget getSizeColumn() const       { return _size; }
      virtual spWidget getPointColumn() const      { return _points; }
      virtual spWidget getHostJoinColumn() const   { return _host; }

      void setMapModel(spMapListModel mlm);

      virtual void action(const string& eventId, gcn::Widget* widget);

   private:
      gcn::Color _buttonColor;
      spTextField _gameName;
      spDropDown _mapList;
      spLabel _players;
      spLabel _size;
      spLabel _points;
      spButton _host;

      spMapListModel _mlm;
};

class GameListBlankRow : public GameListRow
{
   public:
      GameListBlankRow();
      virtual spWidget getGameNameColumn() const   { return _gameName; }
      virtual spWidget getMapNameColumn() const    { return _mapName; }
      virtual spWidget getPlayerColumn() const     { return _players; }
      virtual spWidget getSizeColumn() const       { return _size; }
      virtual spWidget getPointColumn() const      { return _points; }
      virtual spWidget getHostJoinColumn() const   { return _host; }

   private:
      spLabel _gameName;
      spLabel _mapName;
      spLabel _players;
      spLabel _size;
      spLabel _points;
      spLabel _host;
};

typedef boost::shared_ptr<GameListRow> spGameListRow;
typedef boost::shared_ptr<GameListEmptyRow> spGameListEmptyRow;

class GameList;
typedef boost::shared_ptr<GameList> spGameList;

class GameList
{
   public:
      GameList();
      virtual ~GameList();

      void setMaps(vector<MapListItem> maps);

      void update(vector<GameListItem> games);

      void draw();
      void setPosition(int x, int y);

      void addRow(GameListItem item);

      spWidget getGUI();

      static const int rowHeight;
      static const int mapNameWidth;
      static const int gameNameWidth;
      static const int playerWidth;
      static const int sizeWidth;
      static const int pointsWidth;
      static const int hostJoinWidth;

      static const int borderSize;

   protected:

   private:
      void rebuild();
      void addRowToContainer(spGameListRow row, int num);

      static const int glWidth;
      static const int glHeight;

      // temp
      //spGameListRow _test;
      //spGameListRow _test2;


      spGameListRow _header;
      spGameListEmptyRow _empty;
      vector<spGameListRow> _rows;
      vector<spGameListRow> _blankRows;
      spScrollArea _scroll;
      spContainer _cntr;
      
      spMapListModel _mlm;

};

class MapListModel : public gcn::ListModel
{
   public:
      MapListModel(vector<MapListItem> maps);
      virtual ~MapListModel();

      virtual int getNumberOfElements();
      virtual string getElementAt (int i);

      const MapListItem& getMap(string mapName);
      const MapListItem& getMap(int i);

   private:
      vector<MapListItem> _maps;
};

#endif