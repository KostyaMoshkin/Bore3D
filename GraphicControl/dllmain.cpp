// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"



static AFX_EXTENSION_MODULE GraphicControlDLL = { NULL, NULL };

extern HMODULE g_hModule = 0;
extern HMODULE g_hResource = 0;


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (!AfxInitExtensionModule(GraphicControlDLL, hModule))
            return 0;

        g_hModule = GraphicControlDLL.hModule;
        g_hResource = GraphicControlDLL.hResource;

        new CDynLinkLibrary(GraphicControlDLL);
    }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        AfxTermExtensionModule(GraphicControlDLL);
        break;
    }
    return TRUE;
}

