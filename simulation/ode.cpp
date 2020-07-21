// Test numerical integration of ODEs: x"(t) = a
// In this experiment, integration variables are 3d vectors

// Compilable with Microsoft Visual Studio
// 2kb simulation code and 30kb debugging code?! oh no...


// To-do:
// * Runge-Kutta method
// * Adaptive step size



// debug
#define _USE_CONSOLE 0

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <functional>
#pragma warning(disable: 4244 4305 4996)

// ========================================= Win32 Standard =========================================

#pragma region Windows

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>

// debug
wchar_t _DEBUG_OUTPUT_BUF[0x1000];
#define dbgprint(format, ...) { if (_USE_CONSOLE) {printf(format, ##__VA_ARGS__);} else {swprintf(_DEBUG_OUTPUT_BUF, 0x1000, _T(format), ##__VA_ARGS__); OutputDebugStringW(_DEBUG_OUTPUT_BUF);} }


#pragma region Window Macros / Forward Declarations

#define WIN_NAME "ode"
#define WinW_Padding 100
#define WinH_Padding 100
#define WinW_Default 640
#define WinH_Default 400
#define WinW_Min 400
#define WinH_Min 300
#define WinW_Max 3840
#define WinH_Max 2160

void Init();
void render();
void WindowResize(int _oldW, int _oldH, int _W, int _H);
void WindowClose();
void MouseMove(int _X, int _Y);
void MouseWheel(int _DELTA);
void MouseDownL(int _X, int _Y);
void MouseUpL(int _X, int _Y);
void MouseDownR(int _X, int _Y);
void MouseUpR(int _X, int _Y);
void KeyDown(WPARAM _KEY);
void KeyUp(WPARAM _KEY);

HWND _HWND; int _WIN_W, _WIN_H;
HBITMAP _HIMG; COLORREF *_WINIMG;
#define Canvas(x,y) _WINIMG[(y)*_WIN_W+(x)]
#define setColor(x,y,col) do{if((x)>=0&&(x)<_WIN_W&&(y)>=0&&(y)<_WIN_H)Canvas(x,y)=col;}while(0)
double _DEPTHBUF[WinW_Max][WinH_Max];

#pragma endregion


// Win32 Entry

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
#define _RDBK { HDC hdc = GetDC(_HWND), HImgMem = CreateCompatibleDC(hdc); HBITMAP hbmOld = (HBITMAP)SelectObject(HImgMem, _HIMG); render(); BitBlt(hdc, 0, 0, _WIN_W, _WIN_H, HImgMem, 0, 0, SRCCOPY); SelectObject(HImgMem, hbmOld), DeleteDC(HImgMem), DeleteDC(hdc); } break;
	switch (message) {
	case WM_NULL: {_RDBK}
	case WM_CREATE: { if (!_HWND) Init(); break; }
	case WM_CLOSE: { WindowClose(); DestroyWindow(hWnd); return 0; }
	case WM_DESTROY: { PostQuitMessage(0); return 0; }
	case WM_MOVE:; case WM_SIZE: {
		RECT Client; GetClientRect(hWnd, &Client); WindowResize(_WIN_W, _WIN_H, Client.right, Client.bottom); _WIN_W = Client.right, _WIN_H = Client.bottom;
		BITMAPINFO bmi; bmi.bmiHeader.biSize = sizeof(BITMAPINFO), bmi.bmiHeader.biWidth = Client.right, bmi.bmiHeader.biHeight = Client.bottom, bmi.bmiHeader.biPlanes = 1, bmi.bmiHeader.biBitCount = 32; bmi.bmiHeader.biCompression = BI_RGB, bmi.bmiHeader.biSizeImage = 0, bmi.bmiHeader.biXPelsPerMeter = bmi.bmiHeader.biYPelsPerMeter = 0, bmi.bmiHeader.biClrUsed = bmi.bmiHeader.biClrImportant = 0; bmi.bmiColors[0].rgbBlue = bmi.bmiColors[0].rgbGreen = bmi.bmiColors[0].rgbRed = bmi.bmiColors[0].rgbReserved = 0;
		if (_HIMG != NULL) DeleteObject(_HIMG); HDC hdc = GetDC(hWnd); _HIMG = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&_WINIMG, NULL, 0); DeleteDC(hdc); _RDBK
	}
	case WM_GETMINMAXINFO: { LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam; lpMMI->ptMinTrackSize.x = WinW_Min, lpMMI->ptMinTrackSize.y = WinH_Min, lpMMI->ptMaxTrackSize.x = WinW_Max, lpMMI->ptMaxTrackSize.y = WinH_Max; break; }
	case WM_PAINT: { PAINTSTRUCT ps; HDC hdc = BeginPaint(hWnd, &ps), HMem = CreateCompatibleDC(hdc); HBITMAP hbmOld = (HBITMAP)SelectObject(HMem, _HIMG); BitBlt(hdc, 0, 0, _WIN_W, _WIN_H, HMem, 0, 0, SRCCOPY); SelectObject(HMem, hbmOld); EndPaint(hWnd, &ps); DeleteDC(HMem), DeleteDC(hdc); break; }
