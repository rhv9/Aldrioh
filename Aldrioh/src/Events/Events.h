#pragma once
#include "EventHandler.h"

// Key callbacks

struct KeyPressedEventArg : public EventArg
{
	const int Key;
	const int Mods;

	KeyPressedEventArg(int key, int mods) : Key(key), Mods(mods) {}
};

struct KeyReleasedEventArg : public EventArg
{
	const int Key;
	const int Mods;

	KeyReleasedEventArg(int key, int mods) : Key(key), Mods(mods) {}
};


// Mouse callbacks

struct MouseButtonPressedEventArg : public EventArg
{
	const int Button;
	const int Mods;

	MouseButtonPressedEventArg(int button, int mods) : Button(button), Mods(mods) {}
};

struct MouseButtonReleasedEventArg : public EventArg
{
	const int Button;
	const int Mods;

	MouseButtonReleasedEventArg(int button, int mods) : Button(button), Mods(mods) {}
};

struct MouseMoveEventArg : public EventArg
{
	const float XPos, YPos;

	MouseMoveEventArg(float xPos, float yPos) : XPos(xPos), YPos(yPos) {}
};

struct MouseScrolledEventArg : public EventArg
{
	float XOffset, YOffset;

	MouseScrolledEventArg(float xOffset, float yOffset) : XOffset(xOffset), YOffset(yOffset) {}
};


// Window callbacks

struct WindowCloseEventArg : public EventArg
{
};

struct WindowResizeEventArg : public EventArg
{
	const int Width;
	const int Height;

	WindowResizeEventArg(int width, int height) : Width(width), Height(height) {}
};
