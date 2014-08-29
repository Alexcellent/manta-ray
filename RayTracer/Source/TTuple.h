// COMP 371 - Team 5 Final Project

// This TTuple class is taken from the codebase of our assignments throughout the class.

#ifndef T_TUPLE__H_
#define T_TUPLE__H_

/* This is a templated version of tuples and matrices */

#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>

#define M_PI 3.141592654

//namespace T3D{

template<class C, int N>
class TTuple {

private:
	C v[N];// the actual vector

public:

	C& operator[] (int i);
	// better write comments here ....
	const C& operator[] (int i) const;

	TTuple<C, N>& operator= (const TTuple<C, N>& t);
	TTuple<C, N> operator+ (const TTuple<C, N>& a);
	TTuple<C, N> operator+= (const TTuple<C, N>& a);
	TTuple<C, N> operator- (const TTuple<C, N>& a);
	TTuple<C, N> operator-= (const TTuple<C, N>& a);
	TTuple<C, N> operator* (const TTuple<C, N>& a);
	TTuple<C, N> operator*= (const TTuple<C, N>& a);
	TTuple<C, N> operator* (const C& a);
	TTuple<C, N> operator*= (const C& a);
	C operator|(const TTuple<C, N>& a);
};

template<class C, int N>
TTuple<C, N> operator* (const C a, const TTuple<C, N>& t);

template<class T, int L, int C>
class TMatrix { /* want a matrix simillar to OpenGL in structure */
private:
	T v[L*C];

public:

	void setNull();
	void setIdentity();
	TMatrix<T, L, C>();

	// copy constructor + assignment operator
	TMatrix<T, L, C>(const TMatrix<T, L, C>&);
	TMatrix<T, L, C>& operator=(const TMatrix<T, L, C>&);


	T& operator() (int l, int c);

	const T& operator()(int l, int c) const;
	T& elem(int l, int c);

	T& operator[] (int n);

	const T& operator[] (int n) const;

	TMatrix<T, L, C> operator+ (const TMatrix<T, L, C> & a);
	TMatrix<T, L, C> operator+= (const TMatrix<T, L, C> & a);
	TMatrix<T, L, C> operator- (const TMatrix<T, L, C> & a);
	TMatrix<T, L, C> operator-= (const TMatrix<T, L, C> & a);
	T* getBuffer();

};


// Affine geometry stuff
typedef TMatrix<float, 4, 4> TMatrix4x4;
typedef TTuple<float, 3> TTuple3f;


class TPoint;
class TVector {
private:
	TTuple3f v;
public:


	// basic constructors
	TVector();
	TVector(float x);
	TVector(float x, float y);
	TVector(float x, float y, float z);
	TVector(const TPoint& p);
	TVector(const TVector& v);

	float& operator[](int i);
	const float& operator[](int i) const;

	TVector& operator= (const TPoint& t);
	TVector& operator= (const TVector& t);
	TVector operator+ (const TVector & a);
	TVector operator- (const TVector & a);
	TVector operator-();
	friend std::ostream & operator << (std::ostream &os, const TVector &v);

	float operator|(const TVector & a);
	// cross product
	TVector operator*(const TVector & a);

	// scale by a scalar
	TVector operator*(const float f);

	// x*x, y*y, z*z
	TVector mult(const TVector & a);

	float normalize(int change = 1);

	// ADDED FUNCTIONS
	void normalize2();
	static float magnitude(const TVector& v);
	float dot(const TVector& w);

	/// Remove it after done with T-Splines
	// *---*---*  --> T-Vertex in t direction
	//     |
	// *---*---*
	int type;
	// 0 - regular, 
	// 5 - T-vertex in s dir., 
	// LEFT: 4 - extended one bay , 3 - extended 2 bays, 2 - extended 3 bays 
	// RIGHT: 6 - extended one bay , 7 - extended 2 bays, 8 - extended 3 bays 
	// 15 - T-vertex in t dir., 
	// LEFT: 14 - extended one bay, 13 - extended 2 bays, 12 - extended 3 bays
	// RIGHT: 16 - extended one bay, 17 - extended 2 bays, 18 - extended 3 bays
	// 29 - Dummy vertex 

	// ADD DUMMY IN X and Y

private:
	static float normalize(const TVector& v);

};

class TPoint {
private:
	TTuple3f v;
public:

	int type;

	// basic constructors
	TPoint();
	TPoint(float x);
	TPoint(float x, float y);
	TPoint(float x, float y, float z);
	TPoint(const TPoint& p);
	TPoint(const TVector& v);

	float& operator[](int i);
	const float& operator[](int i) const;
	TPoint& operator= (const TPoint& t);
	TPoint& operator= (const TVector& t);
	TPoint operator+ (const TVector & a);

	// points can be added as well (usefull for spline evaluation
	TPoint operator+ (const TPoint & a);

	TVector operator- (const TPoint & a);

	// scale by a scalar
	TPoint operator*(const float f);
};

// float pre-multiplication operators
TVector operator*(const float f, TVector v);
TPoint operator*(const float f, TPoint p);

// matrix operators
TMatrix4x4 operator*(const TMatrix4x4& a, const TMatrix4x4& b);
TVector operator*(const TMatrix4x4& m, const TVector& v);
TPoint operator*(const TMatrix4x4& m, const TPoint& v);

// some presetup matrices

TMatrix4x4 TRotate(double alfa, int axis);/* 0, 1, 2 = x, y, z */
TMatrix4x4 TTranslate(const TVector& v);
TMatrix4x4 TTranslate(float dx, float dy, float dz);
TMatrix4x4 TScale(float sx, float sy, float sz);
TMatrix4x4 TScale(const TVector& v);
float GrRad(float alfa);

// some helper functions
TVector getNormal(TPoint P1, TPoint P2, TPoint P3);


//}

#endif // T_TUPLE__H_
