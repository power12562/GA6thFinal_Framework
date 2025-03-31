#include "pch.h"

EngineCores::EngineCores() = default;
EngineCores::~EngineCores() = default;

std::shared_ptr<EngineCores> EngineCore{};

void EngineCores::Engine::CreateEngineCores()
{
    if (EngineCore)
    {
        assert(!"엔진 코어가 이미 존재합니다.");
        return;
    }
    EngineCore = std::make_shared<EngineCores>();
}

void EngineCores::Engine::DestroyEngineCores()
{
    EngineCore.reset();
}
