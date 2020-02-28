#pragma once

#include "ClientClass.h"
#include "EngineTrace.h"
#include "EntityList.h"
#include "../Interfaces.h"
#include "../Memory.h"
#include "Utils.h"
#include "WeaponId.h"

struct Vector;
struct WeaponInfo;
class Matrix3x4;

class Collideable {
virtual void* pad() = 0;
public:
    virtual const Vector& obbMins() = 0;
    virtual const Vector& obbMaxs() = 0;
};

struct Model {
    void* handle;
    char name[260];
};

#define NETVAR(func_name, offset, type) \
std::add_lvalue_reference_t<type> func_name() noexcept \
{ \
    return *reinterpret_cast<std::add_pointer_t<type>>(this + offset); \
}

class Entity {
public:
    constexpr ClientClass* getClientClass() noexcept
    {
        return callVirtualMethod<ClientClass*>(this + 8, 2);
    }

    constexpr auto isDormant() noexcept
    {
        return callVirtualMethod<bool>(this + 8, 9);
    }

    constexpr auto getModel() noexcept
    {
        return callVirtualMethod<const Model*>(this + 4, 8);
    }

    constexpr auto getCollideable() noexcept
    {
        return callVirtualMethod<Collideable*>(this, 3);
    }

    constexpr auto getAbsOrigin() noexcept
    {
        return callVirtualMethod<Vector&>(this, 10);
    }

    constexpr auto isAlive() noexcept
    {
        return callVirtualMethod<bool>(this, 155) && health() > 0;
    }

    constexpr auto isWeapon() noexcept
    {
        return callVirtualMethod<bool>(this, 165);
    }

    constexpr auto getActiveWeapon() noexcept
    {
        return callVirtualMethod<Entity*>(this, 267);
    }

    auto getEyePosition() noexcept
    {
        Vector vec;
        callVirtualMethod<void, Vector&>(this, 284, vec);
        return vec;
    }

    constexpr auto getObserverTarget() noexcept
    {
        return callVirtualMethod<Entity*>(this, 294);
    }

    constexpr auto getWeaponInfo() noexcept
    {
        return callVirtualMethod<WeaponInfo*>(this, 459);
    }



    bool visibleTo(Entity* other) noexcept
    {
        Trace trace;
        interfaces->engineTrace->traceRay({ other->getEyePosition(), getEyePosition() }, 0x46004009, other, trace);
        return trace.entity == this || trace.fraction > 0.97f;
    }

    bool visible(Vector desPos) noexcept
    {
        Trace trace;
        interfaces->engineTrace->traceRay({getEyePosition(), desPos }, 0x46004009, { this }, trace);
        return trace.entity == this || trace.fraction > 0.97f;
    }

    constexpr bool setupBones(Matrix3x4* out, int maxBones, int boneMask, float currentTime) noexcept
    {
        return callVirtualMethod<bool, Matrix3x4*, int, int, float>(this + 4, 13, out, maxBones, boneMask, currentTime);
    }

    Vector getBonePosition(int bone) noexcept
    {
        if (Matrix3x4 boneMatrices[128]; setupBones(boneMatrices, 128, 256, 0.0f))
            return Vector{ boneMatrices[bone][0][3], boneMatrices[bone][1][3], boneMatrices[bone][2][3] };
        else
            return Vector{ };
    }

    NETVAR(weaponId, 0x2FAA, WeaponId)                                               // CBaseAttributableItem->m_iItemDefinitionIndex

    NETVAR(clip, 0x3244, int)                                                        // CBaseCombatWeapon->m_iClip1
    NETVAR(isInReload, 0x3244 + 0x41, bool)                                          // CBaseCombatWeapon->m_iClip1 + 0x41
    NETVAR(reserveAmmoCount, 0x324C, int)                                            // CBaseCombatWeapon->m_iPrimaryReserveAmmoCount
    NETVAR(nextPrimaryAttack, 0x3218, float)                                         // CBaseCombatWeapon->m_flNextPrimaryAttack

    NETVAR(index, 0x64, int)                                                         // CBaseEntity->m_bIsAutoaimTarget + 0x4
    NETVAR(ownerEntity, 0x14C, int)                                                  // CBaseEntity->m_hOwnerEntity

    NETVAR(aimPunchAngle, 0x302C, Vector)                                            // CBasePlayer->m_aimPunchAngle
    NETVAR(health, 0x100, int)                                                       // CBasePlayer->m_iHealth

    NETVAR(coordinateFrame, 0x444, Matrix3x4)

    NETVAR(gunGameImmunity, 0x3930, bool)

    NETVAR(flashDuration, 0xA410, float)



};
