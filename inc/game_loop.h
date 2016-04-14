#ifndef __GAME_LOOP_H__
#define __GAME_LOOP_H__

#include "game_state.h"
#include <SFML/Graphics.hpp>
#include "mediator.h"

#include <sstream>
#include <iomanip>

class FrameCounter
{
	float _frames, _total_time;
	
public:
	void Reset() { _frames=0; _total_time=0; }
	std::string Rate()
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << _frames / (_total_time/1000.0);
		return ss.str(); 
	}
	void FrameTick(float elapsed) { ++_frames; _total_time+=elapsed; }
	
};

class GameLoop : public event_handler
{
	FrameCounter fc;
	sf::RenderWindow &_w;
	GameState *_cur_state;
	bool _quit;
	void NewGame();
	sf::Font _font;
public:
	GameLoop(sf::RenderWindow &w);
	~GameLoop();
	void NewState(GameState *state);
	void Run();
	virtual void on_event(state_event &evt);
};

#endif