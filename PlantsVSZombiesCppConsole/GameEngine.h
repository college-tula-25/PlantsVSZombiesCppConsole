#pragma once
#include <vector>
#include <list>
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>

#include "ConsoleRender.h"
#include "Plants.cpp"
#include "Zombie.h"
#include "Other.h"

class GameEngine
{
private:
	const int screenWidth = 80;
	const int screenHeight = 25;

	const int gridRows = 5;
	const int gridCols = 9;

	const int cellWidth = 8;
	const int cellHeight = 4;
	const int gridOffsetX = 4;
	const int gridOffsetY = 2;

	int cursorRow{ 0 };
	int cursorCol{ 0 };
	int sunTickCounter{ 0 };

	ConsoleRender renderer;
	bool isRunning{ true };

	int playerSun{ 150 };

	std::vector<std::vector<std::unique_ptr<Plant>>> plantGrid;

	std::list<std::unique_ptr<BaseZombie>> zombies;
	std::list<Projectile> projectiles;
	std::list<Sun> droppedSuns;

	Vector2 GridToScreen(int row, int col) const
	{
		int x = gridOffsetX + (col * cellWidth);
		int y = gridOffsetY + (row * cellHeight);
		return Vector2(x, y);
	}

	void ProcessInput();
	void UpdatePhysics();
	void RenderAll();

public:
	GameEngine();
	void Run();

	void SpawnPlant(int row, int col, char type);
	void SpawnZombie(int row);
};
