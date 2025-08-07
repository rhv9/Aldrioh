#include "pch.h"
#include "Game.h"

//#define EMSCRIPTEN

#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#include <GLFW/emscripten_glfw3.h>
#include <emscripten/emscripten.h>

#else
#include <glad/glad.h>
#endif

#include <Platform/Windows/WindowsWindow.h>
#include <Events/Events.h>
#include "Input/Input.h"

#include "Graphics/Renderer.h"
#include "Graphics/RenderQueue.h"
#include <Graphics/ShaderManager.h>

#include "Game/GameLayer.h"
#include "Game/LevelEditor/LevelEditorLayer.h"

#include <Game/SpriteCollection.h>

#include <Debug/Statistics.h>

//#define DISPLAY_IMGUI_DEBUG

Game& Game::Instance()
{
	static Game instance;
	return instance;
}

Game::Game() {}

void Game::Init()
{
    
    Log::Init();
    
    window = std::make_unique<WindowsWindow>(WindowProps { 600 , 800, "Aldrioh" });

    window->KeyPressedEventHandler += [](KeyPressedEventArg& arg)
        {
            if (arg.Key == Input::KEY_ESCAPE)
                Game::Instance().Shutdown();
        };
    window->WindowCloseEventHandler += std::bind(&Game::OnWindowClose, this, std::placeholders::_1);

    ShaderManager::Get().LoadShaders();
    Renderer::Init();
    RenderQueue::Init();

    Sprites::Init();

#ifdef DISPLAY_IMGUI_DEBUG
    imGuiLayer = new ImGuiLayer();
    layerStack.PushLayer(imGuiLayer);
#endif
    layerStack.PushLayer(new LevelEditorLayer());

    running = true;
}

void Game::Start()
{
    for (Layer* layer : layerStack)
    {
        layer->OnBegin();
    }

    //emscripten_set_main_loop(this->Loop, 60, GLFW_FALSE);
    // This is the render loop
    while (running)
    {
        Iterate();

        Statistics::ResetStats();
    }

    OnClosing();
}

void Game::Loop()
{
#ifdef EMSCRIPTEN
    if (!iter())
    {
        m_Window->Shutdown();
        emscripten_cancel_main_loop();
    }
#endif
}

bool Game::Iterate()
{
    if (!running)
        return false;

    // Calculate FPS logic
    {
        auto timeNow = std::chrono::system_clock::now();
        delta = ((std::chrono::duration<double>)(timeNow - previousTime)).count();
        deltaCummulative += timeNow - previousTime;
        previousTime = timeNow;

        if (deltaCummulative.count() >= 1.0f)
        {
            deltaCummulative--;
            i_gameStats.fps = i_gameStats.fpsCounter;
            i_gameStats.fpsCounter = 0;
        }
        i_gameStats.fpsCounter++;
    }

    for (Layer* layer : layerStack)
    {
        layer->OnUpdate(delta);
    }

#ifdef DISPLAY_IMGUI_DEBUG
    imGuiLayer->BeginRender();
    for (Layer* layer : layerStack)
    {
        layer->OnImGuiRender(delta);
    }
    imGuiLayer->EndRender();
#endif
    window->OnUpdate();

    return true;
}

void Game::Shutdown()
{
    running = false;
}

void Game::OnClosing()
{
    Renderer::Destroy();
}

void Game::BlockEvents(bool val)
{
    imGuiLayer->BlockEvents(val);
}

void Game::OnWindowClose(WindowCloseEventArg arg)
{
    LOG_CORE_INFO("Window Closing...");
    Shutdown();
}
