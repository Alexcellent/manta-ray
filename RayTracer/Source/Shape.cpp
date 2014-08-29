// COMP 371 - Team 5 Final Project
//		Shapes Class Implementation File

#include "Shape.h";

using namespace std;

Ray Shape::getReflectedRay(Ray ray, TVector hit, TVector norm) const
{
	// Get reflection direction and trace that ray
	//		Source for equation: http://paulbourke.net/geometry/reflected/

	TVector direction = ray.direction - 2.0 * norm * (ray.direction|norm);
	direction.normalize2();					// traceRay takes a normalized direction
	TVector origin = hit + norm * bias;		// Calculate origin point of reflected ray using bias

	return Ray(origin, direction);
}

Ray Shape::getRefractedRay(Ray ray, TVector hit, TVector norm, bool inside) const
{
	// Use Snell's law for refraction: http://ray-tracer-concept.blogspot.ca/2011/12/refraction.html
	float refIndex;
	if (inside) 
		refIndex = REF_INDEX;
	else 
		refIndex = 1 / REF_INDEX;

	float cosQi = -(norm | ray.direction);
	float rootThis = 1 - refIndex * refIndex * (1 - cosQi*cosQi);

	TVector direction = (refIndex * cosQi - sqrt(rootThis)) * norm - (refIndex * -ray.direction);
	direction.normalize2();

	TVector origin = hit - norm * bias;

	return Ray(origin, direction);
}

bool Sphere::intersect(Ray ray, float *oHit, float *oOut)
{
	// Use geometric ray-sphere intersection: http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-sphere-intersection/
	float r2 = radius*radius;

	TVector oCenter = position - ray.origin;	// Vector from ray origin to sphere center
	float ca = (oCenter|ray.direction);			// Magnitude of vector which passes closes to sphere center
	
	if (ca < 0) return false;

	float d2 = (oCenter|oCenter) - ca*ca;		// Get smallest distance from center to intersecting ray (squared)

	if (d2 > r2) return false;					// Ray misses sphere (passes beyond length of radius)

	float hc = sqrt(r2 - d2);					// Find half-chord: r2 = d2 + hc2


	if (oHit != NULL && oOut != NULL) {
		*oHit = ca - hc;
		*oOut = ca + hc;
	}
	return true;
}

bool Plane::intersect(Ray ray, float *oHit, float *oOut)
{
	// See scratchapixel.com for equation

	// Check denominator is not too small, which 
	// may happen if plane and ray are almost parallel
	float den = (ray.direction | normal);
	if (abs(den) > 0.0000001) {
		*oHit = ((position - ray.origin) | normal) / den;
		return (*oHit >= 0);
	}
	else {
		return false;
	}
}

bool Rect::intersect(Ray ray, float *oHit, float *oOut)
{
	// Check plane intersection
	float den = (ray.direction | normal);
	if (abs(den) > 0.0000001) {
		*oHit = ((position - ray.origin) | normal) / den;
		if (*oHit < 0) { return false; }

		float tri0, tri1, tri2, tri3;
		TVector hit = ray.origin + ray.direction*(*oHit);

		// Calculate areas of 4 triangles forming rect
		// https://www.youtube.com/watch?v=A85_qJ5P5ik
		tri0 = TVector::magnitude((v0 - hit)*(v1 - hit)) / 2.0;
		tri1 = TVector::magnitude((v1 - hit)*(v2 - hit)) / 2.0;
		tri2 = TVector::magnitude((v2 - hit)*(v3 - hit)) / 2.0;
		tri3 = TVector::magnitude((v3 - hit)*(v0 - hit)) / 2.0;

		// Check that sum of areas = area of rect, with a small bias
		return (abs((tri0 + tri1 + tri2 + tri3) - (width * height)) < 0.0001);

	}
	else {
		return false;
	}
}

bool Tri::intersect(Ray ray, float *oHit, float *oOut)
{
	// Check plane intersection
	float den = (ray.direction | normal);
	if (abs(den) > 0.0000001) {
		*oHit = ((position - ray.origin) | normal) / den;
		if (*oHit < 0) { return false; }

		float tri0, tri1, tri2, tri3;
		TVector hit = ray.origin + ray.direction*(*oHit);

		// Calculate areas of 3 triangles forming tri
		tri0 = TVector::magnitude((v0 - hit)*(v1 - hit)) / 2.0;
		tri1 = TVector::magnitude((v1 - hit)*(v2 - hit)) / 2.0;
		tri2 = TVector::magnitude((v2 - hit)*(v0 - hit)) / 2.0;

		// Check that sum of areas = area of tri, with a small bias
		return (abs((tri0 + tri1 + tri2) - (area)) < 0.0001);

	}
	else {
		return false;
	}
}