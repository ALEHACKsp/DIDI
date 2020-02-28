#pragma once

#include <cstdint>
#include <d3d9.h>
#include <memory>
#include <Windows.h>
#include <Psapi.h>
#include <sstream>
#include <type_traits>
#include "SDK/Vector.h"

class Entity;
struct GlobalVars;

class Memory {
public:
    Memory() noexcept;

    uintptr_t present;
    uintptr_t reset;
    uintptr_t setCursorPos;

    bool(__thiscall* isOtherEnemy)(Entity*, Entity*);
    std::add_pointer_t<bool __cdecl(Vector, Vector, short) noexcept> lineGoesThroughSmoke;
    const GlobalVars* globalVars;
    std::add_pointer_t<void __cdecl(const char* msg, ...)> debugMsg;
private:
    template <typename T = uintptr_t>
    static std::uintptr_t findPattern(const wchar_t* module, const char* pattern, size_t offset = 0) noexcept
    {
        static auto id = 0;
        ++id;

        if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(module), &moduleInfo, sizeof(moduleInfo))) {
            auto start = static_cast<const char*>(moduleInfo.lpBaseOfDll);
            const auto end = start + moduleInfo.SizeOfImage;

            auto first = start;
            auto second = pattern;

            while (first < end && *second) {
                if (*first == *second || *second == '?') {
                    ++first;
                    ++second;
                }
                else {
                    first = ++start;
                    second = pattern;
                }
            }

            if (!*second)
                return reinterpret_cast<std::uintptr_t>(const_cast<char*>(start) + offset);
        }
        MessageBoxW(NULL, (std::wostringstream{ } << L"Failed to find pattern in " << module << L'!').str().c_str(), L"DIDI", MB_OK | MB_ICONERROR);
        std::exit(EXIT_FAILURE);
    }
};

inline std::unique_ptr<Memory> memory;
