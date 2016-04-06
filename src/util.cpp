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

std::ostream & operator<<(std::ostream &os, Vector &v)
{
	os << "(" << v.X << ", " << v.Y << ")";
	return os;
}