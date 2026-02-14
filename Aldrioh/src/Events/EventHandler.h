#pragma once

#include "Input/InputCode.h"
#include <Core/Platform.h>

#define EVENT_BIND_MEMBER_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

struct EventArg
{
	bool isHandled = false;
};

template <typename T>
class EventHandler;

template <typename T>
class EventCallbackID
{
public:
	EventCallbackID() = default;
	EventCallbackID(EventHandler<T>::key_t id, EventHandler<T>* eventHandler) : id(id), eventHandler(eventHandler), refForChecking(eventHandler->ref) {}
	~EventCallbackID();
	EventCallbackID(const EventCallbackID& other) = delete;
	EventCallbackID& operator=(const EventCallbackID& other) = delete;

	EventCallbackID(EventCallbackID&& other) noexcept
	{

		this->id = other.id;
		this->eventHandler = other.eventHandler;
		this->refForChecking = other.refForChecking;
		other.eventHandler = nullptr;
		other.id = std::numeric_limits<EventHandler<T>::key_t>::max();
	}
	EventCallbackID& operator=(EventCallbackID&& other) noexcept
	{
		this->id = other.id;
		this->eventHandler = other.eventHandler;
		this->refForChecking = other.refForChecking;
		other.eventHandler = nullptr;
		other.id = std::numeric_limits<EventHandler<T>::key_t>::max();
		return *this;
	}

	EventHandler<T>::key_t id = std::numeric_limits<EventHandler<T>::key_t>::max()-1;
	EventHandler<T>* eventHandler = nullptr;
	std::weak_ptr<int> refForChecking;

	friend EventHandler<T>;
};


template <class T>
class EventHandler
{
public:
	static_assert(std::is_base_of<EventArg, T>::value, "T must inherit from EventArg!");

	using Callback = std::function<void(T&)>;
	using key_t = uint16_t;
	struct CallbackComponent
	{
		Callback callback;
	};

	void Invoke(T& arg)
	{
		for (auto& [_, cc] : callbackHashMap)
		{
			cc.callback(arg);
			if (arg.isHandled)
				break;
		}
	}

	EventCallbackID<T> RegisterCallback(Callback callbackFunction)
	{
		callbackHashMap.insert({ counter, CallbackComponent{ callbackFunction }});
		EventCallbackID<T> callbackID{ counter++, this };
		return callbackID;
	}

	void RemoveCallback(key_t id)
	{
		if (callbackHashMap.find(id) != callbackHashMap.end())
		{
			callbackHashMap.erase(id);
		}
	}

	EventCallbackID<T> operator += (Callback callbackFunction)
	{
		return RegisterCallback(callbackFunction);
	}

	void operator -= (EventCallbackID<T> callbackID)
	{
		RemoveCallback(callbackID.id);
	}

	int size() const { return callbackHashMap.size(); }

	std::shared_ptr<int> ref = std::make_shared<int>(69);
private:
	key_t counter = 0;
	std::unordered_map<key_t, CallbackComponent> callbackHashMap;
};



template<typename T>
inline EventCallbackID<T>::~EventCallbackID()
{
	if (eventHandler != nullptr)
	{
		if (!refForChecking.expired())
		{
			eventHandler->RemoveCallback(id);
		}
	}
}

