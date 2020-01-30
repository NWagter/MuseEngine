﻿#include "MusePCH.h"
#include "RenderComponent.h"

#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Buffer/BufferLayout.h"
#include "Core/Instrumentor.h"
#include "Core/Renderer/RayTracing/Shape/Shape.h"

namespace Muse
{
    void RenderComponent::SetMesh(float a_Vertices[], int a_VerticesCount, uint32_t a_Indices[], int a_IndicesCount, const BufferLayout& a_BufferLayout)
    {
        MUSE_PROFILE_FUNCTION();

        m_VA = Muse::VertexArray::Create();

        m_VB = Muse::VertexBuffer::Create(a_Vertices, sizeof(float) * a_VerticesCount);
        m_VB->Bind();

        m_VB->SetLayout(a_BufferLayout);
        m_VA->AddVertexBuffer(m_VB);

        //const uint32_t count = sizeof(a_Indices) / sizeof(uint32_t);
        m_IB = Muse::IndexBuffer::Create(a_Indices, a_IndicesCount);
        m_VA->SetIndexBuffer(m_IB);
    }

    Shape& RenderComponent::GetNormal(const glm::vec3& a_Point) const
    {
        m_Shape.GetNormal(a_Point);
    }

    void RenderComponent::OnUpdate(float a_DeltaTime)
    {
        MUSE_PROFILE_FUNCTION();

    }

    void RenderComponent::OnEnable()
    {
        MUSE_PROFILE_FUNCTION();

    }

    void RenderComponent::OnDisable()
    {
        MUSE_PROFILE_FUNCTION();
    }
}

RTTR_REGISTRATION
{
    rttr::registration::class_<Muse::RenderComponent>("RenderComponent")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        );
//.property("Enabled", &Muse::Component::m_isEnabled);
}