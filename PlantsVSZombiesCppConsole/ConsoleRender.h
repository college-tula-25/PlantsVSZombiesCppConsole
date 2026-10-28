#pragma once
#include <iostream>
#include <vector>
#include "GameObject.h"

class ConsoleRender
{
private:
	int screenWidth;
	int screenHeight;
	std::vector<std::vector<char>> screenBuffer;

public:
	ConsoleRender(int width = 80, int height = 25)
		: screenWidth(width), screenHeight(height),
		screenBuffer(height, std::vector<char>(width, ' '))
	{
	}

	void ClearBuffer();
	void DrawObject(const GameObject& obj);
	void Display();
};
