#pragma once
#include <math.h>
#include<d3dx9math.h>
#include <cereal/cereal.hpp>
class Vector2
{
public:
	float x = 0.0f;
	float y = 0.0f;

public:
	Vector2() {}
	Vector2(float x, float y) :x(x), y(y) {}
	
	Vector2 operator+ (Vector2 vec) { return Vector2(x + vec.x, y + vec.y); }
	Vector2 operator- (Vector2 vec) { return Vector2(x - vec.x, y - vec.y); }

	float Length(){return sqrtf(x * x + y * y);}
	float LengthSpr(){return (x * x + y * y);}
};

class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

public:
	//コンストラクタ
	Vector3() {}
	Vector3(float x, float y, float z) :x(x), y(y), z(z){}
	Vector3(D3DXVECTOR3 vec):x(vec.x),y(vec.y),z(vec.z){}

    D3DXVECTOR3 dx()
    {
        D3DXVECTOR3 vec;
        vec.x = x;
        vec.y = y;
        vec.z = z;
        return vec;
    }
	//関数
	//ベクトルの長さを返す
	float Length(){return sqrtf(x * x + y * y + z * z);}
	float LengthSpr(){return (x * x + y * y + z * z);}

	//ベクトルを正規化した値を返す
	Vector3 Normaliz()
	{if (Length() != 0) return Vector3(x / Length(), y / Length(), z / Length());}
	//ベクトルを正規化する
	void NormalizThis()
	{
		if (LengthSpr() != 0)
		{
			x = x / Length();
			y = y / Length();
			z = z / Length();
		}
	}

	//world座標を返す
	static Vector3 Right()   { return Vector3(1.0f, 0.0f, 0.0f); }//x
	static Vector3 Forward() { return Vector3(0.0f, 0.0f, 1.0f); }//z
	static Vector3 Up()	  { return Vector3(0.0f, 1.0f, 0.0f); }//y
	static Vector3 Zero()    { return Vector3(0.0f, 0.0f, 0.0f); }
	static Vector3 One()	  { return Vector3(1.0f, 1.0f, 1.0f); }

	//以下オペレーター
	Vector3 operator- (void) { return Vector3(-x, -y, -z); }

	Vector3 operator+ (Vector3 v){return Vector3(v.x + x, v.y + y, v.z + z);}
	Vector3 operator- (Vector3 v){return  Vector3(x - v.x, y - v.y, z - v.z);}
	Vector3 operator* (Vector3 v){return Vector3(v.x * x, v.y * y, v.z * z);}
	Vector3 operator/ (Vector3 v){ return Vector3(v.x / x, v.y / y, v.z / z); }
	
	Vector3 operator+ (float a) { return Vector3(a + x, a + y, a + z); }
	Vector3 operator- (float a) { return Vector3(a - x, a - y, a - z); }
	Vector3 operator* (float a) { return Vector3(a * x, a * y, a * z); }
	Vector3 operator/ (float a) { return Vector3(a / x, a / y, a / z); }

	
	void operator+= (Vector3 v) { x += v.x, y += v.y, z += v.z; }
	void operator-= (Vector3 v) { x -= v.x, y -= v.y, z -= v.z; }
	void operator*= (Vector3 v) { x *= v.x, y *= v.y, z *= v.z; }
	void operator/= (Vector3 v) { x /= v.x, y /= v.y, z /= v.z; }

	bool operator> (float a)
	{
		if (LengthSpr() > a * a)
			return true;

		return false;
	}
	bool operator< (float a)
	{
		if (LengthSpr() < a * a)
			return true;

		return false;
	}
	bool operator== (float a)
	{
		if (LengthSpr() == a * a)
			return true;

		return false;
	}
	bool operator!= (float a)
	{
		if (LengthSpr() != a * a)
			return true;

		return false;
	}

    operator float* (){
        return &x;
    }


    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z));
    }
};

class Color
{
private:
	Color();
public:
	float r;
	float g;
	float b;
	float a;
public:
	Color(float r, float g, float b, float a) :r(r), g(g), b(b), a(a) {}
};
