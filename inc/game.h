#ifndef __GAME_H__
#define __GAME_H__
#include "util.h"

#include <SFML/Graphics.hpp>


class Component
{
 public:
  virtual ~Component() {} 
  virtual void Update()=0;
  virtual void Draw(sf::RenderWindow &)=0;
};

class Drawable
{
 public:
  virtual ~Drawable() { }
  virtual void Draw(sf::RenderWindow &)=0;
};

class Collidable
{
  
 public:
 	enum Owner { Player, Computer };
  virtual ~Collidable() { }
  virtual void Mark()=0;
  virtual bool IsMarked()=0;
  virtual Owner GetOwner()=0;
  virtual void Collide(Collidable *)=0;
  virtual Vector Position()=0;
  virtual sf::FloatRect GetBounds()=0;
};

class Updateable
{
 public:
  virtual ~Updateable() { }
  virtual void Update()=0;
};


class Game
{
 public:
  static Game* GetGame();
  static void  SetGame(Game *g);

  
 private:
  
  std::vector<Component*> _components;
  

  std::vector<Drawable   *> _drawables;
  std::vector<Collidable *> _collidables;
  std::vector<Updateable *> _updatables;

  void UpdateCollisions();
  

public:
  Game();
  ~Game();
  void Run();

  void Initialize(); // must be called to initialize game components. so fkn stupid but I tied the game component construction to the Game* fk.
  
  void QueueRegisterComponent(Component *c);
  void QueueRemoveComponent(Component *c);
  
  void RegisterComponent(Component *c);
  void RemoveComponent(Component *c);
  void RegisterCollidable(Collidable *c);
  void RemoveCollidable(Collidable *c);
  static Game *_ptr;
  
  
  void Draw(sf::RenderWindow &w); 
  void Update();

};


#endif
