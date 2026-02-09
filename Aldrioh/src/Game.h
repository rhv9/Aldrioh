#pragma once
#include <Core/Window.h>

#include "Core/LayerStack.h"
#include "ImGui/ImGuiLayer.h"


namespace Statistics {
	struct EngineStats;
}

struct GameStats
{
	int fpsCounter = 0;
	int fps = 0;
	int updateTicks = 0;
	int ticksPerSecond = 0;

	uint32_t totalTickCount = 0;
};

class Game
{
public:
	static Game& Instance();

	Game();
	void Init();
	void Start();
	void Loop();
	bool Iterate();
	void Shutdown();
	void OnClosing();

	Timestep GetDelta() const { return delta; }
	Timestep GetFixedTickTimestep() const;
	// Returns the total count of ticks done
	uint32_t GetTickCount() const { return i_gameStats.totalTickCount; }
	// Returns the target ticks per second
	uint32_t GetTicksTarget() const;

	std::unique_ptr<Window>& GetWindow() { return window; }
	void BlockEvents(bool val);
	bool isEventsBlocked() {
		// TODO: Fix this
#ifdef DISPLAY_IMGUI_DEBUG
		return imGuiLayer->isEventsBlocked(); 
#else
		return false;
#endif
	}
	LayerStack& GetLayerStack() { return layerStack; }
	
	const GameStats& gameStats = i_gameStats;
private:
	void OnWindowCloseEvent(WindowCloseEventArg& arg);
	void OnWindowResizeEvent(WindowResizeEventArg& arg);
	void OnMouseButtonEvent(MouseButtonEventArg& arg);
	void OnMouseMoveEvent(MouseMoveEventArg& arg);
	void OnMouseScrolledEvent(MouseScrolledEventArg& arg);
	void OnKeyEvent(KeyEventArg& arg);

private:
	std::chrono::system_clock::time_point previousTime = std::chrono::system_clock::now();
	std::chrono::duration<double> deltaCummulative = std::chrono::system_clock::now() - std::chrono::system_clock::now(); // Change this hack by properly learning you lazy 

	GameStats i_gameStats;
	bool running = false;
	float delta = 0.0f;
	float tickTimer = 0.0f;
	float deltaSinceUpdate = 0.0f;

	std::unique_ptr<Window> window;

	ImGuiLayer* imGuiLayer;
	LayerStack layerStack;

	EventCallbackID<KeyEventArg> callbackKeyID;
	EventCallbackID<MouseButtonEventArg> callbackMouseButtonID;
	EventCallbackID<MouseMoveEventArg> callbackMouseMoveID;
	EventCallbackID<MouseScrolledEventArg> callbackMouseScrolledID;
	EventCallbackID<WindowCloseEventArg> callbackWindowCloseID;
	EventCallbackID<WindowResizeEventArg> callbackWindowResizeID;


	friend Statistics::EngineStats;
};

