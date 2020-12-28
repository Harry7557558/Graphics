// vector, matrix, small useful functions


#ifndef __INC_GEOMETRY_H

#define __INC_GEOMETRY_H

#include <cmath>
#include <math.h>
#include <stdlib.h>

#ifndef PI
#define PI 3.1415926535897932384626
#endif

// defining max and min as macros may cause problems in standard libraries
// NO `using namespace std;`
#undef max
#undef min
template<typename T> inline T max(T x, T y) { return (x > y ? x : y); }
template<typename T> inline T min(T x, T y) { return (x < y ? x : y); }
template<typename T, typename t> inline T clamp(T x, t a, t b) { return (x<a ? a : x>b ? b : x); }
template<typename T, typename f> inline T mix(T x, T y, f a) { return (x * (f(1) - a) + y * a); }  // lerp

#if 1
#define invsqrt(x) (1.0/sqrt(x))
#else
#include <stdint.h>
double invsqrt(double x) {
	// https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf#page=49
	uint64_t i;
	double x2, y;
	x2 = x * 0.5;
	y = x;
	i = *(uint64_t*)&y;
	i = 0x5fe6eb50c7b537a9 - (i >> 1);
	y = *(double*)&i;
	y = y * (1.5 - (x2*y*y));
	y = y * (1.5 - (x2*y*y));
	return y;
}
#endif


// a sketchy planar vector template

struct vec2;
struct ivec2 {
	int x, y;
	ivec2() {}
	ivec2(int a) :x(a), y(a) {}
	ivec2(int x, int y) :x(x), y(y) {}
	ivec2(vec2 p);
	ivec2 operator - () const { return ivec2(-x, -y); }
	ivec2 operator + (const ivec2 &v) const { return ivec2(x + v.x, y + v.y); }
	ivec2 operator - (const ivec2 &v) const { return ivec2(x - v.x, y - v.y); }
};