#define _USER_FUNC_PARAMS GET_X_LPARAM(lParam), _WIN_H - 1 - GET_Y_LPARAM(lParam)
	case WM_MOUSEMOVE: { MouseMove(_USER_FUNC_PARAMS); _RDBK }
	case WM_MOUSEWHEEL: { MouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)); _RDBK }
	case WM_LBUTTONDOWN: { SetCapture(hWnd); MouseDownL(_USER_FUNC_PARAMS); _RDBK }
	case WM_LBUTTONUP: { ReleaseCapture(); MouseUpL(_USER_FUNC_PARAMS); _RDBK }
	case WM_RBUTTONDOWN: { MouseDownR(_USER_FUNC_PARAMS); _RDBK }
	case WM_RBUTTONUP: { MouseUpR(_USER_FUNC_PARAMS); _RDBK }
	case WM_SYSKEYDOWN:; case WM_KEYDOWN: { if (wParam >= 0x08) KeyDown(wParam); _RDBK }
	case WM_SYSKEYUP:; case WM_KEYUP: { if (wParam >= 0x08) KeyUp(wParam); _RDBK }
	} return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	if (_USE_CONSOLE) if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) freopen("CONIN$", "r", stdin), freopen("CONOUT$", "w", stdout), freopen("CONOUT$", "w", stderr);
	WNDCLASSEX wc; wc.cbSize = sizeof(WNDCLASSEX), wc.style = 0, wc.lpfnWndProc = WndProc, wc.cbClsExtra = wc.cbWndExtra = 0, wc.hInstance = hInstance; wc.hIcon = wc.hIconSm = 0, wc.hCursor = LoadCursor(NULL, IDC_ARROW), wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)), wc.lpszMenuName = NULL, wc.lpszClassName = _T(WIN_NAME);
	if (!RegisterClassEx(&wc)) return -1;
	_HWND = CreateWindow(_T(WIN_NAME), _T(WIN_NAME), WS_OVERLAPPEDWINDOW, WinW_Padding, WinH_Padding, WinW_Default, WinH_Default, NULL, NULL, hInstance, NULL);
	ShowWindow(_HWND, nCmdShow); UpdateWindow(_HWND);
	MSG message; while (GetMessage(&message, 0, 0, 0)) { TranslateMessage(&message); DispatchMessage(&message); } return (int)message.wParam;
}

#pragma endregion


// ================================== Vector Classes/Functions ==================================

#pragma region Vector & Matrix

#define PI 3.1415926535897932384626
#define mix(x,y,a) ((x)*(1.0-(a))+(y)*(a))
#define clamp(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))
double mod(double x, double m) { return x - m * floor(x / m); }

