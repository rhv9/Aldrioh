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
	glm::vec2 GetSize() const { return { GetWidth(), GetHeight() }; }

	float GetAspectRatio() const { return (float)GetWidth() / (float)GetHeight(); }

	virtual glm::vec2 GetPos() const = 0;
	virtual void SetPos(int xpos, int ypos) = 0;

	virtual void SetSize(uint32_t width, uint32_t height) = 0;

	virtual void Shutdown() = 0;

	virtual void SetVsync(bool val) = 0;
	virtual bool GetVsync() const { return vsync; }

	virtual void SetTitle(const std::string& title) = 0;


	virtual void* GetNativeWindow() const = 0;

public:
	EventHandler<KeyEventArg> KeyEventHandler;

	EventHandler<MouseButtonEventArg> MouseButtonEventHandler;
	EventHandler<MouseMoveEventArg> MouseMoveEventHandler;
	EventHandler<MouseScrolledEventArg> MouseScrolledEventHandler;

	EventHandler<WindowCloseEventArg> WindowCloseEventHandler;
	EventHandler<WindowResizeEventArg> WindowResizeEventHandler;
protected:
	bool vsync = true;
};