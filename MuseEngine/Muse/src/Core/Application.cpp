#include "MusePCH.h"

#include "Core/Application.h"

#include "Window.h"
#include "Core/System/ResourceSystem.h"
#include "Core/System/SceneSystem.h"
#include "Core/System/SoundSystem.h"
#include "Core/System/Manager/SystemManager.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Timestep.h"
#include "ImGui/ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "Renderer/Renderer.h"
#include "glad/glad.h"

namespace Muse
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ASSERT(!s_Instance, "A instance of Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->WindowCloseEvent.Subscribe(this, std::bind(&Application::OnWindowCloseEvent, this));
        m_Window->WindowResizeEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyPressedEvent.Subscribe(this, std::bind(&Application::OnWindowResizeEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->KeyReleasedEvent.Subscribe(this, std::bind(&Application::OnKeyReleasedEvent, this, std::placeholders::_1));
        m_Window->MouseButtonPressedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonPressedEvent, this, std::placeholders::_1));
        m_Window->MouseButtonReleasedEvent.Subscribe(this, std::bind(&Application::OnMouseButtonReleasedEvent, this, std::placeholders::_1));
        m_Window->MouseScrolledEvent.Subscribe(this, std::bind(&Application::OnMouseScrolledEvent, this, std::placeholders::_1, std::placeholders::_2));
        m_Window->MouseMovedEvent.Subscribe(this, std::bind(&Application::OnMouseMovedEvent, this, std::placeholders::_1, std::placeholders::_2));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_SystemManager = new SystemManager();
        m_SystemManager->CreateSystem<ResourceSystem>();
        m_SystemManager->CreateSystem<SoundSystem>();
        m_SystemManager->CreateSystem<SceneSystem>(*this);
    }

    Application::~Application()
    {
        m_Window->WindowCloseEvent.Unsubscribe(this);
        m_Window->WindowResizeEvent.Unsubscribe(this);
        m_Window->KeyPressedEvent.Unsubscribe(this);
        m_Window->KeyReleasedEvent.Unsubscribe(this);
        m_Window->MouseButtonPressedEvent.Unsubscribe(this);
        m_Window->MouseButtonReleasedEvent.Unsubscribe(this);
        m_Window->MouseScrolledEvent.Unsubscribe(this);
        m_Window->MouseMovedEvent.Unsubscribe(this);

        delete m_SystemManager;
    }

    void Application::Start()
    {
        OnStart();

        while (m_Running)
        {
            Update();
            FixedUpdate();
            Render();
        }
    }

    void Application::Update()
    {
        float time = static_cast<float>(glfwGetTime()); //Platform::GetTime
        float deltaTime = time - m_LastFrameTime;
        m_LastFrameTime = time;


        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(deltaTime);
        }

        m_SystemManager->UpdateSystems(deltaTime);
        OnUpdate(deltaTime);
    }

    void Application::FixedUpdate()
    {
        OnFixedUpdate();
    }

    void Application::Render()
    {
        m_ImGuiLayer->Begin();
        OnImGUIRender();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();

        OnRender();
    }

    void Application::PushLayer(Layer* a_Layer)
    {
        m_LayerStack.PushLayer(a_Layer);
        a_Layer->OnAttach();
    }

    void Application::PushOverlay(Layer* a_Layer)
    {
        m_LayerStack.PushOverlay(a_Layer);
        a_Layer->OnAttach();
    }

    void Application::OnWindowCloseEvent()
    {
        m_Running = false;
    }

    void Application::OnWindowResizeEvent(int a_Width, int a_Height)
    {
        //LOG_ENGINE_INFO("OnWindowResize: {0}, {1}", a_Width, a_Height);
    }

    void Application::OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount)
    {
        //LOG_ENGINE_INFO("OnKeyPressed: {0}, {1}", a_KeyCode, a_RepeatCount);
    }

    void Application::OnKeyReleasedEvent(int a_KeyCode)
    {
        //LOG_ENGINE_INFO("OnKeyReleased: {0}", a_KeyCode);
    }

    void Application::OnMouseButtonPressedEvent(int a_Button)
    {
        //LOG_ENGINE_INFO("OnMouseButtonPressed: {0}", a_Button);
    }

    void Application::OnMouseButtonReleasedEvent(int a_Button)
    {
        //LOG_ENGINE_INFO("OnMouseButtonReleased: {0}", a_Button);
    }

    void Application::OnMouseScrolledEvent(float a_XOffset, float a_YOffset)
    {
        //LOG_ENGINE_INFO("OnMouseScrolled: {0}, {1}", a_XOffset, a_YOffset);
    }

    void Application::OnMouseMovedEvent(float a_X, float a_Y)
    {
        //LOG_ENGINE_INFO("OnMouseMoved: {0}, {1}", a_X, a_Y);
    }
}
