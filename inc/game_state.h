#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <iostream>
#include <vector>
#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

class Game;

class GameState
{
public:
	virtual ~GameState() { };
	virtual void Update() = 0;
	virtual void Draw(sf::RenderWindow &w) = 0;
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
	sf::Text title_;
public:

	using ItemAction = std::function<void()>;
	class MenuItem
	{
		ItemAction action_;
		sf::Text text_;
		sf::Font &font_;

	public:
		MenuItem(const char *text, sf::Font &font, ItemAction action)
			:text_(text, font), action_(action), font_(font) 
		{}
		
		sf::Text& Text() { return text_; }
		void operator()() { action_(); }
	};

protected:
	int _index;
	bool _up, _down, _pressed;
	sf::RectangleShape _cur_item;
	std::vector<MenuItem> _items;
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
	std::unique_ptr<sf::Text> score_, hi_score_;
public:
	LoseMenuGameState();
	~LoseMenuGameState();
	virtual void Draw(sf::RenderWindow &w);
};


#endif
