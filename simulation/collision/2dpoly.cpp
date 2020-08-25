// **INCOMPLETE**
// simulating the elastic collision of a polygon in 2d
// polygon: involves both linear and angular motions

#include <stdio.h>
#include <vector>
#include <string>

#include "numerical/geometry.h"  // vec2
#include "numerical/ode.h"  // RungeKuttaMethod
#include "numerical/rootfinding.h"  // solveQuartic

// debug output
#define printerr(s, ...) fprintf(stderr, s, __VA_ARGS__)


// simulation boundary and shape
#define BOUNDARY 0   // 0: rectangle; 1: circle
const int SHAPE = 2;   // 0-8

struct state {
	double t;  // time
	vec2 x, v;  // position and linear velocity
	double r, w;  // rotation and angular velocity
};
typedef state keyFrame;
typedef struct { vec2 c, d; } splineTo;  // path as quadratic bezier curve
typedef std::vector<keyFrame> sequence;
typedef std::vector<vec2> polygon;
struct object {
	polygon P;  // with center of mass at origin
	double inv_m;  // reciprocal of mass
	double inv_I;  // reciprocal of moment of inertia
};


// constants/parameters/conditions
#if BOUNDARY==0
const double W = 6.0, H = 4.0;  // width and height of simulation box;  unit: m
const vec2 BMax = vec2(0.5*W, 0.5*H), BMin = vec2(-0.5*W, -0.5*H);  // bounding box
#elif BOUNDARY==1
const double W = 6.0, H = 6.0;  // W is the diameter of the circle
const vec2 BMax = vec2(0.5*W, 0.5*H), BMin = vec2(-0.5*W, -0.5*W);  // bounding box
#endif
const double SC = 100.0;  // output scaling
object Shape;  // main object
vec2 X0 = vec2(-1, 0.5), V0 = vec2(5, 2);  // initial state
const vec2 g = vec2(0, -9.8);  // gravitational acceleration, zero horizontal component
const double t_max = 2.0;  // end time

void init() {
	// thanks Adobe Illustrator
	polygon Test_Shapes[9] = {
		polygon({ vec2(256.9,182.6), vec2(510.6,175.1), vec2(598.7,402), vec2(280.8,488.6), vec2(188.2,334.8) }),
		polygon({ vec2(56.8,633.4), vec2(419.5,527.5), vec2(426.9,740.9), vec2(310.5,690.2), vec2(164.2,855.8) }),
		polygon({ vec2(480.7,54.3), vec2(697.1,12.5), vec2(677.7,324.4) }),
		polygon({ vec2(50.8,43.8), vec2(216.5,0.5), vec2(337.4,78.2), vec2(12,279.6), vec2(50.8,43.8) }),
		polygon({ vec2(554.7,936), vec2(614.8,817.9), vec2(632.1,948.2), vec2(743.1,971), vec2(643.8,1020.4), vec2(658.7,1163.6), vec2(574.9,1040), vec2(453.1,1120.8), vec2(523,992.1), vec2(436.5,919) }),
		polygon({ vec2(74.7,950), vec2(295.6,948.5), vec2(353.8,1050), vec2(258.3,1154.5), vec2(74.7,1148.5), vec2(15,1069.4) }),
		polygon({ vec2(679.4,598.7), vec2(630.7,646.7), vec2(578.7,613.3), vec2(603.4,525.3), vec2(736.7,552.7), vec2(730,667.3), vec2(646.7,726.7), vec2(530,685.3), vec2(477.4,568.7), vec2(578.7,550), vec2(558,628), vec2(635.4,673.3), vec2(700.7,597.3), vec2(630,562), vec2(609.4,606), vec2(634,622.7), vec2(664,594.7) }),
		polygon({ vec2(43.2,422.7), vec2(0.5,421.3), vec2(4.5,366), vec2(105.2,334.7), vec2(153.2,401.3), vec2(96.5,418), vec2(101.9,503.3), vec2(168.5,491.3), vec2(171.9,528), vec2(15.9,570.7), vec2(4.5,516.7), vec2(72.5,504) }),
		polygon({ vec2(442.2,816.2), vec2(384.2,769.5), vec2(304.2,809.5), vec2(296.9,898.2), vec2(387.5,934.2), vec2(404.9,911.5), vec2(334.9,880.8), vec2(334.2,822.8), vec2(378.2,804.2), vec2(434.9,844.2) }),
	};
	polygon S = Test_Shapes[SHAPE]; int N = S.size();
	for (int i = 0; i < N; i++) S[i] *= vec2(1, -1);
	// calculate mass, center of mass, and moment of inertia
	// treat the shape as a solid
	const double density = 1.0;
	double M(0.0); vec2 C(0.0); double I(0.0);
	for (int i = 0; i < N; i++) {
		vec2 p = S[i], q = S[(i + 1) % N];
		double dM = .5*density*det(p, q);
		vec2 dC = (dM / 3.)*(p + q);
		double dI = (dM / 6.)*(dot(p, p) + dot(p, q) + dot(q, q));
		M += dM, C += dC, I += dI;
	}
	C /= M;
	if (M < 0.) M = -M, I = -I;
	printerr("<text x='%.1lf' y='%.1lf'>%d</text>\n", C.x, -C.y, SHAPE);
	// translate the shape to the origin
	double sc = 0.002;
	C *= sc, M *= sc * sc, I *= sc * sc * sc * sc;
	for (int i = 0; i < N; i++) S[i] = S[i] * sc - C;
	I -= M * C.sqr();
	// modify global variable
	Shape.P = S;
	Shape.inv_m = 1. / M;
	Shape.inv_I = 0.1 / I;
}


