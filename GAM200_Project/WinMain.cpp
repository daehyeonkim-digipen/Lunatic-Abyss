/*
File name: WinMain.cpp
Project name: Dice Hero
Author(s): Rudy Castan
All content © 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include <exception>

#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <Windows.h>

int main(void);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
    try
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    return main();
}
catch ([[maybe_unused]]std::exception& e)
{
    //MessageBox(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
    return -1;
}
 