#include "util.h"

bool rnd::seeded = false;


Vector GetHorizontalCenter(sf::Text &text)
{
	sf::FloatRect r = text.getLocalBounds();
	float side = (WIDTH - r.width) / 2;
	return Vector(side, 0);
}


float Vector::Length()
{ 
	return sqrt((X*X)+(Y*Y)); 
}

void Vector::Normalize(Vector &vec)
{
	float length = vec.Length();
 
    if(length != 0)
    {
        vec.X = vec.X/length;
        vec.Y = vec.Y/length;
    }
}

bool PointInRect(const Vector &pt, const sf::FloatRect &rect)
{
	return (rect.left <= pt.X && pt.X <= rect.left + rect.width) && (rect.top <= pt.Y && pt.Y <= rect.top + rect.height);
}

std::ostream & operator<<(std::ostream &os, Vector &v)
{
	os << "(" << v.X << ", " << v.Y << ")";
	return os;
}