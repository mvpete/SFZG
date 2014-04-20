#include "animation.h"

TextFade::TextFade(Vector &pos, const std::string &str, float time)
	:_time(time*1000.f)
{
	
	_title = sf::Text(str, _font);
	_title.setPosition(GetHorizontalCenter(_title).X,70.f);
	_timer.restart();
}

void TextFade::Draw(sf::RenderWindow &w)
{
	w.draw(_title);
}

void TextFade::Update()
{
	float elapsed = _timer.getElapsedTime().asMilliseconds();
	float new_fade = std::min(((elapsed / (0.75f * _time)) * 255), 255.f);
	_title.setColor(sf::Color(255,255,255, new_fade));
}

bool TextFade::IsDone()
{
	return _timer.getElapsedTime().asMilliseconds() > _time;
}
