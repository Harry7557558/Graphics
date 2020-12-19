// parametric surface equations for generating objects in run-time
// most surfaces are designed from aesthetic view


#ifndef __INC_GEOMETRY_H
#include "numerical/geometry.h"
#endif

// Parametric surface class
#include <functional>
#include <vector>
template<typename Fun>
class ParametricSurface {
public:
	double u0, u1, v0, v1;  // parameter intervals
	int uD, vD;  // recommended discretization splits
	const Fun P;  // equation, vec3 P(u,v)
	uint32_t id;  // optional
	bool isGaussianSurface;  // indicate whether divergence theorem can be applied
	ParametricSurface(Fun P,
		double u0 = NAN, double u1 = NAN, double v0 = NAN, double v1 = NAN, int uD = 0, int vD = 0,
		bool isGaussianSurface = false, const char* name = nullptr)
		:u0(u0), u1(u1), v0(v0), v1(v1), uD(uD), vD(vD), P(P), isGaussianSurface(isGaussianSurface) {
		id = hash(name);
	}
	static uint32_t hash(const char* s) {
		uint32_t h = 0;
		while (*s) h = 1664525u * h + 1013904223u * *(s++);
		return h;
	}

	// triangulation
	int param2points(std::vector<vec3> &p,
		vec3 translate = vec3(0.), double scale = 1.) const {
		p.reserve(p.size() + (uD + 1)*(vD + 1));
		double du = (u1 - u0) / uD, dv = (v1 - v0) / vD;
		for (int ui = 0; ui <= uD; ui++) {
			for (int vi = 0; vi <= vD; vi++) {
				double u = u0 + ui * du, v = v0 + vi * dv;
				p.push_back(scale * P(u, v) + translate);
			}
		}
		return (uD + 1)*(vD + 1);
	}
	int param2trigs(std::vector<triangle> &p,
		vec3 translate = vec3(0.), double scale = 1.) const {
		p.reserve(p.size() + 2 * uD*vD);
		auto F = [&](double u, double v) {
			return scale * P(u, v) + translate;
		};
		double du = (u1 - u0) / uD, dv = (v1 - v0) / vD;
		for (int ui = 0; ui < uD; ui++) {
			for (int vi = 0; vi < vD; vi++) {
				double u = u0 + ui * du, v = v0 + vi * dv;
				vec3 p00 = F(u, v);
				vec3 p10 = F(u, v + dv);
				vec3 p01 = F(u + du, v);
				vec3 p11 = F(u + du, v + dv);
#if 1
				if ((p01 - p10).sqr() < (p00 - p11).sqr()) {
					p.push_back(triangle{ p10, p00, p01 });
					p.push_back(triangle{ p01, p11, p10 });
				}
				else {
					p.push_back(triangle{ p11, p10, p00 });
					p.push_back(triangle{ p00, p01, p11 });
				}
#else
				p.push_back(triangle{ p10, p00, p01 });
				p.push_back(triangle{ p01, p11, p10 });
#endif
			}
		}
		return 2 * uD * vD;
	}

	// take a point list generated by `param2points` and generate triangles
	int points2trigs(const vec3* P, std::vector<triangle> &T) const {
		T.reserve(T.size() + 2 * uD*vD);
		for (int ui = 0; ui < uD; ui++) {
			for (int vi = 0; vi < vD; vi++) {
				vec3 p00 = P[ui*(vD + 1) + vi];
				vec3 p10 = P[ui*(vD + 1) + (vi + 1)];
				vec3 p01 = P[(ui + 1)*(vD + 1) + vi];
				vec3 p11 = P[(ui + 1)*(vD + 1) + (vi + 1)];
				if ((p01 - p10).sqr() < (p00 - p11).sqr()) {
					T.push_back(triangle{ p10, p00, p01 });
					T.push_back(triangle{ p01, p11, p10 });
				}
				else {
					T.push_back(triangle{ p11, p10, p00 });
					T.push_back(triangle{ p00, p01, p11 });
				}
			}
		}
		return 2 * uD * vD;
	}
};
typedef ParametricSurface<vec3(*)(double, double)> ParametricSurfaceP;
typedef ParametricSurface<std::function<vec3(double, double)>> ParametricSurfaceL;




// surface templates

namespace ParametricSurfaceTemplates {

