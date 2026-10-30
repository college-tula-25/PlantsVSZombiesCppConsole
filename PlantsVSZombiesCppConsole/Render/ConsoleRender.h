#pragma once
#include <vector>
#include <string>
#pragma once
#include <vector>
#include <string>
#include "../Entities/GameObject.h"

enum class ConsoleColor
{
    White,
    Green,
    Red,
    Yellow,
    Brown,
    Blue
};

class ConsoleRender
{
private:
    int screenWidth;
    int screenHeight;

    std::vector<std::vector<wchar_t>> screenBuffer;
    std::vector<std::vector<ConsoleColor>> colorBuffer;

    std::wstring GetColorCode(ConsoleColor color) const;

public:
    ConsoleRender(int width = 80, int height = 25);
    ~ConsoleRender() = default;

    void ClearBuffer();

    void DrawObject(const GameObject& obj, ConsoleColor color = ConsoleColor::White);

    void DrawTextStr(int x, int y, const std::wstring& text, ConsoleColor color = ConsoleColor::White);

    void Display();
};
