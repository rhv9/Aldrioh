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
		this->eventHandler = eventHandler;
		this->refForChecking = refForChecking;
		other.id = 6969;
		other.eventHandler = nullptr;
	}
	EventCallbackID& operator=(EventCallbackID&& other) noexcept
	{
		this->id = other.id;
		this->eventHandler = other.eventHandler;
		this->refForChecking = refForChecking;
		other.eventHandler = nullptr;
		other.id = 6969;
		return *this;
	}

	EventHandler<T>::key_t id = 9000;
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

	~EventHandler();

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
			LOG_CORE_INFO("Succeeded in removing callback id: {}!", id);
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

	std::shared_ptr<int> ref = std::make_shared<int>(69);
private:
	key_t counter = 0;
	std::unordered_map<key_t, CallbackComponent> callbackHashMap;
};



template<typename T>
inline EventCallbackID<T>::~EventCallbackID()
{
	LOG_CORE_INFO("Event Callback ID being removed: {}!", id);

	ASSERT(id != 9000, "WHY!!");

	if (eventHandler != nullptr)
	{
		if (!refForChecking.expired())
		{
			eventHandler->RemoveCallback(id);
		}
	}
}

template<class T>
inline EventHandler<T>::~EventHandler()
{
	LOG_CORE_INFO("Destroying Event Handler!");
}
