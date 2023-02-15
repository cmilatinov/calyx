#include "layers/EditorLayer.h"

#include "scene/serialization/SceneExporter.h"

namespace Calyx::Editor {

    void EditorLayer::OnAttach() {
        AssetRegistry::AddSearchPath("assets");
        m_reflectedComponents = Reflect::Core::GetDerivedClasses<IComponent>();

        const Window& window = Application::GetInstance().GetWindow();

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
        m_scene = CreateScope<Scene>();
        m_sceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_scene.get());
        String assets = "./assets";
        String gameAssets = "./GameAssets";
        FileSystem::create_directories(assets);
        FileSystem::create_directories(gameAssets);
        AssetRegistry::AddSearchPath(assets);
        AssetRegistry::AddSearchPath(gameAssets);
        m_contentBrowserPanel = CreateScope<ContentBrowserPanel>(gameAssets);
        m_statsPanel = CreateScope<StatisticsPanel>();

        m_editorCamera = CreateScope<CameraEditor>();
        m_editorCamera->GetTransform().SetPosition(vec3(1, 1, 1));
        m_editorCamera->GetTransform().SetRotation(vec3(-45, 45, 0));

        m_grid = Assets::ScreenSpaceQuad();
        m_gridShader = AssetRegistry::LoadAsset<Shader>("shaders/grid.glsl");

        RenderCommand::SetViewport(0, 0, window.GetWidth(), window.GetHeight());
        RenderCommand::SetClearColor(vec4(99, 90, 83, 255) / 255.0f);
        CX_CORE_INFO("Window size: {:d} x {:d}", window.GetWidth(), window.GetHeight());

        auto cubeMesh = AssetRegistry::LoadAsset<Mesh>("meshes/cube.obj");
        m_cube = m_scene->CreateGameObject();
        m_cube->AddComponent<MeshComponent>(cubeMesh);
        m_cube->GetTransform().Translate(vec3(1, 0, 0));

