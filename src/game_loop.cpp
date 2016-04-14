#include "game_loop.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

#include "enemy_manager.h"
#include "hud.h"
#include "player.h"

float to_ms(float s)
{
	return s*1000.0f;
}

GameLoop::GameLoop(sf::RenderWindow &w)
	:event_handler("GSM"), _w(w), _quit(false), _cur_state(NULL)
{
	mediator::instance()->register_handler(this);
}

GameLoop::~GameLoop()
{
	mediator::instance()->remove_handler(this);
}

void GameLoop::NewGame()
{
	fc.Reset();
	std::cout << " --- NEW GAME --- " << std::endl;
	if (Game::GetGame())
		delete Game::GetGame();
	Game *g = new Game();
	Game::SetGame(g);
	Game::GetGame()->Initialize();
}

void GameLoop::Run()
{
	sf::Clock clock;
	while (_w.isOpen() && !_quit)
	{
		sf::Event event;
		while (_w.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_w.close();
		}
		float ms = clock.getElapsedTime().asMilliseconds();
		if (ms > to_ms(1.0 / 70.0))
		{
			_cur_state->Update();
			_w.clear();
			_cur_state->Draw(_w);
			sf::Text fr(fc.Rate(), _font);
			fr.setCharacterSize(8);
			fr.setPosition(470, 490);
			_w.draw(fr);
			_w.display();
			clock.restart();
			fc.FrameTick(ms);
		}

		sf::sleep(sf::milliseconds(to_ms(1.0 / 70.0)));
	}

}


void GameLoop::NewState(GameState *state)
{
	if (_cur_state)
	{
		delete _cur_state;
	}
	_cur_state = state;
	_cur_state->Update();
}

void GameLoop::on_event(state_event &evt)
{
	switch (evt.type())
	{
	case state_event::quit:
		_quit = true;
		break;
	case state_event::new_game:
		NewGame();
	case state_event::resume:
		NewState(new PlayableGameState(Game::GetGame()));
		break;
	case state_event::pause:
		NewState(new PauseMenuGameState());
		break;
	case state_event::main_menu:
		NewState(new MainMenuGameState());
		break;
	case state_event::lose:
		NewState(new LoseMenuGameState());
		break;
	default:
		break;
	}
}
