#include "enemy_manager.h"
#include "mediator.h"

#include <iostream>


EnemyManager::EnemyManager()
: _end(sf::Vector2f(500.f,1.f)), level(1), _play(false), _ani(NULL)
{
  _end.setPosition(0,430);
  _wave = EnemyWave::GenerateWave(level);
  std::cout << "end enemy manager" << std::endl;
}

EnemyManager::~EnemyManager()
{
	delete _wave;
	delete _ani;
}

#include <sstream>
void EnemyManager::Update()
{ 
	
	if( !_play )
	{
		if( _ani == NULL )
		{
			Vector v;
			std::stringstream ss;
			ss << "Wave " << level;
			_ani = new TextFade(v, ss.str(), 5.0f);	
		}
		_ani->Update();
		if( _ani->IsDone() )
		{
			delete _ani;
			_ani = NULL;
			_play = true;
		}
	}
	else
	{
		_wave->Update();
  		if( !_wave->IsAlive() )
  		{
  			delete _wave;
  			_wave = EnemyWave::GenerateWave(++level);
  			_play = false;
  			return;
  		}
  		Vector end(0.f,430.f);
  		_wave->CheckWin(end);
	}
	
	
	
	  	
}

void EnemyManager::Draw(sf::RenderWindow &w)
{
	if( _play )
  		_wave->Draw(w);
  	if( _ani )
  		_ani->Draw(w);
  	w.draw(_end);
}
