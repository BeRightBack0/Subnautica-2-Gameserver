// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


int ReturnOne() {
    return 1;
}
void returan() {
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

void CopyToClipboard(const std::string& text)
{
    if (!OpenClipboard(nullptr)) return;
    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hMem) { CloseClipboard(); return; }

    memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
    GlobalUnlock(hMem);

    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

namespace EBrowseReturnVal
{
    enum Type
    {
        /** Successfully browsed to a new map. */
        Success,
        /** Immediately failed to browse. */
        Failure,
        /** A connection is pending. */
        Pending,
    };
}

DefineOriginal(EBrowseReturnVal::Type, sub_479DE10, UEngine* Engine, FWorldContext& WorldContext, FURL URL, FString& Error);
// uengine::browse called like a milion times on the client
EBrowseReturnVal::Type __fastcall sub_479DE10(UEngine* Engine, FWorldContext& WorldContext, FURL URL, FString& Error) {

    URL.Map = FString(L"/Game/Maps/Main/L_Main");
    URL.Protocol = FString(L"unreal");
  //  URL.Host = FString(L"127.0.0.1");
    URL.Port = 7777;

   return sub_479DE10OG(Engine, WorldContext, URL, Error);
}

bool jew() {
    std::cout << "called yo" << std::endl;
    return false;
}

DefineOriginal(UNetDriver*,  jewdriver, UEngine* a1, FWorldContext* a2, FName a3, FName a4);
UNetDriver* jewdriver(UEngine* a1 , FWorldContext* a2, FName a3, FName a4) {

    FName GameNetDriver = SDK::UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");


    return jewdriverOG(a1, a2, GameNetDriver, GameNetDriver);
}

void Main() {
    AllocConsole();
    MH_Initialize();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    freopen_s(&File, "CONOUT$", "w+", stderr);
    freopen_s(&File, "D:\\log.txt", "w", stdout);
    freopen_s(&File, "D:\\log.txt", "w", stderr);

    *(bool*)(ImageBase + 0xCC24A42) = false; // gisclient
    *(bool*)(ImageBase + 0xCC24A43) = true; // gisserver

    *(int*)(ImageBase + 0xCE78798) = 7; // random ass log for listening
    *(int*)(ImageBase + 0xCF118F8) = 1; // ass log that is spammed when u inject late 

   // UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), L"net.AllowEncryption 0", nullptr); // we use it on fortnite so idk we will use it here
    Hook(ImageBase + 0x47088C0, ReturnOne, nullptr); // GetNetMode is not inlined

  //  //auto Gamemode = (AUWELobbyGameMode*)UGameplayStatics::GetGameMode(GetWorld());
 //   Gamemode->StartNewServerGame(ASN2WorldGameMode::StaticClass());

    NullHook(ImageBase + 0x149EDA0);
    NullHook(ImageBase + 0x148CFF0);


    Hook(ImageBase + 0x47A3E50, jewdriver, (void**)&jewdriverOG);
   // UUWEGameModeTypeStatics

  //  Hook(ImageBase + 0x47AFEE0, jews, (void**)&jewsOG); // uengine::loadmap
    //Hook(ImageBase + 0x479DE10, sub_479DE10, (void**)&sub_479DE10OG);
   // Hook(ImageBase + 0x5AEB3E0, returan, nullptr);
    //VFTHook<AGameMode>(0x950 / 8, jew, nullptr); // ready to start match 

  //  VFTHookEvery<UGameInstance>(0x5F, ReturnOne);


    MH_EnableHook(MH_ALL_HOOKS);
  //  UGameEngine::GetEngine()->GameInstance->LocalPlayers.Remove(0);
   // UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), L"open /Game/Maps/Main/L_Main", nullptr);
    // switch or open level might be better ngl



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
