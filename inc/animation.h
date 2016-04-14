#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

#include "game.h"

class Vector;

class TextFade : public Component
{
	sf::Font _font;
	sf::Text _title;
	sf::Clock _timer;
	float _time;
	
public:
	
	TextFade(Vector &pos, const std::string &str, float time=4.0f);
	virtual ~TextFade(){};
	virtual void Update();
	virtual void Draw(sf::RenderWindow &w);
	
	bool IsDone();
	
};


#endif
