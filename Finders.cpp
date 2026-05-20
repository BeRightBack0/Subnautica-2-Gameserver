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

    results.push_back(Memcury::Scanner::FindStringRef(L"Reason for Showing/Hiding LoadingScreen is unknown!").GetFunctionStart());
    results.push_back(Memcury::Scanner::FindPattern("40 55 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 80 B9 ? ? ? ? ? 48 8B F1 0F 85").GetFunctionStart());

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
    results.push_back(Memcury::Scanner::FindPattern("48 83 EC ? 48 8B 01 FF 90 ? ? ? ? 84 C0 74 ? 33 C0").GetFunctionStart()); // the second getnetmode 

    return results;
}