#pragma once

#ifdef CX_PLATFORM_WINDOWS

int main(int argc, char** argv) {
    info("Creating Calyx application...");
    auto* CxApp = Calyx::CreateApplication();
    info("Running Calyx application...");
    CxApp->Run();
    info("Cleaning up Calyx application...");
    delete CxApp;
}

#endif