#include "includes.h"
#include "GameList.h"
#include "ClientNetwork.h"
#include "xcore/GameHostEvent.h"
#include "xcore/GameJoinEvent.h"

const int GameList::rowHeight       = 20;
const int GameList::mapNameWidth    = 200;
const int GameList::gameNameWidth   = 250;
const int GameList::playerWidth     = 75;
const int GameList::sizeWidth       = 75;
const int GameList::pointsWidth     = 75;
const int GameList::hostJoinWidth   = 118;

const int GameList::glWidth         = 800;
const int GameList::glHeight        = 300;
const int GameList::borderSize      = 1;

GameListRow::GameListRow()
{

}

GameListRow::~GameListRow()
{
}

GameListHeaderRow::GameListHeaderRow()
{
   gcn::Color headerColor = gcn::Color(150, 20, 20);
   _gameName = spButton(new gcn::Button("Game"));
   _gameName->setSize(GameList::gameNameWidth, GameList::rowHeight - 2);
   _gameName->setBorderSize(GameList::borderSize);
   _gameName->setAlignment(gcn::Graphics::CENTER);
   _gameName->setEnabled(false);
   _gameName->setBaseColor(headerColor);
   _mapName = spButton(new gcn::Button("Map"));;
   _mapName->setSize(GameList::mapNameWidth, GameList::rowHeight - 2);
   _mapName->setBorderSize(GameList::borderSize);
   _mapName->setAlignment(gcn::Graphics::CENTER);
   _mapName->setBaseColor(headerColor);
   _mapName->setEnabled(false);
   _players = spButton(new gcn::Button("Players"));
   _players->setSize(GameList::playerWidth, GameList::rowHeight - 2);
   _players->setBorderSize(GameList::borderSize);
   _players->setAlignment(gcn::Graphics::CENTER);
   _players->setBaseColor(headerColor);
   _players->setEnabled(false);
   _size = spButton(new gcn::Button("Size"));
   _size->setSize(GameList::playerWidth, GameList::rowHeight - 2);
   _size->setBorderSize(GameList::borderSize);
   _size->setAlignment(gcn::Graphics::CENTER);
   _size->setBaseColor(headerColor);
   _size->setEnabled(false);
   _points = spButton(new gcn::Button("Points"));
   _points->setSize(GameList::pointsWidth, GameList::rowHeight - 2);
   _points->setBorderSize(GameList::borderSize);
   _points->setAlignment(gcn::Graphics::CENTER);
   _points->setBaseColor(headerColor);
   _points->setEnabled(false);
   _hostJoin = spButton(new gcn::Button("Host/Join"));
   _hostJoin->setSize(GameList::hostJoinWidth, GameList::rowHeight - 2);
   _hostJoin->setBorderSize(GameList::borderSize);
   _hostJoin->setAlignment(gcn::Graphics::CENTER);
   _hostJoin->setBaseColor(headerColor);
   _hostJoin->setEnabled(false);
}

GameListEmptyRow::GameListEmptyRow(spMapListModel mlm)
{
   _buttonColor = gcn::Color(20, 20, 180);
   _gameName = spTextField(new gcn::TextField("Game"));
   _gameName->setSize(GameList::gameNameWidth, GameList::rowHeight);
   _gameName->setBorderSize(GameList::borderSize);
   _gameName->setBackgroundColor(gcn::Color(119,119,119));

   _mlm = mlm;

   _mapList = spDropDown(new gcn::DropDown(mlm.get()));
   _mapList->setSize(GameList::mapNameWidth, GameList::rowHeight);
   _mapList->setBorderSize(GameList::borderSize);
   _mapList->setBackgroundColor(gcn::Color(119,119,119));
   _mapList->addActionListener(this);

   _players = spLabel(new gcn::Label("-/-"));
   _players->setSize(GameList::playerWidth, GameList::rowHeight);
   _players->setBorderSize(GameList::borderSize);
   _players->setAlignment(gcn::Graphics::CENTER);

   _size = spLabel(new gcn::Label("---"));
   _size->setSize(GameList::pointsWidth, GameList::rowHeight);
   _size->setBorderSize(GameList::borderSize);
   _size->setAlignment(gcn::Graphics::CENTER);

   _points = spLabel(new gcn::Label("---"));
   _points->setSize(GameList::pointsWidth, GameList::rowHeight);
   _points->setBorderSize(GameList::borderSize);
   _points->setAlignment(gcn::Graphics::CENTER);

   _host = spButton(new gcn::Button("Host"));
   _host->setSize(GameList::hostJoinWidth, GameList::rowHeight - 2);
   _host->setBorderSize(GameList::borderSize);
   _host->setAlignment(gcn::Graphics::CENTER);
   _host->setBaseColor(_buttonColor);
   _host->addActionListener(this);
}

