#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>

// przeci¹¿enie dla tekstur
inline bool loadFromResource(sf::Texture& resource, int resourceId, const std::wstring& type = L"PNG") {
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceId), type.c_str());
    if (!hResource) return false;

    HGLOBAL hMemory = LoadResource(NULL, hResource);
    if (!hMemory) return false;

    DWORD dwSize = SizeofResource(NULL, hResource);
    LPVOID lpData = LockResource(hMemory);

    return resource.loadFromMemory(lpData, dwSize);
}

// przeci¹¿enie dla czcionek
inline bool loadFromResource(sf::Font& resource, int resourceId, const std::wstring& type = L"TTF") {
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceId), type.c_str());
    if (!hResource) return false;

    HGLOBAL hMemory = LoadResource(NULL, hResource);
    if (!hMemory) return false;

    DWORD dwSize = SizeofResource(NULL, hResource);
    LPVOID lpData = LockResource(hMemory);

    return resource.openFromMemory(lpData, dwSize);
}