// path calculated by numerically integrating collision force
sequence IntPath;
void calcIntPath() {
	state st;
	st.x = X0, st.v = V0, st.r = st.w = st.t = 0;
	const unsigned SL = sizeof(state) / sizeof(double);  // should be 7
	printerr("%u\n", SL);
	IntPath.push_back(st);

	double temp0[SL], temp1[SL], temp2[SL];
	double dt = 0.00001, t = 0;
	for (int i = 0, tN = int(t_max / dt); i < tN; i++) {
		RungeKuttaMethod([](const double *x, double t, double* dxdt) {
			const double k = 1e6;  // a large number
			const state* st = (state*)x;
			state* ds = (state*)dxdt;
			double a = st->r, sa = sin(a), ca = cos(a);
			// calculate collision force and torque
			vec2 F(0.0); double T(0.0);
			for (int i = 0, l = Shape.P.size(); i < l; i++) {
				vec2 pr = Shape.P[i];
				pr = vec2(ca * pr.x - sa * pr.y, sa * pr.x + ca * pr.y);
				vec2 p = st->x + pr;
				vec2 dF(0.);
#if BOUNDARY==0
				if (p.x > BMax.x) dF += vec2(k*(BMax.x - p.x), 0);
				if (p.x < BMin.x) dF += vec2(k*(BMin.x - p.x), 0);
				if (p.y > BMax.y) dF += vec2(0, k*(BMax.y - p.y));
				if (p.y < BMin.y) dF += vec2(0, k*(BMin.y - p.y));
#elif BOUNDARY==1
				double d = length(p);
				if (d > .5*W) dF += k * normalize(p) * (.5*W - d);
#endif
				F += dF, T += det(p - st->x, dF);
			}
			ds->t = 1;
			ds->x = st->v;  // linear velocity
			ds->v = F * Shape.inv_m + g;  // linear acceleration
			ds->r = st->w;  // angular velocity
			ds->w = T * Shape.inv_I;  // angular acceleration
		}, (double*)&st, SL, t, dt, temp0, temp1, temp2);

		if ((t += dt) >= t_max) break;
		if (i % (int(0.01 / dt)) == 0) {
			IntPath.push_back(st);
			// check energy conservation
			double El = (.5*st.v.sqr() - dot(g, st.x)) / Shape.inv_m;
			double Ea = (.5*st.w*st.w) / Shape.inv_I;
			//printerr("%lf %lf  %lf\n", El, Ea, El + Ea);
		}
	}
	st.t = t_max;
	IntPath.push_back(st);
}