void GameListEmptyRow::action(const string& eventId, gcn::Widget* widget) 
{
   if (widget == _mapList.get())
   {
      stringstream ss;
      MapListItem mli = _mlm->getMap(_mapList->getSelected());

      if (mli.points == 0)
      {
         ss << "---";
      }
      else
      {
         ss << mli.points;
      }
      _points->setCaption(ss.str());

      ss.str("");
      if (mli.maxPlayers == 0)
      {
         ss << "-/-";
      }
      else
      {
         ss << "-/" << mli.maxPlayers;
      }
      _players->setCaption(ss.str());

      ss.str("");
      if (mli.height == 0 || mli.width == 0)
      {
         ss << "---";
      }
      else
      {
         ss << mli.height << "x" << mli.width;
      }
      _size->setCaption(ss.str());
   }
   else if (widget == _host.get())
   {
      if (_gameName->getText() != "" && _mlm->getElementAt(_mapList->getSelected()) != "")
      {
        ClientNetwork::instance().send(GameHostEvent(_gameName->getText(), _mlm->getElementAt(_mapList->getSelected())));
      }
   }
}

void GameListEmptyRow::setMapModel(spMapListModel mlm)
{
   _mlm = mlm;
   _mapList->setListModel(mlm.get());
}

GameListBlankRow::GameListBlankRow()
{
   _gameName = spLabel(new gcn::Label(""));
   _gameName->setSize(GameList::gameNameWidth, GameList::rowHeight);
   _gameName->setBorderSize(GameList::borderSize);
   _mapName = spLabel(new gcn::Label(""));
   _mapName->setSize(GameList::mapNameWidth, GameList::rowHeight);
   _mapName->setBorderSize(GameList::borderSize);
   _players = spLabel(new gcn::Label(""));
   _players->setSize(GameList::playerWidth, GameList::rowHeight);
   _players->setBorderSize(GameList::borderSize);
   _size = spLabel(new gcn::Label(""));
   _size->setSize(GameList::pointsWidth, GameList::rowHeight);
   _size->setBorderSize(GameList::borderSize);
   _points = spLabel(new gcn::Label(""));
   _points->setSize(GameList::pointsWidth, GameList::rowHeight);
   _points->setBorderSize(GameList::borderSize);
   _host = spLabel(new gcn::Label(""));
   _host->setSize(GameList::hostJoinWidth, GameList::rowHeight);
   _host->setBorderSize(GameList::borderSize);
}

GameListDataRow::GameListDataRow(string gameName, string mapName, 
                                 int currentPlayers, int width, int height, int maxPlayers, int points)
{
   stringstream ss;

   _gameName = spLabel(new gcn::Label(gameName));
   _gameName->setSize(GameList::gameNameWidth, GameList::rowHeight);
   _gameName->setBorderSize(GameList::borderSize);
   //_gameName->setAlignment(gcn::Graphics::CENTER);
   _mapName = spLabel(new gcn::Label(mapName));
   _mapName->setSize(GameList::mapNameWidth, GameList::rowHeight);
   _mapName->setBorderSize(GameList::borderSize);
   //_mapName->setAlignment(gcn::Graphics::CENTER);

   ss << currentPlayers << "/" << maxPlayers;
   _players = spLabel(new gcn::Label(ss.str()));
   _players->setSize(GameList::playerWidth, GameList::rowHeight);
   _players->setBorderSize(GameList::borderSize);
   _players->setAlignment(gcn::Graphics::CENTER);

   ss.str("");
   ss << width << "x" << height;
   _size = spLabel(new gcn::Label(ss.str()));
   _size->setSize(GameList::pointsWidth, GameList::rowHeight);
   _size->setBorderSize(GameList::borderSize);
   _size->setAlignment(gcn::Graphics::CENTER);

   ss.str("");
   ss << points;
   _points = spLabel(new gcn::Label(ss.str()));
   _points->setSize(GameList::pointsWidth, GameList::rowHeight);
   _points->setBorderSize(GameList::borderSize);
   _points->setAlignment(gcn::Graphics::CENTER);

   _join = spButton(new gcn::Button("Join"));
   _join->setSize(GameList::hostJoinWidth, GameList::rowHeight-2);
   _join->setBorderSize(GameList::borderSize);
   _join->setAlignment(gcn::Graphics::CENTER);
   _join->setBaseColor(gcn::Color(20,20,180));
   _join->addActionListener(this);
}

