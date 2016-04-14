#include "enemy_manager.h"
#include "mediator.h"

#include <iostream>
#include <sstream>


EnemyManager::EnemyManager()
: end_(sf::Vector2f(500.f,1.f)), level_(1), play_(false)
{
  end_.setPosition(0,430);
  wave_.reset(EnemyWave::GenerateWave(level_));
  std::cout << "end enemy manager" << std::endl;
}

EnemyManager::~EnemyManager()
{
}


void EnemyManager::Update()
{ 
	
	if( !play_ )
	{
		if( fade_ == NULL )
		{
			Vector v;
			std::stringstream ss;
			ss << "Wave " << level_;
			fade_.reset(new TextFade(v, ss.str(), 5.0f));
		}
		fade_->Update();
		if( fade_->IsDone() )
		{
			fade_.release();
			play_ = true;
		}
	}
	else
	{
		wave_->Update();
  		if( !wave_->IsAlive() )
  		{
  		
  			wave_.reset(EnemyWave::GenerateWave(++level_));
  			play_ = false;
  			return;
  		}
		Vector end(0, 430.f);
  		wave_->CheckWin(end);
	} 	
}

void EnemyManager::Draw(sf::RenderWindow &w)
{
	if( play_ )
  		wave_->Draw(w);
  	if( fade_ )
  		fade_->Draw(w);
  	w.draw(end_);
}
