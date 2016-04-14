#include "player.h"
#include "mediator.h"

#include <cmath>

#include <SFML/Window/Keyboard.hpp>

#include <iostream>


Player::Player()
	: event_handler("P1"), bounds_(0, 450, 30, 30), _activated(false), health_(100),
	sprite_("C:/workspace/playground/sfzg/res/img/BugSprites.png", sf::IntRect(57, 162, 30, 30), 0)
{
	_items.push_back(new Pistol());
	//_items.push_back(new Laser());

	_current_item = _items.back();

	sprite_.SetPosition(bounds_.left, bounds_.top);

	mediator::instance()->register_handler(this);

	adjustment_event base(adjustment_event::health, health_);
	mediator::instance()->send_event("ERRBODY", &base);

	Game::GetGame()->RegisterCollidable(this);
}

Player::~Player()
{
	std::vector<Item*>::iterator i(_items.begin()), i_end(_items.end());
	for (; i != i_end; ++i)
		delete (*i);

	mediator::instance()->remove_handler(this);
	Game::GetGame()->RemoveCollidable(this);
}

void Player::Update()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		state_event p(state_event::pause);
		mediator::instance()->send_event("GSM", &p);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !_activated)
	{
		_current_item->Activate(*this);
		_activated = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_current_item->EndActivate();
		_activated = false;
	}

	if (!_current_item->IsControlling())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && bounds_.left + bounds_.width < 500)
		{
			bounds_.left += 8.0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && bounds_.left > 0)
		{
			bounds_.left -= 8.0;
		}

	}

	sprite_.SetPosition(bounds_.left, bounds_.top);

	_current_item->Update();

}

void Player::Mark()
{
	// do nothing because player doesn't get marked for collision destruction
}

bool Player::IsMarked()
{
	return false;
}

Collidable::Owner Player::GetOwner()
{
	return Collidable::Player;
}

Vector Player::Position()
{
	return Vector(bounds_.top, bounds_.left);
}

Vector Player::Center()
{
	return Vector(bounds_.left + (bounds_.width / 2), bounds_.top + (bounds_.height / 2));
}

void Player::Use(Pistol &p)
{

	p.Fire(Center(), 1);
}

void Player::Use(Laser &l)
{
	l.Fire(Center(), 1);
}

void Player::Draw(sf::RenderWindow &w)
{
	std::vector<Item *>::iterator i(_items.begin()), i_end(_items.end());
	for (; i != i_end; ++i)
	{
		(*i)->Draw(w);
	}
	sprite_.Draw(w);
}

void Player::Collide(Collidable *c)
{
	if (c->IsMarked() || c->GetOwner() == Collidable::Player)
		return;

	Vector v = c->Position();

	if (PointInRect(v, bounds_))
	{
		c->Mark();
		health_ -= 5;

		health_event hit(-5);
		mediator::instance()->send_event("FFF", &hit);
	}
}

sf::FloatRect Player::GetBounds()
{
	return bounds_;
}

void Player::on_event(payload_event &e)
{
	if (e.id().compare("health") == 0)
	{
		int* payload = static_cast<int*>(e.payload());
		*payload = health_;
	}
}

int Player::GetHealth() const
{
	return health_;
}