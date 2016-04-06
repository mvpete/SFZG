#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "enemy.h"
#include "game.h"
#include "enemy_wave.h"

#include "animation.h"

class EnemyManager : public Component
{
  	sf::RectangleShape _end;
  	EnemyWave *_wave;
  	int level;
	TextFade *_ani;
	bool _play;
	
 public:
  EnemyManager();
  ~EnemyManager();
  virtual void Update();
  virtual void Draw(sf::RenderWindow &w);

};


#endif
