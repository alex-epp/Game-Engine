#pragma once

#include <string>
#include <map>
#include <list>
#include <queue>
#include <vector>

#include "Types.h"

using namespace std;

namespace core
{
	class Message
	{
	public:
		enum MsgType
		{
			RESIZE_WINDOW = 0,
			KEY_CHANGE,
			WINDOW_CLOSE,
			MOUSE,
			SET_MOUSE,
			NUM_MSGS
		};

		MsgType type;
		virtual ~Message();
	protected:
		Message(MsgType);

	};

	class MouseMessage : public Message
	{
	public:
		int x, y;

		enum class Button
		{
			None = 0,
			Left = 1,
			Middle = 2,
			Right = 3
		} button;

		MouseMessage(int _x, int _y, Button b = Button::None) : x(_x), y(_y), button(b), Message(MsgType::MOUSE) {}
	};

	class SetMouseMessage : public Message
	{
	public:
		int x, y;

		SetMouseMessage(int _x, int _y) : x(_x), y(_y), Message(MsgType::SET_MOUSE) {}
	};

	class ResizeWindowMessage : public Message
	{
	public:
		ResizeWindowMessage(int w, int h);

		int width, height;
	};

	class KeyChangeMessage : public Message
	{
	public:
		KeyChangeMessage(int k, bool d);

		int key;
		bool down;
	};

	class WindowCloseMessage : public Message
	{
	public:
		WindowCloseMessage() : Message(WINDOW_CLOSE) {}
	};

	class Listener
	{
	public:
		virtual void recieveMsg(Message*) = 0;
		virtual ~Listener() = default;
	};

	class ChangeManager
	{
	private:
		ChangeManager() = default;

	public:
		static ChangeManager &get()
		{
			static ChangeManager instance;

			return instance;
		}

		void add(Listener*, list<Message::MsgType>);
		template <class Type, class... Args>
		void recieveMsg(Args&&... args);
		void distrubuteMsgs();

	private:
		map<Message::MsgType, std::vector<Listener*>> listeners;
		queue<Message*> messageQueue;
	};

	template <class Type, class... Args>
	void ChangeManager::recieveMsg(Args&&... args)
	{
		messageQueue.push(new Type(std::forward<Args>(args)...));
	}
}