#ifndef XCLIENT_CHATBOX_H
#define XCLIENT_CHATBOX_H

#include "includes.h"
#include "xcore/ChatEvent.h"

class ChatListModel : public gcn::ListModel
{
   public:
      ChatListModel();
      virtual ~ChatListModel();

      virtual int getNumberOfElements();
      virtual string getElementAt (int i);

      void addMessage(ChatEvent& evnt);

   private:
      deque<string> _messages;
};

class ChatBox : public gcn::Container, public gcn::ActionListener
{
   public:
      ChatBox();
      virtual ~ChatBox();

      void addMessage(ChatEvent& evnt);

      virtual void action(const string& eventId, gcn::Widget* widget);

   private:
      ChatListModel* _clm;
      gcn::ListBox* _chatArea;
      gcn::TextField* _chatInput;
      gcn::ScrollArea* _scroll;
};

#endif