#pragma once
#include "PlantTypes.h"

class Sunflower : public SunGives
{
public:
	Sunflower(Vector2 pos) : SunGives(pos, {
		" \\|/ ",
		"-(O)-",
		"  |  "
		}, 300) {
	}
};

class Peashooter : public Attackers
{
public:
	Peashooter(Vector2 pos) : Attackers(pos, {
		" O__ ",
		"(xx)-",
		" / \\ "
		}, 300) {
	}
};

class WallNut : public Defensive
{
public:
	WallNut(Vector2 pos) : Defensive(pos, {
		" /---\\ ",
		"| O O |",
		" \\___/ "
		}, 4000) {
	}
};

class CherryBomb : public InstantEffect
{
public:
	CherryBomb(Vector2 pos) : InstantEffect(pos, {
		"  _   _  ",
		" (o) (o) ",
		"  \\___/  "
		}, 300) {
	}
};
