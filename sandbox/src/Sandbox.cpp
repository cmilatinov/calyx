#include "Sandbox.h"

Sandbox::Sandbox() {

}

Sandbox::~Sandbox() {

}

void Sandbox::Run() {
    while(true);
}

Application * Calyx::CreateApplication() {
    return new Sandbox;
}