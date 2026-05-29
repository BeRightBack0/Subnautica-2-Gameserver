// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <cstring>
#include <string>
#include <thread>
#include <random>
#include <numeric>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "SDK/SDK/Basic.hpp"
#include "SDK/SDK/CoreUObject_structs.hpp"
#include "SDK/SDK/Subnautica2_classes.hpp"
#include "SDK/SDK/CoreUObject_classes.hpp"
#include "SDK/SDK/Engine_structs.hpp"
#include "SDK/SDK/Engine_classes.hpp"
#include "SDK/SDK/GameplayAbilities_structs.hpp"
#include "SDK/SDK/GameplayAbilities_classes.hpp"
#include "SDK/SDK/AssetRegistry_classes.hpp"
#include "SDK/SDK/BPC_BlightCoreRewardButton_classes.hpp"
#include "MinHook.h"
#include "memcury.h"
#include <set>
#include <atomic>
#define NAME_None FName(0)

using namespace SDK;
using namespace std;
inline uint64_t ImageBase = (uint64_t)GetModuleHandle(nullptr);
#define DefineOriginal(_Rt, _Name, ...) static inline _Rt (*_Name##OG)(##__VA_ARGS__); static _Rt _Name(##__VA_ARGS__);
#define DefineCall(_Rt, _Name, _Offset, ...) static _Rt (*_Name)(##__VA_ARGS__) = reinterpret_cast<_Rt(*)(__VA_ARGS__)>(_Offset)

inline void nullfunc() {
    return;
}

enum ENetMode
{
    NM_Standalone,
    NM_DedicatedServer,
    NM_ListenServer,
    NM_Client,
    NM_MAX,
};
inline UWorld* GetWorld()
{
    return UWorld::GetWorld();
}
inline void DetourHook(uintptr_t Address, void* Detour, void** Original) {
    MH_CreateHook((LPVOID)Address, Detour, Original);
    MH_EnableHook((LPVOID)Address);
}

inline void NullHook(uintptr_t Address) {

     MH_CreateHook((LPVOID)Address, nullfunc, nullptr);
     MH_EnableHook((LPVOID)Address);
}

template<typename T>
inline void VFTHook(int index, void* detour, void** original = nullptr)
{
	void* object = T::GetDefaultObj();
	if (!object) return;

	void** vtable = *(void***)object;
	if (!vtable || !vtable[index])
		return;

	if (original)
		*original = vtable[index];

	DWORD oldProtect;
	if (VirtualProtect(&vtable[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		vtable[index] = detour;
		VirtualProtect(&vtable[index], sizeof(void*), oldProtect, &oldProtect);
	}
}

template<typename T>
inline void VFTHookEvery(int index, void* detour, void** original = nullptr)
{
    for (int i = 0; i < UObject::GObjects->Num(); i++) {
        UObject* Object = UObject::GObjects->GetByIndex(i);

        if (!Object) continue;

        if (Object->IsA(T::StaticClass())) {
            void** vtable = *(void***)Object;
            if (!vtable || !vtable[index])
                continue;

            DWORD oldProtect;
            if (VirtualProtect(&vtable[index], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
            {
                if (original && !*original)
                    *original = vtable[index];

                vtable[index] = detour;
                VirtualProtect(&vtable[index], sizeof(void*), oldProtect, &oldProtect);
            }
        }
    }
}
#endif //PCH_H
