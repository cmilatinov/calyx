#include "inspector/components/CameraComponentInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    void CameraComponentInspector::CX_ON_INSPECTOR_GUI(CameraComponent& cc) {
        auto& camera = cc.GetCamera();

        bool changed = false;
        if (InspectorGUI::BeginPropertyTable("Camera")) {
            InspectorGUI::Property("FOV");
            changed |= InspectorGUI::FloatControl("fov", *camera.GetFOVPtr(), 0.1f, 60.0f, 120.0f);

            InspectorGUI::Property("Near Plane");
            changed |= InspectorGUI::FloatControl("near", *camera.GetNearPlanePtr(), 0.001f, 0.01f, 1.0f);

            InspectorGUI::Property("Far Plane");
            changed |= InspectorGUI::FloatControl("far", *camera.GetFarPlanePtr(), 1.0f, 100.0f, 1000.0f);

            InspectorGUI::EndPropertyTable();
        }

        if (changed) {
            camera.UpdateProjectionMatrix();
        }
    }

}