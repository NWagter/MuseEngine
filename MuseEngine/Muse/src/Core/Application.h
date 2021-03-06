#pragma once

#include "Layer/LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "Core/Instrumentor.h"

#include <memory>

namespace Muse
{
    class ResourceManager;
    class JobManager;
    class GameWindow; 
    class SceneManager;
    class SystemManager;
    class ImGuiLayer;
    class Window;
    class FrameBuffer;
    class Input;

	class Application
	{
	public:
        static Application& Get() { return *s_Instance; }

		Application(const std::string& a_Name = "Muse Application");
		virtual ~Application();

        std::shared_ptr<Window> GetWindow() const { return m_Window; }
        std::shared_ptr<FrameBuffer> GetViewport() const { return m_ViewportFramebuffer; }
        float GetDeltaTime() const { return m_DeltaTime; }
        std::shared_ptr<JobManager> GetJobManager() const { return m_JobManager; }
        std::shared_ptr<SceneManager> GetSceneManager() const { return m_SceneManager; }

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        void Run();
        void Update();
        void FixedUpdate();
        void LateUpdate();
        void ImGuiRender();
        void Render();
        void Close();

        void PushLayer(Layer* a_Layer);
        void PushOverlay(Layer* a_Layer);

        UpdateEvent m_UpdateEvent;
        FixedUpdateEvent m_FixedUpdateEvent;
        LateUpdateEvent m_LateUpdateEvent;
        ImGuiRenderEvent m_ImGuiRenderEvent;
        RenderEvent m_RenderEvent;

    protected:
        virtual void OnStart() = 0;
        virtual void OnUpdate(float a_DeltaTime) = 0;
        virtual void OnFixedUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnImGuiRender() = 0;

        virtual void OnWindowCloseEvent() {}
        virtual void OnWindowResizeEvent(int a_Width, int a_Height) {}
        virtual void OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount) {}
        virtual void OnKeyReleasedEvent(int a_KeyCode) {}
        virtual void OnMouseButtonPressedEvent(int a_Button) {}
        virtual void OnMouseButtonReleasedEvent(int a_Button) {}
        virtual void OnMouseScrolledEvent(float a_XOffset, float a_YOffset) {}
        virtual void OnMouseMovedEvent(float a_X, float a_Y) {}

        void WindowCloseEvent();
        void WindowResizeEvent(int a_Width, int a_Height);
        void KeyPressedEvent(int a_KeyCode, int a_RepeatCount);
        void KeyReleasedEvent(int a_KeyCode);
        void MouseButtonPressedEvent(int a_Button);
        void MouseButtonReleasedEvent(int a_Button);
        void MouseScrolledEvent(float a_XOffset, float a_YOffset);
        void MouseMovedEvent(float a_X, float a_Y);

    private:
        static Application* s_Instance;
        bool m_Running = true;
        bool m_Minimized = false;
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<SceneManager> m_SceneManager;
        std::shared_ptr<ResourceManager> m_ResourceManager;
        std::shared_ptr<FrameBuffer> m_ViewportFramebuffer;
        std::shared_ptr<JobManager> m_JobManager;
        float m_DeltaTime = 0;
        std::shared_ptr<Input> m_Input;

        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
	};

	// To be defined in client.
	Application* CreateApplication();
}
