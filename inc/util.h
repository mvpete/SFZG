#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include <SFML/Graphics.hpp>


#define WIDTH 500
#define HEIGHT 500

class rnd
{
	static bool seeded;
public:
	rnd() { if(!seeded) { srand(time(NULL)); seeded=true; } }
	int next(int max) { return rand() % max; }
};


class Vector
{
	static void Normalize(Vector &vec);
	
public:
	Vector(): X(0), Y(0) { }
 	Vector(float x, float y) :X(x), Y(y) {}
 	float Length(); 
 	float X,Y;
};

Vector GetHorizontalCenter(sf::Text &text);

std::ostream & operator<<(std::ostream &os, Vector &v);

#endif // __UTIL_H__