        auto* test = m_scene->CreateGameObject("Test");
        test->AddComponent<MeshComponent>(cubeMesh);
        test->GetTransform().Translate(vec3(0, 2, 0));
        test->SetParent(m_cube);
    }

    void EditorLayer::OnUpdate() {
        // Stats
        m_statsPanel->BeginFrame();

        // Update Camera
        if (m_viewport.pressed && !ImGuizmo::IsUsing()) {
            m_editorCamera->Update();
        }

        // Draw scene
        m_msaaFramebuffer->Bind();
        RenderCommand::Clear();
        m_sceneRenderer->RenderScene(*m_editorCamera, m_editorCamera->GetTransform(), *m_scene);

        // Draw grid
        mat4 view = m_editorCamera->GetProjectionViewMatrix();
        mat4 invView = glm::inverse(view);

        m_gridShader->Bind();
        m_gridShader->SetMat4("view", view);
        m_gridShader->SetMat4("invView", invView);
        m_gridShader->SetFloat("nearPlane", m_editorCamera->GetNearPlane());
        m_gridShader->SetFloat("farPlane", m_editorCamera->GetFarPlane());
        m_grid->Draw();
        m_gridShader->Unbind();

        // Blit and resolve MSAA samples
        m_msaaFramebuffer->Blit(m_framebuffer, 0, 0);
        m_msaaFramebuffer->Unbind();

        // Delete scene objects pending deletion
        m_scene->DeleteGameObjects();

        // Stats
        m_statsPanel->EndFrame();
    }

    void EditorLayer::OnGUI() {
        BeginDockspace();
        MenuBar();
        SceneHierarchy();
        Viewport();
        Statistics();
        Inspector();
        ContentBrowser();
        EndDockspace();
//        ImGui::ShowDemoWindow();
    }

    void EditorLayer::OnEvent(Event& event) {
        if (event.GetEventType() == EventType::MouseButtonPress &&
            dynamic_cast<EventMouseButtonPress&>(event).GetMouseButton() == MOUSE_BUTTON_1 &&
            m_viewport.hovered &&
            ScreenSpaceUtils::IsInBounds(Input::GetMousePosition(), m_viewport.bounds)) {
            m_viewport.pressed = true;
        } else if (event.GetEventType() == EventType::MouseButtonRelease &&
                   dynamic_cast<EventMouseButtonRelease&>(event).GetMouseButton() == MOUSE_BUTTON_1) {
            m_viewport.pressed = false;
        }
    }

    void EditorLayer::BeginDockspace() {
        // Setup parent window
        int windowFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin("DockSpace", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        // Setup dock space
        ImGui::DockSpace(ImGui::GetID("DockSpace"), { 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    }

    void EditorLayer::MenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("New", "Ctrl+N");
                ImGui::MenuItem("Open...", "Ctrl+O");
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    SceneExporter exporter(*m_scene);
                    exporter.Save("./test.cx");
                }
                if (ImGui::MenuItem("Exit", "Alt+F4")) {
                    Application::GetInstance().Close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void EditorLayer::Viewport() {
        // Show viewport
        ImGui::SetNextWindowSize(ImVec2{ 1280, 720 }, ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin(
            "Viewport", nullptr,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse
        );
        ImGui::PopStyleVar();

        uint32 textureID = m_framebuffer->GetColorAttachment(0).GetRendererID();
        ImGui::Image(
            reinterpret_cast<void*>(static_cast<size_t>(textureID)), ImGui::GetContentRegionAvail(),
            { 0.0f, 1.0f }, { 1.0f, 0.0f }
        );

        // Resize viewport frame buffers if needed
        ImVec2 viewportSize = ImGui::GetWindowSize();
        if (std::abs(viewportSize.x - m_viewport.size.x) > 1.0f ||
            std::abs(viewportSize.y - m_viewport.size.y) > 1.0f) {
            m_framebuffer->Resize(viewportSize.x, viewportSize.y);
            m_msaaFramebuffer->Resize(viewportSize.x, viewportSize.y);
            m_editorCamera->SetAspect(viewportSize.x / viewportSize.y);
        }
        m_viewport.size = vec2(viewportSize.x, viewportSize.y);

        // Get viewport bounds
        ImGui::GetWindowContentRegionMin();
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 min(pos.x + contentMin.x, pos.y + contentMin.y);
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 max(pos.x + contentMax.x, pos.y + contentMax.y);
        m_viewport.bounds = vec4(min.x, min.y, max.x, max.y);
        m_viewport.hovered = ImGui::IsWindowHovered();
        m_viewport.focused = ImGui::IsWindowFocused();

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

            mat4 projection = m_editorCamera->GetProjectionMatrix();
            mat4 view = m_editorCamera->GetTransform().GetInverseMatrix();
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

        ImGui::End();
    }

    void EditorLayer::SceneHierarchy() {
        m_sceneHierarchyPanel->Draw();
    }

    void EditorLayer::Statistics() {
        m_statsPanel->Draw();
    }

    void EditorLayer::Inspector() {
        ImGui::Begin("Inspector");
        // TODO: Handle multiple object selection
        auto& selection = SelectionManager::GetCurrentSelection();
        if (selection.IsGameObjectSelection() && !selection.IsEmpty()) {
            auto* selected = selection.First().try_cast<GameObject>();

            // Name & ID
            if (InspectorGUI::BeginPropertyTable("GameObject")) {
                InspectorGUI::Property("ID");
                String id = uuids::to_string(selected->GetID());
                InspectorGUI::TextControl("##id", id, true);

                InspectorGUI::Property("Name");
                String name = selected->GetName();
                if (InspectorGUI::TextControl("##name", name)) {
                    selected->SetName(name);
                }

                InspectorGUI::EndPropertyTable();
            }

            // Components
            Set<AssetType> componentSet;
            for (auto& component: m_reflectedComponents) {
                auto storage = m_scene->m_entityRegistry.storage(component.info().hash());
                if (storage != nullptr && storage->contains(selected->m_entityID)) {
                    Inspector::DrawComponentInspector(selected, component, storage->get(selected->m_entityID));
                    componentSet.insert(component.id());
                }
            }

            // Add components
            InspectorGUI::ComponentPicker(selected, componentSet);
        }
        ImGui::End();
    }

    void EditorLayer::ContentBrowser() {
        m_contentBrowserPanel->Draw();
    }

    void EditorLayer::EndDockspace() {
        ImGui::End();
    }

}