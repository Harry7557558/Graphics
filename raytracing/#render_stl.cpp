// (simple??) 3D template

#include <cmath>
#include <stdio.h>
#include <algorithm>
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
#define _USE_CONSOLE 0
wchar_t _DEBUG_OUTPUT_BUF[0x1000];
#define dbgprint(format, ...) { if (_USE_CONSOLE) {printf(format, ##__VA_ARGS__);} else {swprintf(_DEBUG_OUTPUT_BUF, 0x1000, _T(format), ##__VA_ARGS__); OutputDebugStringW(_DEBUG_OUTPUT_BUF);} }


#pragma region Window Macros / Forward Declarations

// First Window (Main Window): UI Editor

#define WIN_NAME "UI"
#define WinW_Padding 100
#define WinH_Padding 100
#define WinW_Default 640
#define WinH_Default 400
#define WinW_Min 400
#define WinH_Min 300
#define WinW_Max 3840
#define WinH_Max 2160

void Init();  // only use this function to initialize variables (or test)
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

double _DEPTHBUF[WinW_Max][WinH_Max];  // how you use this depends on you

#pragma endregion


// Win32 Entry

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
#define _WNDSEL(vm,vt) vm  // no longer useful
#define _WNDSEL_T(vn) vn
	auto hImg = _WNDSEL(&_HIMG, &_HIMG_T);
	auto winImg = _WNDSEL(&_WINIMG, &_WINIMG_T);
	auto winW = _WNDSEL(&_WIN_W, &_WIN_T_W), winH = _WNDSEL(&_WIN_H, &_WIN_T_H);
#define _RD_RAW { HDC hdc = GetDC(hWnd), HImgMem = CreateCompatibleDC(hdc); HBITMAP hbmOld = (HBITMAP)SelectObject(HImgMem, *hImg); _WNDSEL(render, render_t)(); BitBlt(hdc, 0, 0, *winW, *winH, HImgMem, 0, 0, SRCCOPY); SelectObject(HImgMem, hbmOld), DeleteDC(HImgMem), DeleteDC(hdc); }
#define _RDBK { _RD_RAW break; }
	switch (message) {
	case WM_NULL: { _RD_RAW return 0; }
	case WM_CREATE: { if (!_HWND) Init(); break; }
	case WM_CLOSE: { _WNDSEL_T(WindowClose)(); DestroyWindow(hWnd); return 0; }
	case WM_DESTROY: { PostQuitMessage(0); return 0; }
	case WM_MOVE:; case WM_SIZE: {
		RECT Client; GetClientRect(hWnd, &Client); _WNDSEL_T(WindowResize)(*winW, *winH, Client.right, Client.bottom); *winW = Client.right, *winH = Client.bottom;
		BITMAPINFO bmi; bmi.bmiHeader.biSize = sizeof(BITMAPINFO), bmi.bmiHeader.biWidth = Client.right, bmi.bmiHeader.biHeight = Client.bottom, bmi.bmiHeader.biPlanes = 1, bmi.bmiHeader.biBitCount = 32; bmi.bmiHeader.biCompression = BI_RGB, bmi.bmiHeader.biSizeImage = 0, bmi.bmiHeader.biXPelsPerMeter = bmi.bmiHeader.biYPelsPerMeter = 0, bmi.bmiHeader.biClrUsed = bmi.bmiHeader.biClrImportant = 0; bmi.bmiColors[0].rgbBlue = bmi.bmiColors[0].rgbGreen = bmi.bmiColors[0].rgbRed = bmi.bmiColors[0].rgbReserved = 0;
		if (*hImg != NULL) DeleteObject(*hImg); HDC hdc = GetDC(hWnd); *hImg = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)winImg, NULL, 0); DeleteDC(hdc); _RDBK
	}
	case WM_GETMINMAXINFO: { LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam; lpMMI->ptMinTrackSize.x = _WNDSEL(WinW_Min, WinTW_Min), lpMMI->ptMinTrackSize.y = _WNDSEL(WinH_Min, WinTH_Min), lpMMI->ptMaxTrackSize.x = _WNDSEL(WinW_Max, WinTW_Max), lpMMI->ptMaxTrackSize.y = _WNDSEL(WinH_Max, WinTH_Max); break; }
	case WM_PAINT: { PAINTSTRUCT ps; HDC hdc = BeginPaint(hWnd, &ps), HMem = CreateCompatibleDC(hdc); HBITMAP hbmOld = (HBITMAP)SelectObject(HMem, *hImg); BitBlt(hdc, 0, 0, *winW, *winH, HMem, 0, 0, SRCCOPY); SelectObject(HMem, hbmOld); EndPaint(hWnd, &ps); DeleteDC(HMem), DeleteDC(hdc); break; }
#define _USER_FUNC_PARAMS GET_X_LPARAM(lParam), *winH - 1 - GET_Y_LPARAM(lParam)
	case WM_MOUSEMOVE: { _WNDSEL_T(MouseMove)(_USER_FUNC_PARAMS); _RDBK }
	case WM_MOUSEWHEEL: { _WNDSEL_T(MouseWheel)(GET_WHEEL_DELTA_WPARAM(wParam)); _RDBK }
	case WM_LBUTTONDOWN: { SetCapture(hWnd); _WNDSEL_T(MouseDownL)(_USER_FUNC_PARAMS); _RDBK }
	case WM_LBUTTONUP: { ReleaseCapture(); _WNDSEL_T(MouseUpL)(_USER_FUNC_PARAMS); _RDBK }
	case WM_RBUTTONDOWN: { _WNDSEL_T(MouseDownR)(_USER_FUNC_PARAMS); _RDBK }
	case WM_RBUTTONUP: { _WNDSEL_T(MouseUpR)(_USER_FUNC_PARAMS); _RDBK }
	case WM_SYSKEYDOWN:; case WM_KEYDOWN: { if (wParam >= 0x08) _WNDSEL_T(KeyDown)(wParam); _RDBK }
	case WM_SYSKEYUP:; case WM_KEYUP: { if (wParam >= 0x08) _WNDSEL_T(KeyUp)(wParam); _RDBK }
	} return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	//WriteImage(); return 0;
	if (_USE_CONSOLE) if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) freopen("CONIN$", "r", stdin), freopen("CONOUT$", "w", stdout), freopen("CONOUT$", "w", stderr);
	WNDCLASSEX wc; wc.cbSize = sizeof(WNDCLASSEX), wc.style = 0, wc.lpfnWndProc = WndProc, wc.cbClsExtra = wc.cbWndExtra = 0, wc.hInstance = hInstance; wc.hIcon = wc.hIconSm = 0, wc.hCursor = LoadCursor(NULL, IDC_ARROW), wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)), wc.lpszMenuName = NULL, wc.lpszClassName = _T(WIN_NAME);
	if (!RegisterClassEx(&wc)) return -1;
	_HWND = CreateWindow(_T(WIN_NAME), _T(WIN_NAME), WS_OVERLAPPEDWINDOW, WinW_Padding, WinH_Padding, WinW_Default, WinH_Default, NULL, NULL, hInstance, NULL);
	ShowWindow(_HWND, nCmdShow); UpdateWindow(_HWND);
	MSG message; while (GetMessage(&message, 0, 0, 0)) { TranslateMessage(&message); DispatchMessage(&message); } return (int)message.wParam;
}

