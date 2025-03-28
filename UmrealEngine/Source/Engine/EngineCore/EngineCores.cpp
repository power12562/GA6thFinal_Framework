#include "pch.h"

EngineCores::EngineCores() = default;
EngineCores::~EngineCores() = default;

std::unique_ptr<EngineCores> EngineCore{};

std::unique_ptr<EngineCores> EngineCores::Engine::MakeEngineCores()
{
    return std::make_unique<EngineCores>();
}
