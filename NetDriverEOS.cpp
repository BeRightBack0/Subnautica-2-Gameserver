#include "pch.h"
#include "NetDriverEOS.h"

// "Unable to find socket subsystem"
bool NetDriverEOS::InitBase(__int64 a1, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) {
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
bool NetDriverEOS::InitListen(__int64 a1, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) {
    SetConsoleTitleA("Listening on port 7777"); // Game Instance has been notified that Game Mode hooking that might be better 
    return IpNetDriverInitListen(a1, InNotify, LocalURL, bReuseAddressAndPort, Error);
}


void NetDriverEOS::Hook() {

    DetourHook(ImageBase + 0x5323FB0, InitBase, nullptr);
    DetourHook(ImageBase + 0x53246A0, InitListen, nullptr);
}