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

  float _x, _y;
  Sprite sprite_;
  std::vector<Item *> _items;
  Item * _current_item;
  bool _activated;
  
  int _health;
  float _r;
  
 public:
  Player();
  ~Player();
  void Draw(sf::RenderWindow &w);
  void Update();
  
  virtual void Mark() { };
  virtual bool IsMarked() { return false; }
  virtual Collidable::Owner GetOwner() { return Collidable::Player; }
  virtual void Collide(Collidable *);
  virtual Vector Position() { return Vector(_x,_y); }
  virtual sf::FloatRect GetBounds();
  
  virtual void Use(Pistol &p);
  virtual void Use(Laser &l);
  
  virtual void on_event(payload_event &e);
  
    
};

#endif
