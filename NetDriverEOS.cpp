#include "pch.h"
#include "NetDriverEOS.h"
#include "Finders.h"

// "Unable to find socket subsystem"
bool NetDriverEOS::InitBase(__int64 NetDriverEOS, bool bInitAsClient, __int64 InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) {
    // NetDriverEOS->bIsPassthrough // that will make the netdriver eos use the base logic and fully skip eos
    *reinterpret_cast<int*>(NetDriverEOS + 0x9A8) = 1; // theres no way they will change it 

    FURL url{};
    url.Protocol = URL.Protocol;
    url.Host = FString(L"127.0.0.1");
    url.Port = URL.Port;    
    url.Valid = URL.Valid;
    url.Map = FString(L"/Game/Maps/Main/L_Main.L_Main");
    url.RedirectURL = URL.RedirectURL;
    url.Op = URL.Op;

    return IpNetDriverInitBase(NetDriverEOS, false, InNotify, url, bReuseAddressAndPort, Error);
}
bool NetDriverEOS::InitListen(__int64 NetDriverEOS, __int64 InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) {
    SetConsoleTitleA("Listening on port 7777"); // Game Instance has been notified that Game Mode hooking that might be better 
    return IpNetDriverInitListen(NetDriverEOS, InNotify, LocalURL, bReuseAddressAndPort, Error);
}


void NetDriverEOS::Hook() {

    DetourHook(Finders::FindEOSInitBase(), InitBase, nullptr);
    DetourHook(Finders::FindEOSInitListen(), InitListen, nullptr);
}