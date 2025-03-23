// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "windows.h"
#include "stdio.h"

#include <iostream>
#include <pdh.h>

#include "detours/detours.h"

#include "TypeNames.h"
#include <string>
#include "dllmain.h"

#include "curl/curl.h"
#include <fstream>
#include <vector>

int lua_state = NULL;
bool isReady = false;

DWORD* gScriptContext = nullptr;

// ARGS:
// render(path, x, y, url) (image gets POST to the url, set the url to a random VALID url if you don't want to do that)
int Render(uintptr_t L)
{
    size_t length = 0;
    const char* path = Lua::lua_checklstring(L, 1, &length);

    std::cout << path << std::endl;

    int x = Lua::lua_checkint(L, 2);
    int y = Lua::lua_checkint(L, 3);

    bool isGonnaPost = true;

	const char* url = Lua::lua_checklstring(L, 4, &length);

    CURLU* _curl = curl_url();
    CURLUcode result = curl_url_set(_curl, CURLUPART_URL, url, 0);

    if (result != CURLE_OK) {
        std::cout << "the url you entered is invalid" << std::endl;
        curl_easy_cleanup(_curl);
        return 0;
    }

    curl_easy_cleanup(_curl);
    
    HWND window = FindWindowA(nullptr, "Roblox"); // this doesn't matter.
    SetWindowPos(window, 0, 0, 0, x, y, 0);
    OSContext context{ window , x , y };

    DWORD workspace = WorkspaceFuncs::findWorkspace(gScriptContext);
    DWORD datamodel = *(DWORD*)(workspace + 0x38);

    int singleton = CRenderSettings::GetRenderSettings() + 0x84;
    *(BYTE*)(singleton + 0xE1) = 0;

    // NOTE: IF YOU DO NOT USE OPENGL AND USE DIRECT3D9 INSTEAD, THE SKY WILL BE INVISIBLE. USE OPENGL.
    // ^ and do not use automatic graphics mode, set it to opengl.
    DWORD viewBase = ViewBase::CreateView(OpenGL, &context, singleton);

    Workspace::setImageServerView((DWORD*)workspace, 0, 1, (DWORD*)(workspace + 0x160));
    Lighting::setupLighting((DWORD*)datamodel, 0);
    BindW::bindWorkspace((DWORD*)viewBase, 0, datamodel, 0);
    RenderView::renderThumb((DWORD*)viewBase, 0);
    SaveFile::saveRenderTarget((DWORD*)viewBase, 0, path); // you could also use the screenshot function however that yields lower quality results

    std::cout << "saved!" << std::endl;

    Sleep(500); // optional, but keeping incase.

    ViewBase::DeleteView((DWORD*)viewBase + 2, nullptr, 1);

    std::ifstream file(path, std::ios::binary);
    std::vector<char> fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // curl
    //https://stackoverflow.com/questions/9453996/sending-an-image-to-ftp-server-from-gchar-buffer-libcurl
    CURL* curl = curl_easy_init(); // robux engine
    
    if (isGonnaPost == false) {
        return 0;
        std::cout << "done!!!!!!!!!!" << std::endl;
    }

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L); // this enables uploading
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fileData.data());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fileData.size());

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        std::cout << "finished sending" << std::endl;

    }
    else {
        std::cout << "error happeneed with curl";
    }

    return 0;

}

int Close(uintptr_t L) {
    exit(1);
    return 0;
}

void __fastcall openStateHook(DWORD* thisPtr)
{
    std::cout << thisPtr << std::endl;
    ScriptContext::OpenState(thisPtr);
    if (gScriptContext != thisPtr) {
        gScriptContext = thisPtr;

        uintptr_t globalState = thisPtr[34]; // offset
        Lua::lua_pushcclosure(globalState, Render, 0);
        Lua::lua_setfield(globalState, -10002, "render"); 
        Lua::lua_pushcclosure(globalState, Close, 0);
        Lua::lua_setfield(globalState, -10002, "close");
        std::cout << "pushed lua functions" << std::endl;
    }

}

//makes the exe start really fast
PDH_HQUERY* __fastcall CProcessPerfCounterHook(void* thisPtr)
{
    static PDH_HQUERY dummy = nullptr;
    return &dummy;
}

__declspec(dllexport) void __cdecl paracetamol(bool) {};//import

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
        SetConsoleTitle(L"RbxRenderHook");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach(&(PVOID&)ScriptContext::OpenState, openStateHook);
        DetourAttach(&(PVOID&)CProcessPerfCounter::CProcessPerfCounter, CProcessPerfCounterHook);

        DetourTransactionCommit();

        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

