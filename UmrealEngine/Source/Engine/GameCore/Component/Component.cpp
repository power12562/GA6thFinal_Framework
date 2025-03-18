#include "pch.h"

Component::Component() = default;
Component::~Component() = default;

const char* Component::name()
{
    return m_className.c_str();;
}