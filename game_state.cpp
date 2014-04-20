#include "game_state.h"
#include "mediator.h"
#include "util.h"

#include <SFML/Window/Keyboard.hpp>




PlayableGameState::PlayableGameState(Game *game) 
: _game(game)
{
}


void PlayableGameState::Update()
{
	_game->Update();
}

void PlayableGameState::Draw(sf::RenderWindow &w)
{
	_game->Draw(w);
}


MenuGameState::MenuGameState(const std::string &title):_index(0), _up(false),_down(false), initial_update(true)
{
	if(!_font.loadFromFile("/Library/Fonts/Comic Sans MS.ttf"))
		throw "couldn't load font";
	_title = new sf::Text(title, _font);
	
	_title->setPosition(GetHorizontalCenter(*_title).X,70.f);
}

MenuGameState::~MenuGameState()
{
	delete _title;
	std::vector<MenuItem *>::iterator i(_items.begin()), i_end(_items.end());
	for(; i!=i_end; ++i)
	{
		delete *i;
	}
}


void MenuGameState::Update()
{
	

	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !_up )
	{
		if( _index > 0 )
			--_index;
		_up = true;
	}
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !_down )
	{
		if( _index < _items.size()-1 )
			++_index;
		_down = true;
	}
	
	if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && _up )
		_up = false;

	if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && _down )
		_down = false;
		

	
	Vector here(160.f, 100.f);
	for(int i=0; i<_items.size(); ++i)
	{
		
		here.Y+=30.f;
		if( i == _index )
		{
			_items[i]->Text().setStyle(sf::Text::Bold);
			_items[i]->Text().setColor(sf::Color::Yellow);
		}
		else
		{
			_items[i]->Text().setStyle(sf::Text::Regular);
			_items[i]->Text().setColor(sf::Color::Green);
		}
		Vector center(GetHorizontalCenter(_items[i]->Text()));
		_items[i]->Text().setPosition(center.X, here.Y);
			
	}
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !_pressed )
	{
		std::cout << "enter key pressed" << std::endl;
		std::cout << "initial update: " << initial_update << std::endl;
		if(!initial_update)
		{
			std::cout << "activating action" << std::endl;
			(*_items[_index])();
		}
		_pressed = true;
	}
	if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && _pressed ) 
		_pressed = false;
		
	initial_update = false;
}

void MenuGameState::Draw(sf::RenderWindow &w)
{
	w.draw(*_title);
	for(int i=0; i<_items.size(); ++i)
	{
		w.draw(_items[i]->Text());
	}
	
}


struct NewGameAction : public MenuGameState::ItemAction
{
	virtual void operator()()
	{
		std::cout << "New Game Action " << std::endl;
		state_event ng(state_event::new_game);
		mediator::instance()->send_event("GSM", &ng);
	}
};

struct QuitAction : public MenuGameState::ItemAction
{
	virtual void operator()()
	{
		std::cout << "Quit Action" << std::endl;
		state_event q(state_event::quit);
		mediator::instance()->send_event("GSM", &q);
	}
	
};

struct ResumeAction : public MenuGameState::ItemAction
{
	virtual void operator()()
	{
		std::cout << "Resume Action" << std::endl;
		state_event r(state_event::resume);
		mediator::instance()->send_event("GSM", &r);
	}
};

struct ToMainAction : public MenuGameState::ItemAction
{
	virtual void operator()()
	{
		std::cout << "To Main Action" << std::endl;
		state_event mm(state_event::main_menu);
		mediator::instance()->send_event("GSM", &mm);
	}
};


MainMenuGameState::MainMenuGameState()
:MenuGameState("Main Menu")
{	
	MenuItem *new_game = new MenuItem("New Game", _font, new NewGameAction());
	MenuItem *quit = new MenuItem("Quit", _font, new QuitAction());
	_items.push_back(new_game);
	_items.push_back(quit);
}

PauseMenuGameState::PauseMenuGameState()
:MenuGameState("Paused ...")
{
	MenuItem *resume = new MenuItem("Resume Game", _font, new ResumeAction());
	MenuItem *main = new MenuItem("Main Menu", _font, new ToMainAction());
	_items.push_back(resume);
	_items.push_back(main);
}

#include <sstream>
#include <fstream>

class HiScore
{
	std::fstream _file;	
public:
	HiScore()
	{
		_file.open("hi_score", std::fstream::in | std::fstream::out);
		if(!_file.is_open())
		{
			_file.close();
			_file.open("hi_score", std::fstream::out);
			if(!_file.is_open())
				throw "failed to open hi_score file";
			
		}
	}
	~HiScore() 
	{ 
		_file.flush(); 
		_file.close(); 
	}
	
	
	void PutScore(int score) 
	{
		_file.clear();
		_file.seekp(std::ios::beg);
		std::cout << "is the bitch good? " << (_file.good() ? "yes " : "no ") << "eof:" << _file.eof() << "fail:"<< _file.fail() << "bad:" << _file.bad() << std::endl; 
		_file << score;
		_file.flush();	
	}
	
	int GetScore() 
	{ 
		_file.clear();
		_file.seekg(std::ios::beg);
	 	int score; _file >> score; 
	 	return score;  
	}
		
};


LoseMenuGameState::LoseMenuGameState()
:MenuGameState("You Lose!")
{
	int score;
	payload_event evt("score", &score);
	mediator::instance()->send_event("HUD", &evt);
	
	std::stringstream ss;
	ss << "Your score: " << score;	
	_score = new sf::Text(ss.str(), _font);
	_score->setPosition(GetHorizontalCenter(*_score).X, 300);
	
	HiScore hi_score;
	int highest = hi_score.GetScore();
	
	std::stringstream ss1;
	if( highest < score )
	{
		std::cout << "new highest score " << score << std::endl;
		hi_score.PutScore(score);
		ss1 << "New High-Score! ";
	}
	else
	{
		ss1 << "High-Score: ";
	}
		ss1 << hi_score.GetScore();
	
	_hi_score = new sf::Text(ss1.str(), _font);
	
	Vector center(GetHorizontalCenter(*_hi_score));
	
	_hi_score->setPosition(center.X, 340);
	
	MenuItem *main = new MenuItem("Main Menu", _font, new ToMainAction());
	_items.push_back(main);
}

LoseMenuGameState::~LoseMenuGameState()
{
	delete _score;
	delete _hi_score;
}

void LoseMenuGameState::Draw(sf::RenderWindow &w)
{
	MenuGameState::Draw(w);
	w.draw(*_score);
	w.draw(*_hi_score);
}
