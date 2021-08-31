#pragma once

#include <Calyx.h>

class Sandbox : public Calyx::Application {

public:
    Sandbox();
    virtual ~Sandbox() override;

    virtual void Run();

};