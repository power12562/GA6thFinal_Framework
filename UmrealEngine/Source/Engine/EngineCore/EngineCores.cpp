#include "pch.h"

EngineCores::EngineCores() = default;
EngineCores::~EngineCores() = default;

std::shared_ptr<EngineCores> EngineCore{};

std::shared_ptr<EngineCores> EngineCores::Engine::MakeEngineCores()
{
    return std::make_shared<EngineCores>();
}