#pragma endregion

// COLORREF
enum WebSafeColors {
	ALICEBLUE = 0xF0F8FF, ANTIQUEWHITE = 0xFAEBD7, AQUA = 0x00FFFF, AQUAMARINE = 0x7FFFD4, AZURE = 0xF0FFFF, BEIGE = 0xF5F5DC,
	BISQUE = 0xFFE4C4, BLACK = 0x000000, BLANCHEDALMOND = 0xFFEBCD, BLUE = 0x0000FF, BLUEVIOLET = 0x8A2BE2, BROWN = 0xA52A2A,
	BURLYWOOD = 0xDEB887, CADETBLUE = 0x5F9EA0, CHARTREUSE = 0x7FFF00, CHOCOLATE = 0xD2691E, CORAL = 0xFF7F50, CORNFLOWERBLUE = 0x6495ED,
	CORNSILK = 0xFFF8DC, CRIMSON = 0xDC143C, CYAN = 0x00FFFF, DARKBLUE = 0x00008B, DARKCYAN = 0x008B8B, DARKGOLDENROD = 0xB8860B,
	DARKGRAY = 0xA9A9A9, DARKGREY = 0xA9A9A9, DARKGREEN = 0x006400, DARKKHAKI = 0xBDB76B, DARKMAGENTA = 0x8B008B, DARKOLIVEGREEN = 0x556B2F,
	DARKORANGE = 0xFF8C00, DARKORCHID = 0x9932CC, DARKRED = 0x8B0000, DARKSALMON = 0xE9967A, DARKSEAGREEN = 0x8FBC8F, DARKSLATEBLUE = 0x483D8B,
	DARKSLATEGRAY = 0x2F4F4F, DARKSLATEGREY = 0x2F4F4F, DARKTURQUOISE = 0x00CED1, DARKVIOLET = 0x9400D3, DEEPPINK = 0xFF1493, DEEPSKYBLUE = 0x00BFFF,
	DIMGRAY = 0x696969, DIMGREY = 0x696969, DODGERBLUE = 0x1E90FF, FIREBRICK = 0xB22222, FLORALWHITE = 0xFFFAF0, FORESTGREEN = 0x228B22,
	FUCHSIA = 0xFF00FF, GAINSBORO = 0xDCDCDC, GHOSTWHITE = 0xF8F8FF, GOLD = 0xFFD700, GOLDENROD = 0xDAA520, GRAY = 0x808080,
	GREY = 0x808080, GREEN = 0x008000, GREENYELLOW = 0xADFF2F, HONEYDEW = 0xF0FFF0, HOTPINK = 0xFF69B4, INDIANRED = 0xCD5C5C,
	INDIGO = 0x4B0082, IVORY = 0xFFFFF0, KHAKI = 0xF0E68C, LAVENDER = 0xE6E6FA, LAVENDERBLUSH = 0xFFF0F5, LAWNGREEN = 0x7CFC00,
	LEMONCHIFFON = 0xFFFACD, LIGHTBLUE = 0xADD8E6, LIGHTCORAL = 0xF08080, LIGHTCYAN = 0xE0FFFF, LIGHTGOLDENRODYELLOW = 0xFAFAD2, LIGHTGRAY = 0xD3D3D3,
	LIGHTGREY = 0xD3D3D3, LIGHTGREEN = 0x90EE90, LIGHTPINK = 0xFFB6C1, LIGHTSALMON = 0xFFA07A, LIGHTSEAGREEN = 0x20B2AA, LIGHTSKYBLUE = 0x87CEFA,
	LIGHTSLATEGRAY = 0x778899, LIGHTSLATEGREY = 0x778899, LIGHTSTEELBLUE = 0xB0C4DE, LIGHTYELLOW = 0xFFFFE0, LIME = 0x00FF00, LIMEGREEN = 0x32CD32,
	LINEN = 0xFAF0E6, MAGENTA = 0xFF00FF, MAROON = 0x800000, MEDIUMAQUAMARINE = 0x66CDAA, MEDIUMBLUE = 0x0000CD, MEDIUMORCHID = 0xBA55D3,
	MEDIUMPURPLE = 0x9370DB, MEDIUMSEAGREEN = 0x3CB371, MEDIUMSLATEBLUE = 0x7B68EE, MEDIUMSPRINGGREEN = 0x00FA9A, MEDIUMTURQUOISE = 0x48D1CC, MEDIUMVIOLETRED = 0xC71585,
	MIDNIGHTBLUE = 0x191970, MINTCREAM = 0xF5FFFA, MISTYROSE = 0xFFE4E1, MOCCASIN = 0xFFE4B5, NAVAJOWHITE = 0xFFDEAD, NAVY = 0x000080,
	OLDLACE = 0xFDF5E6, OLIVE = 0x808000, OLIVEDRAB = 0x6B8E23, ORANGE = 0xFFA500, ORANGERED = 0xFF4500, ORCHID = 0xDA70D6,
	PALEGOLDENROD = 0xEEE8AA, PALEGREEN = 0x98FB98, PALETURQUOISE = 0xAFEEEE, PALEVIOLETRED = 0xDB7093, PAPAYAWHIP = 0xFFEFD5, PEACHPUFF = 0xFFDAB9,
	PERU = 0xCD853F, PINK = 0xFFC0CB, PLUM = 0xDDA0DD, POWDERBLUE = 0xB0E0E6, PURPLE = 0x800080, REBECCAPURPLE = 0x663399,
	RED = 0xFF0000, ROSYBROWN = 0xBC8F8F, ROYALBLUE = 0x4169E1, SADDLEBROWN = 0x8B4513, SALMON = 0xFA8072, SANDYBROWN = 0xF4A460,
	SEAGREEN = 0x2E8B57, SEASHELL = 0xFFF5EE, SIENNA = 0xA0522D, SILVER = 0xC0C0C0, SKYBLUE = 0x87CEEB, SLATEBLUE = 0x6A5ACD,
	SLATEGRAY = 0x708090, SLATEGREY = 0x708090, SNOW = 0xFFFAFA, SPRINGGREEN = 0x00FF7F, STEELBLUE = 0x4682B4, TAN = 0xD2B48C,
	TEAL = 0x008080, THISTLE = 0xD8BFD8, TOMATO = 0xFF6347, TURQUOISE = 0x40E0D0, VIOLET = 0xEE82EE, WHEAT = 0xF5DEB3,
	WHITE = 0xFFFFFF, WHITESMOKE = 0xF5F5F5, YELLOW = 0xFFFF00, YELLOWGREEN = 0x9ACD32,
};

// ================================== Vector Classes/Functions ==================================

#pragma region Vector & Matrix

//#define Inline inline
#define Inline __inline
//#define Inline __forceinline

#define PI 3.1415926535897932384626
#define mix(x,y,a) ((x)*(1.0-(a))+(y)*(a))
#define clamp(x,a,b) ((x)<(a)?(a):(x)>(b)?(b):(x))
Inline double mod(double x, double m) { return x - m * floor(x / m); }

