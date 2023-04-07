#include "windows/ViewportPanel.h"
#include "assets/Assets.h"
#include "scene/SceneManager.h"
#include "events/Event.h"
#include "events/EventMouse.h"
#include "math/ScreenSpaceUtils.h"
#include "input/Input.h"
#include "icons/MaterialDesign.h"
#include "ui/Widgets.h"

namespace Calyx::Editor {

    ViewportPanel::ViewportPanel() {
        const auto& window = Window::GetMainWindow();
        m_msaaFramebuffer = Framebuffer::Create(
            {
                window.GetWidth(),
                window.GetHeight(),
                32,
                {
                    { IRenderTarget::Type::RENDERBUFFER, TextureFormat::DEPTH32 },
                    { IRenderTarget::Type::TEXTURE, TextureFormat::RGBA8 },
                }
            }
        );
        m_msaaFramebuffer->Unbind();

        m_framebuffer = Framebuffer::Create(
            {
                window.GetWidth(),
                window.GetHeight(),
                1,
                {
                    { IRenderTarget::Type::RENDERBUFFER, TextureFormat::DEPTH32 },
                    { IRenderTarget::Type::TEXTURE, TextureFormat::RGBA8 },
                }
            }
        );
        m_framebuffer->Unbind();

        m_sceneRenderer = CreateScope<SceneRenderer>();
        m_editorCamera.GetTransform().SetPosition(vec3(1, 1, 1));
        m_editorCamera.GetTransform().SetRotation(vec3(-45, 45, 0));

        RenderCommand::SetViewport(0, 0, window.GetWidth(), window.GetHeight());
        RenderCommand::SetClearColor(vec4(99, 90, 83, 255) / 255.0f);
    }

    void ViewportPanel::OnUpdate() {
        // Update Camera
        if (m_viewport.pressed && !ImGuizmo::IsUsing()) {
            m_editorCamera.Update();
        }

        // Draw scene
        m_msaaFramebuffer->Bind();
        RenderCommand::Clear();
        m_sceneRenderer->RenderScene(
            m_editorCamera,
            m_editorCamera.GetTransform(),
            *SceneManager::GetSimulationOrCurrentScene()
        );
        m_sceneRenderer->RenderGrid(m_editorCamera, m_editorCamera.GetTransform());

        // Blit and resolve MSAA samples
        m_msaaFramebuffer->Blit(m_framebuffer, 0, 0);
        m_msaaFramebuffer->Unbind();

        // Delete scene objects pending deletion
        SceneManager::GetCurrentScene()->DeleteGameObjects();
    }

