#include "items.h"
#include "unique_ptr.h"

#include <algorithm>    // std::min
#include <iostream>

UniquePtr<Vector> Intersection(Vector &a1, Vector &a2, Vector &b1, Vector &b2)
{
	float x1 = a1.X;
	float x2 = a2.X;
	float x3 = b1.X;
	float x4 = b2.X;
	float y1 = a1.Y;
	float y2 = a2.Y;
	float y3 = b1.Y;
	float y4 = b2.Y;
 
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return UniquePtr<Vector>();
 
	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
 
	// Check if the x and y coordinates are within both lines
	if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
	x < std::min(x3, x4) || x > std::max(x3, x4) ) return UniquePtr<Vector>();
	if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
	y < std::min(y3, y4) || y > std::max(y3, y4) ) return UniquePtr<Vector>();
 
	// Return the point of intersection
	std::cout << "Intersection @ point (" << x << " ," << y << ")" << std::endl;
	return UniquePtr<Vector>(new Vector(x,y));;
}

struct Line 
{
	Line(float a1, float a2, float b1, float b2)
	{
		Start = Vector(a1, a2);
		End = Vector(b1, b2);
	}
	Line(const Vector &start, const Vector &end)
	:Start(start), End(end) { };
	
	float Length()
	{
		float end_x = End.X - Start.X;
		float end_y = End.Y - Start.Y;
		return sqrt( (end_x * end_x) + (end_y * end_y)); 
	}
	
	
	Vector Start;
	Vector End;
};

std::ostream& operator<<(std::ostream &os, Line &line)
{
	os << "s-" << line.Start << " e-" << line.End;
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream &os, sf::Rect<T> &r)
{
	os << "[ l" << r.left << ", t" << r.top << ", w" << r.width << ", h" << r.height <<"]";
	return os;
}

UniquePtr<Vector> Intersection(Line &l1, Line &l2)
{
	std::cout << "Intersecting: "<< l1 << ", " << l2 << std::endl;
	return Intersection(l1.Start, l1.End, l2.Start, l2.End);
}


UniquePtr<Vector> FindWallIntersection(Line &beam)
{
	Line left(0,0,0,500);
	Line right(0,500,500,500);
	Line top(0,0,500,0);
	
	UniquePtr<Vector> i_left = Intersection(left, beam);
	UniquePtr<Vector> i_right = Intersection(right, beam);
	UniquePtr<Vector> i_top = Intersection(top, beam);
	
	if( i_left != NULL )
		return i_left;
	if( i_right != NULL )
		return i_right;
	if( i_top != NULL )
		return i_top;
		
	return UniquePtr<Vector>();
	
}




Laser::Laser()
:_beam(sf::Vector2f(1.f, 1.f)), _shown(false), _angle(0)
{
	_beam.setFillColor(sf::Color::Red);
	Game::GetGame()->RegisterCollidable(this);
}

Laser::~Laser()
{
	Game::GetGame()->RemoveCollidable(this);
}


void Laser::Fire(Vector &pos, int dir)
{
	_angle = 0;
	_len = -10.f;
	_end = Vector(pos.X-1.5f, pos.Y - _len);
	_start = Vector(pos.X-1.5f, pos.Y);
	sf::Vector2f size(3.f, _len);
	_beam.setSize(size);
	_beam.setPosition(_start.X, _start.Y);
}

void Laser::Activate(ItemConsumer &c)
{
	_shown = true;
	c.Use(*this);
}

void Laser::EndActivate()
{
	_shown = false;
	sf::Vector2<float> zero(0.f,0.f);
	_beam.setSize(zero);
	
}

bool Laser::IsControlling()
{
	return _shown;
}

void Laser::Update()
{
	if(!_shown)
		return;
	if( sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  	{
		// lean right
		if( _angle < 45 )
			_angle+=3;
  	}
  	else if( sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  	{
    	// lean left
    	if( _angle > -45 )
    		_angle-=3;
  	}
  	
  	sf::FloatRect br = _beam.getGlobalBounds();
  	std::cout << "Rect " << br << std::endl;
  	
  	if(_len > -200)
	{
		_len -= 40.f;
		sf::Vector2f size(3.f, _len);
		_beam.setSize(size);
	}
  	
  	_beam.setRotation(_angle);
}

void Laser::Draw(sf::RenderWindow &w)
{
	if(_shown)
		w.draw(_beam);
}


void Laser::Mark()
{

}

bool Laser::IsMarked()
{
	return false;
}

Collidable::Owner Laser::GetOwner()
{
	return Collidable::Player;
}

void Laser::Collide(Collidable *c)
{
}

Vector Laser::Position()
{
	return _end;
}

sf::FloatRect Laser::GetBounds()
{
	return _beam.getGlobalBounds();
}