// path calculated using collision law
sequence analyticPath;
std::vector<splineTo> qPath;
void calcAnalyticPath() {
	state st = state{ 0, X0, V0, 0, 0 };
	st.w = 7.0;
	analyticPath.push_back(st);
	double t = 0;
	while (t < t_max) {
		// calculate the next collision
		double dt = t_max - t;
		double a = st.r, sa = sin(a), ca = cos(a);
		vec2 dp = vec2(0), n;  // change of linear momentum
		for (int i = 0, l = Shape.P.size(); i < l; i++) {
			vec2 r = Shape.P[i]; r = vec2(ca*r.x - sa * r.y, sa*r.x + ca * r.y);
			vec2 p = st.x + r;
			vec2 v = st.v;
			n = vec2(0.0);
#if BOUNDARY==0
			double tt = (BMax.x - p.x) / v.x;
			if (tt > 1e-6 && tt < dt) dt = tt, n = vec2(-1, 0);
			tt = (BMin.x - p.x) / v.x;
			if (tt > 1e-6 && tt < dt) dt = tt, n = vec2(1, 0);
			double a = .5*g.y, b = v.y, c = p.y - BMin.y;
			tt = ((a < 0. ? -1 : 1)*sqrt(b*b - 4 * a*c) - b) / (2.*a);
			if (tt > 1e-6 && tt < dt) dt = tt, n = vec2(0, 1);
			c = p.y - BMax.y, tt = ((a > 0. ? -1 : 1)*sqrt(b*b - 4 * a*c) - b) / (2.*a);
			if (tt > 1e-6 && tt < dt) dt = tt, n = vec2(0, -1);
#elif BOUNDARY==1
			double c4 = .25*g.sqr(), c3 = dot(g, v), c2 = dot(g, p) + v.sqr(), c1 = 2.*dot(p, v), c0 = p.sqr() + r.sqr() - .25*W*W;
			// quartic + 2*r*dot(vec2(cos(w*t),sin(w*t)),p+v*t+g*.5*t*t)=0
			double rt[4]; int N = solveQuartic(c4, c3, c2, c1, c0, rt);
			for (int i = 0; i < N; i++) {
				double tt = refineRoot_quartic(c4, c3, c2, c1, c0, rt[i]);
				if (tt > 1e-6 && tt < dt) {
					dt = tt;
					n = normalize(p + v * dt + g * (.5*dt*dt));
				}
			}
#endif
			if (n != vec2(0.0)) {
				dp = (-2.*dot(v + g * dt, n) / Shape.inv_m)*n;
			}
		}
		vec2 x1 = st.x + st.v * dt + g * (.5*dt*dt);
		vec2 v1 = st.v + g * dt;

		// output
		int tN = int(ceil(dt / 0.04)); double ddt = dt / tN;
		for (int i = 1; i <= tN; i++) {
			double ct = i * ddt;
			analyticPath.push_back(state{ t + ct, st.x + st.v * ct + g * (.5*ct*ct), st.v + g * ct, st.r + st.w*ct, st.w });
			// linear and angular velocities isn't presented in output
		}
		qPath.push_back(splineTo{ st.x + .5*st.v*dt, x1 });

		// collision respond
		st.x = x1, st.v = v1 + dp * Shape.inv_m;
		st.r += st.w*dt, st.w = st.w;
		t += dt;

		// check energy conservation
		double E = (.5*st.v.sqr() - dot(st.x, g)) / Shape.inv_m;
		printerr("%lf %lf\n", t, E);
	}
}


