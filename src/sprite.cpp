#include "sprite.h"

Sprite::Sprite(const std::string &image, const sf::IntRect &rect, int rotation)
	:display_(rect)
{
	texture_.loadFromFile(image);
	texture_.loadFromFile(image);
	sprite_.setTexture(texture_);
	sprite_.setTextureRect(display_);

	sprite_.setRotation(rotation);
	texture_.setSmooth(true);
}

void Sprite::Update()
{
	// nop
}

void Sprite::Draw(sf::RenderWindow &w)
{
	w.draw(sprite_);
}

void Sprite::SetPosition(int x, int y)
{
	sprite_.setPosition(x, y);
}

sf::IntRect& Sprite::DisplayRect()
{
	return display_;
}

sf::Sprite& Sprite::GetUnderlyingSprite()
{
	return sprite_;
}

float Sprite::GetRotation()
{
	return sprite_.getRotation();
}

void Sprite::SetRotation(float angle)
{
	sprite_.setRotation(angle);
}

void Sprite::Rotate(float angle)
{
	sprite_.rotate(angle);
}

void Sprite::SetScale(float x, float y)
{
	sprite_.setScale(x, y);
}

AnimatedSprite::AnimatedSprite(const std::string &image, const sf::IntRect &rect, int rotation, const sf::Vector2i &step, int frames, int frame_delay)
	:Sprite(image,rect,rotation),fc_(0),total_frames_(frames),delay_(frame_delay),begin_(rect),step_(step)
{


	// calculate our end
	end_.x = begin_.left + (total_frames_*step_.x);
	end_.y = begin_.top + (total_frames_*step_.y);
}

void AnimatedSprite::Update()
{
	// do our frame delay
	++fc_;
	if (fc_ < delay_)
		return;
	fc_ = 0;

	sf::IntRect &display = DisplayRect();

	if (display.left < end_.x)
	{
		display.left += step_.x;
	}
	else
		display.left = begin_.left;

	if (display.top < end_.y)
	{
		display.top += step_.y;
	}
	else
		display.top = begin_.top;
	
	GetUnderlyingSprite().setTextureRect(display);
}

