#include "util.h"
#include "enemy.h"
#include "mediator.h"

#include <iostream>
#include <cmath>


Enemy::Enemy(float x, float y, float r, int health)
	: _x(x), _y(y), _r(r), _health(health), _max_health(health),
	sprite_("C:/workspace/playground/sfzg/res/img/BugSprites.png", sf::IntRect(55, 302, 30, 30), 90, sf::Vector2i(35, 0), 5, 5)
{
	std::cout << "new enemy " << this << std::endl;
	Game::GetGame()->RegisterCollidable(this);
}

Enemy::~Enemy()
{
  	std::cout << "deleting enemy " << std::endl;
	adjustment_event e(adjustment_event::score, 100);
	mediator::instance()->send_event("HUD", &e);
	Game::GetGame()->RemoveCollidable(this);
}

void Enemy::Update()
{
	sprite_.Update();
}

void Enemy::Collide(Collidable *c)
{
	if (c->IsMarked() || c->GetOwner() == Collidable::Computer)
		return;

	Vector v = c->Position();
	/*def in_circle(center_x, center_y, radius, x, y):
	  dist = math.sqrt((center_x - x) ** 2 + (center_y - y) ** 2)
	  return dist <= radius*/
	float center_x = _x + _r;
	float center_y = _y + _r;

	float dist = sqrt((center_x - v.X)*(center_x - v.X) + (center_y - v.Y)*(center_y - v.Y));
	if (dist < _r)
	{
		c->Mark();
		_health -= 15;

	}
}

void Enemy::Mark()
{
}

bool Enemy::IsMarked()
{
	return IsAlive();
}

Vector Enemy::Position()
{
	return Vector(_x, _y);
}

void Enemy::Draw(sf::RenderWindow &w)
{
	sprite_.Draw(w);
}

bool Enemy::IsAlive()
{
	return (_health > 0);
}

void Enemy::SetPosition(Vector &v)
{
	_x = v.X;
	_y = v.Y;
	sprite_.SetPosition(v.X + 30, v.Y);
}

float Enemy::Width()
{
	return _r * 2;
}

float Enemy::Height()
{
	return Width();
}

sf::FloatRect Enemy::GetBounds()
{
	return sf::FloatRect(_x, _y, (2 * _r), (2 * _r));
}


ArmedEnemy::ArmedEnemy(float x, float y, float r, int health)
	:Enemy(x, y, r, health),_p("C:/workspace/playground/sfzg/res/snd/laserfire02.ogg")
{
	rnd rn;
	_rand_fire = rn.next(15) + 2;
	std::cout << "random fire rate" << _rand_fire << std::endl;
}


void ArmedEnemy::Update()
{
	Enemy::Update();
	_p.Update();
	if (_timer.getElapsedTime().asSeconds() > _rand_fire)
	{
		_timer.restart();
		Vector v(_x + _r, _y + (3 * _r));
		std::cout << "Enemy firing" << std::endl;
		_p.Fire(v, -1);
	}
}

void ArmedEnemy::Draw(sf::RenderWindow &w)
{
	Enemy::Draw(w);
	_p.Draw(w);
}



