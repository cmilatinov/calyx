#include "core/Application.h"

int main(int argc, char** argv) {
    Log::Init();

    CX_CORE_INFO("Current Directory: {}", FileSystem::current_path().string());
    CX_CORE_INFO("Creating Calyx application ...");
    auto app = Calyx::CreateApplication(argc, argv);

    CX_CORE_INFO("Running Calyx application ...");
    app->Run();

    CX_CORE_INFO("Cleaning up Calyx application ...");
}