	vec3 Archimedean_snail(double u, double v,
		double vert, double offset, double layer_n) {

		/* 0 < u < 2π, 0 < v < 1
		   For a right-handed snail, all of the following are positive:
			- vert: height/sharpness, vertice=(0,0,vert)
			- offset: offset from z-axis, 1.0 for a "standard" snail
			- layer_n: number of layers of the snail
		*/

		return (1. - v)*vec3(
			(cos(u) + offset)*cossin(2.*PI*layer_n*v),
			sin(u)) + vec3(0, 0, vert*v);
	};

	vec3 Logarithmic_snail(double u, double v,
		double exp_k, double vert, double offset, double layer_n) {

		/* 0 < u < 2π, v0 < v < v1
			- exp_k: r=exp(k/2π*θ), positive
			- vert: z_vertice = exp(exp_k)*vert
			- offset: offset from z-axis, 1.0 for a "standard" snail
			- layer_n: number of layers of the snail when -1<v<1
			- v0, v1: custom parameters, v0 can be -INFINITY
		*/

		return exp(exp_k*v) * vec3(
			cossin(PI*layer_n*v)*(offset + cos(u)),
			vert*(exp(exp_k - exp_k * v) - 2.) + sin(u));

		// right-handed version
		return exp(exp_k*v) * vec3(
			sincos(PI*layer_n*v)*(offset + cos(u)),
			vert*(exp(exp_k - exp_k * v) - 2.) - sin(u));
	};


	template<typename vec> vec cubicBezierCurve(vec A, vec B, vec C, vec D, double t) {
		return (((-A + 3.*B - 3.*C + D)*t + (3.*A - 6.*B + 3.*C))*t + (3.*B - 3.*A))*t + A;
	}

}



// preset parametric surfaces

