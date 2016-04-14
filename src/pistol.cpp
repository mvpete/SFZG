#include "items.h"

#include <iostream>

Pistol::Pistol(const std::string &sfx)
	:_fired(false)
{
	music_.openFromFile(sfx);
}

Pistol::Pistol()
	:_fired(false)
{
	music_.openFromFile("C:/workspace/playground/sfzg/res/snd/laserfire01.ogg");
}

Pistol::~Pistol()
{
	std::vector<Bullet *>::iterator i(_bullets.begin()), i_end(_bullets.end());
	for (; i != i_end; ++i)
	{
		delete *i;
	}
}

class is_dead
{
public:
	bool operator()(Pistol::Bullet *b)
	{
		if (!b->OnScreen())
		{
			std::cout << "Deleting bullet" << std::endl;
			delete b;
			return true;
		}
		return false;
	}
};

void Pistol::Activate(ItemConsumer &c)
{
	if (!_fired)
	{
		_fired = true;
		c.Use(*this);
	}
}
void Pistol::EndActivate()
{
	_fired = false;
}


void Pistol::Fire(Vector &pos, int dir)
{
	music_.play();
	_bullets.push_back(new Bullet(pos.X, pos.Y, dir));
}

void Pistol::Update()
{
	_bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(), is_dead()), _bullets.end());
	std::vector<Bullet *>::iterator i(_bullets.begin()), i_end(_bullets.end());
	for (; i != i_end; ++i)
	{
		(*i)->Update();
	}
}

void Pistol::Draw(sf::RenderWindow &w)
{
	std::vector<Bullet *>::iterator i(_bullets.begin()), i_end(_bullets.end());
	for (; i != i_end; ++i)
	{
		(*i)->Draw(w);
	}
}

Pistol::Bullet::Bullet(float x, float y, int dir)
	:sf::CircleShape(2.f), _x(x), _y(y), _marked(false), _dir(dir)
{
	if (dir == -1)
		_owner = Collidable::Computer;
	else
		_owner = Collidable::Player;

	setPosition(_x, _y);
	setFillColor(sf::Color(140, 140, 140));
	Game::GetGame()->RegisterCollidable(this);
}


Pistol::Bullet::~Bullet()
{
	Game::GetGame()->RemoveCollidable(this);
}

void Pistol::Bullet::Update()
{
	_y -= (20.0)*_dir;
	setPosition(_x, _y);
}

void Pistol::Bullet::Draw(sf::RenderWindow &w)
{
	w.draw(*this);
}

bool Pistol::Bullet::OnScreen()
{
	return (_y > 0 && _y < 500 && _x >0 && _x < 500 && !_marked);
}

void Pistol::Bullet::Collide(Collidable *c)
{
}

void Pistol::Bullet::Mark()
{
	_marked = true;
}

bool Pistol::Bullet::IsMarked()
{
	return _marked;
}

Vector Pistol::Bullet::Position()
{
	return Vector(_x, _y);
}

sf::FloatRect Pistol::Bullet::GetBounds()
{
	return sf::FloatRect(_x, _y, (2 * 2.0f), (2 * 2.0f));
}
