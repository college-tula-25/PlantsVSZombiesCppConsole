#include "ConsoleRender.h"
#include <algorithm>

void ConsoleRender::ClearBuffer()
{
	for (auto& row : screenBuffer)
	{
		std::fill(row.begin(), row.end(), ' ');
	}
}

void ConsoleRender::DrawObject(const GameObject& obj)
{
	Vector2 pos = obj.GetPosition();
	int startX = pos.GetX();
	int startY = pos.GetY();

	const auto& sprite = obj.GetSprite();

	for (int i = 0; i < obj.GetHeight(); ++i)
	{
		int currentY = startY + i;
		if (currentY < 0 || currentY >= screenHeight) continue;

		for (int j = 0; j < obj.GetWidth(); ++j)
		{
			int currentX = startX + j;
			if (currentX < 0 || currentX >= screenWidth) continue;

			screenBuffer[currentY][currentX] = sprite[i][j];
		}
	}
}

void ConsoleRender::Display()
{
	std::cout << "\x1b[H";
	for (int y = 0; y < screenHeight; ++y)
	{
		for (int x = 0; x < screenWidth; ++x)
		{
			std::cout << screenBuffer[y][x];
		}
		std::cout << "\n";
	}
}