struct vec2 {
	double x, y;
	explicit vec2() {}
	explicit vec2(const double &a) :x(a), y(a) {}
	explicit vec2(const double &x, const double &y) :x(x), y(y) {}
	explicit vec2(const ivec2 &p) :x(p.x), y(p.y) {}

	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (const vec2 &v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (const vec2 &v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (const vec2 &v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (const double &a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(const vec2 &v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(const vec2 &v) { double m = invsqrt(v.x*v.x + v.y*v.y); return vec2(v.x*m, v.y*m); }
	friend double dot(const vec2 &u, const vec2 &v) { return u.x*v.x + u.y*v.y; }
	friend double det(const vec2 &u, const vec2 &v) { return u.x*v.y - u.y*v.x; }
	friend double ndot(const vec2 &u, const vec2 &v) { return dot(u, v) * invsqrt(u.sqr()*v.sqr()); }
	friend double ndet(const vec2 &u, const vec2 &v) { return det(u, v) * invsqrt(u.sqr()*v.sqr()); }

	void operator += (const vec2 &v) { x += v.x, y += v.y; }
	void operator -= (const vec2 &v) { x -= v.x, y -= v.y; }
	void operator *= (const vec2 &v) { x *= v.x, y *= v.y; }
	friend vec2 operator * (const double &a, const vec2 &v) { return vec2(a*v.x, a*v.y); }
	void operator *= (const double &a) { x *= a, y *= a; }
	vec2 operator / (const double &a) const { return vec2(x / a, y / a); }
	void operator /= (const double &a) { x /= a, y /= a; }

	vec2 yx() const { return vec2(y, x); }
	vec2 rot() const { return vec2(-y, x); }
	vec2 rotr() const { return vec2(y, -x); }
	vec2 rotate(double a) const { double ca = cos(a), sa = sin(a); return vec2(ca*x - sa * y, sa*x + ca * y); }

	bool operator == (const vec2 &v) const { return x == v.x && y == v.y; }
	bool operator != (const vec2 &v) const { return x != v.x || y != v.y; }
	vec2 operator / (const vec2 &v) const { return vec2(x / v.x, y / v.y); }
	void operator /= (const vec2 &v) { x /= v.x, y /= v.x; }
	friend vec2 pMax(const vec2 &a, const vec2 &b) { return vec2(max(a.x, b.x), max(a.y, b.y)); }
	friend vec2 pMin(const vec2 &a, const vec2 &b) { return vec2(min(a.x, b.x), min(a.y, b.y)); }
	friend vec2 abs(const vec2 &a) { return vec2(abs(a.x), abs(a.y)); }
	friend vec2 floor(const vec2 &a) { return vec2(floor(a.x), floor(a.y)); }
	friend vec2 ceil(const vec2 &a) { return vec2(ceil(a.x), ceil(a.y)); }
	friend vec2 sqrt(const vec2 &a) { return vec2(sqrt(a.x), sqrt(a.y)); }
	friend vec2 sin(const vec2 &a) { return vec2(sin(a.x), sin(a.y)); }
	friend vec2 cos(const vec2 &a) { return vec2(cos(a.x), cos(a.y)); }
	friend vec2 atan(const vec2 &a) { return vec2(atan(a.x), atan(a.y)); }
	friend vec2 exp(const vec2 &a) { return vec2(exp(a.x), exp(a.y)); }
	friend vec2 log(const vec2 &a) { return vec2(log(a.x), log(a.y)); }
};

ivec2::ivec2(vec2 p) :x((int)p.x), y((int)p.y) {}

vec2 sincos(double a) { return vec2(sin(a), cos(a)); }
vec2 cossin(double a) { return vec2(cos(a), sin(a)); }



struct mat2 {
public:
	double v[2][2];
	mat2() {}
	explicit mat2(double k) {
		v[0][0] = v[1][1] = k, v[1][0] = v[0][1] = 0;
	}
	explicit mat2(vec2 lambda) {
		v[0][0] = lambda.x, v[1][1] = lambda.y;
		v[1][0] = v[0][1] = 0;
	}
	explicit mat2(vec2 col1, vec2 col2) {
		v[0][0] = col1.x, v[1][0] = col1.y;
		v[0][1] = col2.x, v[1][1] = col2.y;
	}
	explicit mat2(double _00, double _10, double _01, double _11) {  // by columns
		v[0][0] = _00, v[0][1] = _01, v[1][0] = _10, v[1][1] = _11;
	}
	vec2 row(int i) const { return vec2(v[i][0], v[i][1]); }
	vec2 column(int i) const { return vec2(v[0][i], v[1][i]); }
	vec2 diag() const { return vec2(v[0][0], v[1][1]); }
	mat2 operator - () const { return mat2(-v[0][0], -v[1][0], -v[0][1], -v[1][1]); }
	void operator += (const mat2 &m) { for (int i = 0; i < 4; i++) (&v[0][0])[i] += (&m.v[0][0])[i]; }
	void operator -= (const mat2 &m) { for (int i = 0; i < 4; i++) (&v[0][0])[i] -= (&m.v[0][0])[i]; }
	void operator *= (double m) { for (int i = 0; i < 4; i++) (&v[0][0])[i] *= m; }
	mat2 operator + (const mat2 &m) const { mat2 r; for (int i = 0; i < 4; i++) (&r.v[0][0])[i] = (&v[0][0])[i] + (&m.v[0][0])[i]; return r; }
	mat2 operator - (const mat2 &m) const { mat2 r; for (int i = 0; i < 4; i++) (&r.v[0][0])[i] = (&v[0][0])[i] - (&m.v[0][0])[i]; return r; }
	mat2 operator * (double m) const { mat2 r; for (int i = 0; i < 4; i++) (&r.v[0][0])[i] = (&v[0][0])[i] * m; return r; }
	friend mat2 operator * (double a, const mat2 &m) { mat2 r; for (int i = 0; i < 4; i++) (&r.v[0][0])[i] = a * (&m.v[0][0])[i]; return r; }
	mat2 operator * (const mat2 &m) const { return mat2(v[0][0] * m.v[0][0] + v[0][1] * m.v[1][0], v[1][0] * m.v[0][0] + v[1][1] * m.v[1][0], v[0][0] * m.v[0][1] + v[0][1] * m.v[1][1], v[1][0] * m.v[0][1] + v[1][1] * m.v[1][1]); }
	friend double determinant(const mat2 &m) { return m.v[0][0] * m.v[1][1] - m.v[0][1] * m.v[1][0]; }
	mat2 transpose() const { mat2 r; for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) r.v[i][j] = v[j][i]; return r; }
	mat2 inverse() const { double d = 1.0 / (v[0][0] * v[1][1] - v[0][1] * v[1][0]); return mat2(d*v[1][1], -d * v[1][0], -d * v[0][1], d*v[0][0]); }
	friend double trace(const mat2 &m) { return m.v[0][0] + m.v[1][1]; }
	vec2 operator * (const vec2 &a) const { return vec2(v[0][0] * a.x + v[0][1] * a.y, v[1][0] * a.x + v[1][1] * a.y); }
};

mat2 rotationMatrix2d(double a) {
	double ca = cos(a), sa = sin(a);
	return mat2(ca, -sa, sa, ca);
}



// a more sketchy 3d vector template

struct vec3;

struct ivec3 {
	int x, y, z;
	ivec3() {}
	explicit ivec3(int a) :x(a), y(a), z(a) {}
	explicit ivec3(int x, int y, int z) : x(x), y(y), z(z) {}
	explicit ivec3(vec3 p);
};

struct vec3 {
	double x, y, z;
	vec3() {}
	explicit vec3(double a) :x(a), y(a), z(a) {}
	explicit vec3(double x, double y, double z) :x(x), y(y), z(z) {}
	explicit vec3(vec2 p, double z = 0) :x(p.x), y(p.y), z(z) {}
	explicit vec3(ivec3 p) :x(p.x), y(p.y), z(p.z) {}
	vec3 operator - () const { return vec3(-x, -y, -z); }
	vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator * (const vec3 &v) const { return vec3(x*v.x, y*v.y, z*v.z); }  // element wise
	vec3 operator * (const double &k) const { return vec3(k * x, k * y, k * z); }
	double sqr() const { return x * x + y * y + z * z; }
	friend double length(vec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
	friend vec3 normalize(vec3 v) { double m = invsqrt(v.x*v.x + v.y*v.y + v.z*v.z); return vec3(v.x*m, v.y*m, v.z*m); }
	friend double dot(vec3 u, vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
	friend vec3 cross(vec3 u, vec3 v) { return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
	friend double det(vec3 a, vec3 b, vec3 c) { return dot(a, cross(b, c)); }
	friend double ndot(const vec3 &u, const vec3 &v) { return dot(u, v) * invsqrt(u.sqr()*v.sqr()); }
	friend vec3 ncross(vec3 u, vec3 v) { return normalize(cross(u, v)); }

	void operator += (const vec3 &v) { x += v.x, y += v.y, z += v.z; }
	void operator -= (const vec3 &v) { x -= v.x, y -= v.y, z -= v.z; }
	void operator *= (const vec3 &v) { x *= v.x, y *= v.y, z *= v.z; }
	vec3 operator / (const vec3 &v) const { return vec3(x / v.x, y / v.y, z / v.z); }
	void operator /= (const vec3 &v) { x /= v.x, y /= v.y, z /= v.z; }
	friend vec3 operator * (const double &a, const vec3 &v) { return vec3(a*v.x, a*v.y, a*v.z); }
	void operator *= (const double &a) { x *= a, y *= a, z *= a; }
	vec3 operator / (const double &a) const { return vec3(x / a, y / a, z / a); }
	void operator /= (const double &a) { x /= a, y /= a, z /= a; }

	bool operator == (const vec3 &v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator != (const vec3 &v) const { return x != v.x || y != v.y || z != v.z; }
	friend vec3 pMax(const vec3 &a, const vec3 &b) { return vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
	friend vec3 pMin(const vec3 &a, const vec3 &b) { return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
	friend vec3 abs(const vec3 &a) { return vec3(abs(a.x), abs(a.y), abs(a.z)); }
	friend vec3 exp(const vec3 &a) { return vec3(exp(a.x), exp(a.y), exp(a.z)); }

	vec2 xy() const { return vec2(x, y); }
	vec2 xz() const { return vec2(x, z); }
	vec2 yz() const { return vec2(y, z); }
	vec3 xyz() const { return vec3(x, y, z); }
	vec3 xzy() const { return vec3(x, z, y); }
	vec3 yzx() const { return vec3(y, z, x); }
	vec3 yxz() const { return vec3(y, x, z); }
	vec3 zxy() const { return vec3(z, x, y); }
	vec3 zyx() const { return vec3(z, y, x); }
	vec3 rz90() const { return vec3(-y, x, z); }
	vec3 rz180() const { return vec3(-x, -y, z); }
	vec3 rz270() const { return vec3(y, -x, z); }
	vec3 ry90() const { return vec3(z, y, -x); }
	vec3 ry180() const { return vec3(-x, y, -z); }
	vec3 ry270() const { return vec3(-z, y, x); }
	vec3 rx90() const { return vec3(x, -z, y); }
	vec3 rx180() const { return vec3(x, -y, -z); }
	vec3 rx270() const { return vec3(x, z, -y); }
};

ivec3::ivec3(vec3 p) : x((int)p.x), y((int)p.y), z((int)p.z) {}


class mat3 {
public:
	double v[3][3];
	mat3() {}
	explicit mat3(double k) {  // diagonal matrix
		for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
			v[i][j] = k * (i == j);
	}
	explicit mat3(vec3 lambda) {  // diagonal matrix
		for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
			v[i][j] = i == j ? ((double*)&lambda)[i] : 0;
	}
	explicit mat3(vec3 lambda, double xy, double xz, double yz) {  // symmetric matrix
		v[0][0] = lambda.x, v[1][1] = lambda.y, v[2][2] = lambda.z;
		v[0][1] = v[1][0] = xy, v[0][2] = v[2][0] = xz, v[1][2] = v[2][1] = yz;
	}
	explicit mat3(vec3 i, vec3 j, vec3 k) {  // matrix by column vectors
		for (int u = 0; u < 3; u++) v[u][0] = ((double*)&i)[u], v[u][1] = ((double*)&j)[u], v[u][2] = ((double*)&k)[u];
	}
	explicit mat3(double _00, double _01, double _02, double _10, double _11, double _12, double _20, double _21, double _22) {  // ordered in row-wise
		v[0][0] = _00, v[0][1] = _01, v[0][2] = _02, v[1][0] = _10, v[1][1] = _11, v[1][2] = _12, v[2][0] = _20, v[2][1] = _21, v[2][2] = _22;
	}
	vec3 row(int i) const { return vec3(v[i][0], v[i][1], v[i][2]); }
	vec3 column(int i) const { return vec3(v[0][i], v[1][i], v[2][i]); }
	vec3 diag() const { return vec3(v[0][0], v[1][1], v[2][2]); }
	void operator += (const mat3 &m) { for (int i = 0; i < 9; i++) (&v[0][0])[i] += (&m.v[0][0])[i]; }
	void operator -= (const mat3 &m) { for (int i = 0; i < 9; i++) (&v[0][0])[i] -= (&m.v[0][0])[i]; }
	void operator *= (double m) { for (int i = 0; i < 9; i++) (&v[0][0])[i] *= m; }
	mat3 operator + (const mat3 &m) const { mat3 r; for (int i = 0; i < 9; i++) (&r.v[0][0])[i] = (&v[0][0])[i] + (&m.v[0][0])[i]; return r; }
	mat3 operator - (const mat3 &m) const { mat3 r; for (int i = 0; i < 9; i++) (&r.v[0][0])[i] = (&v[0][0])[i] - (&m.v[0][0])[i]; return r; }
	mat3 operator * (double m) const { mat3 r; for (int i = 0; i < 9; i++) (&r.v[0][0])[i] = (&v[0][0])[i] * m; return r; }
	friend mat3 operator * (double a, const mat3 &m) { mat3 r; for (int i = 0; i < 9; i++) (&r.v[0][0])[i] = a * (&m.v[0][0])[i]; return r; }
	friend double determinant(const mat3 &m) { return m.v[0][0] * (m.v[1][1] * m.v[2][2] - m.v[1][2] * m.v[2][1]) - m.v[0][1] * (m.v[1][0] * m.v[2][2] - m.v[1][2] * m.v[2][0]) + m.v[0][2] * (m.v[1][0] * m.v[2][1] - m.v[1][1] * m.v[2][0]); }
	mat3 transpose() const { mat3 r; for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) r.v[i][j] = v[j][i]; return r; }
	friend double trace(const mat3 &m) { return m.v[0][0] + m.v[1][1] + m.v[2][2]; }
	friend double sumsqr(const mat3 &m) { double r = 0; for (int i = 0; i < 9; i++) r += (&m.v[0][0])[i] * (&m.v[0][0])[i]; return r; }  // sum of square of elements

	vec3 operator * (const vec3 &a) const { return vec3(v[0][0] * a.x + v[0][1] * a.y + v[0][2] * a.z, v[1][0] * a.x + v[1][1] * a.y + v[1][2] * a.z, v[2][0] * a.x + v[2][1] * a.y + v[2][2] * a.z); }
};

mat3 tensor(vec3 u, vec3 v) { return mat3(u*v.x, u*v.y, u*v.z); }
mat3 axis_angle(vec3 n, double a) {
	n = normalize(n); double ct = cos(a), st = sin(a);
	return mat3(
		ct + n.x*n.x*(1 - ct), n.x*n.y*(1 - ct) - n.z*st, n.x*n.z*(1 - ct) + n.y*st,
		n.y*n.x*(1 - ct) + n.z*st, ct + n.y*n.y*(1 - ct), n.y*n.z*(1 - ct) - n.x*st,
		n.z*n.x*(1 - ct) - n.y*st, n.z*n.y*(1 - ct) + n.x*st, ct + n.z*n.z*(1 - ct)
	);
}
mat3 rotationMatrix_x(double a) {
	double ca = cos(a), sa = sin(a);
	return mat3(
		1, 0, 0,
		0, ca, -sa,
		0, sa, ca
	);
}
mat3 rotationMatrix_y(double a) {
	double ca = cos(a), sa = sin(a);
	return mat3(
		ca, 0, sa,
		0, 1, 0,
		-sa, 0, ca
	);
}
mat3 rotationMatrix_z(double a) {
	double ca = cos(a), sa = sin(a);
	return mat3(
		ca, -sa, 0,
		sa, ca, 0,
		0, 0, 1
	);
}



// mostly used for debug output
double degree(double rad) {
	return rad * (180. / PI);
}



struct triangle {
	vec3 A, B, C;
	void translate(vec3 d) { A += d, B += d, C += d; }
	void scale(double s) { A *= s, B *= s, C *= s; }
	void applyMatrix(mat3 M) { A = M * A, B = M * B, C = M * C; }
	double area() const { return 0.5*length(cross(B - A, C - A)); }
};


#endif // __INC_GEOMETRY_H

