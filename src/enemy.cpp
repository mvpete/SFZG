#include "util.h"
#include "enemy.h"
#include "mediator.h"

#include <iostream>
#include <cmath>


Enemy::Enemy(float x, float y, float side, int health)
	: bounds_(x,y,side,side), _health(health), _max_health(health),
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

	if (PointInRect(v,bounds_))
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
	return Vector(bounds_.left, bounds_.top);
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
	bounds_.left = v.X;
	bounds_.top = v.Y;
	sprite_.SetPosition(v.X + Width(), v.Y);
}

float Enemy::Width()
{
	return bounds_.width;
}

float Enemy::Height()
{
	return bounds_.height;
}

sf::FloatRect Enemy::GetBounds()
{
	return bounds_;
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
		Vector v(bounds_.left+(bounds_.width/2), bounds_.top + bounds_.height);
		std::cout << "Enemy firing" << std::endl;
		_p.Fire(v, -1);
	}
}

void ArmedEnemy::Draw(sf::RenderWindow &w)
{
	Enemy::Draw(w);
	_p.Draw(w);
}



