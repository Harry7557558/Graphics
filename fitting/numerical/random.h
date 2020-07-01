// define _RANDOM_H_BETTER_QUALITY before including this file for better random number quality


#ifndef _INC_RANDOM_H

#define _INC_RANDOM_H


#ifndef __INC_GEOMETRY_H
#include "geometry.h"
#endif



// hash function
unsigned hashu(unsigned x) {
	x = ((x >> 16) ^ x) * 0x45d9f3bu;
	x = ((x >> 16) ^ x) * 0x45d9f3bu;
	return x = (x >> 16) ^ x;
}



// random number generators
// use to generate floatpoint random number

#ifndef _RANDOM_H_BETTER_QUALITY

// from Numerical Recipes
unsigned int _IDUM = 0;
unsigned randu() { return _IDUM = _IDUM * 1664525 + 1013904223; }

#else

// a better random number generator
static unsigned _SEED_X = 123456789, _SEED_Y = 362436069, _SEED_Z = 521288629;
unsigned randu() {
	_SEED_X ^= _SEED_X << 16; _SEED_X ^= _SEED_X >> 5; _SEED_X ^= _SEED_X << 1;
	unsigned t = _SEED_X; _SEED_X = _SEED_Y; _SEED_Y = _SEED_Z; _SEED_Z = t ^ _SEED_X ^ _SEED_Y;
	return _SEED_Z;
}
void _SRAND(unsigned i) { _SEED_X = hashu(i); _SEED_Y = hashu(i + 1); _SEED_Z = hashu(i + 2); }

#endif



// float-point random number generators

#ifndef PI
#define PI 3.1415926535897932384626
#endif

// approximation of inverse error function, use to generate normal distribution
double erfinv(double x) {
	double n = log(1 - x * x), t = 0.5 * n + 2 / (PI*0.147);
	return (x < 0 ? -1 : 1) * sqrt(-t + sqrt(t*t - n / 0.147));
}

double randf(double a, double b) { return a + (randu() / 4294967296.)*(b - a); }  // uniform distribution in [a,b)
vec2 randv(double r) { double m = randf(0, r), a = randf(0, 2.*PI); return vec2(m*cos(a), m*sin(a)); }  // default distribution in |v|<r
vec2 randv_u(double r) { double m = sqrt(randf(0, r*r)), a = randf(0, 2.*PI); return vec2(m*cos(a), m*sin(a)); }  // uniform distribution in |v|<r
double randf_n(double a) { return sqrt(2.) * a * erfinv(2. * randf(0., 1.) - 1.); }  // normal distribution by standard deviation
vec2 randv_n(double a) { return vec2(randf_n(a), randf_n(a)); }  // normal distribution by standard deviation


#endif

