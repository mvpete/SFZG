#include "hud.h"
#include <iostream>

Hud::Hud():event_handler("HUD"),_line(sf::Vector2f(500.f, 1.f))
{
  mediator::instance()->register_handler(this);

  _font.loadFromFile("comic.ttf");
    // Create a text
  std::cout << "Loaded Font" << std::endl;

  _score_text = sf::Text("score: ", _font);
  _health_text = sf::Text("health: 000", _font);
 
  _score_text.setCharacterSize(14);
  _score_text.setColor(sf::Color::White); 
  _score_text.setPosition(5,1);
  
  _health_text.setCharacterSize(14);
  _health_text.setColor(sf::Color::White); 
  sf::FloatRect r = _health_text.getLocalBounds();
  _health_text.setPosition((495 - r.width),1);
  
 //_text->setStyle(sf::Text::Bold);
  _line.setPosition(0,20);
  
  _score = 0;
  _health = 0;

  std::cout << "Finished creating" << std::endl;
}

Hud::~Hud()
{
	mediator::instance()->remove_handler(this);
}


void Hud::on_event(adjustment_event &evt)
{
	switch( evt.type() )
	{
	case adjustment_event::health:
		evt.adjust(_health);
		break;
	case adjustment_event::score:
		evt.adjust(_score);
		break;
	default:
		break;
	}
}

void Hud::on_event(payload_event &evt)
{
	if(evt.id().compare("score") == 0)
	{
		int* score=static_cast<int*>(evt.payload());
		*score = _score;
	}	
}

#include <sstream>

void Hud::Update()
{
  std::stringstream ss;
  ss << "score: " << _score;
  _score_text.setString(ss.str());
  
  ss.str("");
  ss << "health: " << _health;
  _health_text.setString(ss.str());

}

void Hud::Draw(sf::RenderWindow &w)
{
  w.draw(_score_text);
  w.draw(_health_text);
  w.draw(_line);
  
}
