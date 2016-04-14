#include "enemy_wave.h"
#include "mediator.h"

#include <iostream>

#define _DEBUG


class is_dead
{
public:
  bool operator()(Enemy *e)
  {
    if( !e->IsAlive() )
    {
    	delete e;
      	return true;
    }
    return false;
  }
};

sf::FloatRect GetBounds(EnemyWave::EnemyList &enemies)
{
	Vector top(0,0), bottom(0,0);
	EnemyWave::EnemyList::iterator i(enemies.begin()), i_end(enemies.end());
	for(; i!=i_end; ++i)
	{
		Vector lc((*i)->Position());
		
		Vector rc(lc);
		rc.X += (*i)->Width();
		rc.Y += (*i)->Height();
		
		if(top.X ==0 && top.Y == 0)
		{
			top.X = lc.X;
			top.Y = lc.Y;
		}
		
		if( lc.X < top.X )
			top.X = lc.X;
			
		if( lc.Y < top.Y )
			top.Y = lc.Y;
		
		if( rc.X > bottom.X )
			bottom.X = rc.X;
		
		if( rc.Y > bottom.Y )
			bottom.Y = rc.Y;
		
	}
	
	return sf::FloatRect(top.X,top.Y, bottom.X-top.X, bottom.Y-top.Y);
}

EnemyWave* EnemyWave::GenerateWave(int level)
{
	return new SquareWave(level);
}

bool EnemyWave::IsAlive()
{
	_enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(), is_dead()), _enemies.end());
	return _enemies.size() > 0;
}

void EnemyWave::CheckWin(Vector &endpoint)
{
	int p_health;
	payload_event e("health", &p_health);
	mediator::instance()->send_event("P1", &e);
	if(p_health <= 0)
	{
		state_event l(state_event::lose);
    	mediator::instance()->send_event("GSM", &l);
		return;
	}

 	std::vector<Enemy *>::iterator i(_enemies.begin()), i_end(_enemies.end());
  	for(; i!=i_end; ++i)
  	{
  		if(*i != NULL)
  		{
  			Vector v = (*i)->Position();
			if(v.Y+30 >= endpoint.Y)
    		{
    			state_event l(state_event::lose);
    			mediator::instance()->send_event("GSM", &l);
    			
    			break;
    		}
    	}
	}
}

void EnemyWave::Draw(sf::RenderWindow &w)
{

#ifdef _DEBUG
	sf::FloatRect bounds(GetBounds(_enemies));
	sf::RectangleShape rs(sf::Vector2<float>(bounds.width, bounds.height));
	rs.setPosition(bounds.left, bounds.top);
	rs.setOutlineColor(sf::Color::Red);
	rs.setOutlineThickness(1);
	rs.setFillColor(sf::Color(255,255,255,0));
	w.draw(rs);
#endif

  	std::vector<Enemy *>::iterator i(_enemies.begin()), i_end(_enemies.end());
  	for(; i!=i_end; ++i)
  	{
  		if(*i != NULL)
  		{
  			(*i)->Draw(w);
  		}
  	}
}


SquareWave::SquareWave(int level)
:_space(0), _right(true), _step_rate(8.0f), _scroll_rate(6.0f), _dir(Right)
{
  float x(0),y(20);
  std::cout << "Creating EnemyManager" << std::endl;
  for( int r(0); r < level; ++r )
  {
    for( int c(0); c < 10; ++c )
    {
      _enemies.push_back(new ArmedEnemy(x,y,30.f, level*15));
      x+=35.f;
    }
    x = 0;
    y+=35.f;
  }
  
  _total_enemies = _enemies.size();
}



void SquareWave::Update()
{
	sf::FloatRect bounds(GetBounds(_enemies));
	Vector v1(0.f,0.f);
	if( _dir == Right )
	{
		// do I have enough space to move right
		float rs(bounds.left + bounds.width);
		if( rs < WIDTH - _scroll_rate )
		{
			v1.X += _scroll_rate;
		}
		else
		{
			v1.X += (WIDTH - rs);
			v1.Y += _step_rate;
			_dir = Left;
		}
	}
	else
	{
		float ls(bounds.left);
		if( ls > 0 + _scroll_rate )
		{
			v1.X -= _scroll_rate;
		}
		else
		{
			v1.X -= ls;
			v1.Y += _step_rate;
			_dir = Right;
		}
	}

   
  std::vector<Enemy *>::iterator i(_enemies.begin()), i_end(_enemies.end());
  for(; i!=i_end; ++i)
  {
    Vector v = (*i)->Position();
    v.X += v1.X;
    v.Y += v1.Y;
    
    //std::cout << "Moving enemy X " << v.X << " Y " << v.Y << std::endl;
    (*i)->SetPosition(v);
    (*i)->Update();
  }
  

}



ColumnWave::ColumnWave()
{
}

void ColumnWave::Update()
{
}