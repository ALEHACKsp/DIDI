#pragma once

#include <sstream>
#include <type_traits>
#include <Windows.h>

#include "SDK/InputSystem.h"
#include "SDK/Client.h"
#include "SDK/Engine.h"
#include "SDK/EntityList.h"
#include "SDK/Surface.h"

class EngineTrace;
class InputSystem;
class Client;
class Engine;
class EntityList;
class Surface;

class Interfaces {
public:
    Client* client = find<Client>(L"client_panorama", "VClient018");
    Engine* engine = find<Engine>(L"engine", "VEngineClient014");
    EngineTrace* engineTrace = find<EngineTrace>(L"engine", "EngineTraceClient004");
    EntityList* entityList = find<EntityList>(L"client_panorama", "VClientEntityList003");
    InputSystem* inputSystem = find<InputSystem>(L"inputsystem", "InputSystemVersion001");
    Surface* surface = find<Surface>(L"vguimatsurface", "VGUI_Surface031");
private:
    template <typename T>
    static auto find(const wchar_t* module, const char* name) noexcept
    {
        if (const auto createInterface = reinterpret_cast< std::add_pointer_t<T* __cdecl (const char* name, int* returnCode)> >(GetProcAddress(GetModuleHandleW(module), "CreateInterface")))
            if (T* foundInterface = createInterface(name, nullptr))
                return foundInterface;

        MessageBoxA(nullptr, (std::ostringstream{ } << "Failed to find " << name << " interface!").str().c_str(), "GOESP", MB_OK | MB_ICONERROR);
        std::exit(EXIT_FAILURE);
    }
};

inline std::unique_ptr<const Interfaces> interfaces;