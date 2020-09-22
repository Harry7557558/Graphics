#include <stdio.h>
#include "numerical/integration.h"


#include <chrono>
typedef std::chrono::high_resolution_clock NTime;
typedef std::chrono::duration<double> fsec;


// length of a parabola
void debug_lengthCalcTest() {
	auto time0 = NTime::now();
	const auto C = [](double x)->vec2 { return vec2(x, x*x); };
	const auto dC = [](double x)->vec2 { return vec2(1, 2 * x); };
	const double t0 = 0., t1 = 1.;
	const double EXACT = 1.478942857544597433827906;  // (asinh(2)+2*sqrt(5))/4

	double logerr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	auto updateValue = [&](int id, double x) {
		double e = -log2(abs(x - EXACT));
		printf("%9lf%10lf\t", e, e - logerr[id]);
		logerr[id] = e;
	};

	for (int i = 1; i <= 20; i++) {
		int N = 1 << i;
		printf("%d\t", N);
		updateValue(0, NIntegrate_Simpson<double>([&](double t) { return length(dC(t)); }, t0, t1, N));  // Simpson's method
		updateValue(1, NIntegrate_trapzoid<double>([&](double t) { return length(dC(t)); }, t0, t1, N));  // trapzoid method
		updateValue(2, NIntegrate_midpoint<double>([&](double t) { return length(dC(t)); }, t0, t1, N));  // midpoint rectangle method
		updateValue(3, NIntegrate_AL_midpoint_t<double, vec2>([](double t) { return 1.; }, C, t0, t1, N));  // O(N⁻²) method on curve
		updateValue(4, NIntegrate_AL_Simpson_t<double, vec2>([](double t) { return 1.; }, C, t0, t1, N));  // O(N⁻⁴) method on curve
		updateValue(5, NIntegrate_AL_midpoint_p<double, vec2>([](vec2 p) { return 1.; }, C, t0, t1, N));  // O(N⁻²) method on curve, should be identical as the previous
		updateValue(6, NIntegrate_AL_Simpson_p<double, vec2>([](vec2 p) { return 1.; }, C, t0, t1, N));  // O(N⁻⁴) method on curve, should be identical as the previous
		updateValue(7, NIntegrate_Gaussian2<double>([&](double t) { return length(dC(t)); }, t0, t1, N));  // Gaussian quadrature with 2 samples
		printf("\n");
	}
	fprintf(stderr, "%lfsecs\n", fsec(NTime::now() - time0).count());
	printf("\n");
}

// moment of inertia of a quadratic parametric curve with unit line density
void debug_mInertiaCalcTest() {
	auto time0 = NTime::now();
	const auto C = [](double t)->vec2 { return vec2(2, 1) + vec2(-2, 2)*t + vec2(-1, -3)*(t*t); };
	const auto dC = [](double t)->vec2 { return vec2(-2, 2) + vec2(-2, -6)*t; };
	const double t0 = -.5, t1 = 1.;
	const double EXACT = 19.15808493320045537235252846;  // (13696*sqrt(10)*arsinh(2)+13696*sqrt(10)*arsinh(7/4)+49135*sqrt(26)+35*2^(17/2))/20000

	double logerr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	auto updateValue = [&](int id, double x) {
		double e = -log2(abs(x - EXACT));
		printf("%9lf%10lf\t", e, e - logerr[id]);
		logerr[id] = e;
	};

	for (int i = 1; i <= 20; i++) {
		int N = 1 << i;
		printf("%d\t", N);
		updateValue(0, NIntegrate_Simpson<double>([&](double t) { return C(t).sqr()*length(dC(t)); }, t0, t1, N));  // Simpson's method
		updateValue(1, NIntegrate_trapzoid<double>([&](double t) { return C(t).sqr()*length(dC(t)); }, t0, t1, N));  // trapzoid method
		updateValue(2, NIntegrate_midpoint<double>([&](double t) { return C(t).sqr()*length(dC(t)); }, t0, t1, N));  // midpoint rectangle method
		updateValue(3, NIntegrate_AL_midpoint_t<double, vec2>([&](double t) { return C(t).sqr(); }, C, t0, t1, N));  // O(N⁻²) method on curve
		updateValue(4, NIntegrate_AL_Simpson_t<double, vec2>([&](double t) { return C(t).sqr(); }, C, t0, t1, N));  // O(N⁻⁴) method on curve
		updateValue(5, NIntegrate_AL_midpoint_p<double, vec2>([](vec2 p) { return p.sqr(); }, C, t0, t1, N));  // O(N⁻²) method on curve, should be identical as the previous
		updateValue(6, NIntegrate_AL_Simpson_p<double, vec2>([](vec2 p) { return p.sqr(); }, C, t0, t1, N));  // O(N⁻⁴) method on curve, should be identical as the previous
		updateValue(7, NIntegrate_Gaussian2<double>([&](double t) { return C(t).sqr()*length(dC(t)); }, t0, t1, N));  // Gaussian quadrature with 2 samples
		printf("\n");
	}
	fprintf(stderr, "%lfsecs\n", fsec(NTime::now() - time0).count());
	printf("\n");
}

