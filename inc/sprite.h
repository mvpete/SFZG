#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "game.h"


#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

class Sprite : public Component
{
	sf::Texture texture_;
	sf::Sprite sprite_;
	sf::IntRect display_;

protected:
	sf::IntRect& DisplayRect();
	sf::Sprite& GetUnderlyingSprite();

public:
	Sprite(const std::string &image, const sf::IntRect &rect, int rotation);
	virtual void Update();
	virtual void Draw(sf::RenderWindow &w);
	void SetPosition(int x, int y);
	void SetRotation(float rotation);
	void Rotate(float rotation);
	void SetScale(float x, float y);
	float GetRotation();
};

class AnimatedSprite : public Sprite
{
	

	int delay_;
	int total_frames_;
	int fc_;

	sf::IntRect begin_;
	sf::Vector2i step_;
	sf::Vector2i end_;
public:
	AnimatedSprite(const std::string &image, const sf::IntRect &rect, int rotation, const sf::Vector2i &step, int frames, int frame_delay);
	virtual void Update();
	
};



#endif // __SPRITE_H__