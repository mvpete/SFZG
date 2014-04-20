#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "game.h"


class GameState
{
 public:
  virtual ~GameState() { };
  virtual void Update()=0;
  virtual void Draw(sf::RenderWindow &w)=0;
};


class PlayableGameState : public GameState
{
  Game *_game;
 public:
  PlayableGameState(Game *game);
  virtual void Update();
  virtual void Draw(sf::RenderWindow &w);
};

class MenuGameState : public GameState
{
protected:
	sf::Font _font;
	sf::Text *_title;
public:
  struct ItemAction
  {
  public: 
    virtual void operator()()=0;
  };
  
  class MenuItem
  {   
    ItemAction *_action;
    sf::Text _item;
  public:
  	MenuItem(const char *text, sf::Font &font, ItemAction *action)
  	:_item(text, font), _action(action){}
    ~MenuItem() { delete _action; }
    sf::Text & Text() { return _item; }
    void operator()() { (*_action)(); }
  };
  
protected:
  	int _index;
  	bool _up,_down, _pressed;
  	sf::RectangleShape _cur_item;
  	std::vector<MenuItem *> _items;
	bool initial_update;


 public:
  MenuGameState(const std::string &title);
  virtual ~MenuGameState();
  virtual void Update();
  virtual void Draw(sf::RenderWindow &w);
};

class MainMenuGameState : public MenuGameState
{
public:
	MainMenuGameState();
};

class PauseMenuGameState : public MenuGameState
{
public:
	PauseMenuGameState();
};

class LoseMenuGameState : public MenuGameState
{
	sf::Text *_score, *_hi_score;
public:
	LoseMenuGameState();
	~LoseMenuGameState();
	virtual void Draw(sf::RenderWindow &w);	
};


#endif
