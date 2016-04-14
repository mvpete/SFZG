#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "enemy.h"
#include "game.h"
#include "enemy_wave.h"
#include "animation.h"

#include <memory>

class EnemyManager : public Component
{
  	sf::RectangleShape end_;
  	std::unique_ptr<EnemyWave> wave_;
  	int level_;
	bool play_;

	std::unique_ptr<TextFade> fade_;
	
 public:
  EnemyManager();
  ~EnemyManager();
  virtual void Update();
  virtual void Draw(sf::RenderWindow &w);

};


#endif
