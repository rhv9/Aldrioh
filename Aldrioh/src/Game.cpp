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
#include <Audio/SoundManager.h>

#include <Core/LayerInitialiser.h>

#include <Game/SpriteCollection.h>
#include <UI/Font.h>

#include <Debug/Statistics.h>

#include <File/Settings.h>

#define DISPLAY_IMGUI_DEBUG

Game& Game::Instance()
{
	static Game instance;
	return instance;
}

Game::Game() {}

void Game::Init()
{
    Log::Init();
    
    //window = std::make_unique<WindowsWindow>(WindowProps { 600 , 800, "Aldrioh" });
    window = std::make_unique<WindowsWindow>(WindowProps { 500 , 500, "Aldrioh" });

    callbackWindowCloseID = window->WindowCloseEventHandler.RegisterCallback(std::bind(&Game::OnWindowCloseEvent, this, std::placeholders::_1));
    callbackWindowResizeID = window->WindowResizeEventHandler.RegisterCallback(std::bind(&Game::OnWindowResizeEvent, this, std::placeholders::_1));

    callbackMouseMoveID = window->MouseMoveEventHandler.RegisterCallback(std::bind(&Game::OnMouseMoveEvent, this, std::placeholders::_1));
    callbackMouseScrolledID = window->MouseScrolledEventHandler.RegisterCallback(std::bind(&Game::OnMouseScrolledEvent, this, std::placeholders::_1));
    callbackMouseButtonID = window->MouseButtonEventHandler.RegisterCallback(std::bind(&Game::OnMouseButtonEvent, this, std::placeholders::_1));

    callbackKeyID = window->KeyEventHandler.RegisterCallback(std::bind(&Game::OnKeyEvent, this, std::placeholders::_1));


    ShaderManager::Get().LoadShaders();
    Renderer::Init();
    RenderQueue::Init();
    Font::InitGlobalFonts();
    Sprites::Init();
    SoundManager::Init();

    Settings::Get().LoadOrCreate();
    Settings::Get().Apply();

#ifdef DISPLAY_IMGUI_DEBUG
    imGuiLayer = new ImGuiLayer();
    layerStack.PushLayer(imGuiLayer);
#endif
    layerStack.PushLayer(LayerInitialiser::PushFirstLayer());

    running = true;
}

void Game::Start()
{
    std::vector<Layer*> otherLayers = LayerInitialiser::OtherLayers();
    for (Layer* layer : otherLayers)
    {
        layer->OnBegin();
        layer->SetInitialized(true);
    }

    //emscripten_set_main_loop(this->Loop, 60, GLFW_FALSE);
    // This is the render loop
    previousTime = std::chrono::system_clock::now();
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

constexpr uint32_t TICK_PER_SECOND = 30;
constexpr float TICK_TIMESTEP = 1.0f / (float)TICK_PER_SECOND;

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
            i_gameStats.ticksPerSecond = i_gameStats.updateTicks;
            i_gameStats.updateTicks = 0;
            
            if (window)
                GetWindow()->SetTitle(std::format("Aldrioh | tick: {} | fps: {}", i_gameStats.ticksPerSecond, i_gameStats.fps));
        }
        i_gameStats.fpsCounter++;
    }

    tickTimer += delta;
    deltaSinceUpdate += delta;

    while (tickTimer >= TICK_TIMESTEP)
    {
        layerStack.Update(TICK_TIMESTEP);

        tickTimer -= TICK_TIMESTEP;
        ++i_gameStats.updateTicks;
        ++i_gameStats.totalTickCount;
        deltaSinceUpdate = 0;
    }

    // TODO: maybe want to skip the render if has done one? Let's leave it like this for now
    bool hasDoneSwap = layerStack.HandleQueuedTasks();

    Renderer::ClearColourBuffer();
    Renderer::ClearDepthBuffer();
    layerStack.Render(deltaSinceUpdate / TICK_TIMESTEP);

#ifdef DISPLAY_IMGUI_DEBUG
    imGuiLayer->BeginRender();
    layerStack.ImGuiRender(delta);
    imGuiLayer->EndRender();
#endif
    window->OnUpdate();

    SoundManager::RecycleFinishedSounds();

    return true;
}

void Game::Shutdown()
{
    running = false;
}

void Game::OnClosing()
{
    Font::DestroyGlobalFonts();
    Renderer::Destroy();
    SoundManager::Destroy();
}

Timestep Game::GetFixedTickTimestep() const
{
    return TICK_TIMESTEP;
}

uint32_t Game::GetTicksTarget() const
{
    return TICK_PER_SECOND;
}

void Game::BlockEvents(bool val)
{
    imGuiLayer->BlockEvents(val);
}

void Game::OnWindowCloseEvent(WindowCloseEventArg& arg)
{
    layerStack.OnWindowCloseEvent(arg);

    LOG_CORE_INFO("Window Closing...");
    Shutdown();
}

void Game::OnWindowResizeEvent(WindowResizeEventArg& arg)
{
    layerStack.OnWindowResizeEvent(arg);
}

void Game::OnKeyEvent(KeyEventArg& arg)
{
    layerStack.OnKeyEvent(arg);
}

void Game::OnMouseButtonEvent(MouseButtonEventArg& arg)
{
    layerStack.OnMouseButtonEvent(arg);
}

void Game::OnMouseMoveEvent(MouseMoveEventArg& arg)
{
    layerStack.OnMouseMoveEvent(arg);
}

void Game::OnMouseScrolledEvent(MouseScrolledEventArg& arg)
{
    layerStack.OnMouseScrolledEvent(arg);
}

