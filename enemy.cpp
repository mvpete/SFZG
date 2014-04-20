#include "util.h"
#include "enemy.h"
#include "mediator.h"

#include <iostream>
#include <cmath>


Enemy::Enemy(float x, float y, float r, int health)
: sf::CircleShape(r), _x(x), _y(y), _r(r), _health(health), _max_health(health)
{
  std::cout << "New Enemy" << this << std::endl;
  setPosition(_x,_y);
  setFillColor(sf::Color::Green);
  Game::GetGame()->RegisterCollidable(this);

}

Enemy::~Enemy()
{
  std::cout << "Deleting Enemy" << std::endl;
  adjustment_event e(adjustment_event::score, 100);
  mediator::instance()->send_event("HUD", &e);
  Game::GetGame()->RemoveCollidable(this);
}

void Enemy::Update()
{
  if( _health < (0.80 * _max_health) )
    setFillColor(sf::Color::Yellow);
  if( _health < (0.50 * _max_health) )
    setFillColor(sf::Color::Red);
}

void Enemy::Collide(Collidable *c)
{
  if( c->IsMarked() || c->GetOwner() == Collidable::Computer)
    return;

  Vector v = c->Position();
  /*def in_circle(center_x, center_y, radius, x, y):
    dist = math.sqrt((center_x - x) ** 2 + (center_y - y) ** 2)
    return dist <= radius*/
  float center_x=_x+_r;
  float center_y=_y+_r;

  float dist = sqrt((center_x - v.X)*(center_x - v.X) + (center_y - v.Y)*(center_y - v.Y));
  if( dist < _r )
  {
    c->Mark();
    _health-=15;
   std::cout << "Collided w/ Enemy" << std::endl;
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
  w.draw(*this);
}

bool Enemy::IsAlive()
{
  return (_health > 0);
}

void Enemy::SetPosition(Vector &v)
{
  _x = v.X;
  _y = v.Y;
  setPosition(v.X, v.Y);
}

float Enemy::Width()
{
	return _r*2;
}

float Enemy::Height()
{
	return Width();
}

sf::FloatRect Enemy::GetBounds()
{
	return sf::FloatRect(_x,_y,(2*_r),(2*_r));
}


ArmedEnemy::ArmedEnemy(float x, float y, float r, int health)
:Enemy(x,y,r,health)
{
	rnd rn;
	_rand_fire = rn.next(15) + 2;
	std::cout << "random fire rate" << _rand_fire << std::endl;
}


void ArmedEnemy::Update()
{
	Enemy::Update();
	_p.Update();
	if( _timer.getElapsedTime().asSeconds() > _rand_fire )
	{
		_timer.restart();
		Vector v(_x+_r, _y+(3*_r));
		std::cout << "Enemy firing" << std::endl;
		_p.Fire(v, -1);
	}
}

void ArmedEnemy::Draw(sf::RenderWindow &w)
{
	Enemy::Draw(w);
	_p.Draw(w);
}