class vec2 {
public:
	double x, y;
	Inline explicit vec2() {}
	Inline explicit vec2(const double &a) :x(a), y(a) {}
	Inline explicit vec2(const double &x, const double &y) :x(x), y(y) {}
	Inline vec2 operator - () const { return vec2(-x, -y); }
	Inline vec2 operator + (const vec2 &v) const { return vec2(x + v.x, y + v.y); }
	Inline vec2 operator - (const vec2 &v) const { return vec2(x - v.x, y - v.y); }
	Inline vec2 operator * (const vec2 &v) const { return vec2(x * v.x, y * v.y); }	// not standard
	Inline vec2 operator * (const double &a) const { return vec2(x*a, y*a); }
	Inline double sqr() const { return x * x + y * y; } 	// not standard
	Inline friend double length(const vec2 &v) { return sqrt(v.x*v.x + v.y*v.y); }
	Inline friend vec2 normalize(const vec2 &v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y)); }
	Inline friend double dot(const vec2 &u, const vec2 &v) { return u.x*v.x + u.y*v.y; }
	Inline friend double det(const vec2 &u, const vec2 &v) { return u.x*v.y - u.y*v.x; } 	// not standard
#if 1
	Inline void operator += (const vec2 &v) { x += v.x, y += v.y; }
	Inline void operator -= (const vec2 &v) { x -= v.x, y -= v.y; }
	Inline void operator *= (const vec2 &v) { x *= v.x, y *= v.y; }
	Inline friend vec2 operator * (const double &a, const vec2 &v) { return vec2(a*v.x, a*v.y); }
	Inline void operator *= (const double &a) { x *= a, y *= a; }
	Inline vec2 operator / (const double &a) const { return vec2(x / a, y / a); }
	Inline void operator /= (const double &a) { x /= a, y /= a; }
#endif
	Inline vec2 yx() const { return vec2(y, x); }
	Inline vec2 rot() const { return vec2(-y, x); }
	Inline vec2 rotr() const { return vec2(y, -x); }
#if 1
	// added when needed
	Inline bool operator == (const vec2 &v) const { return x == v.x && y == v.y; }
	Inline bool operator != (const vec2 &v) const { return x != v.x || y != v.y; }
	Inline vec2 operator / (const vec2 &v) const { return vec2(x / v.x, y / v.y); }
	Inline friend vec2 pMax(const vec2 &a, const vec2 &b) { return vec2(max(a.x, b.x), max(a.y, b.y)); }
	Inline friend vec2 pMin(const vec2 &a, const vec2 &b) { return vec2(min(a.x, b.x), min(a.y, b.y)); }
	Inline friend vec2 abs(const vec2 &a) { return vec2(abs(a.x), abs(a.y)); }
	Inline friend vec2 floor(const vec2 &a) { return vec2(floor(a.x), floor(a.y)); }
	Inline friend vec2 ceil(const vec2 &a) { return vec2(ceil(a.x), ceil(a.y)); }
	Inline friend vec2 sqrt(const vec2 &a) { return vec2(sqrt(a.x), sqrt(a.y)); }
	Inline friend vec2 sin(const vec2 &a) { return vec2(sin(a.x), sin(a.y)); }
	Inline friend vec2 cos(const vec2 &a) { return vec2(cos(a.x), cos(a.y)); }
	Inline friend vec2 atan(const vec2 &a) { return vec2(atan(a.x), atan(a.y)); }
#endif
};

