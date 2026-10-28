#pragma once
#include "Plant.h"
#include "Other.h"

class SunGives : public Plant
{
protected:
	int sunsCount{ 25 };
public:
	SunGives(Vector2 pos, const std::vector<std::string>& spr, int health)
		: Plant(pos, spr, health) {
	}
};

class Attackers : public Plant
{
protected:
	int damage{ 20 };
	bool isReloaded{ true };
public:
	Attackers(Vector2 pos, const std::vector<std::string>& spr, int health)
		: Plant(pos, spr, health) {
	}

	Projectile Shoot()
	{
		Vector2 spawnPos(position.GetX() + width, position.GetY() + (height / 2));
		return Projectile(spawnPos);
	}

	bool CanShoot() const { return isReloaded; }
	void SetReloaded(bool state) { isReloaded = state; }
};


class Defensive : public Plant
{
public:
	Defensive(Vector2 pos, const std::vector<std::string>& spr, int health = 4000)
		: Plant(pos, spr, health) {
	}
};


class InstantEffect : public Plant
{
protected:
	int explosionRadius{ 1 };
	int explosionDamage{ 1800 };
public:
	InstantEffect(Vector2 pos, const std::vector<std::string>& spr, int health = 300)
		: Plant(pos, spr, health) {
	}

	int GetExplosionDamage() const { return explosionDamage; }
	int GetExplosionRadius() const { return explosionRadius; }
};
