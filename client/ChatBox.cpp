#include "includes.h"
#include "ChatBox.h"
#include "xcore/ChatEvent.h"
#include "ClientNetwork.h"

#define CHAT_HISTORY_LENGTH 13

ChatListModel::ChatListModel()
{
   for (int i = 0; i < CHAT_HISTORY_LENGTH; i++)
   {
      _messages.push_back("");
   }
}

ChatListModel::~ChatListModel()
{
}

int ChatListModel::getNumberOfElements()
{
   return _messages.size();
}

string ChatListModel::getElementAt (int i)
{
   return _messages[i];
}

void ChatListModel::addMessage(ChatEvent& evnt)
{
   if (_messages.size() >= CHAT_HISTORY_LENGTH)
   {
      _messages.pop_front();
   }
   string message = evnt.getPlayerName() + ": " + evnt.getMessage();
   _messages.push_back(message);
}

ChatBox::ChatBox() :
   gcn::Container()
{
   setSize(450, 220);

   _chatArea = new gcn::ListBox();
   _clm = new ChatListModel();
   _chatArea->setListModel(_clm);
   _chatArea->setSize(450, 20);
   _chatArea->setBackgroundColor(gcn::Color(0,0,0));
   
   _chatInput = new gcn::TextField();
   _chatInput->addActionListener(this);
   _chatInput->setSize(450, 20);
   _chatInput->setBackgroundColor(gcn::Color(0,0,0));

   _scroll = new gcn::ScrollArea(_chatArea, gcn::ScrollArea::SHOW_NEVER, gcn::ScrollArea::SHOW_AUTO);
   _scroll->setBackgroundColor(gcn::Color(0,0,0));
   _scroll->setSize(450, 200);
   setBorderSize(2);

   add(_scroll, 0, 0);

   _scroll->setVerticalScrollAmount(_scroll->getVerticalMaxScroll());

   add(_chatInput, 0, 200);
}

ChatBox::~ChatBox()
{
   SAFE_DELETE(_clm);
   SAFE_DELETE(_chatArea);
   SAFE_DELETE(_chatInput);
   SAFE_DELETE(_scroll);
}

void ChatBox::addMessage(ChatEvent& evnt)
{
   _clm->addMessage(evnt);
   cout << _scroll->getVerticalMaxScroll() << endl;
   _scroll->setVerticalScrollAmount(_scroll->getVerticalMaxScroll());
}

void ChatBox::action(const string& eventId, gcn::Widget* widget)
{
   if (_chatInput->getText() != "")
   {
      ClientNetwork::instance().send(ChatEvent(_chatInput->getText()));
   }
   _chatInput->setText("");
}