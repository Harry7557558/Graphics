// Fitting ellipse to point set experiment (incomplete)

/* To-do:
	 - try to make it not fit to hyperbolas (test all eigenvalues)
	 - handle the case when the matrix is not invertible
	 - implement orthogonal least square fitting
*/


#include <stdio.h>
#include <cmath>

#include <chrono>

#define PI 3.1415926535897932384626
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define clamp(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))

// vector template
class vec2 {
public:
	double x, y;
	vec2() {}
	explicit vec2(double a) :x(a), y(a) {}
	vec2(double x, double y) :x(x), y(y) {}
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (vec2 v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (vec2 v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (vec2 v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (double a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(vec2 v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(vec2 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend double dot(vec2 u, vec2 v) { return u.x*v.x + u.y*v.y; }
	friend double det(vec2 u, vec2 v) { return u.x*v.y - u.y*v.x; }
	vec2 rot() const { return vec2(-y, x); }
};

// random numbers
unsigned int _IDUM = 0;
unsigned randu() { return _IDUM = _IDUM * 1664525 + 1013904223; }  // from Numerical Recipes
double erfinv(double x) {
	// inverse error function approximation
	double n = log(1 - x * x), t = 0.5 * n + 2 / (PI*0.147);
	return (x < 0 ? -1 : 1) * sqrt(-t + sqrt(t*t - n / 0.147));
}
double randf(double a, double b) { return a + (randu() / 4294967296.)*(b - a); }  // uniform distribution in [a,b)
vec2 randv(double r) { double m = randf(0, r), a = randf(0, 2.*PI); return vec2(m*cos(a), m*sin(a)); }  // default distribution in |v|<r
vec2 randv_u(double r) { double m = sqrt(randf(0, r*r)), a = randf(0, 2.*PI); return vec2(m*cos(a), m*sin(a)); }  // uniform distribution in |v|<r
double randf_n(double a) { return sqrt(2.) * a * erfinv(2. * randf(0., 1.) - 1.); }  // normal distribution by standard deviation
vec2 randv_n(double a) { return vec2(randf_n(a), randf_n(a)); }  // normal distribution by standard deviation



// generate intended matrix, should be positive (semi)definite
void generateMatrix(double M[6][6], const vec2 *P, int N) {
	for (int i = 0; i < 6; i++) for (int j = 0; j < 6; j++) M[i][j] = 0;
	for (int n = 0; n < N; n++) {
		vec2 v = P[n];
		double k[6] = { v.x*v.x, v.x*v.y, v.y*v.y, v.x,v.y, 1. };
		for (int i = 0; i < 6; i++) for (int j = 0; j < 6; j++) {
			M[i][j] += k[i] * k[j];
		}
	}
}

// debug functions
void printMatrix(const double M[6][6], const char end[] = "\\\\\n") {  // latex output
	printf("\\begin{bmatrix}");
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (abs(M[i][j]) < 1e-6) printf("0");
			else printf("%.6g", M[i][j]);
			if (j < 5) putchar('&');
		}
		printf("\\\\");
	}
	printf("\\end{bmatrix}%s", end);
}
void printVector(const double v[6], const char end[] = "\n") {
	printf("(");
	for (int i = 0; i < 6; i++) printf("%lf%c", v[i], i == 5 ? ')' : ',');
	printf("%s", end);
}
void printPolynomial(const double C[], int N, const char end[] = "\n") {
	printf("%lgx^{%d}", C[0], N);
	for (int i = 1; i < N; i++) {
		printf("%+.16lf*x^{%d}", C[i], N - i);
	}
	printf("%+.16lf%s", C[N], end);
}




// matrix calculations
// all matrixes are 6x6

#pragma region Matrix

// copy a matrix
void matcpy(const double src[6][6], double res[6][6]) {
	for (int i = 0; i < 6; i++) for (int j = 0; j < 6; j++) res[i][j] = src[i][j];
}

// matrix multiplication, C needs to be different from A B
void matmul(const double A[6][6], const double B[6][6], double C[6][6]) {
	for (int i = 0; i < 6; i++) for (int j = 0; j < 6; j++) {
		C[i][j] = 0;
		for (int k = 0; k < 6; k++) C[i][j] += A[i][k] * B[k][j];
	}
}

// matrix times vector, b needs to be different from x
void matmul(const double A[6][6], const double x[6], double b[6]) {
	for (int i = 0; i < 6; i++) {
		b[i] = 0;
		for (int j = 0; j < 6; j++) b[i] += A[i][j] * x[j];
	}
}

// trace of a matrix
double trace(const double M[6][6]) {
	double res = 0;
	for (int i = 0; i < 6; i++) res += M[i][i];
	return res;
}

// determinant of a matrix
double determinant(const double M[6][6]) {
	double A[6][6]; matcpy(M, A);
	double det = 1;
	for (int i = 0; i < 6; i++) {
		for (int j = i + 1; j < 6; j++) {
			double m = -A[j][i] / A[i][i];
			for (int k = i; k < 6; k++) A[j][k] += m * A[i][k];
		}
		det *= A[i][i];
	}
	if (0.0*det != 0.0) return 0.0;
	return det;
}

// matrix inversion
void matinv(const double M[6][6], double I[6][6]) {
	double A[6][6]; matcpy(M, A);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 6; j++) I[i][j] = double(i == j);
	for (int i = 0; i < 6; i++) {
		double m = 1.0 / A[i][i];
		for (int k = 0; k < 6; k++) {
			A[i][k] *= m, I[i][k] *= m;
		}
		for (int j = 0; j < 6; j++) if (j != i) {
			m = -A[j][i];
			for (int k = 0; k < 6; k++) {
				A[j][k] += m * A[i][k];
				I[j][k] += m * I[i][k];
			}
		}
	}
}

#pragma endregion



// find the eigenpair of a matrix with the smallest eigenvalue
// assume all matrixes are 6x6 positive definite matrix

#pragma region Eigens

// find the smallest eigenvalue of the matrix by solving its characteristic equation
double EigenValue0_expand(const double M[6][6]) {
	// expanding characteristic polynomial, works in O(n^4)
	// method discribed at https://mathworld.wolfram.com/CharacteristicPolynomial.html
	double C[7], Ci = -1;
	double B[6][6]; matcpy(M, B);
	C[0] = 1;
	for (int i = 1; i <= 6; i++) {
		Ci = trace(B) / i;
		double T[6][6]; matcpy(B, T);
		for (int k = 0; k < 6; k++) T[k][k] -= Ci;
		matmul(M, T, B);
		C[i] = -Ci;
	}
	//printPolynomial(C, 6);
	// Newton's iteration method starting at x=0
	double x = 0;
	for (int i = 0; i < 64; i++) {
		double y = 0, dy = 0;
		for (int i = 0; i <= 6; i++) {
			y = y * x + C[i];
			if (6 - i) dy = dy * x + (6 - i)*C[i];
		}
		double dx = y / dy;
		x -= dx;
		if (dx*dx < 1e-24) break;
	}
	return x;
}

// find an eigenpair using power iteration and inverse iteration
void EigenPair_powIter(const double M[6][6], double &u, double a[6]) {
	for (int i = 0; i < 6; i++) a[i] = sqrt(1. / 6);
	double A[6][6]; matcpy(M, A);
#if 1
	double m = 1. / pow(determinant(A), 1. / 6);  // should be non-negative
	if (0.*m == 0.) {
		for (int i = 0; i < 6; i++) for (int j = 0; j < 6; j++) A[i][j] *= m;
		for (int i = 0; i < 2; i++) {
			double T[6][6]; matmul(A, A, T);
			matcpy(T, A);
		}
	}
#endif
	for (int i = 0; i < 1024; i++) {
		double v[6]; matmul(A, a, v);
		double m = 0;
		for (int j = 0; j < 6; j++) m += v[j] * v[j];
		m = 1. / sqrt(m);
		double err = 0;
		for (int j = 0; j < 6; j++) {
			v[j] *= m;
			err += v[j] * a[j];
			a[j] = v[j];
		}
		if (abs(abs(err) - 1) < 1e-12) break;
		// warning: floatpoint precision error
	}
	u = 0;
	double v[6]; matmul(M, a, v);
	for (int i = 0; i < 6; i++) u += v[i] * v[i];
	u = sqrt(u);
}
void EigenPair_invIter(const double M[6][6], double &u, double a[6]) {
	double A[6][6]; matinv(M, A);
	EigenPair_powIter(A, u, a);
	u = 1. / u;
}

#pragma endregion




// ellipse fitting - v contains the coefficients of { x^2, xy, y^2, x, y, 1 }
void fitEllipse(const vec2 *P, int N, double v[6]) {
	// Minimize v*Mv, subject to v*v=1
	// result may be hyperbola instead of ellipse
	double M[6][6]; generateMatrix(M, P, N);
	double lambda; EigenPair_invIter(M, lambda, v);
}
void fitEllipse1(const vec2 *P, int N, double v[6]) {
	double M[6][6]; generateMatrix(M, P, N);
	const double F[6][6] = { {0,0,2,0,0,0},{0,-1,0,0,0,0},{2,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	double I[6][6]; matinv(M, I);
	double B[6][6]; matmul(I, F, B);
	double u; EigenPair_powIter(B, u, v);
	//printf("%lf\n", 4.*v[0] * v[2] - v[1] * v[1]);
	u = 1. / u;
}





// visualizing ellipse fitting results
// contains graphing classes/variables/functions

#pragma region Visualization

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libraries/stb_image_write.h"

// color structs
typedef unsigned char byte;
typedef struct { byte r, g, b; } COLOR;
class COLORf {
public:
	double r, g, b;
	COLORf() {}
	COLORf(const double &r, const double &g, const double &b) :r(r), g(g), b(b) {}
	COLORf(const COLOR &col) :r(col.r / 255.0), g(col.g / 255.0), b(col.b / 255.0) {}
	COLORf operator - () const { return COLORf(-r, -g, -b); }
	COLORf operator + (const COLORf &v) const { return COLORf(r + v.r, g + v.g, b + v.b); }
	COLORf operator - (const COLORf &v) const { return COLORf(r - v.r, g - v.g, b - v.b); }
	COLORf operator * (const COLORf &v) const { return COLORf(r * v.r, g * v.g, b * v.b); }
	COLORf operator * (const double &k) const { return COLORf(k * r, k * g, k * b); }
};
COLOR toCOLOR(double c) {
	byte k = (byte)(255.0*clamp(c, 0., 1.));
	return COLOR{ k, k, k };
}
COLOR toCOLOR(double r, double g, double b) {
	return COLOR{
		(byte)(255.0*clamp(r, 0., 1.)),
		(byte)(255.0*clamp(g, 0., 1.)),
		(byte)(255.0*clamp(b, 0., 1.)),
	};
}
COLOR toCOLOR(const COLORf &col) {
	return toCOLOR(col.r, col.g, col.b);
}
COLOR mix(COLOR a, COLOR b, double d) {
	return toCOLOR(COLORf(a)*(1.0 - d) + COLORf(b)*d);
}

// image variables
#define W 900
#define H 600
COLOR canvas[W*H];
double buffer[W*H];
#define Scale 50
#define Center vec2(0, 0)
const vec2 fromCoord(0.5*W - Scale * Center.x, Scale*Center.y - (0.5 - 0.5*H));
const vec2 fromScreen(-0.5*W / Scale + Center.x, (0.5*H - 0.5) / Scale + Center.y);

// painting functions
void drawAxis(double width, COLOR col) {
	width *= 0.5;
	// go through all pixels (may be slow)
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			vec2 p = vec2(i, -j) * (1.0 / Scale) + fromScreen;
			p = vec2(abs(p.x), abs(p.y));
			double d = min(p.x, p.y) * Scale - width;
			//d = min(d, Scale * (.5 - max(abs(fmod(p.x, 1.) - .5), abs(fmod(p.y, 1.) - .5))));  // grid
			if (d < 0) canvas[j*W + i] = col;
			else if (d < 1) canvas[j*W + i] = mix(col, canvas[j*W + i], d);
		}
	}
}
void drawDot(vec2 c, double r, COLOR col) {
	vec2 C = vec2(c.x, -c.y) * Scale + fromCoord;
	r -= 0.5;
	int i0 = max(0, (int)floor(C.x - r - 1)), i1 = min(W - 1, (int)ceil(C.x + r + 1));
	int j0 = max(0, (int)floor(C.y - r - 1)), j1 = min(H - 1, (int)ceil(C.y + r + 1));
	for (int j = j0; j <= j1; j++) {
		for (int i = i0; i <= i1; i++) {
			vec2 p = vec2(i - 0.5*W, 0.5*H - (j + 1)) * (1.0 / Scale) + Center;
			double d = length(p - c) * Scale - r;
			if (d < 0) canvas[j*W + i] = col;
			else if (d < 1) canvas[j*W + i] = mix(col, canvas[j*W + i], d);  // shade by distance to anti-aliase
		}
	}
}
void drawQuadraticCurve(const double v[6], double width, COLOR col) {
	double r = 0.5*width;
	// initialize a value buffer
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			vec2 p = vec2(i, -j) * (1.0 / Scale) + fromScreen;
			double x = p.x, y = p.y;
			buffer[j*W + i] = v[0] * x*x + v[1] * x*y + v[2] * y*y + v[3] * x + v[4] * y + v[5];
		}
	}
	// map the value of the buffer to the image
	for (int j = 1; j < H - 1; j++) {
		for (int i = 1; i < W - 1; i++) {
			// calculate numerical gradient from neighbourhood values
			double dx = buffer[j*W + i + 1] - buffer[j*W + i - 1];
			double dy = buffer[j*W + i + W] - buffer[j*W + i - W];
			double m = .5*sqrt(dx * dx + dy * dy);  // magnitude of gradient
			double d = abs(buffer[j*W + i] / m) - r;  // divide by gradient to estimate distance
			if (d < 0) canvas[j*W + i] = col;
			else if (d < 1) canvas[j*W + i] = mix(col, canvas[j*W + i], d);
		}
	}
}

// initialize and save image
void init() {
	for (int i = 0, l = W * H; i < l; i++) canvas[i] = COLOR{ 255,255,255 };
}
bool save(const char* path) {
	return stbi_write_png(path, W, H, 3, canvas, 3 * W);
}

#pragma endregion






// random tests for debug

// test eigenvalue calculation
void randomTest_eigen() {
	auto t0 = std::chrono::high_resolution_clock::now();

	double M[6][6];
	for (int i = 0; i < 100000; i++)
	{
		_IDUM = i;
		vec2 P[10];
		for (int i = 0; i < 10; i++) P[i] = randv(1.0);
		generateMatrix(M, P, 10);
		//printMatrix(M);

		// calculate the smallest eigenvalue
		double lambda, eigv[6];
		double lambda0 = EigenValue0_expand(M);
		EigenPair_invIter(M, lambda, eigv);
		double e = lambda - lambda0;  // Note: it is better to compare the value of the characteristic polynomial
		if (e*e > 1e-12) {
			printf("#%d %lg\n", __LINE__, e);
		}

		// check
		double Ax[6]; matmul(M, eigv, Ax);
		double m = 0; for (int i = 0; i < 6; i++) m += Ax[i] * Ax[i]; m = 1. / sqrt(m);
		if ((e = abs(m * lambda - 1)) > 1e-6) {
			printf("#%d %lg\n", __LINE__, e);
		}
		for (int i = 0; i < 6; i++) Ax[i] *= m;
		m = 0; for (int i = 0; i < 6; i++) m += Ax[i] * eigv[i];
		if ((e = abs(m - 1)) > 1e-6) {
			printf("#%d %lg\n", __LINE__, e);
		}
	}

	printf("%lfs elapsed\n", std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count());
}

// generate point data shaped like an ellipse
void randomPointData(vec2 *P, int N) {
	// parameters of the ellipse
	double rx = abs(randf_n(1.) + 3.) + 1.;
	double ry = abs(randf_n(1.) + 3.) + 1.;
	double a = randf(0, 2.*PI);
	double x = randf(-4.0, 4.0);
	double y = randf(-3.5, 3.5);
	double ca = cos(a), sa = sin(a), rx2 = rx * rx, ry2 = ry * ry;
	double c[6];
	c[0] = ca * ca / rx2 + sa * sa / ry2, c[2] = sa * sa / rx2 + ca * ca / ry2;
	c[1] = 2. * sa * ca * (1. / ry2 - 1. / rx2);
	c[3] = c[4] = 0, c[5] = -1;
	// parameters of random number generator
	vec2 g = randv_n(5.0);
	double v = randf_n(2.0); v = v * v + 2.0;
	double f = randf_n(0.4); f = f * f + 0.1;
	// generating random points
	for (int i = 0; i < N; i++) {
		vec2 p = g + randv_n(v);
		// using iteration to make points close to the ellipse
		for (int t = 0; t < 6; t++) {
			double x = p.x, y = p.y;
			double z = c[0] * x*x + c[1] * x*y + c[2] * y*y + c[3] * x + c[4] * y + c[5];
			vec2 dz = vec2(2.*c[0] * x + c[1] * y + c[3], c[1] * x + 2.*c[2] * y + c[4]);
			p = p - dz * (z / dot(dz, dz));
		}
		// add noise
		P[i] = p + randv_n(f);
	}
}

// write lots of pictures to see fitting results
void randomTest_image() {
	for (int i = 0; i < 100; i++) {
		// generate point data
		_IDUM = i;
		const int N = 200;
		vec2 *P = new vec2[N]; randomPointData(P, N);
		// fitting
		double c0[6]; fitEllipse(P, N, c0);
		double c1[6]; fitEllipse1(P, N, c1);
		// visualization
		init();
		drawQuadraticCurve(c0, 8, COLOR{ 192,255,128 });
		drawQuadraticCurve(c1, 8, COLOR{ 255,192,128 });
		drawAxis(5, COLOR{ 255,0,0 });
		for (int i = 0; i < N; i++) {
			drawDot(P[i], 5, COLOR{ 0,0,255 });
		}
		// save image
		char s[] = "tests\\test00.png";
		s[10] = i / 10 + '0', s[11] = i % 10 + '0';
		save(s);
	}
}





int main() {
	randomTest_image();
	return 0;
}