class vec3 {
public:
	double x, y, z;
	Inline explicit vec3() {}
	Inline explicit vec3(const double &a) :x(a), y(a), z(a) {}
	Inline explicit vec3(const double &x, const double &y, const double &z) :x(x), y(y), z(z) {}
	Inline explicit vec3(const vec2 &v, const double &z) :x(v.x), y(v.y), z(z) {}
	Inline vec3 operator - () const { return vec3(-x, -y, -z); }
	Inline vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
	Inline vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
	Inline vec3 operator * (const vec3 &v) const { return vec3(x * v.x, y * v.y, z * v.z); }
	Inline vec3 operator * (const double &k) const { return vec3(k * x, k * y, k * z); }
	Inline double sqr() const { return x * x + y * y + z * z; } 	// non-standard
	Inline friend double length(vec3 v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
	Inline friend vec3 normalize(vec3 v) { return v * (1. / sqrt(v.x*v.x + v.y*v.y + v.z*v.z)); }
	Inline friend double dot(vec3 u, vec3 v) { return u.x*v.x + u.y*v.y + u.z*v.z; }
	Inline friend vec3 cross(vec3 u, vec3 v) { return vec3(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x); }
#if 1
	Inline void operator += (const vec3 &v) { x += v.x, y += v.y, z += v.z; }
	Inline void operator -= (const vec3 &v) { x -= v.x, y -= v.y, z -= v.z; }
	Inline void operator *= (const vec3 &v) { x *= v.x, y *= v.y, z *= v.z; }
	Inline friend vec3 operator * (const double &a, const vec3 &v) { return vec3(a*v.x, a*v.y, a*v.z); }
	Inline void operator *= (const double &a) { x *= a, y *= a, z *= a; }
	Inline vec3 operator / (const double &a) const { return vec3(x / a, y / a, z / a); }
	Inline void operator /= (const double &a) { x /= a, y /= a, z /= a; }
#endif
	Inline vec2 xy() const { return vec2(x, y); }
	//Inline vec2& xy() { return *(vec2*)this; }
	Inline vec2 xz() const { return vec2(x, z); }
	Inline vec2 yz() const { return vec2(y, z); }
#if 1
	Inline bool operator == (const vec3 &v) const { return x == v.x && y == v.y && z == v.z; }
	Inline bool operator != (const vec3 &v) const { return x != v.x || y != v.y || z != v.z; }
	Inline vec3 operator / (const vec3 &v) const { return vec3(x / v.x, y / v.y, z / v.z); }
	Inline friend vec3 pMax(const vec3 &a, const vec3 &b) { return vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }
	Inline friend vec3 pMin(const vec3 &a, const vec3 &b) { return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
	Inline friend vec3 abs(const vec3 &a) { return vec3(abs(a.x), abs(a.y), abs(a.z)); }
	Inline friend vec3 floor(const vec3 &a) { return vec3(floor(a.x), floor(a.y), floor(a.z)); }
	Inline friend vec3 ceil(const vec3 &a) { return vec3(ceil(a.x), ceil(a.y), ceil(a.z)); }
	Inline friend vec3 mod(const vec3 &a, double m) { return vec3(mod(a.x, m), mod(a.y, m), mod(a.z, m)); }
#endif
};

const vec3 vec0(0, 0, 0), veci(1, 0, 0), vecj(0, 1, 0), veck(0, 0, 1);
#define SCRCTR vec2(0.5*_WIN_W,0.5*_WIN_H)

// 4x4 matrix
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


#pragma endregion Add __inline so compiler will expand them in debug mode


#pragma region Ray Tracing Functions

// Many from https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
// /**/: check later
// All direction vectors should be normalized

// Intersection functions - return the distance, NAN means no intersection
double intHorizon(double z, vec3 p, vec3 d) {
	return (z - p.z) / d.z;
}
double intSphere(vec3 O, double r, vec3 p, vec3 d) {
	p = p - O;
#if 0
	if (dot(p, d) >= 0.0) return NAN;
	vec3 k = cross(p, d); double rd2 = dot(k, k); if (rd2 > r*r) return NAN;
	return sqrt(dot(p, p) - rd2) - sqrt(r*r - rd2);
#else
	// works when p is inside the sphere (and its slightly faster)
	double b = -dot(p, d), c = dot(p, p) - r * r;  // require d to be normalized
	double delta = b * b - c;
	if (delta < 0.0) return NAN;
	delta = sqrt(delta);
	c = b - delta;
	return c > 0. ? c : b + delta;  // usually we want it to be positive
#endif
}
double intTriangle(vec3 v0, vec3 v1, vec3 v2, vec3 ro, vec3 rd) {
	vec3 v1v0 = v1 - v0, v2v0 = v2 - v0, rov0 = ro - v0;
	vec3 n = cross(v1v0, v2v0);
	vec3 q = cross(rov0, rd);
	double d = 1.0 / dot(rd, n);
	double u = d * dot(-q, v2v0); if (u<0. || u>1.) return NAN;
	double v = d * dot(q, v1v0); if (v<0. || (u + v)>1.) return NAN;
	return d * dot(-n, rov0);
}
/**/double intCapsule(vec3 pa, vec3 pb, double r, vec3 ro, vec3 rd) {
	vec3 ba = pb - pa, oa = ro - pa;
	double baba = dot(ba, ba), bard = dot(ba, rd), baoa = dot(ba, oa), rdoa = dot(rd, oa), oaoa = dot(oa, oa);
	double a = baba - bard * bard, b = baba * rdoa - baoa * bard, c = baba * oaoa - baoa * baoa - r * r*baba;
	double h = b * b - a * c;
	if (h >= 0.0) {
		double t = (-b - sqrt(h)) / a;
		double y = baoa + t * bard;
		if (y > 0.0 && y < baba) return t;
		vec3 oc = (y <= 0.0) ? oa : ro - pb;
		b = dot(rd, oc), c = dot(oc, oc) - r * r, h = b * b - c;
		if (h > 0.0) return -b - sqrt(h);
	}
	return NAN;
}
/**/double intCylinder(vec3 pa, vec3 pb, double ra, vec3 ro, vec3 rd) {
	vec3 ca = pb - pa, oc = ro - pa;
	double caca = dot(ca, ca), card = dot(ca, rd), caoc = dot(ca, oc);
	double a = caca - card * card, b = caca * dot(oc, rd) - caoc * card, c = caca * dot(oc, oc) - caoc * caoc - ra * ra*caca;
	double h = b * b - a * c;
	if (h < 0.0) return NAN;
	h = sqrt(h);
	double t = (-b - h) / a;
	double y = caoc + t * card;
	if (y > 0.0 && y < caca) return t;
	t = (((y < 0.0) ? 0.0 : caca) - caoc) / card;
	if (abs(b + a * t) < h) return t;
	return NAN;
}
/**/double intBoxC(vec3 R, vec3 ro, vec3 rd) {
	vec3 m = vec3(1.0) / rd, n = m * ro;
	vec3 k = abs(m)*R;
	vec3 t1 = -n - k, t2 = -n + k;
	double tN = max(max(t1.x, t1.y), t1.z);
	double tF = min(min(t2.x, t2.y), t2.z);
	if (tN > tF || tF < 0.0) return NAN;
	return tN;
}
/**/double intCone(vec3 pa, vec3 pb, double r, vec3 ro, vec3 rd) {
	vec3 ba = pb - pa, oa = ro - pa, ob = ro - pb;
	double m0 = dot(ba, ba), m1 = dot(oa, ba), m2 = dot(ob, ba), m3 = dot(rd, ba);
	if (m1 < 0.0 && (oa*m3 - rd * m1).sqr() < (r*r*m3*m3)) return (-m1 / m3);
	double m4 = dot(rd, oa), m5 = dot(oa, oa);
	double rr = r, hy = m0 + rr * rr;
	double k2 = m0 * m0 - m3 * m3*hy;
	double k1 = m0 * m0*m4 - m1 * m3*hy + m0 * r*(rr*m3*1.0);
	double k0 = m0 * m0*m5 - m1 * m1*hy + m0 * r*(rr*m1*2.0 - m0 * r);
	double h = k1 * k1 - k2 * k0; if (h < 0.0) return NAN;
	double t = (-k1 - sqrt(h)) / k2;
	double y = m1 + t * m3; if (y > 0.0 && y < m0) return t;
	return NAN;
}
double intCircle(vec3 n, vec3 c, double r, vec3 ro, vec3 rd) {
	vec3 q = ro - c;
	double t = -dot(n, q) / dot(rd, n);
	q = q + rd * t;
	return dot(q, q) < r*r ? t : NAN;
}


// Normal calculation
vec3 nCapsule(vec3 a, vec3 b, double r, vec3 p) {
	vec3 ba = b - a, pa = p - a;
	double h = dot(pa, ba) / dot(ba, ba);
	return (pa - clamp(h, 0., 1.) * ba) / r;
}

// SDF functions
double sdBox(vec2 p, vec2 b) {
	vec2 d = abs(p) - b;
	return length(pMax(d, vec2(0))) + min(max(d.x, d.y), 0.0);
}
double sdBox(vec3 p, vec3 b) {
	vec3 q = abs(p) - b;
	return length(pMax(q, vec0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}
double sdCapsule(vec3 p, vec3 a, vec3 b, double r) {
	vec3 pa = p - a, ba = b - a;
	double h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
	return length(pa - ba * h) - r;
}
double sdEllipsoid(vec3 p, vec3 r) {
	double k0 = length(p / r);
	double k1 = length(p / (r*r));
	return k0 * (k0 - 1.0) / k1;
}
double sdCylinder(vec3 p, vec3 a, vec3 b, double r) {
	vec3  ba = b - a;
	vec3  pa = p - a;
	double baba = dot(ba, ba);
	double paba = dot(pa, ba);
	double x = length(pa*baba - ba * paba) - r * baba;
	double y = abs(paba - baba * 0.5) - baba * 0.5;
	double x2 = x * x;
	double y2 = y * y*baba;
	double d = (max(x, y) < 0.0) ? -min(x2, y2) : (((x > 0.0) ? x2 : 0.0) + ((y > 0.0) ? y2 : 0.0));
	return (d > 0 ? 1. : -1.)*sqrt(abs(d)) / baba;
}

double smin(double d1, double d2, double k) {
	double h = 0.5 + 0.5*(d2 - d1) / k; h = clamp(h, 0.0, 1.0);
	return mix(d2, d1, h) - k * h*(1.0 - h);
}
double smax(double d1, double d2, double k) {
	double h = 0.5 - 0.5*(d2 - d1) / k; h = clamp(h, 0.0, 1.0);
	return mix(d2, d1, h) + k * h*(1.0 - h);
}

// Bounding box calculation
void rangeSphere(vec3 c, double r, vec3 &Min, vec3 &Max) {
	Min = c - vec3(r), Max = c + vec3(r);
}
void rangeCapsule(vec3 pa, vec3 pb, double r, vec3 &Min, vec3 &Max) {
	Min = pMin(pa, pb) - vec3(r), Max = pMax(pa, pb) + vec3(r);
}

// closest point to a straight line
double closestPoint(vec3 P, vec3 d, vec3 ro, vec3 rd) {  // P+t*d, return t
	vec3 n = cross(rd, cross(rd, d));
	return dot(ro - P, n) / dot(d, n);
}

#pragma endregion Intersector, Normal, SDF, Bounding Box


// ======================================== Data / Parameters ========================================

// viewport
vec3 Center(0.0, 0.0, 0.0);  // view center in world coordinate
double rz = 0.2*PI, rx = 0.15*PI, ry = 0.0, dist = 12.0, Unit = 100.0;  // yaw, pitch, row, camera distance, scale to screen

#pragma region General Global Variables

// window parameters
char text[64];	// window title
Affine Tr;  // matrix
vec3 CamP, ScrO, ScrA, ScrB;  // camera and screen
auto scrDir = [](vec2 pixel) { return normalize(ScrO + (pixel.x / _WIN_W)*ScrA + (pixel.y / _WIN_H)*ScrB - CamP); };

// user parameters
vec2 Cursor = vec2(0, 0), clickCursor;  // current cursor and cursor position when mouse down
bool mouse_down = false;
bool Ctrl = false, Shift = false, Alt = false;  // these variables are shared by both windows

#pragma endregion Window, Camera/Screen, Mouse/Key

#pragma region Global Variable Related Functions

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

// these functions often need to handle perspective case
void projRange_Triangle(vec3 A, vec3 B, vec3 C, vec2 &p0, vec2 &p1) {
	vec2 p = (Tr*A).xy(); p0 = p1 = p;
	p = (Tr*B).xy(); p0 = pMin(p0, p), p1 = pMax(p1, p);
	p = (Tr*C).xy(); p0 = pMin(p0, p), p1 = pMax(p1, p);
}
void projRange_Circle(vec3 P, vec3 u, vec3 v, vec2 &p0, vec2 &p1) {  // any space curve defined by C(t)=P+u*cos(t)+v*sin(t)
	vec2 Ru(dot(Tr.u, u), dot(Tr.v, u)), Rv(dot(Tr.u, v), dot(Tr.v, v)), Rp(dot(Tr.u, P), dot(Tr.v, P));
	double Pu = dot(Tr.p, u), Pv = dot(Tr.p, v), Pp = dot(Tr.p, P);
	vec2 a = Rv * (Pp + Tr.s) - Pv * (Rp + Tr.t.xy()), b = -Ru * (Pp + Tr.s) + Pu * (Rp + Tr.t.xy()), c = Pv * Ru - Rv * Pu;
	vec2 d = sqrt(a * a + b * b - c * c);
	vec2 t0 = 2.0*atan((b + d) / (a + c)), t1 = 2.0*atan((b - d) / (a + c));
	p0.x = (Tr * (P + cos(t0.x)*u + sin(t0.x)*v)).x, p1.x = (Tr * (P + cos(t1.x)*u + sin(t1.x)*v)).x; if (p0.x > p1.x) std::swap(p0.x, p1.x);
	p0.y = (Tr * (P + cos(t0.y)*u + sin(t0.y)*v)).y, p1.y = (Tr * (P + cos(t1.y)*u + sin(t1.y)*v)).y; if (p0.y > p1.y) std::swap(p0.y, p1.y);
}
void projRange_Cylinder(vec3 A, vec3 B, double r, vec2 &p0, vec2 &p1) {
	vec3 d = B - A, u = r * normalize(cross(d, vec3(1.2345, 6.5432, -1.3579))), v = r * normalize(cross(u, d));
	projRange_Circle(A, u, v, p0, p1);
	vec2 q0, q1; projRange_Circle(B, u, v, q0, q1);
	p0 = pMin(p0, q0), p1 = pMax(p1, q1);
}
void projRange_Sphere(vec3 P, double r, vec2 &p0, vec2 &p1) {
	/*if (dot(Tr.p, P) + Tr.s < r * length(Tr.p)) {
		//if (dot(Tr.p, P) + Tr.s < -r * length(Tr.p)) {
		p0 = p1 = vec2(NAN); return;
	}*/
	vec3 O = ScrO - CamP, k = CamP - P;
	vec3 Ak = cross(ScrA, k), Bk = cross(ScrB, k), Ok = cross(O, k);
	double r2 = r * r;
	// A x² + B y² + C xy + D x + E y + F = 0
	double A = r2 * ScrA.sqr() - Ak.sqr();
	double B = r2 * ScrB.sqr() - Bk.sqr();
	double C = 2.0*(r2*dot(ScrA, ScrB) - dot(Ak, Bk));
	double D = 2.0*(r2*dot(ScrA, O) - dot(Ak, Ok));
	double E = 2.0*(r2*dot(ScrB, O) - dot(Bk, Ok));
	double F = r2 * O.sqr() - Ok.sqr();
	double a, b, c, delta, t0, t1;
	if (abs(C / F) < 1e-6) {  // not sure if I use the right formula
		a = 4 * A*B, b = 4 * A*E, c = 4 * A*F - D * D;
		delta = sqrt(b*b - 4 * a*c);
		t0 = (-b + delta) / (2.0*a), t1 = (-b - delta) / (2.0*a); if (t0 > t1) std::swap(t0, t1);
		p0.y = t0 * _WIN_H, p1.y = t1 * _WIN_H;
		a = 4 * A*B, b = 4 * B*D, c = 4 * B*F - E * E;
		delta = sqrt(b*b - 4 * a*c);
		t0 = (-b + delta) / (2.0*a), t1 = (-b - delta) / (2.0*a); if (t0 > t1) std::swap(t0, t1);
		p0.x = t0 * _WIN_W, p1.x = t1 * _WIN_W;
	}
	else {
		a = 4 * A*A*B - A * C*C, b = 4 * A*B*D - 2 * A*C*E, c = B * D*D - C * D*E + C * C*F;
		delta = sqrt(b*b - 4 * a*c);
		t0 = (-b + delta) / (2.0*a), t1 = (-b - delta) / (2.0*a);
		t0 = (-D - 2 * A*t0) / C, t1 = (-D - 2 * A*t1) / C; if (t0 > t1) std::swap(t0, t1);
		p0.y = t0 * _WIN_H, p1.y = t1 * _WIN_H;
		a = 4 * A*B*B - B * C*C, b = 4 * A*B*E - 2 * B*C*D, c = A * E*E - C * D*E + C * C*F;
		delta = sqrt(b*b - 4 * a*c);
		t0 = (-b + delta) / (2.0*a), t1 = (-b - delta) / (2.0*a);
		t0 = (-E - 2 * B*t0) / C, t1 = (-E - 2 * B*t1) / C; if (t0 > t1) std::swap(t0, t1);
		p0.x = t0 * _WIN_W, p1.x = t1 * _WIN_W;
	}
}
void projRange_Cone(vec3 A, vec3 B, double r, vec2 &p0, vec2 &p1) {
	vec3 d = B - A, u = r * normalize(cross(d, vec3(1.2345, 6.5432, -1.3579))), v = r * normalize(cross(u, d));
	projRange_Circle(A, u, v, p0, p1);
	vec2 q = (Tr*B).xy();
	p0 = pMin(p0, q), p1 = pMax(p1, q);
}

#pragma endregion Get Ray/Screen, projection




struct Triangle {
	vec3 n, A, B, C;
} *STL;
int STL_N;
vec3 COM;
void readBinarySTL(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	fseek(fp, 80, SEEK_SET);
	fread(&STL_N, sizeof(int), 1, fp);
	STL = new Triangle[STL_N];
	dbgprint("%d\n", STL_N);
	auto readf = [&](double &x) {
		float t; fread(&t, sizeof(float), 1, fp);
		x = (double)t;
	};
	auto readTrig = [&](Triangle &T) {
		readf(T.n.x); readf(T.n.y); readf(T.n.z);
		readf(T.A.x); readf(T.A.y); readf(T.A.z);
		readf(T.B.x); readf(T.B.y); readf(T.B.z);
		readf(T.C.x); readf(T.C.y); readf(T.C.z);
		short c; fread(&c, 2, 1, fp);
		vec3 n = normalize(cross(T.B - T.A, T.C - T.A));
		if (abs(dot(n, T.n) - 1.0) > 0.001) {
			dbgprint("Error!\n");
		}
		T.n = n;
	};
	for (int i = 0; i < STL_N; i++) {
		readTrig(STL[i]);
	}
	fclose(fp);
}
void writeSTL(const char* filename) {
	FILE* fp = fopen(filename, "wb");
	for (int i = 0; i < 80; i++) fputc(0, fp);
	fwrite(&STL_N, 4, 1, fp);
	auto writef = [&](double x) {
		float t = (float)x;
		fwrite(&t, sizeof(float), 1, fp);
	};
	for (int i = 0; i < STL_N; i++) {
		Triangle T = STL[i];
		writef(T.n.x); writef(T.n.y); writef(T.n.z);
		writef(T.A.x); writef(T.A.y); writef(T.A.z);
		writef(T.B.x); writef(T.B.y); writef(T.B.z);
		writef(T.C.x); writef(T.C.y); writef(T.C.z);
		fputc(0, fp); fputc(0, fp);
	}
	fclose(fp);
}
void BoundingBox(Triangle *P, int N, vec3 &p0, vec3 &p1) {
	p0 = vec3(INFINITY), p1 = vec3(-INFINITY);
	for (int i = 0; i < N; i++) {
		p0 = pMin(pMin(p0, P[i].A), pMin(P[i].B, P[i].C));
		p1 = pMax(pMax(p1, P[i].A), pMax(P[i].B, P[i].C));
	}
}
void CenterOfMass(Triangle *P, int N, vec3 &C, double &V) {  // center of mass and volume
	// Gauss formula, assume the triangles enclose the shape and the normals are outward
	V = 0.0; C = vec3(0.0);
	const double _3 = 1. / 3, _6 = 1. / 6, _12 = 1. / 12;
	for (int i = 0; i < N; i++) {
		vec3 a = P[i].A, b = P[i].B, c = P[i].C;
		double dV = _6 * dot(a, cross(b, c));
		V += dV, C += 0.25*dV*(a + b + c);
	}
	C /= V;
}



// ============================================ Rendering ============================================

#pragma region Rasterization functions

auto drawLine = [](vec2 p, vec2 q, COLORREF col) {
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
auto drawCross = [&](vec2 p, double r, COLORREF Color = WHITE) {
	drawLine(p - vec2(r, 0), p + vec2(r, 0), Color);
	drawLine(p - vec2(0, r), p + vec2(0, r), Color);
};
auto drawCircle = [&](vec2 c, double r, COLORREF Color) {
	int s = int(r / sqrt(2) + 0.5);
	int cx = (int)c.x, cy = (int)c.y;
	for (int i = 0, im = min(s, max(_WIN_W - cx, cx)) + 1; i < im; i++) {
		int u = sqrt(r*r - i * i) + 0.5;
		setColor(cx + i, cy + u, Color); setColor(cx + i, cy - u, Color); setColor(cx - i, cy + u, Color); setColor(cx - i, cy - u, Color);
		setColor(cx + u, cy + i, Color); setColor(cx + u, cy - i, Color); setColor(cx - u, cy + i, Color); setColor(cx - u, cy - i, Color);
	}
};
auto fillCircle = [&](vec2 c, double r, COLORREF Color) {
	int x0 = max(0, int(c.x - r)), x1 = min(_WIN_W - 1, int(c.x + r));
	int y0 = max(0, int(c.y - r)), y1 = min(_WIN_H - 1, int(c.y + r));
	int cx = (int)c.x, cy = (int)c.y, r2 = int(r*r);
	for (int x = x0, dx = x - cx; x <= x1; x++, dx++) {
		for (int y = y0, dy = y - cy; y <= y1; y++, dy++) {
			if (dx * dx + dy * dy < r2) Canvas(x, y) = Color;
		}
	}
};
auto drawTriangle = [](vec2 A, vec2 B, vec2 C, COLORREF col, bool stroke = false, COLORREF strokecol = WHITE) {
	int x0 = max((int)min(min(A.x, B.x), C.x), 0), x1 = min((int)max(max(A.x, B.x), C.x), _WIN_W - 1);
	int y0 = max((int)min(min(A.y, B.y), C.y), 0), y1 = min((int)max(max(A.y, B.y), C.y), _WIN_H - 1);
	for (int i = y0; i <= y1; i++) for (int j = x0; j <= x1; j++) {
		// the slow way
		vec2 P(j, i);
		if (((det(P - A, P - B) < 0) + (det(P - B, P - C) < 0) + (det(P - C, P - A) < 0)) % 3 == 0)
			Canvas(j, i) = col;
	}
	if (stroke) {
		drawLine(A, B, strokecol); drawLine(A, C, strokecol); drawLine(B, C, strokecol);
	}
};
auto drawBox = [](vec2 Min, vec2 Max, COLORREF col = RED) {
	drawLine(vec2(Min.x, Min.y), vec2(Max.x, Min.y), col);
	drawLine(vec2(Max.x, Min.y), vec2(Max.x, Max.y), col);
	drawLine(vec2(Max.x, Max.y), vec2(Min.x, Max.y), col);
	drawLine(vec2(Min.x, Max.y), vec2(Min.x, Min.y), col);
};
auto fillBox = [](vec2 Min, vec2 Max, COLORREF col = RED) {
	int x0 = max((int)Min.x, 0), x1 = min((int)Max.x, _WIN_W - 1);
	int y0 = max((int)Min.y, 0), y1 = min((int)Max.y, _WIN_H - 1);
	for (int x = x0; x <= x1; x++) for (int y = y0; y <= y1; y++) Canvas(x, y) = col;
};
auto drawSquare = [](vec2 C, double r, COLORREF col = ORANGE) {
	drawBox(C - vec2(r, r), C + vec2(r, r), col);
};
auto fillSquare = [](vec2 C, double r, COLORREF col = ORANGE) {
	fillBox(C - vec2(r, r), C + vec2(r, r), col);
};

auto drawLine_F = [](vec3 A, vec3 B, COLORREF col = WHITE) {
	double u = dot(Tr.p, A) + Tr.s, v = dot(Tr.p, B) + Tr.s;
	if (u > 0 && v > 0) { drawLine((Tr*A).xy(), (Tr*B).xy(), col); return; }
	if (u < 0 && v < 0) return;
	if (u < v) std::swap(A, B), std::swap(u, v);
	double t = u / (u - v) - 1e-4;
	B = A + (B - A)*t;
	drawLine((Tr*A).xy(), (Tr*B).xy(), col);
};
auto drawTriangle_F = [](vec3 A, vec3 B, vec3 C, COLORREF col) {
	double u = dot(Tr.p, A) + Tr.s, v = dot(Tr.p, B) + Tr.s, w = dot(Tr.p, C) + Tr.s;
	if (u > 0 && v > 0 && w > 0) { drawTriangle((Tr*A).xy(), (Tr*B).xy(), (Tr*C).xy(), col); return; }
	if (u < 0 && v < 0 && w < 0) return;
	// debug
};
auto drawCross3D = [&](vec3 P, double r, COLORREF col = WHITE) {
	r /= Unit;
	drawLine_F(P - vec3(r, 0, 0), P + vec3(r, 0, 0), col);
	drawLine_F(P - vec3(0, r, 0), P + vec3(0, r, 0), col);
	drawLine_F(P - vec3(0, 0, r), P + vec3(0, 0, r), col);
};


auto drawTriangle_ZB = [](vec3 A, vec3 B, vec3 C, COLORREF col) {
	A = Tr * A, B = Tr * B, C = Tr * C;
	vec2 a = A.xy(), b = B.xy(), c = C.xy();
	vec2 e1 = b - a, e2 = c - a; double m = 1.0 / det(e1, e2);
	int x0 = max((int)min(min(A.x, B.x), C.x), 0), x1 = min((int)max(max(A.x, B.x), C.x), _WIN_W - 1);
	int y0 = max((int)min(min(A.y, B.y), C.y), 0), y1 = min((int)max(max(A.y, B.y), C.y), _WIN_H - 1);
	for (int i = x0; i <= x1; i++) for (int j = y0; j <= y1; j++) {
		vec2 p(i, j);
		//vec2 a1 = p - a, b1 = p - b, c1 = p - c; if (((det(a1, b1) < 0) + (det(b1, c1) < 0) + (det(c1, a1) < 0)) % 3 == 0) {
		if (((det(p - a, p - b) < 0) + (det(p - b, p - c) < 0) + (det(p - c, p - a) < 0)) % 3 == 0) {
			p -= a;
			double u = m * det(p, e2), v = m * det(e1, p);
			double z = (1. - u - v)*A.z + u * B.z + v * C.z;
			if (z < _DEPTHBUF[i][j]) {
				Canvas(i, j) = col;
				_DEPTHBUF[i][j] = z;
			}
		}
	}
};

// simply fill color, no normal calculation
auto drawRod_RT = [](vec3 A, vec3 B, double r, COLORREF col) {
	vec2 p0, p1; projRange_Cylinder(A, B, r, p0, p1);
	int x0 = max((int)p0.x, 0), x1 = min((int)p1.x, _WIN_W - 1), y0 = max((int)p0.y, 0), y1 = min((int)p1.y, _WIN_H - 1);
	double t; for (int i = x0; i <= x1; i++) for (int j = y0; j <= y1; j++)
		if ((t = intCylinder(A, B, r, CamP, scrDir(vec2(i, j)))) > 0 && t < _DEPTHBUF[i][j]) Canvas(i, j) = col, _DEPTHBUF[i][j] = t;
};
auto drawSphere_RT = [](vec3 P, double r, COLORREF col) {
	vec2 p0, p1; projRange_Sphere(P, r, p0, p1);
	int x0 = max((int)p0.x, 0), x1 = min((int)p1.x, _WIN_W - 1), y0 = max((int)p0.y, 0), y1 = min((int)p1.y, _WIN_H - 1);
	double t; for (int i = x0; i <= x1; i++) for (int j = y0; j <= y1; j++)
		if ((t = intSphere(P, r, CamP, scrDir(vec2(i, j)))) > 0 && t < _DEPTHBUF[i][j]) Canvas(i, j) = col, _DEPTHBUF[i][j] = t;
};
auto drawTriangle_RT = [](vec3 A, vec3 B, vec3 C, COLORREF col) {
	vec2 p0, p1; projRange_Triangle(A, B, C, p0, p1);
	int x0 = max((int)p0.x, 0), x1 = min((int)p1.x, _WIN_W - 1), y0 = max((int)p0.y, 0), y1 = min((int)p1.y, _WIN_H - 1);
	double t; for (int i = x0; i <= x1; i++) for (int j = y0; j <= y1; j++)
		if ((t = intTriangle(A, B, C, CamP, scrDir(vec2(i, j)))) > 0 && t < _DEPTHBUF[i][j]) Canvas(i, j) = col, _DEPTHBUF[i][j] = t;
	// can be accelerated by precomputing the triangle edges but hope the compiler already helps me doing that
};
auto drawArrow_RT = [](vec3 A, vec3 B, double r, COLORREF col) {
	vec2 p0, p1; projRange_Cone(A, B, r, p0, p1);
	int x0 = max((int)p0.x, 0), x1 = min((int)p1.x, _WIN_W - 1), y0 = max((int)p0.y, 0), y1 = min((int)p1.y, _WIN_H - 1);
	double t; for (int i = x0; i <= x1; i++) for (int j = y0; j <= y1; j++)
		if ((t = intCone(A, B, r, CamP, scrDir(vec2(i, j)))) > 0 && t < _DEPTHBUF[i][j]) Canvas(i, j) = col, _DEPTHBUF[i][j] = t;
};
auto drawCircle_RT = [](vec3 C, vec3 n, double r, COLORREF col) {
	vec3 u = r * normalize(cross(n, vec3(1.2345, 6.5432, -1.3579))), v = cross(u, normalize(n));
	vec2 p0, p1; projRange_Circle(C, u, v, p0, p1);
	int x0 = max((int)p0.x, 0), x1 = min((int)p1.x, _WIN_W - 1), y0 = max((int)p0.y, 0), y1 = min((int)p1.y, _WIN_H - 1);
	double t; for (int i = x0; i <= x1; i++) for (int j = y0; j <= y1; j++)
		if ((t = intCircle(n, C, r, CamP, scrDir(vec2(i, j)))) > 0 && t < _DEPTHBUF[i][j]) Canvas(i, j) = col, _DEPTHBUF[i][j] = t;
	// n is already here so recalculation in the intersection function is unnecessary
};
auto drawVector_RT = [](vec3 P, vec3 d, double r, COLORREF col, bool relative = false) {
	double SC = relative ? dot(P + .5*d, Tr.p) + Tr.s : 1.0;
	drawRod_RT(P, P + d, r * SC, col);
	if (relative) SC = dot(P + d, Tr.p) + Tr.s;
	drawArrow_RT(P + d, P + ((3.5*r*SC + 1.)*d), 2.*r *SC, col);
};

#pragma endregion


#include <chrono>
typedef std::chrono::high_resolution_clock NTime;
typedef std::chrono::duration<double> fsec;


COLORREF color(vec3 p, vec3 n) {
	//return n;
	const vec3 light = normalize(vec3(0.5, 0.5, 1));
	vec3 d = normalize(p - CamP); d -= 2.0*dot(d, n)*n;
	vec3 bkg = vec3(0.2) + 0.2*n;
	vec3 dif = vec3(0.5) * max(dot(n, light), 0.0);
	vec3 spc = vec3(0.1) * pow(max(dot(d, light), 0.0), 5.0);
	vec3 col = bkg + dif + spc;
	COLORREF C; byte* c = (byte*)&C;
	c[2] = (byte)(255 * clamp(col.x, 0., 1.));
	c[1] = (byte)(255 * clamp(col.y, 0., 1.));
	c[0] = (byte)(255 * clamp(col.z, 0., 1.));
	return C;
}

// brute-force rasterization
void render_Raster_BF() {
	for (int i = 0; i < STL_N; i++) {
		drawTriangle_ZB(STL[i].A, STL[i].B, STL[i].C, color(STL[i].A, STL[i].n));
	}
}

// brute-force ray tracing
void render_RT_BF() {
	for (int i = 0; i < STL_N; i++) {
		drawTriangle_RT(STL[i].A, STL[i].B, STL[i].C, color(STL[i].A, STL[i].n));
	}
}

// BVH ray tracing
#include <vector>
struct BVH {
	Triangle *Obj = 0;
	BVH *b1 = 0, *b2 = 0;  // children
	vec3 C, R;  // bounding box
} *BVH_R = 0;
void constructBVH(BVH* &R, const std::vector<Triangle*> &T) {  // R should not be null and T should not be empty
	vec3 Min(INFINITY), Max(-INFINITY);
	for (int i = 0, n = T.size(); i < n; i++) {
		Min = pMin(pMin(Min, T[i]->A), pMin(T[i]->B, T[i]->C));
		Max = pMax(pMax(Max, T[i]->A), pMax(T[i]->B, T[i]->C));
	}
	R->C = 0.5*(Max + Min), R->R = 0.5*(Max - Min);
	if (T.size() == 1) {
		R->Obj = T[0];
		return;
	}
	Min = vec3(INFINITY), Max = vec3(-INFINITY);
	for (int i = 0, n = T.size(); i < n; i++) {
		vec3 C = T[i]->A + T[i]->B + T[i]->C;
		Min = pMin(Min, C), Max = pMax(Max, C);
	}
	Max /= 3., Min /= 3.;
	vec3 dP = Max - Min;
	std::vector<Triangle*> c1, c2;
	if (dP.x >= dP.y && dP.x >= dP.z) {
		double x = 0.5*(Min.x + Max.x);
		for (int i = 0, n = T.size(); i < n; i++) {
			if (T[i]->A.x + T[i]->B.x + T[i]->C.x < 3.*x) c1.push_back(T[i]);
			else c2.push_back(T[i]);
		}
	}
	else if (dP.y >= dP.x && dP.y >= dP.z) {
		double y = 0.5*(Min.y + Max.y);
		for (int i = 0, n = T.size(); i < n; i++) {
			if (T[i]->A.y + T[i]->B.y + T[i]->C.y < 3.*y) c1.push_back(T[i]);
			else c2.push_back(T[i]);
		}
	}
	else if (dP.z >= dP.x && dP.z >= dP.y) {
		double z = 0.5*(Min.z + Max.z);
		for (int i = 0, n = T.size(); i < n; i++) {
			if (T[i]->A.z + T[i]->B.z + T[i]->C.z < 3.*z) c1.push_back(T[i]);
			else c2.push_back(T[i]);
		}
	}
	if (c1.empty() || c2.empty()) {
		throw("ERROR: Empty BVH.\n");
	}
	R->b1 = new BVH, constructBVH(R->b1, c1);
	R->b2 = new BVH, constructBVH(R->b2, c2);
}
void rayIntersectBVH(const BVH* R, const vec3 &ro, const vec3 &rd, double &mt, Triangle* &obj) {  // assume ray already intersects current BVH
	const double eps = 1e-6;
	if (R->Obj) {
		Triangle *T = R->Obj;
		double t = intTriangle(T->A, T->B, T->C, ro, rd);
		if (t > eps && t < mt) mt = t, obj = T;
	}
	else {
		double t = intBoxC(R->b1->R, ro - R->b1->C, rd);
		if (t > eps && t < mt) rayIntersectBVH(R->b1, ro, rd, mt, obj);
		t = intBoxC(R->b2->R, ro - R->b2->C, rd);
		if (t > eps && t < mt) rayIntersectBVH(R->b2, ro, rd, mt, obj);
	}
}
void render_RT_BVH() {
	if (!BVH_R) {
		auto t0 = NTime::now();
		std::vector<Triangle*> T;
		for (int i = 0; i < STL_N; i++) T.push_back(&STL[i]);
		BVH_R = new BVH;
		constructBVH(BVH_R, T);
		dbgprint("BVH constructed in %lfs\n", fsec(NTime::now() - t0).count());
	}
	for (int i = 0; i < _WIN_W; i++) for (int j = 0; j < _WIN_H; j++) {
		vec3 d = scrDir(vec2(i, j));
		double mt = intBoxC(BVH_R->R, CamP - BVH_R->C, d);
		if (mt > 0.) {
			Triangle* obj = 0;
			mt = INFINITY;
			rayIntersectBVH(BVH_R, CamP, d, mt, obj);
			if (obj) Canvas(i, j) = color(CamP + mt*d, obj->n);
		}
	}
}


void render() {
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
	}

	render_RT_BVH();
	//render_Raster_BF();

	drawCross3D(COM, 6, RED);

	double t = fsec(NTime::now() - t0).count();
	sprintf(text, "[%d×%d]  %.1fms (%.1ffps)\n", _WIN_W, _WIN_H, 1000.0*t, 1. / t);
	SetWindowTextA(_HWND, text);
}


// ============================================== User ==============================================


void Init() {
	//readBinarySTL("D:\\3D Models\\Blender_Cube.stl");  // 12
	//readBinarySTL("D:\\3D Models\\Utah_Teapot.stl");  // 9438
	//readBinarySTL("D:\\3D Models\\Blender_Suzanne.stl");  // 62976
	//readBinarySTL("D:\\3D Models\\Stanford_Bunny.stl");  // 112402
	readBinarySTL("D:\\3D Models\\The_Thinker.stl");  // 837482
	//readBinarySTL("D:\\3D Models\\Stanford_Dragon.stl");  // 871414
	//readBinarySTL("D:\\3D Models\\Blender_Pipe.stl");  // 3145728, 8 subdivs
	//readBinarySTL("D:\\3D Models\\Stanford_Lucy.stl");  // 28055742, std::bad_alloc
	vec3 p0, p1; BoundingBox(STL, STL_N, p0, p1);
	vec3 c = 0.5*(p1 + p0), b = 0.5*(p1 - p0);
	double r = cbrt(b.x*b.y*b.z);
	Center = vec3(vec2(0.0), b.z / r);
	vec3 t = vec3(-c.x, -c.y, -p0.z);
	auto T = [&](vec3 &p) { p = (p + t) / r; };
	for (int i = 0; i < STL_N; i++) T(STL[i].A), T(STL[i].B), T(STL[i].C);
	CenterOfMass(STL, STL_N, COM, r);
}


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
	if (Shift) {
		double s = exp(-0.001*_DELTA);
		dist *= s;
	}
	else {
		// zoom
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

	// drag to rotate scene
	if (mouse_down) {
		if (Shift) {
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
}
void KeyDown(WPARAM _KEY) {
	keyDownShared(_KEY);
}
void KeyUp(WPARAM _KEY) {
	keyUpShared(_KEY);
}

