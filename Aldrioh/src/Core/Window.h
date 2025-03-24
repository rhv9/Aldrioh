#pragma once

#include <stdint.h>
#include <Events/Events.h>

struct WindowProps
{
	uint32_t Width, Height;
	std::string Title;
};

class Window
{
public:
	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void SetSize(uint32_t width, uint32_t height) = 0;

	virtual void Shutdown() = 0;

	virtual void SetVsync(bool val) = 0;
	virtual bool GetVsync() const { return vsync; }

	virtual void* GetNativeWindow() const = 0;

public:
	EventHandler<KeyPressedEventArg> KeyPressedEventHandler;
	EventHandler<KeyReleasedEventArg> KeyReleasedEventHandler;

	EventHandler<MouseButtonPressedEventArg> MouseButtonPressedEventHandler;
	EventHandler<MouseButtonReleasedEventArg> MouseButtonReleasedEventHandler;
	EventHandler<MouseMoveEventArg> MouseMoveEventHandler;
	EventHandler<MouseScrolledEventArg> MouseScrolledEventHandler;

	EventHandler<WindowCloseEventArg> WindowCloseEventHandler;
	EventHandler<WindowResizeEventArg> WindowResizeEventHandler;
protected:
	bool vsync = true;
};