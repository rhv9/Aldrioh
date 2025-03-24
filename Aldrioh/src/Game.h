#pragma once
#include <Core/Window.h>

#include "Core/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

struct GameStats
{
	int fpsCounter = 0;
	int fps = 0;
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

	std::unique_ptr<Window>& GetWindow() { return window; }
	void BlockEvents(bool val);
	bool isEventsBlocked() { return imGuiLayer->isEventsBlocked(); }
	
	const GameStats& gameStats = i_gameStats;
private:
	void OnWindowClose(WindowCloseEventArg arg);

private:
	std::chrono::system_clock::time_point previousTime = std::chrono::system_clock::now();
	std::chrono::duration<double> deltaCummulative = std::chrono::system_clock::now() - std::chrono::system_clock::now(); // Change this hack by properly learning you lazy 

	GameStats i_gameStats;
	bool running = false;
	float delta = 0.0f;

	std::unique_ptr<Window> window;
	ImGuiLayer* imGuiLayer;

	LayerStack layerStack;
};