    void ViewportPanel::OnEvent(Event& event) {
        if (event.GetEventType() == EventType::MouseButtonPress &&
            dynamic_cast<EventMouseButtonPress&>(event).GetMouseButton() == MOUSE_BUTTON_1 &&
            m_viewport.hovered &&
            ScreenSpaceUtils::IsInBounds(Input::GetMousePosition(), m_viewport.bounds)) {
            m_viewport.pressed = true;
            m_focusNextFrame = true;
        } else if (event.GetEventType() == EventType::MouseButtonRelease &&
                   dynamic_cast<EventMouseButtonRelease&>(event).GetMouseButton() == MOUSE_BUTTON_1) {
            m_viewport.pressed = false;
        }
    }
    void ViewportPanel::OnGUI() {
        if (m_focusNextFrame) {
            ImGui::SetNextWindowFocus();
            m_focusNextFrame = false;
        }
        ImGui::SetNextWindowSize(ImVec2{ 1280, 720 }, ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin(
            "Viewport", nullptr,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse
        );
        ImGui::PopStyleVar();
        Toolbar();
        SceneViewport();
        UpdateViewport();
        Gizmos();
        ImGui::End();
    }

    void ViewportPanel::Toolbar() {
        auto cursor = ImGui::GetCursorPos();
        cursor.x += 3;
        cursor.y += 3;
        ImGui::SetCursorPos(cursor);

        Widgets::PushFont(CX_FONT_BOLD + CX_FONT_SIZE_LARGE);
        Widgets::PushFrameStyle(true);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 5, 3 });
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 7 });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3);

        auto& sceneManager = SceneManager::GetInstance();
        auto canStop = sceneManager.CanStopSimulation();
        if (!canStop) ImGui::BeginDisabled();
        if (ImGui::Button(ICON_MD_STOP)) {
            RestoreSelection(SelectionManager::GetCurrentSelection(), *SceneManager::GetCurrentScene());
            sceneManager.StopSimulation();
        }
        if (!canStop) ImGui::EndDisabled();
        ImGui::SameLine();

        auto canPause = sceneManager.CanPauseSimulation();
        if (!canPause) ImGui::BeginDisabled();
        if (ImGui::Button(ICON_MD_PAUSE)) sceneManager.PauseSimulation();
        if (!canPause) ImGui::EndDisabled();
        ImGui::SameLine();

        auto canStart = sceneManager.CanStartSimulation();
        if (!canStart) ImGui::BeginDisabled();
        if (ImGui::Button(ICON_MD_PLAY_ARROW)) {
            auto selection = SelectionManager::GetCurrentSelection();
            SelectionManager::ClearSelection();
            sceneManager.StartSimulation();
            RestoreSelection(selection, *SceneManager::GetSimulationScene());
        }
        if (!canStart) ImGui::EndDisabled();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 2 });
        ImGui::Separator();

        ImGui::PopStyleVar(4);
        Widgets::PopFrameStyle();
        Widgets::PopFont();
    }

    void ViewportPanel::SceneViewport() {
        uint32 textureID = m_framebuffer->GetColorAttachment(0).GetRendererID();
        ImGui::Image(
            reinterpret_cast<void*>(static_cast<size_t>(textureID)),
            ImGui::GetContentRegionAvail(),
            { 0.0f, 1.0f }, { 1.0f, 0.0f }
        );
    }

    void ViewportPanel::UpdateViewport() {
        // Get viewport bounds
        auto min = ImGui::GetItemRectMin();
        auto max = ImGui::GetItemRectMax();
        m_viewport.bounds = vec4(min.x, min.y, max.x, max.y);
        m_viewport.hovered = ImGui::IsWindowHovered();
        m_viewport.focused = ImGui::IsWindowFocused();

        // Resize viewport frame buffers if needed
        ImVec2 viewportSize = ImGui::GetItemRectSize();
        if (std::abs(viewportSize.x - m_viewport.size.x) > 1.0f ||
            std::abs(viewportSize.y - m_viewport.size.y) > 1.0f) {
            m_framebuffer->Resize(viewportSize.x, viewportSize.y);
            m_msaaFramebuffer->Resize(viewportSize.x, viewportSize.y);
            m_editorCamera.SetAspect(viewportSize.x / viewportSize.y);
        }
        m_viewport.size = vec2(viewportSize.x, viewportSize.y);
    }

    void ViewportPanel::Gizmos() {
        // Gizmos
        auto& selection = SelectionManager::GetCurrentSelection();
        if (selection.IsGameObjectSelection() && !selection.IsEmpty()) {
            auto* selected = selection.First().try_cast<GameObject>();
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(
                m_viewport.bounds.x, m_viewport.bounds.y,
                m_viewport.bounds.z - m_viewport.bounds.x,
                m_viewport.bounds.w - m_viewport.bounds.y
            );

            mat4 projection = m_editorCamera.GetProjectionMatrix();
            mat4 view = m_editorCamera.GetTransform().GetInverseMatrix();
            mat4 transform = selected->GetTransform().GetMatrix();
            if (ImGuizmo::Manipulate(
                glm::value_ptr(view), glm::value_ptr(projection),
                static_cast<ImGuizmo::OPERATION>(m_gizmoType), ImGuizmo::MODE::LOCAL,
                glm::value_ptr(transform), nullptr, nullptr
            )) {
                selected->GetTransform().SetWorldMatrix(transform);
            }
        }

        if (!m_viewport.pressed && m_viewport.focused) {
            if (Input::GetKeyDown(KEY_Q))
                m_gizmoType = 0;
            if (Input::GetKeyDown(KEY_W))
                m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            if (Input::GetKeyDown(KEY_E))
                m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            if (Input::GetKeyDown(KEY_R))
                m_gizmoType = ImGuizmo::OPERATION::SCALE;
        }
    }

    void ViewportPanel::RestoreSelection(const Selection& selection, const Scene& targetScene) {
        if (selection.IsEmpty() || !selection.IsOfType<GameObject>())
            return;

        Selection newSelection = Selection::Create<GameObject>();
        for (const auto& item: selection) {
            if (auto* gameObject = item.try_cast<GameObject>(); gameObject != nullptr) {
                newSelection.AddItem(targetScene.GetGameObject(gameObject->GetID()));
            }
        }
        SelectionManager::SetCurrentSelection(newSelection);
    }

}