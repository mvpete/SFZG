#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "game.h"
#include "items.h"
#include "sprite.h"



class Enemy : public Collidable
{
	AnimatedSprite sprite_;

protected:
	float _x, _y, _r;
	int _health, _max_health;

public:
	Enemy(float x, float y, float r, int health);
	virtual ~Enemy();
	virtual void Update();
	virtual void Draw(sf::RenderWindow &w);
	virtual void Collide(Collidable *c);
	virtual Collidable::Owner GetOwner() { return Collidable::Computer; }
	Vector Position();
	void Mark();
	bool IsMarked();

	virtual sf::FloatRect GetBounds();

	bool IsAlive();
	void SetPosition(Vector &v);

	float Width(), Height();
};

class ArmedEnemy : public Enemy
{
	sf::Clock _timer;
	Pistol _p;
	int _rand_fire;
public:
	ArmedEnemy(float x, float y, float r, int health);
	virtual void Update();
	virtual void Draw(sf::RenderWindow &w);
};


#endif
