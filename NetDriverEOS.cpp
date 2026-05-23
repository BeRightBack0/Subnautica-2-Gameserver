#include "pch.h"
#include "NetDriverEOS.h"
#include "Finders.h"

// "Unable to find socket subsystem"
bool NetDriverEOS::InitBase(__int64 NetDriverEOS, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) {
    *(int*)(NetDriverEOS + 0x9A8) = 1; // that offset will never change prob

    FURL url{};
    url.Protocol = URL.Protocol;
    url.Host = FString(L"127.0.0.1");
    url.Port = URL.Port;
    url.Valid = URL.Valid;
    url.Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
    url.RedirectURL = URL.RedirectURL;
    url.Op = URL.Op;
    url.Op.AddGrow(FString(L"listen"));

    return IpNetDriverInitBase(NetDriverEOS, false, InNotify, url, bReuseAddressAndPort, Error);
}
bool NetDriverEOS::InitListen(__int64 NetDriverEOS, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) {
    SetConsoleTitleA("Listening on port 7777"); // Game Instance has been notified that Game Mode hooking that might be better 
    LocalURL.Op.AddGrow(FString(L"listen"));

    return IpNetDriverInitListen(NetDriverEOS, InNotify, LocalURL, bReuseAddressAndPort, Error);
}
DefineOriginal(__int64, bruh, __int64* a1);
__int64 bruh(__int64* a1) {
    SetConsoleTitleA("Listening on port 7777");
    return bruhOG(a1);
}

void NetDriverEOS::Hook() {

    DetourHook(Finders::FindEOSInitBase(), InitBase, nullptr);
    DetourHook(Finders::FindEOSInitListen(), InitListen, nullptr);
    DetourHook(ImageBase + 0x61CCA70, bruh, (void**)&bruhOG); // make a finder for that later
}