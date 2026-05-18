// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

int ReturnOne() {
    return 1;
}

enum ENetMode  
{
    NM_Standalone,  
    NM_DedicatedServer,  
    NM_ListenServer,
    NM_Client,  
    NM_MAX,  
};

// "Unable to find socket subsystem"
static bool (*IpNetDriverInitBase)(__int64, bool, __int64, const FURL&, bool, FString&) = decltype(IpNetDriverInitBase)(ImageBase + 0x52C1340);

DefineOriginal(bool, NetDriverEOSInitBase, __int64 a1, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error);
bool NetDriverEOSInitBase(__int64 a1, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) {
    // NetDriverEOS->bIsPassthrough
    *reinterpret_cast<int*>(a1 + 0x9A8) = 1;

    FURL url{};
    url.Protocol = URL.Protocol;
    url.Host = FString(L"127.0.0.1");
    url.Port = URL.Port;
    url.Valid = URL.Valid;
    url.Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
    url.RedirectURL = URL.RedirectURL;
    url.Op = URL.Op;


    return IpNetDriverInitBase(a1, false, InNotify, url, bReuseAddressAndPort, Error);
}
// "%s IpNetDriver listening on port %i"
static bool (*IpNetDriverInitListen)(__int64, __int64, FURL&, bool, FString&) = decltype(IpNetDriverInitListen)(ImageBase + 0x52C2B30);
DefineOriginal(bool, NetDriverEOSInitListen, __int64 self, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error);
bool NetDriverEOSInitListen(__int64 self, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) {
    SetConsoleTitleA("Listening on port 7777"); // Game Instance has been notified that Game Mode hooking that might be better 
    return IpNetDriverInitListen(self, InNotify, LocalURL, bReuseAddressAndPort, Error);
}
// it actually might be useless but whatever
DefineOriginal(__int64, CreateGameModeForURL, UGameInstance* a1, const FURL& a2, UWorld* a3);
__int64 CreateGameModeForURL(UGameInstance* a1, const FURL& a2, UWorld* a3) {
    FURL url{};
    url.Protocol = a2.Protocol;
    url.Host = FString(L"127.0.0.1");
    url.Port = a2.Port;
    url.Valid = a2.Valid;
    url.Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
    url.RedirectURL = a2.RedirectURL;
    FString ipOption = FString(L"bUseIPSockets");
    url.Op.Add(ipOption);

   return CreateGameModeForURLOG(a1, url, a3);
}

DefineOriginal(bool, LoadMap, UEngine* Engine, FWorldContext& WorldContext, FURL URL, UPendingNetGame* Pending, FString& Error);
bool LoadMap(UEngine* Engine, FWorldContext& WorldContext, FURL URL, UPendingNetGame* Pending, FString& Error) {

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


    // "AllowCommandletRendering" at the top of the func there will be 2 bytes set to 1
    *(bool*)(ImageBase + 0xCC24A42) = false; // gisclient
    *(bool*)(ImageBase + 0xCC24A43) = true; // gisserver

    *(int*)(ImageBase + 0xCE78798) = 7; // random ass log for listening
    *(int*)(ImageBase + 0xCF118F8) = 1; // ass log that is spammed when u inject late 
    *(int*)(ImageBase + 0xD0E5C18) = 1; // gay loading screen spam bc im unproper for the loading screen handling
    *(int*)(ImageBase + 0xCF12378) = 1; // LogUWEAssetRegistrySubsystem
    *(int*)(ImageBase + 0xCD1CA90) = 1; // LogSlate
    *(int*)(ImageBase + 0xD0F4A20) = 0; // LogFMOD

   // UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), L"net.AllowEncryption 0", nullptr); // we use it on fortnite so idk i will need to check if i can join without 
    Hook(ImageBase + 0x47088C0, ReturnOne, nullptr); // GetNetMode is not inlined "listen"
    Hook(ImageBase + 0x41F72F0, ReturnOne, nullptr); // actually no idea but related to netmode // 48 83 EC ? 48 8B 01 FF 90 ? ? ? ? 84 C0 74 ? 33 C0 // its in the main netmode func its called like atleast 2 times

    // idk if its actually needed but whatever
    Hook(ImageBase + 0x3F84900, CreateGameModeForURL, (void**)&CreateGameModeForURLOG);

    NullHook(ImageBase + 0x149EDA0); // RequestExit
    NullHook(ImageBase + 0x148CFF0); // second RequestExit
    NullHook(ImageBase + 0x1637F40); // random ass exit func that prob doesnt need to be hooked anyway "Executing StaticShutdownAfterError"
    NullHook(ImageBase + 0x5AEB3E0); // first loading screen crash  "Showing loading screen when 'IsShowingInitialLoadingScreen()' is true."
    NullHook(ImageBase + 0x5AEA630); // second loading screen crash "Reason for Showing/Hiding LoadingScreen is unknown!"

    Hook(ImageBase + 0x5323FB0, NetDriverEOSInitBase, nullptr);
    Hook(ImageBase + 0x53246A0, NetDriverEOSInitListen, nullptr);
    Hook(ImageBase + 0x47AFEE0, LoadMap, (void**)&LoadMapOG);
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
