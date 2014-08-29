// COMP 371 - Team 5 Final Project
//		Ray Class Definition File

#include "TTuple.h";

class Ray {
public:
	TVector origin, direction;

	Ray() : origin(), direction() {}
	Ray(TVector origin, TVector direction) : origin(origin), direction(direction){}
};