#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "resource_pool.h"
#include "mediator.h"
#include "game_loop.h"
#include "game_state.h"
#include "player.h"
#include "enemy_manager.h"
#include "hud.h"

#include <vector>
#include <iostream>


sf::Font _global_font;
int main()
{
	sf::RenderWindow window(sf::VideoMode(500,500),"PFZG!");
  	
	if(!_global_font.loadFromFile("/Library/Fonts/Comic Sans MS.ttf"))
		throw "couldn't load font";
	

  	GameLoop loop(window);
  	loop.NewState(new MainMenuGameState());
  	loop.Run();
  	
  return 0;
}
