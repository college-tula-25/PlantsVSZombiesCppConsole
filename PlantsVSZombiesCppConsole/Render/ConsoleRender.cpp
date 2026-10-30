#include "ConsoleRender.h"
#include <iostream>
#include <algorithm>
#include <fcntl.h> // Для перевода консоли в UTF-16/UTF-8 режим на Windows

#ifdef _WIN32
#include <io.h>
#endif

ConsoleRender::ConsoleRender(int width, int height)
    : screenWidth(width), screenHeight(height),
    screenBuffer(height, std::vector<wchar_t>(width, L' ')),
    colorBuffer(height, std::vector<ConsoleColor>(width, ConsoleColor::White))
{
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT); // Включаем режим вывода широких строк для Windows
#else
    std::wcout.sync_with_stdio(false);
    std::wcout.imbue(std::locale("en_US.UTF-8")); // Настройка локали для Linux/macOS
#endif

    // Скрываем мигающий курсор с помощью ANSI-кода
    std::wcout << L"\x1b[?25l" << std::flush;
}

void ConsoleRender::ClearBuffer()
{
    for (int y = 0; y < screenHeight; ++y)
    {
        std::fill(screenBuffer[y].begin(), screenBuffer[y].end(), L' ');
        std::fill(colorBuffer[y].begin(), colorBuffer[y].end(), ConsoleColor::White);
    }
}

void ConsoleRender::DrawObject(const GameObject& obj, ConsoleColor color)
{
    Vector2 pos = obj.GetPosition();
    int startX = pos.GetX();
    int startY = pos.GetY();

    const auto& sprite = obj.GetSprite();

    for (size_t i = 0; i < sprite.size(); ++i)
    {
        int currentY = startY + static_cast<int>(i);
        if (currentY < 0 || currentY >= screenHeight) continue;

        const std::wstring& row = sprite[i];

        for (size_t j = 0; j < row.size(); ++j)
        {
            int currentX = startX + static_cast<int>(j);
            if (currentX < 0 || currentX >= screenWidth) continue;

            if (row[j] != L' ' && row[j] != L'\u2800') // \u2800 — это пустой символ Брайля (тоже пробел)
            {
                screenBuffer[currentY][currentX] = row[j];
                colorBuffer[currentY][currentX] = color;
            }
        }
    }
}

void ConsoleRender::Display()
{
    // Команда перемещения курсора в координаты (0,0) без мерцания экрана
    std::wstring output = L"\x1b[H";

    ConsoleColor lastColor = ConsoleColor::White;
    output += GetColorCode(lastColor);

    for (int y = 0; y < screenHeight; ++y)
    {
        for (int x = 0; x < screenWidth; ++x)
        {
            if (colorBuffer[y][x] != lastColor)
            {
                lastColor = colorBuffer[y][x];
                output += GetColorCode(lastColor);
            }
            output += screenBuffer[y][x];
        }
        output += L"\n";
    }

    std::wcout << output << std::flush;
}

std::wstring ConsoleRender::GetColorCode(ConsoleColor color) const
{
    switch (color)
    {
    case ConsoleColor::Green:  return L"\x1b[32m";
    case ConsoleColor::Red:    return L"\x1b[31m";
    case ConsoleColor::Yellow: return L"\x1b[33m";
    case ConsoleColor::Brown:  return L"\x1b[38;5;130m";
    case ConsoleColor::Blue:   return L"\x1b[34m";
    case ConsoleColor::White:
    default:                   return L"\x1b[37m";
    }
}

void ConsoleRender::DrawTextStr(int x, int y, const std::wstring& text, ConsoleColor color)
{
    if (y < 0 || y >= screenHeight) return;

    for (size_t i = 0; i < text.size(); ++i)
    {
        int currentX = x + static_cast<int>(i);
        if (currentX < 0 || currentX >= screenWidth) continue;

        screenBuffer[y][currentX] = text[i];
        colorBuffer[y][currentX] = color;
    }
}
