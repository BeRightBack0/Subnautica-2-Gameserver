#include "pch.h"
#include "Finders.h"

uintptr_t Finders::FindEOSInitBase()
{
    return Memcury::Scanner::FindStringRef(L"Could not bind local address").GetFunctionStart();
}

uintptr_t Finders::FindEOSInitListen()
{
    return Memcury::Scanner::FindStringRef(L"Could not listen").GetFunctionStart();
}

uintptr_t Finders::FindNetDriverInitBase()
{
    return Memcury::Scanner::FindStringRef(L"IpNetDriver Receive Thread: %s").GetFunctionStart();
}

uintptr_t Finders::FindNetDriverInitListen()
{
    return Memcury::Scanner::FindPattern("48 8D 1D ? ? ? ? 74 ? 48 8B 3F EB ? 48 8B FB 45 33 C0 48 8D 54 24 ? 48 8B CE E8 ? ? ? ? 83 78 ? ? 74 ? 48 8B 18 4C 8B CF 48 8D 15 ? ? ? ? 4C 8B C3 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 4C 24 ? 48 85 C9 74 ? E8 ? ? ? ? 32 C0").GetFunctionStart();
}

uintptr_t Finders::FindCreateGameModeForURL()
{
    return Memcury::Scanner::FindStringRef(L"LoadForAllGameModes").GetFunctionStart(); 
}
// all of the loading screen nullfuncs
std::vector<uintptr_t> Finders::FindNullFuncs()
{
    std::vector<uintptr_t> results;

  // results.push_back(Memcury::Scanner::FindStringRef(L"Reason for Showing/Hiding LoadingScreen is unknown!").GetFunctionStart());
   // results.push_back(Memcury::Scanner::FindPattern("40 55 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B F1 0F 85").GetFunctionStart());
    results.push_back(Memcury::Scanner::FindPattern("40 55 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B F1 0F 85").GetFunctionStart()); // loading screen crashes not proper very scuffed
    results.push_back(Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B DA 48 8B F9 E8 ? ? ? ? 48 8B 8B ? ? ? ? 48 85 C9").GetFunctionStart()); // i have no fucking idea what it is
    results.push_back(Memcury::Scanner::FindPattern("40 53 48 83 EC ? 80 3D ? ? ? ? ? 0F B6 D9 72 ? 48 85 D2").GetFunctionStart()); // requestexit we love steam (string ref should work too)
    results.push_back(Memcury::Scanner::FindPattern("40 55 53 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 80 3D").GetFunctionStart()); // gameserver tries to render something with -nullrhi nice
    results.push_back(Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 49 8B D8 48 8B F2 8B E9").GetFunctionStart()); // thats the message box when u run with nosteam
    results.push_back(Memcury::Scanner::FindPattern("40 55 53 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B D9 4D 8B F1").GetFunctionStart()); // i actually forgot what it is its localplayer vft index

    return results;
}

uintptr_t Finders::FindLoadMap()
{
    return Memcury::Scanner::FindStringRef(L"Loading Map %s").GetFunctionStart();
}

std::vector<uintptr_t> Finders::FindNetModes()
{
    std::vector<uintptr_t> results;

    results.push_back(Memcury::Scanner::FindPattern("75 ? 48 8B 8B ? ? ? ? E8 ? ? ? ? 84 C0 74 ? 48 8B 83").GetFunctionStart()); // main/internal getnetmode 
    //results.push_back(Memcury::Scanner::FindStringRef(L"listen").GetFunctionStart()); i would need to figure out to get all xrefs of listen string or smth like that idk maybe zydis?
    //results.push_back(Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 01 FF 90 ? ? ? ? 84 C0 74 ? 33 C0").GetFunctionStart()); // the second getnetmode 

    return results;
}
uintptr_t Finders::FindScuffness() {
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 48 8D 35").GetFunctionStart();
}
uintptr_t Finders::FindProperium() {
    return Memcury::Scanner::FindPattern("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8D 99 ? ? ? ? 4C 8B F9 48 8B CB").GetFunctionStart();
}
// scuffness but works for that ue version
uintptr_t Finders::FindGIsClient() {
    auto ResolveByteAddr = [](uintptr_t instrAddr) -> uintptr_t {
        int32_t disp = *(int32_t*)(instrAddr + 2);
        return instrAddr + 6 + disp;
    };

    auto funcStart = Memcury::Scanner::FindStringRef(L"AllowCommandletRendering").GetFunctionStart();

    auto scuffness = Memcury::Scanner(funcStart).ScanFor({ 0xC6, 0x05 }).Get();

    return ResolveByteAddr(scuffness);
}

uintptr_t Finders::FindPasting() {
    return Memcury::Scanner::FindPattern("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 85 C0 74 ? E8").GetFunctionStart();
}