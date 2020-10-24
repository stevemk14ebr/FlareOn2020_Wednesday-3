#include <Windows.h>
#include <iostream>
#include <memory>

#ifdef _WIN64
#define _PTR_MAX_VALUE ((PVOID)0x000F000000000000)
#else
#define _PTR_MAX_VALUE ((PVOID)0xFFF00000)
#endif
__forceinline bool IsValidPtr(PVOID p) { return (p >= (PVOID)0x10000) && (p < _PTR_MAX_VALUE) && p != nullptr; }

class ConOut : public std::streambuf {
public:
    ConOut() {
        setp(0, 0);
    }

    virtual int_type overflow(int_type c = traits_type::eof()) {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

static ConOut ob;
std::streambuf* origCppStdout = nullptr;

void CreateConsole()
{
    // freopen_s alone fails for C++ streams
    // https://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows
    AttachConsole(ATTACH_PARENT_PROCESS);

    // redirect C streams
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    // re-direct C++ streams to use C apis
    origCppStdout = std::cout.rdbuf(&ob);
}

void DestroyConsole() {
    FreeConsole();

    // restore C++ stream output, C stuff is fine
    std::cout.rdbuf(origCppStdout);
}

#include "DudeSdk.h"
bool EntityHasTag(Entity* pEnt, const char* tag) {
    SeqNimString* pTags = pEnt->m_Tags;
    if (!IsValidPtr(pTags))
        return false;

    for (uint32_t i = 0; i < pTags->Length; i++) {
        if (_strcmpi(pTags->m_strings[i]->Data, tag) == 0)
            return true;
    }
    return false;
}

void keydown(uint8_t vk) {
    keybd_event(vk, MapVirtualKeyW(vk, MAPVK_VK_TO_VSC), 0, 0);
}

void keyup(uint8_t vk) {
    keybd_event(vk, MapVirtualKeyW(vk, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
}

void delaykey() {
    uint32_t delay = 0;
    SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0);
    Sleep(delay / 2); // sleep half the time required so we're not too slow for next press
}

Entity* getClosestEntWTag(Entity* source, SeqEntity* pEntities, const char* Tag) {
    if (!IsValidPtr(pEntities))
        return nullptr;

    double closest = 9999999.0;
    Entity* pFinalEnt = nullptr;
    for (uint32_t i = 0; i < pEntities->m_Length; i++) {
        Entity* pEnt = pEntities->m_entities[i];
        if (!IsValidPtr(pEnt))
            continue;

        if (!EntityHasTag(pEnt, Tag))
            continue;

        double diff = abs(pEnt->m_posX - source->m_posX);
        if (diff < closest) {
            closest = diff;
            pFinalEnt = pEnt;
        }
    }
    return pFinalEnt;
}

HMODULE DllInst = 0;
DWORD WINAPI InitThread(LPVOID lparam)
{
    CreateConsole();

    Game* pGame = nullptr;
    while (true) {
        if (!IsValidPtr(p_GAME))
            continue;

        pGame = *p_GAME;
        if (!IsValidPtr(pGame))
            continue;

        printf("Game at: %X\n", pGame);
        printf("Waiting for main scene\n");
        break;
    }

    while (true) {
        Scene* pScene = pGame->m_fscene;
        if (!IsValidPtr(pScene) || pScene->m_Type->Value != MAIN_SCENE_TYPE)
            continue;

        SeqEntity* pEntities = pScene->fList;
        if (!IsValidPtr(pEntities))
            continue;

        Entity* pPlayer = pScene->m_PlayerEntity;
        if (!IsValidPtr(pPlayer))
            continue;

       
        Entity* pCheckEnt = getClosestEntWTag(pPlayer, pEntities, "check");
        if (!IsValidPtr(pCheckEnt))
            continue;

        double xDiff = abs(pCheckEnt->m_posX - pPlayer->m_posX);
        if (xDiff < 80) {
            if (pCheckEnt->m_MoveState == MoveState::Duck) {
                printf("Duck!\n");
                keydown(VK_DOWN);
                delaykey();
                Sleep(750);
            } else if (pCheckEnt->m_MoveState == MoveState::Jump) {
                printf("Jump\n!");
                keydown(VK_UP);
                delaykey();
                Sleep(750);
            }
        } else {
            printf("Release!\n");
            if (pPlayer->m_MoveState == MoveState::Duck) {
                keyup(VK_DOWN);
                delaykey();
            } else if (pPlayer->m_MoveState == MoveState::Jump) {
                keyup(VK_UP);
                delaykey();
            }
        }
    }
    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        DllInst = hModule;
        HANDLE hThread = CreateThread(NULL, NULL, InitThread, NULL, NULL, NULL);
        CloseHandle(hThread);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

