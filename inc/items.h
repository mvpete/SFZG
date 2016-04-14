#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "game.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>

class ItemConsumer;
class Item
{
public:
	Item() {}
	virtual ~Item() {}
	virtual void Activate(ItemConsumer &c) = 0;
	virtual void EndActivate() { };

	virtual bool IsControlling() { return false; };

	/// This is fucking stupid.
	/// All the objects should just get registered

	virtual void Update() = 0;
	virtual void Draw(sf::RenderWindow &w) = 0;
};


class Pistol : public Item
{
public:
	class Bullet : public sf::CircleShape, public Component, public Collidable
	{
		float _x, _y;
		bool _marked;
		int _dir;
		Collidable::Owner _owner;
	public:
		Bullet(float x, float y, int dir);
		~Bullet();
		virtual void Update();
		virtual void Draw(sf::RenderWindow &w);
		bool OnScreen();
		void Collide(Collidable *c);
		Collidable::Owner GetOwner() { return _owner; }
		Vector Position();
		void Mark();
		bool IsMarked();
		sf::FloatRect GetBounds();


	};
	bool _fired;
	std::vector<Bullet*> _bullets;
	sf::Music music_;
public:
	Pistol();
	Pistol(const std::string &sfx);
	~Pistol();

	virtual void Activate(ItemConsumer &c);
	virtual void EndActivate();
	void Fire(Vector &pos, int dir);

	virtual void Update();
	virtual void Draw(sf::RenderWindow &w);
};

class Laser : public Item, public Collidable
{

	sf::RectangleShape _beam;
	Vector _start, _end;
	float _len;
	int _angle;
	bool _shown;

	void UpdatePosition();
public:

	Laser();
	~Laser();
	void Fire(Vector &pos, int dir);

	virtual void Activate(ItemConsumer &c);
	virtual void EndActivate();
	virtual bool IsControlling();

	virtual void Update();
	virtual void Draw(sf::RenderWindow &w);

	virtual void Mark();
	virtual bool IsMarked();
	virtual Collidable::Owner GetOwner();
	virtual void Collide(Collidable *);
	virtual Vector Position();
	virtual sf::FloatRect GetBounds();

};

class ItemConsumer
{
public:
	virtual ~ItemConsumer() { }
	virtual void Use(Pistol &p) { }
	virtual void Use(Laser &l) { }
};


#endif
