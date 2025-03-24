#pragma once

#include "Input/InputCode.h"

#define EVENT_BIND_MEMBER_FUNCTION(x) std::bind(&x, this, std::placeholders::_1);

struct EventArg
{
	bool isHandled = false;
};

template <class T> 
class EventHandler
{
	static_assert(std::is_base_of<EventArg, T>::value, "T must inherit from EventArg!");

	using Callback = std::function<void(T&)>;
public:
	void Invoke(T& arg)
	{
		for (int i = 0; i < Callbacks.size(); i++)
		{
			Callbacks[i](arg);

			if (arg.isHandled)
				break;
		}
	}

	void RegisterCallback(Callback callbackFunction)
	{
		Callbacks.push_back(callbackFunction);
	}

	// TODO: This needs refactoring
	void RemoveCallback(Callback callbackFunction)
	{
		for (int i = 0; i < Callbacks.size(); ++i)
		{
			if (Callbacks[i] == callbackFunction)
			{
				Callbacks.erase(i);
				return;
			}
		}
	}

	void operator += (Callback callbackFunction)
	{
		RegisterCallback(callbackFunction);
	}

	void operator -= (Callback callbackFunction)
	{
		RemoveCallback(callbackFunction);
	}

private:
	std::vector<Callback> Callbacks;
};




