#pragma once

#ifdef CX_PLATFORM_WINDOWS

int main(int argc, char** argv) {
    Log::Init();

    CX_CORE_INFO("Creating Calyx application...");
    auto* CxApp = Calyx::CreateApplication();

    CX_CORE_INFO("Running Calyx application...");
    CxApp->Run();

    CX_CORE_INFO("Cleaning up Calyx application...");
    delete CxApp;
}

#endif