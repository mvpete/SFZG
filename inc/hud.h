#ifndef __HUD_H__
#define __HUD_H__

#include "game.h"
#include "mediator.h"

class Player;
class Hud : public Component, public event_handler
{
  sf::Font _font;
  sf::Text _score_text, _health_text;
  sf::RectangleShape _line;
  int _score;
  std::string _text;

  Player &player_;


 public:

  Hud(Player &p);
  ~Hud();
  virtual void Update();
  virtual void Draw(sf::RenderWindow &w);

  virtual void on_event(adjustment_event	&evt);
  virtual void on_event(payload_event 		&evt);
};

#endif
