#pragma once

#include "Input/InputCode.h"

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
	EventCallbackID(entt::entity id, EventHandler<T>* eventHandler) : id(id), eventHandler(eventHandler) {}
	~EventCallbackID();
	EventCallbackID(const EventCallbackID& other) = delete;
	EventCallbackID& operator=(const EventCallbackID& other) = delete;

	EventCallbackID(EventCallbackID&& other) noexcept
	{
		this->id = other.id;
		this->eventHandler = eventHandler;
		other.eventHandler = nullptr;
	}
	EventCallbackID& operator=(EventCallbackID&& other) noexcept
	{
		this->id = other.id;
		this->eventHandler = eventHandler;
		other.eventHandler = nullptr;
		return *this;
	}

	entt::entity id = entt::null;
	EventHandler<T>* eventHandler = nullptr;

	friend EventHandler<T>;
};


template <class T>
class EventHandler
{
	static_assert(std::is_base_of<EventArg, T>::value, "T must inherit from EventArg!");

	using Callback = std::function<void(T&)>;

	struct CallbackComponent
	{
		Callback callback;

		CallbackComponent() = default;
		CallbackComponent(Callback callback) : callback(callback) {}
	};

public:
	void Invoke(T& arg)
	{
		auto view = registry.view<CallbackComponent>();

		for (entt::entity e : view)
		{
			CallbackComponent& cc = view.get<CallbackComponent>(e);
			cc.callback(arg);

			if (arg.isHandled)
				break;
		}
	}

	EventCallbackID<T> RegisterCallback(Callback callbackFunction)
	{
		entt::entity id = registry.create();
		registry.emplace<CallbackComponent>(id, callbackFunction);
		EventCallbackID<T> callbackID{ id, this };
		return callbackID;
	}

	void RemoveCallback(entt::entity id)
	{
		if (registry.valid(id))
		{
			LOG_CORE_TRACE("Removing callback");
			registry.destroy(id);

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

private:
	std::vector<Callback> Callbacks;
	entt::registry registry;
};



template<typename T>
inline EventCallbackID<T>::~EventCallbackID()
{
	if (eventHandler != nullptr)
	{
		eventHandler->RemoveCallback(id);
	}
}