void GameListDataRow::action(const string& eventId, gcn::Widget* widget) 
{
   if (widget == _join.get())
   {
      ClientNetwork::instance().send(GameJoinEvent(_gameName->getCaption()));
   }
}


GameList::GameList()
{
   _header = spGameListRow(new GameListHeaderRow());

   vector<MapListItem> m;
   m.push_back(MapListItem());
   _mlm = spMapListModel(new MapListModel(m));
   _empty = spGameListEmptyRow(new GameListEmptyRow(_mlm));

   rebuild();
}

GameList::~GameList()
{
}

void GameList::draw()
{
   
}

void GameList::setMaps(vector<MapListItem> maps)
{
   vector<MapListItem> m;
   MapListItem dummy;
   dummy.height = 0;
   dummy.width = 0;
   dummy.points = 0;
   dummy.mapName = "";
   dummy.maxPlayers = 0;
   m.push_back(dummy);
   vector<MapListItem>::iterator iter;
   for (iter = maps.begin(); iter != maps.end(); ++iter)
   {
      m.push_back(*iter);
   }
   _mlm = spMapListModel(new MapListModel(m));
   _empty->setMapModel(_mlm);
   //_empty = spGameListRow(new GameListEmptyRow(_mlm));
   //addRowToContainer(_empty, _rows.size()+1);
}

void GameList::update(vector<GameListItem> games)
{
   vector<GameListItem>::iterator iter;
   _rows.clear();

   for (iter = games.begin(); iter != games.end(); ++iter)
   {
      MapListItem mli = _mlm->getMap(iter->mapName);
      _rows.push_back(spGameListRow(new GameListDataRow(iter->gameName, iter->mapName, iter->currentPlayers,
         iter->width, iter->height, iter->maxPlayers, iter->points)));
   }
   rebuild();
}

void GameList::rebuild()
{
   _cntr = spContainer(new gcn::Container());
   int visibleRows = glHeight / rowHeight;
   int size = max<int>(_rows.size() + 2, visibleRows);  // +2 for the header and the empty row
   _cntr->setDimension(gcn::Rectangle(0, 0, 800, size * rowHeight));
   _cntr->setBaseColor(gcn::Color(42,42,42));

   int row = 0;

   addRowToContainer(_header, row++);

   vector<spGameListRow>::iterator iter;

   for (iter = _rows.begin(); iter != _rows.end(); ++iter)
   {
      addRowToContainer((*iter), row++);
   }

   addRowToContainer(_empty, row++);

   _blankRows.clear();

   int tmp = row;
   while (tmp < visibleRows)
   {
      _blankRows.push_back(spGameListRow(new GameListBlankRow()));
      tmp++;
   }

   for (iter = _blankRows.begin(); iter != _blankRows.end(); ++iter)
   {
      addRowToContainer((*iter), row++);
   }

   _scroll = spScrollArea(new gcn::ScrollArea(_cntr.get(), gcn::ScrollArea::ALWAYS, gcn::ScrollArea::NEVER));
   _scroll->setSize(glWidth, glHeight);
   _scroll->setBorderSize(2);
}

void GameList::addRowToContainer(spGameListRow row, int num)
{
   int y = num * rowHeight;
   int columnWidth = 0;
   
   _cntr->add(row->getGameNameColumn().get(), columnWidth, y);
   columnWidth += gameNameWidth;
   _cntr->add(row->getMapNameColumn().get(), columnWidth, y);
   columnWidth += mapNameWidth;
   _cntr->add(row->getPlayerColumn().get(), columnWidth, y);
   columnWidth += playerWidth;
   _cntr->add(row->getSizeColumn().get(), columnWidth, y);
   columnWidth += sizeWidth;
   _cntr->add(row->getPointColumn().get(), columnWidth, y);
   columnWidth += pointsWidth + 0;
   _cntr->add(row->getHostJoinColumn().get(), columnWidth, y); 
}

spWidget GameList::getGUI()
{
   return _scroll;
}


MapListModel::MapListModel(vector<MapListItem> maps) :
   _maps(maps)
{

}

MapListModel::~MapListModel()
{
}

int MapListModel::getNumberOfElements()
{
   return _maps.size();
}

string MapListModel::getElementAt (int i)
{
   return _maps[i].mapName;
}

const MapListItem& MapListModel::getMap(string mapName)
{
   vector<MapListItem>::iterator iter;
   for (iter = _maps.begin(); iter != _maps.end(); ++iter)
   {
      if (iter->mapName == mapName)
      {
         return *iter;
      }
   }
   return *(_maps.begin());
}

const MapListItem& MapListModel::getMap(int i)
{
   return _maps[i];
}
