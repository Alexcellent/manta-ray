// COMP 371 - Team 5 Final Project
//		Shapes Class Definition File

#include "Ray.h";

const float bias = 0.001;	 // Bias used for point intersection
const float REF_INDEX = 1.1; // Refractive index of glass

class Shape {
public:
	Shape(TVector position, TVector color, float transparency, float reflection, bool isLight) :
		position(position),
		color(color),
		transp(transparency),
		refl(reflection),
		isLight(isLight)
	{
		isTextured = false;
	};

	// Each shape will implement it's own interesction function
	// oHit and oOut represent the distance from the origin to the
	//		point of entry and exit respectively
	virtual bool intersect(Ray ray, float *oHit, float *oOut) = 0;

	// Get the norm of the hit point for the shape
	virtual TVector getHitNorm(TVector hit) const = 0;

	// Calculate reflected and refracted rays
	Ray getReflectedRay(Ray ray, TVector hit, TVector hitNorm) const;
	Ray getRefractedRay(Ray ray, TVector hit, TVector hitNorm, bool inside) const;


	// Although not the best style, all possible instance variables are made public
	// in parent class, for easy access whnever theyre needed. Up to programmer to know
	// what shape is to be used where
	TVector position, color;	// Position and color of shape
	float transp, refl;			// Transparency and reflectivity factors
	bool isLight;				// Whether it is a light or not
	float radius;				// Radius for spheres
	TVector v0, v1, v2, v3;		// Vertices for eventual rectangle and triangles
	float width, height;		// Length and width for rectangles
	bool isTextured;			// Whether a rectangle is texture
	TVector normal;				// Normal for planes and derived shaoes
	float area;					// Area of triangle

};

class Sphere : public Shape
{
public:
	// Sphere(position, color, transp, refl, isLight, radius)
	Sphere(TVector position, TVector color, float transparency, float reflection, bool isLight, float radius) :
		Shape(position, color, transparency, reflection, isLight)
	{
		this->radius = radius;
	};

	virtual bool intersect(Ray ray, float *oHit, float *oOut);
	virtual TVector getHitNorm(TVector hit) const {
		TVector temp = (hit - position);
		temp.normalize2();
		return temp;
	}

};

class Plane : public Shape
{
public:
	// Plane(position, color, transp, refl, isLight, normal)
	Plane(TVector position, TVector color, float transparency, float reflection, bool isLight, TVector normal) :
		Shape(position, color, transparency, reflection, isLight)
	{
		this->normal = normal;
		this->normal.normalize2();

	};

	virtual TVector getHitNorm(TVector hit) const { return normal; }
	virtual bool intersect(Ray ray, float *oHit, float *oOut);

};

class Rect : public Shape
{ // Actually just any quad, but too late to change name now
public:
	// Rect(color, transp, refl, isLight, v0, v1, v2, v3) (Clockwise)
	Rect(TVector color, float transparency, float reflection, bool isLight, TVector v0, TVector v1, TVector v2, TVector v3) :
		Shape(TVector(), color, transparency, reflection, isLight)
	{
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;

		width = TVector::magnitude(v0 - v1);
		height = TVector::magnitude(v1 - v2);
		position = v3 + (v1 - v3)*(0.5);
		normal = (v0 - v1)*(v3 - v0);
		normal.normalize2();
	};

	virtual TVector getHitNorm(TVector hit) const { return normal; }
	virtual bool intersect(Ray ray, float *oHit, float *oOut);
};

class Tri : public Shape
{
public:
	// Tri(color, transp, refl, isLight, v0, v1, v2) (Clockwise)
	Tri(TVector color, float transparency, float reflection, bool isLight, TVector v0, TVector v1, TVector v2) :
		Shape(TVector(), color, transparency, reflection, isLight)
	{
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;

		area = TVector::magnitude((v2 - v0)*(v1 - v0)) / 2.0;

		// Calculate centroid http://www.dummies.com/how-to/content/how-to-pinpoint-the-position-of-a-triangle.html
		TVector b = v2 + (v1 - v2)*(1.0 / 2.0);
		position = v0 + (b - v0)*(2.0 / 3.0);
		normal = (v2 - v0)*(v1 - v0);
		normal.normalize2();
	};

	virtual TVector getHitNorm(TVector hit) const { return normal; }
	virtual bool intersect(Ray ray, float *oHit, float *oOut);
};