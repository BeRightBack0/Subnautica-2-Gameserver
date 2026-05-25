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
    url.Op.AddGrow(ipOption);

   return CreateGameModeForURLOG(GameInstance, url, InWorld);
}
DefineOriginal(bool, LoadMap, __int64* Engine, __int64& WorldContext, FURL* URL, __int64* Pending, FString& Error);
bool LoadMap(__int64* Engine, __int64& WorldContext, FURL* URL, __int64* Pending, FString& Error) {
    if (URL) {
        URL->Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
        URL->Op.AddGrow(FString(L"listen"));
    }
    return LoadMapOG(Engine, WorldContext, URL, Pending, Error);
}
inline char returna1() { return 1; }
inline __int64 returna2(__int64 a1, __int64 a2) { return a2; }


using namespace Memcury::ASM;
void Main() {
    AllocConsole();
    MH_Initialize();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    freopen_s(&File, "CONOUT$", "w+", stderr);

    Memcury::Scanner::SetTargetModule("Subnautica2-Win64-Shipping.exe"); // prob useless

    DetourHook(Finders::FindProperium(), returna1, nullptr);
    DetourHook(Finders::FindScuffness(), returna2, nullptr);

    for (auto nullfunc : Finders::FindNullFuncs()) {
        NullHook(nullfunc);
    }

    //Memcury::Util::CopyToClipboard(std::to_string(ImageBase));

    // making finder for these bytes are gonna be so ahhh
    // "AllowCommandletRendering" at the top of the func there will be 2 bytes set to 1
    
    
    *(bool*)(Finders::FindGIsClient()) = false; // gisclient
    *(bool*)(Finders::FindGIsClient() + 1) = true; // gIsServer

    //*(bool*)(ImageBase + 0xCC25A42) = false; // gisclient
  //  *(bool*)(ImageBase + 0xCC25A43) = true; // gisserver
    //*(int*)(ImageBase + 0xD0E6C18) = 0; // that was meant to be some ahh log but it doesnt care i guess
    for (auto addr : Finders::FindNetModes()) {
        DetourHook(addr, GetNetMode, nullptr);
    }

    NetDriverEOS::Hook();

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