// integrate position on a line spiral, varible is 3d vector, result is the center of mass multiplied by the mass
void debug_centerCalcTest() {
	auto time0 = NTime::now();
	const auto C = [](double t)->vec3 { return vec3(cos(t), sin(t), .5*t*t); };
	const auto dC = [](double t)->vec3 { return vec3(-sin(t), cos(t), t); };
	const double t0 = -1., t1 = 3.;
	const vec3 EXACT = vec3(-0.1626063624736737, 3.221734260283637, 11.36204045440409);  // NumberForm[NIntegrate[{Cos[t],Sin[t],t^2/2}*Sqrt[t^2+1],{t,-1,3},PrecisionGoal->10],20]

	double logerr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	auto updateValue = [&](int id, vec3 x) {
		double e = -.5*log2((x - EXACT).sqr());
		printf("%9lf%10lf\t", e, e - logerr[id]);
		logerr[id] = e;
	};

	for (int i = 1; i <= 20; i++) {
		int N = 1 << i;
		printf("%d\t", N);
		updateValue(0, NIntegrate_Simpson<vec3>([&](double t) { return C(t)*length(dC(t)); }, t0, t1, N));  // Simpson's method
		updateValue(1, NIntegrate_trapzoid<vec3>([&](double t) { return C(t)*length(dC(t)); }, t0, t1, N));  // trapzoid method
		updateValue(2, NIntegrate_midpoint<vec3>([&](double t) { return C(t)*length(dC(t)); }, t0, t1, N));  // midpoint rectangle method
		updateValue(3, NIntegrate_AL_midpoint_t<vec3, vec3>([&](double t) { return C(t); }, C, t0, t1, N));  // O(N⁻²) method on curve
		updateValue(4, NIntegrate_AL_Simpson_t<vec3, vec3>([&](double t) { return C(t); }, C, t0, t1, N));  // O(N⁻⁴) method on curve
		updateValue(5, NIntegrate_AL_midpoint_p<vec3, vec3>([](vec3 p) { return p; }, C, t0, t1, N));  // O(N⁻²) method on curve, should be identical as the previous
		updateValue(6, NIntegrate_AL_Simpson_p<vec3, vec3>([](vec3 p) { return p; }, C, t0, t1, N));  // O(N⁻⁴) method on curve, should be identical as the previous
		updateValue(7, NIntegrate_Gaussian2<vec3>([&](double t) { return C(t)*length(dC(t)); }, t0, t1, N));  // Gaussian quadrature with 2 samples
		printf("\n");
	}
	fprintf(stderr, "%lfsecs\n", fsec(NTime::now() - time0).count());
	printf("\n");
}

// integrate the inertia tensor matrix of a curve with unit line density, about the origin
void debug_inertiaTCalcTest() {
	auto time0 = NTime::now();
	const auto C = [](double t)->vec3 { return vec3(sin(t), cos(t) + .5*sin(t), t*t*(3 - 2 * t)); };
	const auto dC = [](double t)->vec3 { return vec3(cos(t), -sin(t) + .5*cos(t), 6 * t*(1 - t)); };
	const double t0 = -0.8, t1 = 1.8;

	// P[t_]={{Sin[t],Cos[t]+.5*Sin[t],t^2*(3-2*t)}}
	// M[t_]=Part[P[t].Transpose[P[t]],1,1]*IdentityMatrix[3]-(Transpose[P[t]].P[t])
	// W[t_]=Sqrt[Cos[t]^2+(-Sin[t]+.5*Cos[t])^2+(6*t*(1-t))^2]
	// NumberForm[NIntegrate[M[t]*W[t],{t,-0.8,1.8},PrecisionGoal->10],20]
	const auto M = [](vec3 p)->mat3 { return mat3(dot(p, p)) - tensor(p, p); };
	const mat3 EXACT = mat3(
		vec3(15.83109399967058, -1.390562584670683, 3.601126975018459),
		vec3(-1.390562584670683, 16.25181515166788, -2.674708304444605),
		vec3(3.601126975018459, -2.674708304444605, 8.03276815292674));

	double logerr[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	auto updateValue = [&](int id, mat3 x) {
		double e = -.5*log2(sumsqr(x - EXACT));
		printf("%9lf%10lf\t", e, e - logerr[id]);
		logerr[id] = e;
	};

	for (int i = 1; i <= 20; i++) {
		int N = 1 << i;
		printf("%d\t", N);
		updateValue(0, NIntegrate_Simpson<mat3>([&](double t) { return M(C(t))*length(dC(t)); }, t0, t1, N));  // Simpson's method
		updateValue(1, NIntegrate_trapzoid<mat3>([&](double t) { return M(C(t))*length(dC(t)); }, t0, t1, N));  // trapzoid method
		updateValue(2, NIntegrate_midpoint<mat3>([&](double t) { return M(C(t))*length(dC(t)); }, t0, t1, N));  // midpoint rectangle method
		updateValue(3, NIntegrate_AL_midpoint_t<mat3, vec3>([&](double t)->mat3 { return M(C(t)); }, C, t0, t1, N));  // O(N⁻²) method on curve
		updateValue(4, NIntegrate_AL_Simpson_t<mat3, vec3>([&](double t)->mat3 { return M(C(t)); }, C, t0, t1, N));  // O(N⁻⁴) method on curve
		updateValue(5, NIntegrate_AL_midpoint_p<mat3, vec3>([&](vec3 p)->mat3 { return M(p); }, C, t0, t1, N));  // O(N⁻²) method on curve, should be identical as the previous
		updateValue(6, NIntegrate_AL_Simpson_p<mat3, vec3>([&](vec3 p)->mat3 { return M(p); }, C, t0, t1, N));  // O(N⁻⁴) method on curve, should be identical as the previous
		updateValue(7, NIntegrate_Gaussian2<mat3>([&](double t) { return M(C(t))*length(dC(t)); }, t0, t1, N));  // Gaussian quadrature with 2 samples
		printf("\n");
	}
	fprintf(stderr, "%lfsecs\n", fsec(NTime::now() - time0).count());
	printf("\n");
}






int main() {
	debug_lengthCalcTest();
	debug_mInertiaCalcTest();
	debug_centerCalcTest();
	debug_inertiaTCalcTest();
	return 0;
}

