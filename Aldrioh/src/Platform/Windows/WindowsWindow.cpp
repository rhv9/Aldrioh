#include <pch.h>
#include "WindowsWindow.h"
#include "Events/EventHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void glfwErrorCallbackFunction(int error_code, const char* description)
{
	LOG_CORE_CRITICAL("GLFW Error: {}", description);
}

WindowsWindow::WindowsWindow(const WindowProps& windowProps) : Data(this)
{
	Init(windowProps);
}

void WindowsWindow::Init(const WindowProps& windowProps)
{

	// Temporary
#ifdef EMSCRIPTEN
	std::cout << "EMSCRIPTEN mode!" << std::endl;
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_EMSCRIPTEN);
#endif

	Data.Width = windowProps.Width;
	Data.Height = windowProps.Height;
	Data.Title = windowProps.Title;

	LOG_CORE_INFO("Creating Window: Width={} Height={} Title={}", Data.Width, Data.Height, Data.Title);

	glfwSetErrorCallback(glfwErrorCallbackFunction);

	int success = glfwInit();
	ASSERT(success, "Failed to initialize Windows window: glfwInit");
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	LOG_CORE_INFO("GLFW Version: {} | Platform: {}", glfwGetVersionString(), glfwGetPlatform());

	// Construct the window
	m_Window = glfwCreateWindow(Data.Width, Data.Height, Data.Title.c_str(), nullptr, nullptr);

	ASSERT(m_Window, "Failed to create GLFW window");

	glfwMakeContextCurrent(m_Window);

	SetVsync(true);

	glfwSetWindowUserPointer(m_Window, &Data);

	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEventArg keyPressedEventArg{ key, mods };
				windowData->WindowObj->KeyPressedEventHandler.Invoke(keyPressedEventArg);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEventArg keyReleasedEventArg{ key, mods };
				windowData->WindowObj->KeyReleasedEventHandler.Invoke(keyReleasedEventArg);
				break;
			}
			case GLFW_REPEAT:
			{
				// TODO: Do not need it now
				break;
			}
			}
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
				{
					MouseButtonPressedEventArg mouseButtonPressed{ button, mods };
					windowData->WindowObj->MouseButtonPressedEventHandler.Invoke(mouseButtonPressed);
					break;
				}
			case GLFW_RELEASE:
				{
					MouseButtonReleasedEventArg mouseButtonReleased{ button, mods };
					windowData->WindowObj->MouseButtonReleasedEventHandler.Invoke(mouseButtonReleased);
					break;
				}
			default:
				break;
			}
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);

			MouseMoveEventArg mouseMoveEventArg{ (float)xpos, (float)ypos };
			windowData->WindowObj->MouseMoveEventHandler.Invoke(mouseMoveEventArg);
		});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEventArg windowCloseEventArg;
			windowData->WindowObj->WindowCloseEventHandler.Invoke(windowCloseEventArg);
		});

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);

			glViewport(0, 0, width, height);

			windowData->WindowObj->SetSize(width, height);

			WindowResizeEventArg windowResizeEventArg{ width, height };
			windowData->WindowObj->WindowResizeEventHandler.Invoke(windowResizeEventArg);
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEventArg mouseScrollEventArg{ (float)xoffset, (float)yoffset };
			windowData->WindowObj->MouseScrolledEventHandler.Invoke(mouseScrollEventArg);
		});



	// TODO: This should not be here
	success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ASSERT(success, "Failed to initialize GLAD");

	LOG_CORE_INFO("OpenGL Info:");
	LOG_CORE_INFO("  Vendor {0}", (const char*)glGetString(GL_VENDOR));
	LOG_CORE_INFO("  Renderer {0}", (const char*)glGetString(GL_RENDERER));
	LOG_CORE_INFO("  Version {0}", (const char*)glGetString(GL_VERSION));

	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
	LOG_CORE_INFO("  Major Version {0}", versionMajor);
	LOG_CORE_INFO("  Minor Version {0}", versionMinor);
}

void WindowsWindow::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(m_Window);
}

uint32_t WindowsWindow::GetWidth() const
{
	return Data.Width;
}

uint32_t WindowsWindow::GetHeight() const
{
	return Data.Height;
}

void WindowsWindow::SetSize(uint32_t width, uint32_t height)
{
	Data.Width = width;
	Data.Height = height;
}

void WindowsWindow::SetVsync(bool val)
{
	if (val)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	vsync = val;
	LOG_CORE_INFO("Vsync:{}", vsync ? "on" : "off");
}

void* WindowsWindow::GetNativeWindow() const
{
	return (void*)m_Window;
}

void WindowsWindow::Shutdown()
{
	glfwTerminate();
}