// visualization
int main(int argc, char** argv) {
	// export format: animated svg
	freopen(argv[1], "w", stdout);

	// simulation
	init();
	calcIntPath();
	calcAnalyticPath();

	// decimal to string optimized for size
	auto str = [](double x, int d = 3)->std::string {
		char format[16]; sprintf(format, "%%.%dlf", d);
		char st[256]; sprintf(st, format, x);
		std::string s = st;
		while (s.back() == '0') s.pop_back();
		if (s.back() == '.') s.pop_back();
		if (s[0] == '0') s = s.erase(0, 1);
		return s == "" ? "0" : s;
	};
	// world coordinate to svg coordinate
	auto transform = [](vec2 p)->vec2 {
		return SC * vec2(p.x - BMin.x, BMax.y - p.y);
	};

	// svg header
	printf("<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' version='1.1' width='%d' height='%d'>\n",
		int((BMax.x - BMin.x)*SC + 1), int((BMax.y - BMin.y)*SC + 1));
	// shape
	printf("<defs>\n <g id='shape'><polygon points='");
	for (int i = 0, l = Shape.P.size(); i <= l; i++) {
		vec2 p = SC * vec2(1, -1) * Shape.P[i%l];
		printf("%s %s%c", &str(p.x, 1)[0], &str(p.y, 1)[0], i == l ? '\'' : ' ');
	}
	printf("/><circle cx='0' cy='0' r='3' style='fill:black' /></g>\n</defs>\n");
	// boundary
	if (BOUNDARY == 0)
		printf("<rect width='%s' height='%s' style='fill:white;stroke-width:1;stroke:black'/>\n", &str(W*SC, 1)[0], &str(H*SC, 1)[0]);
	else if (BOUNDARY == 1)
		printf("<circle r='%s' cx='%s' cy='%s' style='fill:white;stroke-width:1;stroke:black'/>\n", &str(.5*W*SC, 1)[0], &str(.5*W*SC, 1)[0], &str(.5*H*SC, 1)[0]);

	// analytical path
	printf("<path stroke-dasharray='3,4' style='fill:none;stroke-width:1;stroke:gray'\n d='");
	vec2 q0 = transform(X0); printf("M%s,%s", &str(q0.x, 1)[0], &str(q0.y, 1)[0]);
	for (int i = 0, l = qPath.size(); i < l; i++) {
		vec2 q = transform(qPath[i].c);
		printf("Q%s,%s", &str(q.x, 1)[0], &str(q.y, 1)[0]);
		q = transform(qPath[i].d);
		printf(" %s,%s", &str(q.x, 1)[0], &str(q.y, 1)[0]);
	}
	printf("'/>\n");

	// animation
	auto printPath = [&](const sequence &p, const char* style) {
		static int id = 0; id++;
		printf("<use id='obj%d' xlink:href='#shape' style='%s' />\n", id, style);
		printf("<style>\n");
		printf("#obj%d{animation:path%d %ss infinite; animation-timing-function:linear;}\n", id, id, &str(5 * t_max)[0]);
		printf("@keyframes path%d {\n", id);
		for (int i = 0, N = p.size(); i < N; i++) {
			vec2 q = transform(p[i].x);
			printf(" %s%%{transform:translate(%spx,%spx)rotate(%sdeg)}\n", &str(100 * p[i].t / t_max, 2)[0], &str(q.x, 1)[0], &str(q.y, 1)[0], &str(p[i].r*(-180 / PI), 1)[0]);
		}
		printf("} </style>\n");
	};
	printPath(analyticPath, "fill:blue");
	printPath(IntPath, "fill:red");

	//printf("<text x='20' y='30'>Blue ball: path is calculated using collision law;</text>\n");
	//printf("<text x='20' y='50'>Red ball: path is calculated by integrating collision force numerically;</text>\n");
	printf("</svg>");
	return 0;
}