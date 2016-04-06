#include "player.h"
#include "mediator.h"

#include <cmath>


#include <SFML/Window/Keyboard.hpp>

#include <iostream>


Player::Player()
: event_handler("P1"), _r(20.f), _sprite(20.0f), _x(0.0), _y(450.0), _activated(false), _health(100)
{
   	_items.push_back(new Pistol());
   	//_items.push_back(new Laser());

  	_current_item = _items.back();
 
  	_sprite.setPosition(_x, _y);
  	_sprite.setFillColor(sf::Color::Magenta);
  	
  	mediator::instance()->register_handler(this);
  
  	adjustment_event base(adjustment_event::health, _health);
  	mediator::instance()->send_event("HUD", &base);
  
  	Game::GetGame()->RegisterCollidable(this);
}

Player::~Player()
{
  std::vector<Item*>::iterator i(_items.begin()), i_end(_items.end());
  for(; i!=i_end; ++i)
    delete (*i);

  mediator::instance()->remove_handler_by_id("player");
  Game::GetGame()->RemoveCollidable(this);
}

void Player::Update()
{

 	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
  	{
  		state_event p(state_event::pause);
  		mediator::instance()->send_event("GSM", &p);
  	}
  	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !_activated)
  	{
  		_current_item->Activate(*this);
  		_activated = true;
  	}
  	else if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Space) )
  	{
  		_current_item->EndActivate();
  		_activated=false;
  	}

	if( !_current_item->IsControlling() )
	{
  		if( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && _x < 460 )
  		{
		  _x+=8.0;
  		}
  		else if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && _x > 0 )
  		{
		  _x-=8.0;
  		}
  	}
  
  _sprite.setPosition(_x,_y);
  
  _current_item->Update();

}

void Player::Use(Pistol &p)
{
	Vector pos(_x+15, _y);
	p.Fire(pos, 1);
}

void Player::Use(Laser &l)
{
	Vector pos(_x+_r, _y+_r);
	l.Fire(pos, 1);
}

void Player::Draw(sf::RenderWindow &w)
{
 
  std::vector<Item *>::iterator i(_items.begin()),i_end(_items.end());
  for( ; i!=i_end; ++i)
  {
    (*i)->Draw(w);
  }
	w.draw(_sprite);
}

void Player::Collide(Collidable *c)
{
	if( c->IsMarked() || c->GetOwner() == Collidable::Player )
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
    	_health-=5;
    	
    	health_event hit(-5);
    	mediator::instance()->send_event("HUD", &hit);
    	
    	   	
    	std::cout << "Player Health : " << _health << std::endl;
   		std::cout << "Collided w/ Bullet" << std::endl;
  	} 
}

sf::FloatRect Player::GetBounds()
{
	return sf::FloatRect(_x,_y, (2*_r), (2*_r));
}

void Player::on_event(payload_event &e)
{
	if(e.id().compare("health") == 0)
	{
		int* payload=static_cast<int*>(e.payload());
		*payload = _health;
	}		
}