const std::vector<ParametricSurfaceL> ParamSurfaces({

	/*[0]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Archimedean_snail(u, v, 1.2, 1.0, 3.0);
	}, 0., 2.*PI, 0., 1., 40, 120, false, "land snail"),

	/*[1]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Archimedean_snail(u, v, 2.2, 1.0, 4.5);
	}, 0., 2.*PI, 0., 1., 40, 120, false, "river snail"),

	/*[2]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Archimedean_snail(u, v, 1.1, 0.5, 2.0);
	}, 0., 2.*PI, 0., 1., 40, 120, false, "field snail"),

	/*[3]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Archimedean_snail(u, v, 1.8, 0.2, 2.4);
	}, 0., 2.*PI, 0., 1., 40, 120, false, "pond snail"),

	/*[4]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Archimedean_snail(u, v, 3.5, 0.2, 8.0);
	}, 0., 2.*PI, 0., 1., 40, 120, false, "cone snail"),

	/*[5]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Logarithmic_snail(u, v, 0.45, 1.7, 0.35, 2.1);
	}, 0., 2.*PI, -7., 1., 40, 120, false, "pot snail"),

	/*[6]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Logarithmic_snail(u, v, 0.65, 0.55, 0.86, 1.0);
	}, 0., 2.*PI, -6., 1., 40, 160, false, "moon snail"),

	/*[7]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Logarithmic_snail(u, v, 0.7, 0.0, 1.05, 1.0);
	}, 0., 2.*PI, -5., 1., 40, 160, false, "snail (in)"),

	/*[8]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Logarithmic_snail(u, v, 0.9, 0.0, 2.0, 1.0);
	}, 0., 2.*PI, -6., -1., 40, 160, false, "snail (out)"),

	/*[9]*/ ParametricSurfaceL([](double u, double v) {
		return ParametricSurfaceTemplates::Logarithmic_snail(u, v, 1.8, 1.2, 2.7, 1.0);
	}, 0., 2.*PI, -3., 1., 40, 160, false, "snail (outer)"),

	/*[10]*/ ParametricSurfaceL([](double u, double v) {
		return vec3(
			cossin(3.*PI*v)*(2.0 + cos(u)) + 0.05*cossin(60.*PI*v),
			sin(u) + 0.05*sin(10.*u))*exp(v);
	}, 0., 2.*PI, -4., 1., 80, 1000, false, "textured snail 1"),

	/*[11]*/ ParametricSurfaceL([](double u, double v) {
		return vec3(
			sincos(3.*PI*v)*(1. + cos(u)) + 0.1*cossin(-10.*u),
			4. - (v + 4.) - 0.9*sin(u))*exp(0.8*v);
	}, 0., 2.*PI, -4., 1., 100, 800, false, "textured snail 2"),

	/*[12]*/ ParametricSurfaceL([](double u, double v) {
		vec3 p = vec3(
			cossin(PI*3.*v)*(1. + cos(u)) + 0.1*cossin(40.*PI*v),
			(exp(1. - v) - 2.) + sin(u));
		p += 0.06*vec3(sin(10.*p.x)*sin(10.*p.y)*cos(10.*p.z));
		return p * exp(v);
	}, 0., 2.*PI, -3., 1., 100, 1600, false, "textured snail 3"),

	/*[13]*/ ParametricSurfaceL([](double u, double v) {
		return vec3(
			sincos(3.*PI*v)*(0.9 + cos(u)) + 0.05*cossin(40.*PI*v)*cos(10.*u)*(1. + cos(u)),
			(exp(1.5 - 0.9*v) - 3.) - 1.1*sin(u))*exp(0.9*v);
	}, 0., 2.*PI, -4., 1., 100, 800, false, "textured snail 4"),

	/*[14]*/ ParametricSurfaceL([](double u, double v) {
		return vec3(sincos(3.*PI*v)*(1. + cos(u)), -1.1*sin(u))
			*exp(0.8*v)*(.5*cos(20.*PI*v) + .9)*(.05*cos(10.*u) + 1.)
			+ vec3(0, 0, 5. - 4.*exp(v));
	}, 0., 2.*PI, -4., 1., 80, 1000, false, "textured snail #1"),

	/*[15]*/ ParametricSurfaceL([](double u, double v) {
		return vec3(
			(1 - .05*exp(sin(u)))*(cossin(3.*PI*v)*(2.0 + cos(u)) + 0.05*cossin(60.*PI*v)),
			-exp(1. - .5*v) + sin(u) + 0.1*sin(10.*u)*sin(20.*PI*v)*exp(v))*exp(v)
			+ vec3(0, 0, 4.);
	}, 0., 2.*PI, -4., 1., 100, 1000, false, "textured snail #2"),

	/*[16]*/ ParametricSurfaceL([](double u, double v) {
		return exp(v) * vec3(
			cossin(PI*3.*v)*(1. + cos(u)),
			(exp(1. - v) - 2.) + exp(v)*sin(u));
	}, 0., 2.*PI, -3., 1., 60, 300, false, "melon shell"),

	/*[17]*/ ParametricSurfaceL([](double u, double v) {
		vec3 p = exp(v) * vec3(
			cossin(PI*3.*v)*(1. + cos(u)),
			.55*(exp(1. - v) - 2.) + sin(u));
		return p + vec3(0, 0, 3. - exp(-.55*p.z));
	}, 0., 2.*PI, -3., 1., 60, 300, false, "bailer shell"),

	/*[18]*/ ParametricSurfaceL([](double u, double v) {
		vec2 cr(cos(u) + .2*sin(u)*sin(u), -1.3*sin(u) + cos(u)*cos(u));
		return exp(.4*v)*vec3((.5 + .8*cr.x + .3*cr.y)*sincos(PI*v),
			2.8*(exp(-.4*v) - 1.) + cr.y - .3*cr.x);
	}, 0., 2.*PI, -9., 1., 60, 300, false, "wonder shell"),

	/*[19]*/ ParametricSurfaceL([](double u, double v) {
		vec2 cr = vec2(0.510504, -1.134042)
			+ mat2(0.598892, 0.541083, -0.609437, -3.788403) * cossin(u)
			+ mat2(-0.120320, 0.732847, -0.350955, 0.994199) * cossin(2.*u)
			+ mat2(-0.078252, -0.453444, -0.022159, 0.324241) * cossin(3.*u);
		cr.y = 5.*tanh(.2*cr.y);  // too sharp
		cr.y = .2*log(exp(5.*cr.y) + exp(5.*-3.6));  // clamp at y=-3.6 like that of natural snail
		return exp(.3*v)*vec3(cr.x*sincos(PI*v),
			3.*(exp(-.3*v) - 1.) + cr.y);
	}, 0., 2.*PI, -9., 1., 60, 300, false, "pear shell"),

	/*[20]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 1.0;
		return vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 40, 80, true, "torus"),

	/*[21]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 0.7 + 0.4*cos(v);
		vec2 uv = vec2(cos(u) - 0.2*cos(2.*u), sin(u) + 0.2*sin(2.*u));
		return vec3(cossin(v)*(R + r * uv.x), r*uv.y);
	}, 0., 2.*PI, 0, 2.*PI, 60, 100, true, "uneven torus"),

	/*[22]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 1.0 + 0.8*sin(5.*v)*sin(u);
		return vec3(cossin(v)*(R + r * cos(u)), sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 60, 120, true, "deformed torus 1"),

	/*[23]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 0.8*exp(sin(5.*v)) - 0.2;
		return vec3(cossin(v)*(R + r * cos(u)), .5*r*sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 60, 200, true, "deformed torus 2"),

	/*[28]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 0.8 - .1*pow(.5 - .5*min(sin(12.*v), sin(10.*u)), 10.);
		return vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 180, 360, true, "textured torus (tire)"),

	/*[29]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 0.8 + .1*pow(abs(min(sin(10.*u), sin(10.*v))), 10.);
		return vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 300, 480, true, "textured torus (cells)"),

	/*[30]*/ ParametricSurfaceL([](double u, double v) {
		double texture = 0.1*pow(1.0 - pow((sin(5.*u) - .5*sin(16.*v)) / 1.5, 2.), 20.);
		double R = 2.0, r = 0.8 + texture;
		return vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 180, 360, true, "textured torus (wavy)"),

	/*[31]*/ ParametricSurfaceL([](double u, double v) {
		vec2 p = vec2(20.*u, 66.*v / sqrt(3.)) / (2.*PI);
		p = vec2(fmod(p.x, 3.), fmod(.5*p.x + sqrt(3)*.5*p.y, 3.));
		if (p.y > p.x) p = p.yx();
		if (p.x + p.y > 3.) p = vec2(3.) - p.yx();
		double texture = p.y > 1. ? p.x - p.y : p.x > 2. ? abs(p.x - p.y - 2.) : p.y > p.x - 1. ? abs(p.x - 1.) : p.y;
		double R = 2.0, r = 0.8 + 0.1*pow(1.0 - texture * texture, 20.);
		return vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
	}, 0., 2.*PI, 0, 2.*PI, 180, 360, true, "textured torus (hexa)"),

	/*[24]*/ ParametricSurfaceL([](double u, double v) {
		double R = hypot(2.5*cos(v) - 0.1*cos(3.*v), 2.5*sin(v) + 0.1*sin(3.*v));
		double r = hypot(0.8*cos(u) - 0.05*cos(3.*u), 0.8*sin(u) + 0.05*sin(3.*u));
		vec3 p = vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
		return p + vec3(0, 0, 0.1*p.x*p.y + 0.1*p.y*p.y);
	}, 0., 2.*PI, 0, 2.*PI, 60, 100, true, "bended torus"),

	/*[25]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 0.8;
		vec3 p = vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
		return rotationMatrix_x(.5*PI)*(rotationMatrix_y(1.5*p.y)*p);
	}, 0., 2.*PI, 0, 2.*PI, 60, 200, true, "twisted torus"),

	/*[26]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0;
		double r = hypot(0.3*cos(u) - 0.02*cos(3.*u), 0.3*sin(u) + 0.02*sin(3.*u));
		vec3 p = vec3(cossin(v)*(R + r * cos(u)), 0.4*r*sin(u));
		return p * vec3(1, 0.8, 1) + vec3(0, 1.1*(pow(p.x*p.x + 1e-4, .25) - 1.), 0);
	}, 0., 2.*PI, 0, 2.*PI, 40, 400, true, "toric heart"),

	/*[27]*/ ParametricSurfaceL([](double u, double v) {
		double R = 2.0, r = 0.3;
		vec3 p = vec3(cossin(v)*(R + r * cos(u)), r*sin(u));
		p += asin(sin(5.*atan2(p.y, p.x))) * vec3(normalize(p.xy()));
		return p * vec3(1, 1, 0.04*(p.x*p.x + p.y*p.y) + 0.8);
	}, 0., 2.*PI, 0, 2.*PI, 40, 400, true, "toric cercis"),

	/*[32]*/ ParametricSurfaceL([](double u, double v) {
		vec2 c = ParametricSurfaceTemplates::cubicBezierCurve(vec2(0, -1), vec2(1.2, -1), vec2(0.6, 1), vec2(0, 1), u);
		return vec3(cossin(v)*c.x, c.y);
	}, 0., 1., 0, 2.*PI, 40, 80, true, "egg"),

	/*[33]*/ ParametricSurfaceL([](double u, double v) {
		vec2 c = ParametricSurfaceTemplates::cubicBezierCurve(vec2(0, -1), vec2(1, -1), vec2(0.6, 0.3), vec2(0, 1), u);
		vec3 p = vec3(cossin(v)*c.x, c.y);
		return p - vec3(0, 0.1*p.z*p.z, 0);
	}, 0., 1., 0, 2.*PI, 40, 80, true, "droplet"),

	/*[34]*/ ParametricSurfaceL([](double u, double v) {
		vec2 c = vec2(0.7*sin(u) + 0.25*sin(2.*u) + 0.3*sin(3.*u), -2.*cos(u) - 0.3*cos(2.*u) - 0.3*cos(3.*u));
		return vec3(cossin(v)*c.x, c.y);
	}, 0., PI, 0, 2.*PI, 60, 60, true, "bowling"),

	/*[35]*/ ParametricSurfaceL([](double u, double v) {
		u *= 1.45; vec3 s1 = vec3(cossin(v)*(0.7*sin(u) + 0.25*sin(2.*u) + 0.3*sin(3.*u)), (-1.*cos(u) - 0.2*cos(2.*u) - 0.2*cos(3.*u)));
		u /= 1.45; vec3 s2 = vec3(cossin(v)*(0.075*sin(u) + 0.015*sin(3.*u)), (-1.6*cos(u) + 0.2*cos(3.*u))) + vec3(0, .04*cos(2.*u), 0);
		return s1 + (s2 - s1)*(.5 - .5*tanh(40.*(.5 + cos(u))));
	}, 0., PI, 0, 2.*PI, 200, 60, true, "gourd"),

	/*[36]*/ ParametricSurfaceL([](double u, double v) {
		u *= 1.34; vec2 c = vec2(sin(u) + 0.1*pow(sin(u), 5), -cos(u) + 0.3*pow(cos(u), 5));
		c *= vec2(1.0) + vec2(0.08, 0.08*c.x*c.x)*sin(cos(8.*v));
		vec3 s1 = vec3(cossin(v)*c.x, c.y);
		u /= 1.34; vec3 s2 = vec3(cossin(v)*(0.2*sin(u) + 0.04*sin(3.*u)), (-1.25*cos(u) + 0.16*cos(3.*u))) + vec3(0, .06*cos(2.3*u), 0);
		return s1 + (s2 - s1)*(.5 - .5*tanh(40.*(.6 + cos(u))));
	}, 0., PI, 0, 2.*PI, 200, 120, true, "pumpkin"),

	/*[37]*/ ParametricSurfaceL([](double u, double v) {
		vec2 c = vec2(sin(u) - 0.04*sin(2.*u), -cos(u) - exp(1.3*cos(u)) + 0.6*sin(u)*sin(u) + 1.3);
		vec3 s = vec3(cossin(v)*c.x, c.y);
		s.x -= 0.08*s.z*s.z; return s;
	}, 0., PI, 0, 2.*PI, 60, 60, true, "pepper (without stem)"),

	/*[38]*/ ParametricSurfaceL([](double u, double v) {
		vec2 c = vec2(sin(u), -1.2*cos(u) - 0.2*cos(2.*u));
		c *= vec2(1.0) + vec2(0.05, 0.05*c.x*c.x)*asin(0.95*cos(12.*v)) + vec2(0.02, 0.)*cos(12.*(u + v));
		return vec3(cossin(v)*c.x, c.y);
	}, 0., PI, 0, 2.*PI, 90, 200, true, "ball cactus"),

	/*[39]*/ ParametricSurfaceL([](double u, double v) {
		vec2 c = ParametricSurfaceTemplates::cubicBezierCurve(vec2(0, -1), vec2(2.2, -1), vec2(0.3, 1), vec2(0, 2), u);
		c *= vec2(1.0) + vec2(0.1, 0.1*c.x*c.x)*asin(0.98*cos(5.*v));
		return rotationMatrix_z(c.y)*(vec3(cossin(v)*c.x, c.y) + vec3(0.1*sin(c.y), 0, 0));
	}, 0., 1., 0, 2.*PI, 60, 120, true, "ice cream"),
});





// functions that may be useful for normalizing test shapes

// calculate the center of mass of an object
// assume the object is a surface with uniform surface density
vec3 calcCOM_shell(const triangle* T, int N) {
	double A = 0; vec3 C(0.);
	for (int i = 0; i < N; i++) {
		double dA = T[i].area();
		vec3 dC = (1. / 3.)*(T[i].A + T[i].B + T[i].C);
		A += dA, C += dA * dC;
	}
	return C * (1. / A);
}
// calculate the axis-aligned bounding box, return the center
vec3 calcAABB(const triangle* T, int N, vec3* rad = 0) {
	const vec3* P = (vec3*)T; N *= 3;
	vec3 Min(INFINITY), Max(-INFINITY);
	for (int i = 0; i < N; i++) {
		Max = pMax(Max, P[i]);
		Min = pMin(Min, P[i]);
	}
	if (rad) *rad = .5*(Max - Min);
	return .5*(Max + Min);
}

// translate the object so its center is the origin
void translateShape(triangle* T, int N, vec3 d) {
	vec3* P = (vec3*)T; N *= 3;
	for (int i = 0; i < N; i++) P[i] += d;
}
void translateToCOM_shell(triangle* T, int N) {
	translateShape(T, N, -calcCOM_shell(T, N));
}
void translateToAABBCenter(triangle* T, int N) {
	translateShape(T, N, -calcAABB(T, N));
}


// calculate the maximum "radius" of the object from the origin
double calcMaxRadius(const triangle* T, int N) {
	const vec3* P = (vec3*)T; N *= 3;
	double maxR = 0.;
	for (int i = 0; i < N; i++) {
		double r = P[i].sqr();
		if (r > maxR) maxR = r;
	}
	return sqrt(maxR);
}
// calculate the radius of gyration of an object, assuming uniform surface density
double calcGyrationRadius_shell(const triangle* T, int N) {
	// not sure if I calculated this correctly
	double A = 0.; mat3 I(0.);
	for (int i = 0; i < N; i++) {
		vec3 a = T[i].A, b = T[i].B, c = T[i].C;
		double dA = 0.5*length(cross(b - a, c - a));
		A += dA;
		I += dA / 6. *(mat3(dot(a, a) + dot(b, b) + dot(c, c) + dot(a, b) + dot(a, c) + dot(b, c)) -
			(tensor(a, a) + tensor(b, b) + tensor(c, c) + 0.5*(tensor(a, b) + tensor(a, c) + tensor(b, a) + tensor(b, c) + tensor(c, a) + tensor(c, b))));
	}
	return sqrt(cbrt(determinant(I)) / A);
}
// calculate sqrt[∫(r²dS)/∫(dS)], assume the object is a surface
double calcRotationRadius_shell(const triangle* T, int N) {
	double I = 0., S = 0.;
	for (int i = 0; i < N; i++) {
		vec3 a = T[i].A, b = T[i].B, c = T[i].C;
		double dS = 0.5*length(cross(b - a, c - a));
		double dI = (1. / 12.)*(a.sqr() + b.sqr() + c.sqr() + dot(a, b) + dot(a, c) + dot(b, c));
		S += dS, I += dI * dS;
	}
	return sqrt(I / S);
}


// scale the object to a fixed size
// visually, scaleGyrationRadiusTo works best but it is the slowest
void scaleMaxRadiusTo(triangle* T, int N, double r) {
	double s = r / calcMaxRadius(T, N);
	for (int i = 0; i < N; i++) T[i].scale(s);
}
void scaleGyrationRadiusTo_shell(triangle* T, int N, double r) {
	double s = r / calcGyrationRadius_shell(T, N);
	for (int i = 0; i < N; i++) T[i].scale(s);
}
void scaleRotationRadiusTo_shell(triangle* T, int N, double r) {
	double s = r / calcRotationRadius_shell(T, N);
	for (int i = 0; i < N; i++) T[i].scale(s);
}

