#pragma once
#include <math.h>

// �^�����m��ł��g�������
template<typename T>
struct Vector2D
{
	Vector2D() : x(0), y(0) {}
	Vector2D(T inx, T iny) : x(inx), y(iny) {}
	T x, y;

	// ���Z(�񎟌�)
	void operator+= (const Vector2D<T> in)
	{
		x += in.x;
		y += in.y;
	}

	// ���Z(�񎟌�)
	void operator-= (const Vector2D<T> in)
	{
		x -= in.x;
		y -= in.y;
	}

	// ��Z(�񎟌�)
	void operator*= (const Vector2D<T> in)
	{
		x *= in.x;
		y *= in.y;
	}

	// ���Z(�񎟌�)
	void operator/= (const Vector2D<T> in)
	{
		x /= in.x;
		y /= in.y;
	}

	// �������̒l��Ԃ�(Vector2)
	float Magnitude()
	{
		return hypotf(x, y);
	}

	// �l�𐳋K������(Vector2)
	void Normalize()
	{
		float mag = Magnitude();
		x /= mag;
		y /= mag;
	}

	// ���K���ς݂̒l��Ԃ�(Vector2)
	Vector2D<T> Normalized()
	{
		float mag = Magnitude();
		return (x / mag, y / mag);
	}
};

// Vector2 + Vector2
template<typename T>
Vector2D<T> operator+(const Vector2D<T>& va, const Vector2D<T>& vb)
{
	return Vector2D<T>(va.x + vb.x, va.y + vb.y);
}

// Vector2 - Vector2
template<typename T>
Vector2D<T> operator-(const Vector2D<T>& va, const Vector2D<T>& vb)
{
	return Vector2D<T>(va.x - vb.x, va.y - vb.y);
}

// Vector2 * float
template<typename T>
Vector2D<T> operator*(const Vector2D<T>& va, const float k)
{
	return Vector2D<T>(va.x * k, va.y * k);
}

// Vector2 * float
template<typename T>
Vector2D<T> operator/(const Vector2D<T> va, const float k)
{
	return Vector2D<T>(va.x / k, va.y / k);
}

// ���ς̌v�Z(Vector2)
template<typename T>
T Dot(const Vector2D<T>& va, const Vector2D<T>& vb)
{
	return T(va.x * vb.x + va.y * vb.y);
}

// �O�ς̌v�Z(Vector2)
template<typename T>
T Cross(const Vector2D<T>& va, const Vector2D<T>& vb)
{
	return T(va.x * vb.y - va.y * vb.x);
}

typedef Vector2D<int> Vector2;
typedef Vector2 Position2;

typedef Vector2D<float> Vector2f;
typedef Vector2f Position2f;

template<typename T>
struct Vector3D
{
	Vector3D() : x(0), y(0), z(0) {}
	Vector3D(T inx, T iny, T inz) : x(inx), y(iny), z(inz) {}
	T x, y, z;

	// ���Z(�O����)
	void operator+= (const Vector3D<T> in)
	{
		x += in.x;
		y += in.y;
		z += in.z;
	}

	// ���Z(�O����)
	void operator-= (const Vector3D<T> in)
	{
		x -= in.x;
		y -= in.y;
		z -= in.z;
	}

	// ��Z(�O����)
	void operator*= (const Vector3D<T> in)
	{
		x *= in.x;
		y *= in.y;
		z *= in.z;
	}

	// ���Z(�O����)
	void operator/= (const Vector3D<T> in)
	{
		x /= in.x;
		y /= in.y;
		z /= in.z;
	}

	// �������̒l��Ԃ�(Vector3)
	float Magnitude()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	// �l�𐳋K������(Vector3)
	void Normalize()
	{
		float mag = Magnitude();

		x /= mag;
		y /= mag;
		z /= mag;
	}

	// ���K���ς݂̒l��Ԃ�(Vector3)
	Vector3D<T> Normalized()
	{
		float mag = Magnitude();
		return (x / mag, y / mag, z / mag);
	}

};

// Vector3 + Vector3
template <typename T>
Vector3D <T> operator+(const Vector3D<T>& va, const Vector3D<T>& vb)
{
	return (va.x + vb.x, va.y + vb.y, va.z + vb.z);
}

// Vector3 - Vector3
template <typename T>
Vector3D<T> operator-(const Vector3D<T>& va, const Vector3D<T>& vb)
{
	return (va.x - vb.x, va.y - vb.y, va.z - vb.z);
}

// Vector3 * float
template <typename T>
Vector3D<T> operator*(const Vector3D<T>& va, const float k)
{
	return (va.x * k, va.y * k, va.z * k);
}

// Vector3 / float
template <typename T>
Vector3D<T> operator/(const  Vector3D<T> va, const float k)
{
	return (va.x / k, va.y / k, va.z / k);
}

// ���ς̌v�Z(Vector3)
template <typename T>
T Dot(Vector3D<T>& va, Vector3D<T>& vb)
{
	return T((va.x * vb.x) + (va.y * vb.y) + (va.z * vb.z));
}

// �O�ς̌v�Z(Vector3)
template<typename T>
Vector3D<T> Cross(const Vector3D<T> va, const Vector3D<T> vb)
{
	return Vector3D<T>(va.y * vb.z - va.z * vb.y,
					   va.z * vb.x - va.x * vb.z,
					   va.x * vb.y - va.y * vb.x);
}

typedef Vector3D<int> Vector3;
typedef Vector3 Position3;

typedef Vector3D<float> Vector3f;
typedef Vector3f Position3f;

struct Size
{
	Size();
	Size(int inx, int iny);

	int width;
	int height;
};

struct Rect
{
	Vector2 center;
	Size size;
	Rect();
	Rect(int x, int y, int w, int h);
	Rect(Vector2 & p, Size & s);

	const int Left() const;
	const int Right() const;
	const int Top() const;
	const int Bottom() const;
	const int Width() const;
	const int Height() const;
};


class Geometry
{
public:
	Geometry();
	~Geometry();
};

