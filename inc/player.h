#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "game.h"
#include <SFML/Graphics.hpp>
#include <vector>

#include "sprite.h"
#include "items.h"
#include "mediator.h"


class Player : public Component, public Collidable, public ItemConsumer, public event_handler
{

	Sprite sprite_;
	std::vector<Item *> _items;
	Item * _current_item;
	bool _activated;
	sf::FloatRect bounds_;

	int health_;

	Vector Center();

public:
	Player();
	~Player();
	void Draw(sf::RenderWindow &w);
	void Update();

	virtual void Mark();
	virtual bool IsMarked();
	virtual Collidable::Owner GetOwner();
	virtual void Collide(Collidable *);
	virtual Vector Position();

	virtual sf::FloatRect GetBounds();

	virtual void Use(Pistol &p);
	virtual void Use(Laser &l);

	virtual void on_event(payload_event &e);

	int GetHealth() const;

};

#endif
