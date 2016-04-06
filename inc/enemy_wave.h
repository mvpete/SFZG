#ifndef __ENEMY_WAVE_H__
#define __ENEMY_WAVE_H__

#include "enemy.h"
#include "unique_ptr.h"

#include <vector>


class EnemyWave
{
public:
	typedef std::vector<Enemy *> EnemyList;
	//typedef EnemyList::iterator Iterator;
protected:
		int _speed;
		std::vector<Enemy *> _enemies;
public:
	
	static EnemyWave* GenerateWave(int level);
	
	virtual ~EnemyWave(){}
	bool IsAlive();
	void CheckWin(Vector &endpoint);
	virtual void Update()=0;
	virtual void Draw(sf::RenderWindow &w);
};

class SquareWave : public EnemyWave
{
	enum ScrollDir { Left, Right };
	ScrollDir _dir;
	
	int _space;
	bool _right;
	float _step_rate, _scroll_rate;
	float _total_enemies;
public:
	SquareWave(int level);
	virtual void Update();
};

class ColumnWave : public EnemyWave
{
public:
	ColumnWave();
	virtual void Update();
};

#endif