// vector templates - huge!
struct vec2 {
	double x, y;
	explicit vec2() {}
	explicit vec2(const double &a) :x(a), y(a) {}
	explicit vec2(const double &x, const double &y) :x(x), y(y) {}
	vec2 operator - () const { return vec2(-x, -y); }
	vec2 operator + (const vec2 &v) const { return vec2(x + v.x, y + v.y); }
	vec2 operator - (const vec2 &v) const { return vec2(x - v.x, y - v.y); }
	vec2 operator * (const vec2 &v) const { return vec2(x * v.x, y * v.y); }
	vec2 operator * (const double &a) const { return vec2(x*a, y*a); }
	double sqr() const { return x * x + y * y; }
	friend double length(const vec2 &v) { return sqrt(v.x*v.x + v.y*v.y); }
	friend vec2 normalize(const vec2 &v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	friend double dot(const vec2 &u, const vec2 &v) { return u.x*v.x + u.y*v.y; }
	friend double det(const vec2 &u, const vec2 &v) { return u.x*v.y - u.y*v.x; }
	void operator += (const vec2 &v) { x += v.x, y += v.y; }
	void operator -= (const vec2 &v) { x -= v.x, y -= v.y; }
	void operator *= (const vec2 &v) { x *= v.x, y *= v.y; }
	friend vec2 operator * (const double &a, const vec2 &v) { return vec2(a*v.x, a*v.y); }
	void operator *= (const double &a) { x *= a, y *= a; }
	vec2 operator / (const double &a) const { return vec2(x / a, y / a); }
	void operator /= (const double &a) { x /= a, y /= a; }
	vec2 rot() const { return vec2(-y, x); }
};
struct vec3 {
	double x, y, z;
	explicit vec3() {}
	explicit vec3(const double &a) :x(a), y(a), z(a) {}
	explicit vec3(const double &x, const double &y, const double &z) :x(x), y(y), z(z) {}
	explicit vec3(const vec2 &v, const double &z) :x(v.x), y(v.y), z(z) {}
	vec3 operator - () const { return vec3(-x, -y, -z); }
	vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	vec3 operator * (const vec3 &v) const { return vec3(x * v.x, y * v.y, z * v.z); }
	vec3 operator * (const double &k) const { return vec3(k * x, k * y, k * z); }
	double sqr() const { return x * x + y * y + z * z; } 	// non-standard
	friend double length(vec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
	friend vec3 normalize(vec3 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y + v.z*v.z)); }
	friend double dot(vec3 u, vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
	friend vec3 cross(vec3 u, vec3 v) { return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
	void operator += (const vec3 &v) { x += v.x, y += v.y, z += v.z; }
	void operator -= (const vec3 &v) { x -= v.x, y -= v.y, z -= v.z; }
	void operator *= (const vec3 &v) { x *= v.x, y *= v.y, z *= v.z; }
	friend vec3 operator * (const double &a, const vec3 &v) { return vec3(a*v.x, a*v.y, a*v.z); }
	void operator *= (const double &a) { x *= a, y *= a, z *= a; }
	vec3 operator / (const double &a) const { return vec3(x / a, y / a, z / a); }
	void operator /= (const double &a) { x /= a, y /= a, z /= a; }
	vec2 xy() const { return vec2(x, y); }
};

const vec3 vec0(0, 0, 0), veci(1, 0, 0), vecj(0, 1, 0), veck(0, 0, 1);
#define SCRCTR vec2(0.5*_WIN_W,0.5*_WIN_H)

// 4x4 matrix (simple)
struct Affine {
	vec3 u, v, w;  // first row, second row, third row
	vec3 t, p;  // translation, perspective
	double s;  // scaling
};
vec3 operator * (Affine T, vec3 p) {
	vec3 q = vec3(dot(T.u, p), dot(T.v, p), dot(T.w, p)) + T.t;
	double d = 1.0 / (dot(T.p, p) + T.s);
	return d < 0.0 ? vec3(NAN) : q * d;
	//return q * d;
}
Affine operator * (const Affine &A, const Affine &B) {
	Affine R;
	R.u = A.u.x*B.u + A.u.y*B.v + A.u.z*B.w + A.t.x*B.p;
	R.v = A.v.x*B.u + A.v.y*B.v + A.v.z*B.w + A.t.y*B.p;
	R.w = A.w.x*B.u + A.w.y*B.v + A.w.z*B.w + A.t.z*B.p;
	R.t = vec3(dot(A.u, B.t), dot(A.v, B.t), dot(A.w, B.t)) + A.t*B.s;
	R.p = vec3(A.p.x*B.u.x + A.p.y*B.v.x + A.p.z*B.w.x, A.p.x*B.u.y + A.p.y*B.v.y + A.p.z*B.w.y, A.p.x*B.u.z + A.p.y*B.v.z + A.p.z*B.w.z) + B.p*A.s;
	R.s = dot(A.p, B.t) + A.s*B.s;
	return R;
}


#pragma endregion


// ======================================== Data / Parameters ========================================

// viewport
// Ctrl/Shift + Drag/Wheel to adjust these variables
vec3 Center(0.0, 0.0, 0.0);  // view center in world coordinate
double rz = -0.8, rx = 0.3, ry = 0.0, dist = 120.0, Unit = 100.0;  // yaw, pitch, row, camera distance, scale to screen

#pragma region General Global Variables and Functions

// window parameters
char text[64];	// window title
Affine Tr;  // matrix
vec3 CamP, ScrO, ScrA, ScrB;  // camera and screen
auto scrDir = [](vec2 pixel) { return normalize(ScrO + (pixel.x / _WIN_W)*ScrA + (pixel.y / _WIN_H)*ScrB - CamP); };

// user parameters
vec2 Cursor = vec2(0, 0), clickCursor;  // current cursor and cursor position when mouse down
bool mouse_down = false;
bool Ctrl = false, Shift = false, Alt = false;  // these variables are shared by both windows

// projection
Affine axisAngle(vec3 axis, double a) {
	axis = normalize(axis); double ct = cos(a), st = sin(a);
	return Affine{
		vec3(ct + axis.x*axis.x*(1 - ct), axis.x*axis.y*(1 - ct) - axis.z*st, axis.x*axis.z*(1 - ct) + axis.y*st),
		vec3(axis.y*axis.x*(1 - ct) + axis.z*st, ct + axis.y*axis.y*(1 - ct), axis.y*axis.z*(1 - ct) - axis.x*st),
		vec3(axis.z*axis.x*(1 - ct) - axis.y*st, axis.z*axis.y*(1 - ct) + axis.x*st, ct + axis.z*axis.z*(1 - ct)),
		vec3(0), vec3(0), 1.0
	};
}
void calcMat() {
	double cx = cos(rx), sx = sin(rx), cz = cos(rz), sz = sin(rz), cy = cos(ry), sy = sin(ry);
	Affine D{ veci, vecj, veck, -Center, vec3(0), 1.0 };  // world translation
	Affine R{ vec3(-sz, cz, 0), vec3(-cz * sx, -sz * sx, cx), vec3(-cz * cx, -sz * cx, -sx), vec3(0), vec3(0), 1.0 };  // rotation
	R = Affine{ vec3(cy, -sy, 0), vec3(sy, cy, 0), vec3(0, 0, 1), vec3(0), vec3(0), 1.0 } *R;  // camera roll (ry)
	Affine P{ veci, vecj, veck, vec3(0), vec3(0, 0, 1.0 / dist), 1.0 };  // perspective
	Affine S{ veci, vecj, veck, vec3(0), vec3(0), 1.0 / Unit };  // scale
	Affine T{ veci, vecj, veck, vec3(SCRCTR, 0.0), vec3(0), 1.0 };  // screen translation
	Tr = T * S * P * R * D;
}
void getRay(vec2 Cursor, vec3 &p, vec3 &d) {
	p = CamP;
	d = normalize(ScrO + (Cursor.x / _WIN_W)*ScrA + (Cursor.y / _WIN_H)*ScrB - CamP);
}
void getScreen(vec3 &P, vec3 &O, vec3 &A, vec3 &B) {  // O+uA+vB
	double cx = cos(rx), sx = sin(rx), cz = cos(rz), sz = sin(rz);
	vec3 u(-sz, cz, 0), v(-cz * sx, -sz * sx, cx), w(cz * cx, sz * cx, sx);
	Affine Y = axisAngle(w, -ry); u = Y * u, v = Y * v;
	u *= 0.5*_WIN_W / Unit, v *= 0.5*_WIN_H / Unit, w *= dist;
	P = Center + w;
	O = Center - (u + v), A = u * 2.0, B = v * 2.0;
}

// rasterization forward declaration
void drawLine(vec2 p, vec2 q, COLORREF col);
void drawCross(vec2 p, double r, COLORREF col);
void drawCircle(vec2 c, double r, COLORREF col);
void fillCircle(vec2 c, double r, COLORREF col);
void drawLine_F(vec3 A, vec3 B, COLORREF col);
void drawCross3D(vec3 P, double r, COLORREF col, bool relative);

#pragma endregion


#pragma region Simulation Test Cases

class Test_Case {
public:
	double dt, t1;
	vec3 P0, V0;
	std::function<vec3(vec3, vec3, double)> Acceleration;
	std::function<void(double)> additional_render;
	template<typename Fun>
	Test_Case(Fun acc, double t1, double dt, vec3 p0, vec3 v0) {
		this->dt = dt, this->t1 = t1; P0 = p0, V0 = v0; Acceleration = acc;
		additional_render = [](double) {};
	}
	template<typename Fun, typename Rd>
	Test_Case(Fun acc, double t1, double dt, vec3 p0, vec3 v0, Rd render = [](double) {}) {
		this->dt = dt, this->t1 = t1; P0 = p0, V0 = v0; Acceleration = acc;
		additional_render = render;
	}
};

// Gravitational Acceleration
vec3 g = vec3(0, 0, -9.81);

// Acceleration due to gravity
Test_Case Projectile([](vec3 p, vec3 v, double t)->vec3 {
	return g;
}, 2.0, 0.1, vec3(0, 0, 1), vec3(1.5, 1.5, 2));

// Acceleration due to gravity + v³ air resistance
Test_Case Projectile_R([](vec3 p, vec3 v, double t)->vec3 {
	double r = 0.1*dot(v, v);
	return g - r * v;
}, 2.0, 0.06, vec3(0, 0, 1), vec3(2, 2, 3));

// *Drops onto an elastic surface and bounces up
Test_Case Projectile_RB([](vec3 p, vec3 v, double t)->vec3 {
	vec3 r = -0.1*dot(v, v)*v;
	vec3 b = min(p.z, 0.)*vec3(0, 0, -50);
	return g + r + b;
}, 6.0, 0.02, vec3(-2.5, 0, 1), vec3(3, 0, 3));

// **Drops into heavy liquid and bounces up
Test_Case Projectile_RW([](vec3 p, vec3 v, double t)->vec3 {
	vec3 r = -(p.z > 0. ? 0.05 : 0.5)*dot(v, v)*v;
	vec3 b = (p.z < 0. ? 1. : 0.)*vec3(0, 0, 50);
	return g + r + b;
}, 6.0, 0.01, vec3(-3, 0, 1), vec3(3, 0, 3));

// On a one-meter-long non-deformable rod
// An accurate solution should not deviate the unit sphere
Test_Case Pendulum([](vec3 p, vec3 v, double t)->vec3 {
	vec3 u = normalize(cross(p, cross(p, veck))) * (g * length(p.xy()) / length(p));
	vec3 w = -p * dot(v, v);
	return u + w;
}, 6.0, 0.02, vec3(0, 1, 0), vec3(1, 0, 0));

// `On a one-meter-long spring with air resistance
Test_Case Pendulum_S([](vec3 p, vec3 v, double t)->vec3 {
	vec3 d = p - vec3(0, 0, 2);
	vec3 N = -10.0*(length(d) - 1)*normalize(d);
	vec3 r = -0.01*dot(v, v)*v;
	return g + N + r;
}, 6.0, 0.05, vec3(0, 1, 0), vec3(3, 0, 3));

// A "sun" in the center
// The solution is an ellipse
Test_Case NBody_1([](vec3 p, vec3 v, double t)->vec3 {
	double m = length(p);
	return -20.0*p / (m*m*m);
}, 3.0, 0.05, vec3(2, 2, 0), vec3(1, -1, -0.02));

// `Two "suns" with equal mass
Test_Case NBody_2([](vec3 p, vec3 v, double t)->vec3 {
	vec3 q = p - vec3(0, 1, 0);
	double m = length(q);
	vec3 a = q / (m*m*m);
	q = p + vec3(0, 1, 0);
	m = length(q);
	a += q / (m*m*m);
	return -10.0*a;
}, 8.0, 0.01, vec3(2, 2, 0), vec3(1, -0.5, -0.2), [](double t) {
	fillCircle((Tr*vec3(0, 1, 0)).xy(), 4, 0x00A0FF);
	fillCircle((Tr*vec3(0, -1, 0)).xy(), 4, 0x00A0FF);
});

// `One sun and one mobilized planet
Test_Case NBody_m([](vec3 p, vec3 v, double t)->vec3 {
	double m = length(p);
	vec3 F = -9.*p / (m*m*m);
	p -= vec3(cos(3.*t), sin(3.*t), 0);
	m = length(p);
	return F - p / (m*m*m);
}, 2.*PI, 0.02, vec3(2, 2, 0), vec3(1, -.5, -.2), [](double t) {
	fillCircle((Tr*vec3(cos(3.*t), sin(3.*t), 0)).xy(), 4, 0x00FF00);
});

// `Artificial equation #1
Test_Case TimeTest_1([](vec3 p, vec3 v, double t)->vec3 {
	vec3 w = vec3(0, 0, sin(t) + cos(t) + 1);
	return cross(v, w) - w.sqr() * p - 2.*p + vec3(0, 0, -1) - 0.01*v.sqr()*v;
}, 6.0, 0.05, vec3(1, 1, 1), vec3(0, 0, 0));

// **`Artificial equation #2
Test_Case TimeTest_2([](vec3 p, vec3 v, double t)->vec3 {
	double s = floor(2.*t*t + 2.);
	double a = fmod(1000 * sin(100 * floor(t)), 1.0);
	double b = fmod(2000 * cos(200 * floor(t)), 1.0);
	double c = fmod(3000 * sin(300 * floor(t)), 1.0);
	vec3 w = 2.0*vec3(c, b, a) - vec3(1.0);
	return cross(v, w) - (length(w) + 2.) * p + 0.2*w;
}, 6.0, 0.05, vec3(1, 1, 1), vec3(-1, 0, 0));

// `Artifical equation #3 (continuous)
Test_Case TimeTest_3([](vec3 p, vec3 v, double t)->vec3 {
	vec3 a = vec3(cos(p.x + t), sin(v.y - t), sin(p.z + t));
	vec3 b = vec3(cos(v.x + t), exp(p.y), cos(v.z));
	vec3 c = vec3(sin(t), cos(t), sin(t + .25*PI));
	return 4.*(a + b + c - 5.*p);
}, 6.0, 0.05, vec3(0), vec3(0));

#pragma endregion Projectile, Projectile_R, Projectile_RB, Projectile_RW, Pendulum, Pendulum_S, NBody_1, NBody_2, NBody_m, TimeTest_1, TimeTest_2, TimeTest_3

Test_Case T = Projectile;



// ============================================ Rendering ============================================

#pragma region Rasterization functions

void drawLine(vec2 p, vec2 q, COLORREF col) {
	vec2 d = q - p;
	double slope = d.y / d.x;
	if (abs(slope) <= 1.0) {
		if (p.x > q.x) std::swap(p, q);
		int x0 = max(0, int(p.x)), x1 = min(_WIN_W - 1, int(q.x)), y;
		double yf = slope * x0 + (p.y - slope * p.x);
		for (int x = x0; x <= x1; x++) {
			y = (int)yf;
			if (y >= 0 && y < _WIN_H) Canvas(x, y) = col;
			yf += slope;
		}
	}
	else {
		slope = d.x / d.y;
		if (p.y > q.y) std::swap(p, q);
		int y0 = max(0, int(p.y)), y1 = min(_WIN_H - 1, int(q.y)), x;
		double xf = slope * y0 + (p.x - slope * p.y);
		for (int y = y0; y <= y1; y++) {
			x = (int)xf;
			if (x >= 0 && x < _WIN_W) Canvas(x, y) = col;
			xf += slope;
		}
	}
};
void drawCross(vec2 p, double r, COLORREF col = 0xFFFFFF) {
	drawLine(p - vec2(r, 0), p + vec2(r, 0), col);
	drawLine(p - vec2(0, r), p + vec2(0, r), col);
};
void drawCircle(vec2 c, double r, COLORREF col) {
	int s = int(r / sqrt(2) + 0.5);
	int cx = (int)c.x, cy = (int)c.y;
	for (int i = 0, im = min(s, max(_WIN_W - cx, cx)) + 1; i < im; i++) {
		int u = sqrt(r*r - i * i) + 0.5;
		setColor(cx + i, cy + u, col); setColor(cx + i, cy - u, col); setColor(cx - i, cy + u, col); setColor(cx - i, cy - u, col);
		setColor(cx + u, cy + i, col); setColor(cx + u, cy - i, col); setColor(cx - u, cy + i, col); setColor(cx - u, cy - i, col);
	}
};
void fillCircle(vec2 c, double r, COLORREF col) {
	int x0 = max(0, int(c.x - r)), x1 = min(_WIN_W - 1, int(c.x + r));
	int y0 = max(0, int(c.y - r)), y1 = min(_WIN_H - 1, int(c.y + r));
	int cx = (int)c.x, cy = (int)c.y, r2 = int(r*r);
	for (int x = x0, dx = x - cx; x <= x1; x++, dx++) {
		for (int y = y0, dy = y - cy; y <= y1; y++, dy++) {
			if (dx * dx + dy * dy < r2) Canvas(x, y) = col;
		}
	}
};

void drawLine_F(vec3 A, vec3 B, COLORREF col = 0xFFFFFF) {
	double u = dot(Tr.p, A) + Tr.s, v = dot(Tr.p, B) + Tr.s;
	if (u > 0 && v > 0) { drawLine((Tr*A).xy(), (Tr*B).xy(), col); return; }
	if (u < 0 && v < 0) return;
	if (u < v) std::swap(A, B), std::swap(u, v);
	double t = u / (u - v) - 1e-4;
	B = A + (B - A)*t;
	drawLine((Tr*A).xy(), (Tr*B).xy(), col);
};
void drawCross3D(vec3 P, double r, COLORREF col = 0xFFFFFF, bool relative = true) {
	return;  // comment this line to make it like barbed wire
	if (relative) r *= dot(Tr.p, P) + Tr.s;
	drawLine_F(P - vec3(r, 0, 0), P + vec3(r, 0, 0), col);
	drawLine_F(P - vec3(0, r, 0), P + vec3(0, r, 0), col);
	drawLine_F(P - vec3(0, 0, r), P + vec3(0, 0, r), col);
};

#pragma endregion


#pragma region Time
#include <chrono>
typedef std::chrono::high_resolution_clock NTime;
typedef std::chrono::duration<double> fsec;
auto start_time = NTime::now();
double iTime;
#pragma endregion hold alt to "freeze"


#pragma region Reference Path
vec3 *RefPath;
int RefN;
#define REFPATH_DT 0.00001
#define REFPATH_DT_N 100
void initReferencePath() {
	const double dt = REFPATH_DT;
	const int dtN = REFPATH_DT_N;
	vec3 p0 = T.P0, v0 = T.V0, p = p0, v = v0, a;
	RefN = (int)(T.t1 / (dtN*dt));
	int N = (int)(T.t1 / dt);
	RefPath = new vec3[RefN + 1];
	RefPath[RefN] = vec3(NAN);
	for (int i = 0; i < N; i++) {
		if (i % dtN == 0) RefPath[i / dtN] = p0;
		double t = i * dt;
		a = T.Acceleration(p, v, t);
		vec3 _p = p;
		p += v * dt + a * (.5*dt*dt);
		v += T.Acceleration(.5*(_p + p), v + a * (.5*dt), t + .5*dt) * dt;
		p0 = p, v0 = v;
	}
	if (0.0*RefPath[RefN].sqr() != 0.0) RefN--;
}
#pragma endregion


#pragma region Simulation

#define plotPath(Color) \
	drawLine_F(p0, p, Color); \
	drawCross3D(p, 2, Color); \
	double u = t + dt - iTime; if (u > 0 && u < dt) { u /= dt; fillCircle((Tr*(u*p0 + (1 - u)*p)).xy(), 6, Color); }

// orange
template<typename Fun>
void EulersMethod(vec3 p0, vec3 v0, Fun acc, double dt, double tMax) {  // p: 1/6 h³ p³(t0); v: 1/2 h² v"(t0)
	vec3 p = p0, v = v0, a;
	for (double t = 0.0; t < tMax; t += dt) {
		a = acc(p, v, t);
		p += v * dt + a * (.5*dt*dt);
		v += a * dt;
		plotPath(0xFF8000);
		p0 = p, v0 = v;
	}
}

// yellow
template<typename Fun>
void Euler_Midpoint(vec3 p0, vec3 v0, Fun acc, double dt, double tMax) {  // p: 1/6 h³ p³(t0); v: 1/24 h³ v³(t0) ??
	dt *= 2.0;  // to be fair
	vec3 p = p0, v = v0, a;
	for (double t = 0.0; t < tMax; t += dt) {
		a = acc(p, v, t);
		vec3 _p = p;
		p += v * dt + a * (.5*dt*dt);
		v += acc(.5*(_p + p), v + a * (.5*dt), t + .5*dt) * dt;
		plotPath(0xFFFF00);
		p0 = p, v0 = v;
	}
}

// yellow green, accuracy similar to Euler_Midpoint
template<typename Fun>
void Midpoint_1(vec3 p0, vec3 v0, Fun acc, double dt, double tMax) {  // p: 1/6 h³ p³(t0); v: 5/12 h³ v³(t0) ??
	vec3 p = p0, v = v0, a0, a = a0 = acc(p0, v0, 0);
	for (double t = 0.0; t < tMax; t += dt) {
		a = acc(p, v, t);
		p += v * dt + a * (.5*dt*dt);
		v += (1.5*a - 0.5*a0) * dt;
		plotPath(0xA0FF00);
		p0 = p, v0 = v, a0 = a;
	}
}

// sky blue, works best when acceleration only depend on the position
template<typename Fun>
void Verlet_Modified(vec3 p0, vec3 v0, Fun acc, double dt, double tMax) {  // p: 1/12 h⁴ p⁴(t0); v: 5/12 h³ v³(t0) ????
	vec3 a = acc(p0, v0, 0);
	vec3 v = v0 + a * dt, p = p0 + v0 * dt + a * (.5*dt*dt);
	vec3 a0 = acc(p0 - v0 * dt + a * (.5*dt*dt), v0 - a * dt, -dt);
	for (double t = 0.; t < tMax; t += dt) {
		a = acc(p, v, t);
		vec3 p1 = 2.*p - p0 + a * (dt*dt);
		v += (1.5*a - 0.5*a0) * dt;
		plotPath(0x0080FF);
		p0 = p, a0 = a, p = p1;
	}
}


#pragma endregion



void render() {
	if (!_WINIMG) return;

	auto t0 = NTime::now();
	// initialize window
	for (int i = 0, l = _WIN_W * _WIN_H; i < l; i++) _WINIMG[i] = 0;
	for (int i = 0; i < _WIN_W; i++) for (int j = 0; j < _WIN_H; j++) _DEPTHBUF[i][j] = INFINITY;
	calcMat();
	getScreen(CamP, ScrO, ScrA, ScrB);

	// axis and grid
	{
		const double R = 20.0;
		for (int i = -R; i <= R; i++) {
			drawLine_F(vec3(-R, i, 0), vec3(R, i, 0), 0x404040);
			drawLine_F(vec3(i, -R, 0), vec3(i, R, 0), 0x404040);
		}
		drawLine_F(vec3(0, -R, 0), vec3(0, R, 0), 0x409040);
		drawLine_F(vec3(-R, 0, 0), vec3(R, 0, 0), 0xC04040);
		drawLine_F(vec3(0, 0, -R), vec3(0, 0, R), 0x4040FF);
		drawCross3D(Center, 4, 0x00FF00);
	}


	// simulation
	if (!Alt) iTime = fmod(fsec(NTime::now() - start_time).count(), T.t1);

	// reference path
	for (int i = 0; i < RefN; i++) {
		if (i % 20 < 10) drawLine_F(RefPath[i], RefPath[i + 1], 0x606080);
	}
	fillCircle((Tr*RefPath[(int)(iTime / (REFPATH_DT*REFPATH_DT_N))]).xy(), 6, 0x606080);

	// simulation paths with large step
	vec3 P0 = T.P0, V0 = T.V0;
	double t_step = T.dt, tMax = T.t1;
	auto Acceleration = T.Acceleration;
	EulersMethod(P0, V0, Acceleration, t_step, tMax);
	Euler_Midpoint(P0, V0, Acceleration, t_step, tMax);
	Midpoint_1(P0, V0, Acceleration, t_step, tMax);
	Verlet_Modified(P0, V0, Acceleration, t_step, tMax);

	T.additional_render(iTime);

	double t = fsec(NTime::now() - t0).count();
	sprintf(text, "[%d×%d]  %.1fms (%.1ffps)\n", _WIN_W, _WIN_H, 1000.0*t, 1. / t);
	SetWindowTextA(_HWND, text);
}



#include <thread>
bool inited = false;
void Init() {
	if (inited) return; inited = true;
	initReferencePath();
	new std::thread([](int x) {while (1) {
		SendMessage(_HWND, WM_NULL, NULL, NULL);
		Sleep(20);
	}}, 5);
}


// ============================================== User ==============================================


void keyDownShared(WPARAM _KEY) {
	if (_KEY == VK_CONTROL) Ctrl = true;
	else if (_KEY == VK_SHIFT) Shift = true;
	else if (_KEY == VK_MENU) Alt = true;
}
void keyUpShared(WPARAM _KEY) {
	if (_KEY == VK_CONTROL) Ctrl = false;
	else if (_KEY == VK_SHIFT) Shift = false;
	else if (_KEY == VK_MENU) Alt = false;
}


void WindowResize(int _oldW, int _oldH, int _W, int _H) {
	if (_W*_H == 0 || _oldW * _oldH == 0) return;  // window is minimized
	double pw = _oldW, ph = _oldH, w = _W, h = _H;
	double s = sqrt((w * h) / (pw * ph));
	Unit *= s, dist /= s;
}
void WindowClose() {
}

void MouseWheel(int _DELTA) {
	if (Ctrl) Center.z += 0.1 * _DELTA / Unit;
	else if (Shift) dist *= exp(-0.001*_DELTA);
	else {
		double s = exp(0.001*_DELTA);
		Unit *= s, dist /= s;
	}
}
void MouseDownL(int _X, int _Y) {
	clickCursor = Cursor = vec2(_X, _Y);
	mouse_down = true;
}
void MouseMove(int _X, int _Y) {
	vec2 P0 = Cursor, P = vec2(_X, _Y), D = P - P0;
	Cursor = P;

	if (mouse_down) {
		if (Ctrl) {
			vec3 d = scrDir(P0);
			vec3 p = CamP.z / d.z * d;
			d = scrDir(P);
			vec3 q = CamP.z / d.z * d;
			Center += q - p;
		}
		else if (Shift) {
			ry += 0.005*D.y;
		}
		else {
			vec2 d = 0.01*D;
			rz -= cos(ry)*d.x + sin(ry)*d.y, rx -= -sin(ry)*d.x + cos(ry)*d.y;
			//rz -= d.x, rx -= d.y;
		}
	}

}
void MouseUpL(int _X, int _Y) {
	Cursor = vec2(_X, _Y);
	bool moved = (int)length(clickCursor - Cursor) != 0;   // be careful: coincidence
	mouse_down = false;
}
void MouseDownR(int _X, int _Y) {
	Cursor = vec2(_X, _Y);
}
void MouseUpR(int _X, int _Y) {
	Cursor = vec2(_X, _Y);
#ifdef _DEBUG
	bool topmost = GetWindowLong(_HWND, GWL_EXSTYLE) & WS_EX_TOPMOST;
	SetWindowPos(_HWND, topmost ? HWND_NOTOPMOST : HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif
}
void KeyDown(WPARAM _KEY) {
	keyDownShared(_KEY);
}
void KeyUp(WPARAM _KEY) {
	keyUpShared(_KEY);
}

