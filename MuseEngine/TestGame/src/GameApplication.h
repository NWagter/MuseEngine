#pragma once

#include "Core/Application.h"
#include "glm/vec3.hpp"
#include "Core/Renderer/ShaderLibrary.h"

namespace Muse
{
    class Texture2D;
    class Scene;
    class OrthographicCamera;
    class Shader;
    class VertexBuffer;
    class IndexBuffer;
    class VertexArray;
}

class GameApplication : public Muse::Application
{
public:
    GameApplication() = default;
    virtual ~GameApplication() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnImGUIRender() override;

    virtual void OnWindowCloseEvent() override;
    virtual void OnWindowResizeEvent(int a_Width, int a_Height) override;
    virtual void OnKeyPressedEvent(int a_KeyCode, int a_RepeatCount) override;
    virtual void OnKeyReleasedEvent(int a_KeyCode) override;
    virtual void OnMouseButtonPressedEvent(int a_Button) override;
    virtual void OnMouseButtonReleasedEvent(int a_Button) override;
    virtual void OnMouseScrolledEvent(float a_XOffset, float a_YOffset) override;
    virtual void OnMouseMovedEvent(float a_X, float a_Y) override;

private:
    std::shared_ptr<Muse::Texture2D> m_CheckerboardTexture, m_RaymanTexture;
    Muse::Scene* m_Scene;
    Muse::ShaderLibrary m_ShaderLibrary;

    glm::vec3 m_FlatShaderColor = { 0.8, 0.2, 0.4 };

    const float m_CameraMoveSpeed = 0.3f;

};
