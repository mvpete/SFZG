#include "game.h"
#include <iostream>

#include "hud.h"
#include "player.h"
#include "enemy_manager.h"

Game* Game::_ptr = NULL;

Game* Game::GetGame() 
{
  return _ptr;
}

void Game::SetGame(Game *g)
{
   _ptr=g;
}


Game::Game()
{
	
}

Game::~Game()
{
  std::vector<Component*>::iterator i(_components.begin()), i_end(_components.end());
  for(; i!=i_end; ++i)
  {
    std::cout << "Deleting Component" << *i << std::endl;
    delete (*i);
  }

}

void Game::RegisterComponent(Component *c)
{
	std::cout << "registering component " << c << std::endl;
  	_components.push_back(c);
}

void Game::RemoveComponent(Component *c)
{
	std::cout << "removing component " << c << std::endl;
  _components.erase(std::find(_components.begin(), _components.end(), c), _components.end());
}

void Game::RegisterCollidable(Collidable *c)
{
  _collidables.push_back(c);
}

void Game::RemoveCollidable(Collidable *c)
{
  std::vector<Collidable*>::iterator i = std::find(_collidables.begin(), _collidables.end(), c);
  if( i != _collidables.end() )
  {
    std::cout << "erasing collidable " << *i << std::endl;
    _collidables.erase(i);
  }

}

void Game::Initialize()
{

  Hud *h = new Hud();	
  Player *p1 = new Player();
  EnemyManager  *e  = new EnemyManager();
  std::cout << "Finished creating game components" << std::endl;

  RegisterComponent(p1);
  RegisterComponent(e);
  RegisterComponent(h);


}


void Game::Update()
{
  UpdateCollisions();
  /*std::vector<Component*>::iterator i(_components.begin()), i_end(_components.end());
  for ( ; i!=i_end; ++i)
  {
    (*i)->Update();
  }*/
  
  for(int i(0); i<_components.size(); ++i)
  {
  	_components[i]->Update();
  }
  
}
void Game::UpdateCollisions()
{
  std::vector<Collidable*>::iterator i(_collidables.begin()), i_end(_collidables.end());
  for(; i!=i_end; ++i)
  {
    std::vector<Collidable*>::iterator j(i+1);
    for(; j!=i_end; ++j)
    {
      (*i)->Collide(*j);
    }
  }
}

void Game::Draw(sf::RenderWindow &w)
{
  /*	std::vector<Collidable*>::iterator i1(_collidables.begin()), i1_end(_collidables.end());
	for(; i1!=i1_end; ++i1)
	{
		sf::FloatRect r = (*i1)->GetBounds();
		sf::RectangleShape rs(sf::Vector2f(r.width, r.height));
		rs.setPosition(r.left, r.top);
		rs.setFillColor(sf::Color(255,255,255,0));
		rs.setOutlineColor(sf::Color::Green);
		rs.setOutlineThickness(1.0f);
		w.draw(rs); 
	}
  */
  	std::vector<Component*>::iterator i(_components.begin()), i_end(_components.end());
  	for(; i!=i_end; ++i)
  	{
    	(*i)->Draw(w);
  	}
}
