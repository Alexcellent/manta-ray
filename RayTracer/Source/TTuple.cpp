#include <math.h>

#include "TTuple.h"

template<class C, int N>
C& TTuple<C, N>::operator[] (int i){
	i = i % N;
	return v[i];
}

// better write comments here ....
template<class C, int N>
const C& TTuple<C, N>::operator[] (int i) const {
	i = i % N;
	return v[i];
}

template<class C, int N>
TTuple<C, N>& TTuple<C, N>::operator= (const TTuple<C, N>& t) {
	for (int i = 0; i<N; i += 1) {
		v[i] = t[i];
	}
	return (*this);
}


template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator+ (const TTuple<C, N>& a){
	TTuple<C, N> ret;
	for (int i = 0; i<N; i += 1) {
		ret[i] = v[i] + a[i];
	}
	return ret;
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator+= (const TTuple<C, N>& a){
	for (int i = 0; i<N; i += 1) {
		v[i] += a[i];
	}
	return (*this);
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator- (const TTuple<C, N>& a){
	TTuple<C, N> ret;
	for (int i = 0; i<N; i += 1) {
		ret[i] = v[i] - a[i];
	}
	return ret;
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator-= (const TTuple<C, N>& a){
	for (int i = 0; i<N; i += 1) {
		v[i] -= v[i];
	}
	return (*this);
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator* (const TTuple<C, N>& a){
	TTuple<C, N> ret;
	for (int i = 0; i<N; i += 1) {
		ret[i] = v[i] * a[i];
	}
	return ret;
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator*= (const TTuple<C, N>& a){
	for (int i = 0; i<N; i += 1) {
		v[i] *= a[i];
	}
	return (*this);
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator* (const C& a){
	TTuple<C, N> ret;
	for (int i = 0; i<N; i += 1) {
		ret[i] = v[i] * a;
	}
	return ret;
}

template<class C, int N>
TTuple<C, N> TTuple<C, N>::operator*= (const C& a){
	for (int i = 0; i<N; i += 1) {
		v[i] *= a;
	}
	return (*this);
}


template<class C, int N>
C TTuple<C, N>::operator|(const TTuple<C, N>& a){
	C ret = 0;
	for (int i = 0; i<N; i += 1) {
		ret += v[i] * a[i];
	}
	return ret;
}


template<class C, int N>
TTuple<C, N> operator* (const C a, const TTuple<C, N>& t){
	TTuple<C, N> ret;
	for (int i = 0; i<N; i += 1) {
		ret[i] = t[i] * a;
	}
	return ret;
}

/// matrix stuff

template<class T, int L, int C>
TMatrix< T, L, C>::TMatrix(const TMatrix<T, L, C>& M){
	for (int i = 0; i<L; i += 1)
	for (int j = 0; j<C; j += 1)
		elem(i, j) = M(i, j);
}

template<class T, int L, int C>
TMatrix<T, L, C>& TMatrix<T, L, C>::operator=(const TMatrix<T, L, C>& M){
	for (int i = 0; i<L; i += 1)
	for (int j = 0; j<C; j += 1)
		elem(i, j) = M(i, j);

	return *this;
}


template<class T, int L, int C>
void TMatrix<T, L, C>::setNull(){
	for (int i = 0; i<L; i += 1)
	for (int j = 0; j<C; j += 1)
		elem(i, j) = 0;
}

template<class T, int L, int C>
void TMatrix<T, L, C>::setIdentity(){
	int min = (L<C) ? L : C;

	setNull();

	for (int i = 0; i<min; i += 1)
		elem(i, i) = 1;
}


template<class T, int L, int C>
TMatrix<T, L, C>::TMatrix(){
	setIdentity();
}

template<class T, int L, int C>
T& TMatrix<T, L, C>::operator() (int l, int c){
	return elem(l, c);
}

template<class T, int L, int C>
const T&TMatrix<T, L, C>:: operator()(int l, int c) const{
	return  v[c*L + l];
}

template<class T, int L, int C>
T& TMatrix<T, L, C>::elem(int l, int c){
	return v[c*L + l];
}


template<class T, int L, int C>
T& TMatrix<T, L, C>::operator[] (int n){
	return v[n];
}

template<class T, int L, int C>
const T& TMatrix<T, L, C>::operator[] (int n) const{
	return v[n];
}

template<class T, int L, int C>
TMatrix<T, L, C> TMatrix<T, L, C>::operator+ (const TMatrix<T, L, C> & a){
	TMatrix<T, L, C> ret;
	for (int i = 0; i<L*C; i += 1) {
		ret[i] = v[i] + a[i];
	}
	return ret;
}

template<class T, int L, int C>
TMatrix<T, L, C> TMatrix<T, L, C>::operator+= (const TMatrix<T, L, C> & a){
	for (int i = 0; i<L*C; i += 1) {
		v[i] += a[i];
	}
	return (*this);
}

template<class T, int L, int C>
TMatrix<T, L, C> TMatrix<T, L, C>::operator- (const TMatrix<T, L, C> & a){
	TMatrix<T, L, C> ret;
	for (int i = 0; i<L*C; i += 1) {
		ret[i] = v[i] - a[i];
	}
	return ret;
}

template<class T, int L, int C>
TMatrix<T, L, C> TMatrix<T, L, C>::operator-= (const TMatrix<T, L, C> & a){
	for (int i = 0; i<L*C; i += 1) {
		v[i] -= a[i];
	}
	return (*this);
}

template<class T, int L, int C>
T* TMatrix<T, L, C>::getBuffer(){
	return v;
}


/* does not work, don't know why */
/*
template<int K>
TMatrix<T, L, K> operator* (const TMatrix<T, C, K> & a){
TMatrix<T, L, K> ret;
for (int i=0; i<L; i+=1)
for(int j = 0;j<K;j+=1){
ret(i, j) = 0;
for(int k=0;k<C;k+=1)
ret(i,j) += elem(i, k) * a(k, j);
}
return ret;
}
*/


// Vector stuff

TVector::TVector(){
	v[0] = v[1] = v[2] = 0;

	type = 0;
}

TVector::TVector(float x){
	v[0] = x;
	v[1] = v[2] = 0;

	type = 0;
}
TVector::TVector(float x, float y){
	v[0] = x;
	v[1] = y;
	v[2] = 0;

	type = 0;
}

TVector::TVector(float x, float y, float z){
	v[0] = x;
	v[1] = y;
	v[2] = z;

	type = 0;
}


TVector::TVector(const TPoint& p){
	for (int i = 0; i<3; i += 1) {
		v[i] = p[i];
	}

	type = 0;
}

TVector::TVector(const TVector& t){
	for (int i = 0; i<3; i += 1) {
		v[i] = t[i];
	}
	type = t.type;
}


float& TVector::operator[](int i){
	i = i % 3;
	return v[i];
}

const float& TVector::operator[](int i) const{
	i = i % 3;
	return v[i];
}


TVector& TVector::operator= (const TPoint& t) {
	for (int i = 0; i<3; i += 1) {
		v[i] = t[i];
	}

	type = t.type;
	return (*this);
}

TVector& TVector::operator= (const TVector& t) {
	for (int i = 0; i<3; i += 1) {
		v[i] = t[i];
	}

	type = t.type;

	return (*this);
}


TVector TVector::operator+ (const TVector & a){
	TVector ret;
	for (int i = 0; i<3; i += 1) {
		ret[i] = v[i] + a[i];
	}
	return ret;
}

TVector TVector::operator- (const TVector & a){
	TVector ret;
	for (int i = 0; i<3; i += 1) {
		ret[i] = v[i] - a[i];
	}
	return ret;
}

TVector TVector::operator-() {
	TVector ret;
	for (int i = 0; i<3; i += 1) {
		ret[i] = -v[i];
	}
	return ret;
}

float TVector::operator|(const TVector & a){
	float ret = 0;
	for (int i = 0; i<3; i += 1)
		ret += v[i] * a[i];

	return ret;
}

// cross product
TVector TVector::operator*(const TVector & a){
	TVector ret;

	ret[0] = v[1] * a[2] - v[2] * a[1];
	ret[1] = v[2] * a[0] - v[0] * a[2];
	ret[2] = v[0] * a[1] - v[1] * a[0];

	return ret;
}

TVector TVector::operator*(const float f){
	TVector ret;
	ret.v = v * f;
	return ret;
}

TVector TVector::mult(const TVector & a){
	TVector ret;
	ret[0] = v[0] * a[0];
	ret[1] = v[1] * a[1];
	ret[2] = v[2] * a[2];
	return ret;
}


std::ostream & operator << (std::ostream &os, const TVector &v)
{
	os << "[" << v[0] << " " << v[1] << " " << v[2] << "]";
	return os;
}


float TVector::normalize(int change){

	float ret = normalize(*this);
	if (!change)
		return ret;

	float m = 1 / ret;

	for (int i = 0; i<3; i += 1)
		v[i] *= m;

	return ret;
}

float TVector::normalize(const TVector& v){
	float ret = 0;
	for (int i = 0; i<3; i += 1)
		ret += v[i] * v[i];

	return sqrt(ret);
}

// Added function
void TVector::normalize2(){
	float length = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
	for (int i = 0; i < 3; i++)
		v[i] /= length;
}

float TVector::magnitude(const TVector& v) {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float TVector::dot(const TVector& w) {
	return (v[0] * w[0] + v[1] * w[1] + v[2] * w[2]);
}


// TPoint starts here

TPoint::TPoint(){
	v[0] = v[1] = v[2] = 0;

	type = 0;
}

TPoint::TPoint(float x){
	v[0] = x;
	v[1] = v[2] = 0;

	type = 0;

}
TPoint::TPoint(float x, float y){
	v[0] = x;
	v[1] = y;
	v[2] = 0;

	type = 0;
}

TPoint::TPoint(float x, float y, float z){
	v[0] = x;
	v[1] = y;
	v[2] = z;

	type = 0;
}


TPoint::TPoint(const TPoint& p){
	for (int i = 0; i<3; i += 1) {
		v[i] = p[i];
	}

	type = p.type;
}

TPoint::TPoint(const TVector& t){
	for (int i = 0; i<3; i += 1) {
		v[i] = t[i];
	}

	type = t.type;

}

float& TPoint::operator[](int i){
	i = i % 3;
	return v[i];
}

const float& TPoint::operator[](int i) const{
	i = i % 3;
	return v[i];
}

TPoint& TPoint::operator= (const TPoint& t) {
	for (int i = 0; i<3; i += 1) {
		v[i] = t[i];
	}

	type = t.type;

	return (*this);
}

TPoint& TPoint::operator= (const TVector& t) {
	for (int i = 0; i<3; i += 1) {
		v[i] = t[i];
	}

	type = t.type;

	return (*this);
}


TPoint TPoint::operator+ (const TVector & a){
	TPoint ret;
	for (int i = 0; i<3; i += 1) {
		ret[i] = v[i] + a[i];
	}
	return ret;
}

TPoint TPoint::operator+ (const TPoint & a){
	TPoint ret;
	for (int i = 0; i<3; i += 1) {
		ret[i] = v[i] + a[i];
	}
	return ret;
}

TVector TPoint::operator- (const TPoint & a){
	TVector ret;
	for (int i = 0; i<3; i += 1) {
		ret[i] = v[i] - a[i];
	}
	return ret;
}

TPoint TPoint::operator*(const float f){
	TPoint ret;
	ret.v = v * f;
	return ret;

}

// float pre-multiplication operators
TVector operator*(const float f, TVector v){
	return v*f;
}

TPoint operator*(const float f, TPoint p){
	return p*f;
}


/// matrix operations


TMatrix4x4 operator*(const TMatrix4x4& a, const TMatrix4x4& b){
	TMatrix4x4 ret;

	for (int i = 0; i<4; i += 1)
	for (int j = 0; j<4; j += 1){
		ret(i, j) = 0;
		for (int k = 0; k<4; k += 1)
			ret(i, j) += a(i, k) * b(k, j);

	}
	return ret;
}

// matrix operators
TVector operator*(const TMatrix4x4& m, const TVector& v){
	TVector ret;

	for (int i = 0; i<3; i += 1){
		ret[i] = 0;
		for (int j = 0; j<3; j += 1)
			ret[i] += m(i, j) * v[j];

	}
	return ret;
}

// matrix operators
TPoint operator*(const TMatrix4x4& m, const TPoint& v){
	TPoint ret;

	for (int i = 0; i<3; i += 1){
		ret[i] = 0;
		for (int j = 0; j<3; j += 1)
			ret[i] += m(i, j) * v[j];

		// add the last element
		ret[i] += m(i, 3);
	}
	return ret;
}



// some presetup matrices

TMatrix4x4 TRotate(double alfa, int axis)/* 0, 1, 2 = x, y, z */
{
	TMatrix4x4 matrix;

	switch (axis)
	{
	case 0:
		matrix(1, 1) = cos(alfa);
		matrix(1, 2) = -sin(alfa);
		matrix(2, 1) = sin(alfa);
		matrix(2, 2) = cos(alfa);
		break;
	case 1:
		matrix(0, 0) = cos(alfa);
		matrix(0, 2) = sin(alfa);
		matrix(2, 0) = -sin(alfa);
		matrix(2, 2) = cos(alfa);
		break;
	case 2:
		matrix(0, 0) = cos(alfa);
		matrix(0, 1) = -sin(alfa);
		matrix(1, 0) = sin(alfa);
		matrix(1, 1) = cos(alfa);
		break;
	default:

		break;
	}

	return matrix;
}

TMatrix4x4 TTranslate(const TVector& v)
{
	TMatrix4x4 ret;

	for (int i = 0; i<3; i += 1)
		ret(i, 3) = v[i];

	return ret;
}

TMatrix4x4 TTranslate(float dx, float dy, float dz)
{
	TMatrix4x4 matrix;

	matrix(0, 3) = dx;
	matrix(1, 3) = dy;
	matrix(2, 3) = dz;

	return matrix;
}

TMatrix4x4 TScale(float sx, float sy, float sz)
{
	TMatrix4x4 matrix;
	matrix(0, 0) = sx;
	matrix(1, 1) = sy;
	matrix(2, 2) = sz;

	return matrix;
}

TMatrix4x4 TScale(const TVector& v)
{
	TMatrix4x4 ret;

	for (int i = 0; i<3; i += 1)
		ret(i, i) = v[i];

	return ret;
}


float GrRad(float alfa)
{
	float ret;

	ret = (float)(((float)(alfa * (float)M_PI)) / (float)180);

	return ret;
}
// explicit instantiation here
template class TMatrix<float, 4, 4>;
template class TTuple<float, 3>;


TVector getNormal(TPoint P1, TPoint P2, TPoint P3){
	TVector V1 = P2 - P1;
	TVector V2 = P3 - P1;
	return V1*V2;
}
