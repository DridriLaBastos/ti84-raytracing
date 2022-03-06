#include <stdlib.h>
#include <stdint.h>

#ifdef DEBUG
#include <cstdio>
#endif

constexpr unsigned int SCREEN_WIDTH		= 320;
constexpr unsigned int SCREEN_HEIGHT	= 240;

union VRAMPixel
{
	uint16_t v;
	struct
	{
		uint16_t b:5,g:6,r:5;
	};
};

/* 24 bits fixed point real number
format = 16bits.8bits */
struct f24
{
	template <typename T>
	constexpr f24(const T x): v(x << 8){}
	constexpr f24(const float x): v(x * 256.f){}
	constexpr f24(const double x): v(x * 256.0){}

	template<typename T>
	constexpr operator T (void) const { return v >> 8; }
	constexpr operator float (void) const { return (float)v / 256.f; }
	constexpr operator double (void) const { return (double)v / 256.0; }

	template <typename T>
	constexpr f24& operator= (const T x) { v = x << 8; return *this; }
	constexpr f24& operator= (const float x) { v = x * 256.f; return *this; }
	constexpr f24& operator= (const double x) { v = x * 256.0; return *this; }

	template <typename T> constexpr f24& operator+= (const T& x) { v+=((f24)x).v; return *this; }
	template <typename T> constexpr f24& operator-= (const T& x) { v-=((f24)x).v; return *this; }
	template <typename T> constexpr f24& operator*= (const T& x) { v*=((f24)x).v; return *this; }
	template <typename T> constexpr f24& operator/= (const T& x) { v/=((f24)x).v; return *this; }

	private:
		uint24_t v;
};

template <typename T> constexpr f24 operator+ (const f24& f, const T& x) { f24 tmp = f; return tmp += x; }
template <typename T> constexpr f24 operator- (const f24& f, const T& x) { f24 tmp = f; return tmp -= x; }
template <typename T> constexpr f24 operator* (const f24& f, const T& x) { f24 tmp = f; return tmp *= x; }
template <typename T> constexpr f24 operator/ (const f24& f, const T& x) { f24 tmp = f; return tmp /= x; }

struct vec3
{
	union
	{
		struct { f24 x, y, z; };
		struct { f24 r, g, b; };
	};

	constexpr vec3(const f24& v1, const f24& v2, const f24& v3): x(v1), y(v2), z(v3) {}

	template<typename T> constexpr vec3& operator+= (const T& v) { x += v; y += v; z += v; return *this; }
	template<typename T> constexpr vec3& operator-= (const T& v) { x -= v; y -= v; z -= v; return *this; }
	template<typename T> constexpr vec3& operator*= (const T& v) { x *= v; y *= v; z *= v; return *this; }
	template<typename T> constexpr vec3& operator/= (const T& v) { x /= v; y /= v; z /= v; return *this; }

	f24 sqrdLength (void) const { return x*x + y*y + z*z; }

	static f24 dot (const vec3&& u, const vec3&& v) { return u.x * v.x + u.y * v.y + u.z * v.z; }
	static vec3 cross (const vec3&& u, const vec3 && v) { return { u.y * v.z - u.z * u.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x }; }
};

template<typename T> constexpr vec3 operator+ (const vec3& f, const T& x) { vec3 tmp = f; return tmp += x; }
template<typename T> constexpr vec3 operator- (const vec3& f, const T& x) { vec3 tmp = f; return tmp -= x; }
template<typename T> constexpr vec3 operator* (const vec3& f, const T& x) { vec3 tmp = f; return tmp *= x; }
template<typename T> constexpr vec3 operator/ (const vec3& f, const T& x) { vec3 tmp = f; return tmp /= x; }

using colour = vec3;

static volatile VRAMPixel* VRAM = (VRAMPixel*)0xD40000;

template <int RED = 31, int GREEN = 63, int BLUE = 31>
__attribute__((always_inline)) constexpr void writePixel (const colour& c, const size_t index)
{
	VRAM[index].r = c.r * RED; 
	VRAM[index].g = c.g * GREEN; 
	VRAM[index].b = c.r * BLUE; 
}

//Called from main function from assebly file.
//From this point, the rest of the program supposes that the CPU is running in APL mode
int main (void)
{
	for (size_t y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (size_t x = 0; x < SCREEN_WIDTH; ++x)
		{
			const f24 rFactor = (f24)y / (SCREEN_HEIGHT - 1);
			const f24 gFactor = (f24)x / (SCREEN_WIDTH - 1);
			const f24 bFactor = 0;

			writePixel({rFactor, gFactor, bFactor}, y * SCREEN_WIDTH + x);
		}
	}
	return 1;
}
