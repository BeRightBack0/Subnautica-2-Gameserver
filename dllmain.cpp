// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "NetDriverEOS.h"
#include "Finders.h"
// TODO
// gIsServer and gIsClient finder
// all the spam log finders to disable log from them
ENetMode GetNetMode() {
    return ENetMode::NM_DedicatedServer;
}

// it actually might be useless but whatever
DefineOriginal(__int64, CreateGameModeForURL, __int64* a1, const FURL& a2, __int64* a3);
__int64 CreateGameModeForURL(__int64* GameInstance, const FURL& URL, __int64* InWorld) {
    FURL url{};
    url.Protocol = URL.Protocol;
    url.Host = FString(L"127.0.0.1");
    url.Port = URL.Port;
    url.Valid = URL.Valid;
    url.Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
    url.RedirectURL = URL.RedirectURL;
    FString ipOption = FString(L"bUseIPSockets");
    url.Op.Add(ipOption);

   return CreateGameModeForURLOG(GameInstance, url, InWorld);
}

DefineOriginal(bool, LoadMap, __int64* Engine, __int64& WorldContext, FURL URL, __int64* Pending, FString& Error);
bool LoadMap(__int64* Engine, __int64& WorldContext, FURL URL, __int64* Pending, FString& Error) {

    URL.Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
   return LoadMapOG(Engine, WorldContext, URL, Pending, Error);
}

void Main() {
    AllocConsole();
    MH_Initialize();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    freopen_s(&File, "CONOUT$", "w+", stderr);
    Sleep(3000);
    Memcury::Scanner::SetTargetModule("Subnautica2-Win64-Shipping.exe");

    // we patch them first bc it loves to crash at the start
    for (auto nullfunc : Finders::FindNullFuncs()) {
        NullHook(nullfunc);
    }

    // making finder for these bytes are gonna be so ahhh
    // "AllowCommandletRendering" at the top of the func there will be 2 bytes set to 1
    *(bool*)(ImageBase + 0xCC28A42) = false; // gisclient
    *(bool*)(ImageBase + 0xCC28A43) = true; // gisserver


    /*
    *(int*)(ImageBase + 0xCE78798) = 7; // random ass log for listening
    *(int*)(ImageBase + 0xCF118F8) = 1; // ass log that is spammed when u inject late 
    *(int*)(ImageBase + 0xD0E5C18) = 1; // gay loading screen spam bc im unproper for the loading screen handling
    *(int*)(ImageBase + 0xCF12378) = 1; // LogUWEAssetRegistrySubsystem
    *(int*)(ImageBase + 0xCD1CA90) = 1; // LogSlate
    *(int*)(ImageBase + 0xD0F4A20) = -1; // LogFMOD
    *(int*)(ImageBase + 0xCF0E290) = -1; // LogEOSSDK */

    NetDriverEOS::Hook();
    for (auto addr : Finders::FindNetModes()) {
        DetourHook(addr, GetNetMode, nullptr);
    }

    DetourHook(Finders::FindCreateGameModeForURL(), CreateGameModeForURL, (void**)&CreateGameModeForURLOG);
    DetourHook(Finders::FindLoadMap(), LoadMap, (void**)&LoadMapOG);
}
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == 1)
    {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, nullptr, 0, nullptr);
    }

    return TRUE;
}
