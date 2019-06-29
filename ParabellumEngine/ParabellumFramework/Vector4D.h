#ifndef _VECTROR4D_H_
#define _VECTROR4D_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "MemorySettings.h"
#include "SystemSettings.h"
#include "Convenctions.h"

#include "TypesC.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include "MathHelper.h"



#define VECTOR4D_Set(Vector4D_ref, x, y, z, w) Vector4D_ref.X = x; Vector4D_ref.Y = y; Vector4D_ref.Z = z; Vector4D_ref.W = w;


namespace ParabellumFramework
{

	//
	// Vector4D - supports SIMD
	//
	class XYZ_API PF_ALLIGN(16) Vector4D
	{
	public:
		union
		{
			struct
			{
				EFLOAT32 X;
				EFLOAT32 Y;
				EFLOAT32 Z;
				EFLOAT32 W;
			};

			struct
			{
				__m128 m128;
			};

			// 16 Bytes variable - for shaders parametrs purpose
			struct
			{
				EFLOAT32 data[4];
			} DATA;


			struct
			{
				Vector3D XYZ;
				EFLOAT32 W;
			};

			struct
			{
				EFLOAT32 X;
				Vector3D YZW;	
			};

			struct
			{
				Vector2D XY;
				Vector2D ZW;
			};

			struct
			{
				Vector2D C0;
				Vector2D C1;
			};
		};


	public:

		Vector4D();
		Vector4D(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 pw);
		Vector4D(const Vector4D& copy);
		~Vector4D();

		// get the sum of all 4 components
		PF_INLINE EFLOAT32 Sum();

		PF_INLINE static Vector4D* PF_FASTCALL CreateVector4D(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 pw);

		PF_INLINE EFLOAT32 Length();

		PF_INLINE void Normalize();
		PF_INLINE static void PF_FASTCALL Normalize(_IO_ Vector4D* v1);


		PF_INLINE static EFLOAT32 PF_FASTCALL Angle(Vector4D* v1, Vector4D* v2);
		PF_INLINE static EFLOAT32 PF_FASTCALL Dot(_IN_ Vector4D* v1, _IN_ Vector4D* v2);

		PF_INLINE void PF_FASTCALL Cross(_IN_ Vector4D* in1, _IN_ Vector4D* in2);
		PF_INLINE static void PF_FASTCALL Cross(_IN_ Vector4D* in1, _IN_ Vector4D* in2, _OUT_ Vector4D* out);

		PF_INLINE static void PF_FASTCALL Min(_IN_ Vector4D* in1, _IN_ Vector4D* in2, _OUT_ Vector4D* out);
		PF_INLINE static void PF_FASTCALL Max(_IN_ Vector4D* in1, _IN_ Vector4D* in2, _OUT_ Vector4D* out);

		// common vectors
		PF_INLINE void MakeOne();
		PF_INLINE void MakeZero();
		PF_INLINE void MakeUp();
		PF_INLINE void MakeDown();
		PF_INLINE void MakeLeft();
		PF_INLINE void MakeRight();
		PF_INLINE void MakeBackward();
		PF_INLINE void MakeForward();
		PF_INLINE void MakeXUnit();
		PF_INLINE void MakeYUnit();
		PF_INLINE void MakeZUnit();



	//
	//
	//
	public:
		std::string ToString()
		{
			std::string str;
			str = std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z) + "," + std::to_string(W);
			return str;
		}

	//
	//	Operators
	//
	public:


		PF_INLINE Vector4D& operator=(Vector3D& rhs)
		{
			this->X = rhs.X;
			this->Y = rhs.Y;
			this->Z = rhs.Z;

			return *this;
		}

		PF_INLINE void operator=(Vector4D& rhs)
		{			
			#ifdef PF_MATH_SIMD 
				this->m128 = rhs.m128;
			#else
				memcpy(&this->DATA, &rhs.DATA, sizeof(DATA));
			#endif
		}


		PF_INLINE Vector4D& operator +=(Vector4D& rhs)
		{
			#ifdef PF_MATH_SIMD 
				this->m128 = _mm_add_ps(this->m128, rhs.m128);
			#else
				this->X += rhs.X;
				this->Y += rhs.Y;
				this->Z += rhs.Z;
				this->W += rhs.W;
			#endif

			return *this;
		}

		PF_INLINE Vector4D& operator -=(Vector4D& rhs)
		{
			#ifdef PF_MATH_SIMD 
				this->m128 = _mm_sub_ps(this->m128, rhs.m128);
			#else
				this->X -= rhs.X;
				this->Y -= rhs.Y;
				this->Z -= rhs.Z;
				this->W -= rhs.W;
			#endif

			return *this;
		}

		PF_INLINE Vector4D& operator *=(EFLOAT32 rhs)
		{
			#ifdef PF_MATH_SIMD 
				this->m128 = _mm_mul_ps(this->m128, _mm_set1_ps(rhs));
			#else
				this->X *= rhs;
				this->Y *= rhs;
				this->Z *= rhs;
				this->W *= rhs;
			#endif

			return *this;
		}

		PF_INLINE Vector4D& operator /=(EFLOAT32 rhs)
		{
			#ifdef PF_MATH_SIMD 
				this->m128 = _mm_div_ps(this->m128, _mm_set1_ps(rhs));
			#else
				this->X /= rhs;
				this->Y /= rhs;
				this->Z /= rhs;
				this->W /= rhs;
			#endif

			return *this;
		}


		//
		//
		//
		PF_INLINE friend Vector4D operator+(Vector4D& lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_add_ps(lhs.m128, rhs.m128);
			#else
				result.X = lhs.X + rhs.X;
				result.Y = lhs.Y + rhs.Y;
				result.Z = lhs.Z + rhs.Z;
				result.W = lhs.W + rhs.W;
			#endif

			return result;
		}


		PF_INLINE friend Vector4D operator-(Vector4D& lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_sub_ps(lhs.m128, rhs.m128);
			#else
				result.X = lhs.X - rhs.X;
				result.Y = lhs.Y - rhs.Y;
				result.Z = lhs.Z - rhs.Z;
				result.W = lhs.W - rhs.W;
			#endif

			return result;
		}


		PF_INLINE friend Vector4D operator*(Vector4D& lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_mul_ps(lhs.m128, rhs.m128);
			#else
				result.X = lhs.X * rhs.X;
				result.Y = lhs.Y * rhs.Y;
				result.Z = lhs.Z * rhs.Z;
				result.W = lhs.Z * rhs.Z;
			#endif

			return result;
		}

		PF_INLINE friend Vector4D operator*(Vector4D& lhs, EFLOAT32 rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_mul_ps(lhs.m128, _mm_set1_ps(rhs));
			#else
				result.X = lhs.X * rhs;
				result.Y = lhs.Y * rhs;
				result.Z = lhs.Z * rhs;
				result.W = lhs.Z * rhs;
			#endif

			return result;
		}

		PF_INLINE friend Vector4D operator*(EFLOAT32 lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_mul_ps(_mm_set1_ps(lhs), rhs.m128);
			#else
				result.X = rhs / lhs.X;
				result.Y = rhs / lhs.Y;
				result.Z = rhs / lhs.Z;
				result.W = rhs / lhs.W;
			#endif

			return result;
		}

		PF_INLINE friend Vector4D operator/(Vector4D& lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_div_ps(lhs.m128, rhs.m128);
			#else
				result.X = lhs.X / rhs.X;
				result.Y = lhs.Y / rhs.Y;
				result.Z = lhs.Z / rhs.Z;
				result.W = lhs.W / rhs.W;
			#endif

			return result;
		}


		PF_INLINE friend Vector4D operator/(Vector4D& lhs, EFLOAT32 rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_div_ps(lhs.m128, _mm_set1_ps(rhs));
			#else
				result.X = lhs.X / rhs;
				result.Y = lhs.Y / rhs;
				result.Z = lhs.Z / rhs;
				result.W = lhs.W / rhs;
			#endif

			return result;
		}

		PF_INLINE friend Vector4D operator/(EFLOAT32 lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_div_ps(_mm_set1_ps(lhs), rhs.m128);
			#else
				result.X = rhs / lhs.X;
				result.Y = rhs / lhs.Y;
				result.Z = rhs / lhs.Z;
				result.W = rhs / lhs.W;
			#endif

			return result;
		}


		//
		// Cast operators
		operator Vector3D() const 
		{ 
			return this->XYZ;
		}



		//
		// Gets and sets
		//
		public:
				
		PF_INLINE void Set(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 pw);

		PF_INLINE void Set(const Vector2D& src, EFLOAT32 pz, EFLOAT32 pw);
		PF_INLINE void Set(EFLOAT32 px, const Vector2D& src, EFLOAT32 pw);
		PF_INLINE void Set(EFLOAT32 px, EFLOAT32 py, const Vector2D& src);
		PF_INLINE void Set(const Vector2D& src0, const Vector2D& src1);
		PF_INLINE void Set(const Vector3D& src, EFLOAT32 pw);
		PF_INLINE void Set(EFLOAT32 px, const Vector3D& src);
		PF_INLINE void Set(const Vector4D& src);

		PF_INLINE void Set(const Vector2D* src, EFLOAT32 pz, EFLOAT32 pw);
		PF_INLINE void Set(EFLOAT32 px, const Vector2D* src, EFLOAT32 pw);
		PF_INLINE void Set(EFLOAT32 px, EFLOAT32 py, const Vector2D* src);
		PF_INLINE void Set(const Vector2D* src0, const Vector2D* src1);
		PF_INLINE void Set(const Vector3D* src, EFLOAT32 pw);
		PF_INLINE void Set(EFLOAT32 px, const Vector3D* src);
		PF_INLINE void Set(const Vector4D* src);



		Vector2D XX() { return Vector2D(X, X); }
		Vector3D XXX() { return Vector3D(X, X, X); }
		Vector4D XXXX() { return Vector4D(X, X, X, X); }
		Vector4D XXXY() { return Vector4D(X, X, X, Y); }
		Vector4D XXXZ() { return Vector4D(X, X, X, Z); }
		Vector4D XXXW() { return Vector4D(X, X, X, W); }
		Vector3D XXY() { return Vector3D(X, X, Y); }
		Vector4D XXYX() { return Vector4D(X, X, Y, X); }
		Vector4D XXYY() { return Vector4D(X, X, Y, Y); }
		Vector4D XXYZ() { return Vector4D(X, X, Y, Z); }
		Vector4D XXYW() { return Vector4D(X, X, Y, W); }
		Vector3D XXZ() { return Vector3D(X, X, Z); }
		Vector4D XXZX() { return Vector4D(X, X, Z, X); }
		Vector4D XXZY() { return Vector4D(X, X, Z, Y); }
		Vector4D XXZZ() { return Vector4D(X, X, Z, Z); }
		Vector4D XXZW() { return Vector4D(X, X, Z, W); }
		Vector3D XXW() { return Vector3D(X, X, W); }
		Vector4D XXWX() { return Vector4D(X, X, W, X); }
		Vector4D XXWY() { return Vector4D(X, X, W, Y); }
		Vector4D XXWZ() { return Vector4D(X, X, W, Z); }
		Vector4D XXWW() { return Vector4D(X, X, W, W); }
		//Vector2D XY() { return Vector2D(X, Y); }
		Vector3D XYX() { return Vector3D(X, Y, X); }
		Vector4D XYXX() { return Vector4D(X, Y, X, X); }
		Vector4D XYXY() { return Vector4D(X, Y, X, Y); }
		Vector4D XYXZ() { return Vector4D(X, Y, X, Z); }
		Vector4D XYXW() { return Vector4D(X, Y, X, W); }
		Vector3D XYY() { return Vector3D(X, Y, Y); }
		Vector4D XYYX() { return Vector4D(X, Y, Y, X); }
		Vector4D XYYY() { return Vector4D(X, Y, Y, Y); }
		Vector4D XYYZ() { return Vector4D(X, Y, Y, Z); }
		Vector4D XYYW() { return Vector4D(X, Y, Y, W); }
		//Vector3D XYZ() { return Vector3D(X, Y, Z); }
		Vector4D XYZX() { return Vector4D(X, Y, Z, X); }
		Vector4D XYZY() { return Vector4D(X, Y, Z, Y); }
		Vector4D XYZZ() { return Vector4D(X, Y, Z, Z); }
		Vector4D XYZW() { return Vector4D(X, Y, Z, W); }
		Vector3D XYW() { return Vector3D(X, Y, W); }
		Vector4D XYWX() { return Vector4D(X, Y, W, X); }
		Vector4D XYWY() { return Vector4D(X, Y, W, Y); }
		Vector4D XYWZ() { return Vector4D(X, Y, W, Z); }
		Vector4D XYWW() { return Vector4D(X, Y, W, W); }
		Vector2D XZ() { return Vector2D(X, Z); }
		Vector3D XZX() { return Vector3D(X, Z, X); }
		Vector4D XZXX() { return Vector4D(X, Z, X, X); }
		Vector4D XZXY() { return Vector4D(X, Z, X, Y); }
		Vector4D XZXZ() { return Vector4D(X, Z, X, Z); }
		Vector4D XZXW() { return Vector4D(X, Z, X, W); }
		Vector3D XZY() { return Vector3D(X, Z, Y); }
		Vector4D XZYX() { return Vector4D(X, Z, Y, X); }
		Vector4D XZYY() { return Vector4D(X, Z, Y, Y); }
		Vector4D XZYZ() { return Vector4D(X, Z, Y, Z); }
		Vector4D XZYW() { return Vector4D(X, Z, Y, W); }
		Vector3D XZZ() { return Vector3D(X, Z, Z); }
		Vector4D XZZX() { return Vector4D(X, Z, Z, X); }
		Vector4D XZZY() { return Vector4D(X, Z, Z, Y); }
		Vector4D XZZZ() { return Vector4D(X, Z, Z, Z); }
		Vector4D XZZW() { return Vector4D(X, Z, Z, W); }
		Vector3D XZW() { return Vector3D(X, Z, W); }
		Vector4D XZWX() { return Vector4D(X, Z, W, X); }
		Vector4D XZWY() { return Vector4D(X, Z, W, Y); }
		Vector4D XZWZ() { return Vector4D(X, Z, W, Z); }
		Vector4D XZWW() { return Vector4D(X, Z, W, W); }
		Vector2D XW() { return Vector2D(X, W); }
		Vector3D XWX() { return Vector3D(X, W, X); }
		Vector4D XWXX() { return Vector4D(X, W, X, X); }
		Vector4D XWXY() { return Vector4D(X, W, X, Y); }
		Vector4D XWXZ() { return Vector4D(X, W, X, Z); }
		Vector4D XWXW() { return Vector4D(X, W, X, W); }
		Vector3D XWY() { return Vector3D(X, W, Y); }
		Vector4D XWYX() { return Vector4D(X, W, Y, X); }
		Vector4D XWYY() { return Vector4D(X, W, Y, Y); }
		Vector4D XWYZ() { return Vector4D(X, W, Y, Z); }
		Vector4D XWYW() { return Vector4D(X, W, Y, W); }
		Vector3D XWZ() { return Vector3D(X, W, Z); }
		Vector4D XWZX() { return Vector4D(X, W, Z, X); }
		Vector4D XWZY() { return Vector4D(X, W, Z, Y); }
		Vector4D XWZZ() { return Vector4D(X, W, Z, Z); }
		Vector4D XWZW() { return Vector4D(X, W, Z, W); }
		Vector3D XWW() { return Vector3D(X, W, W); }
		Vector4D XWWX() { return Vector4D(X, W, W, X); }
		Vector4D XWWY() { return Vector4D(X, W, W, Y); }
		Vector4D XWWZ() { return Vector4D(X, W, W, Z); }
		Vector4D XWWW() { return Vector4D(X, W, W, W); }
		Vector2D YX() { return Vector2D(Y, X); }
		Vector3D YXX() { return Vector3D(Y, X, X); }
		Vector4D YXXX() { return Vector4D(Y, X, X, X); }
		Vector4D YXXY() { return Vector4D(Y, X, X, Y); }
		Vector4D YXXZ() { return Vector4D(Y, X, X, Z); }
		Vector4D YXXW() { return Vector4D(Y, X, X, W); }
		Vector3D YXY() { return Vector3D(Y, X, Y); }
		Vector4D YXYX() { return Vector4D(Y, X, Y, X); }
		Vector4D YXYY() { return Vector4D(Y, X, Y, Y); }
		Vector4D YXYZ() { return Vector4D(Y, X, Y, Z); }
		Vector4D YXYW() { return Vector4D(Y, X, Y, W); }
		Vector3D YXZ() { return Vector3D(Y, X, Z); }
		Vector4D YXZX() { return Vector4D(Y, X, Z, X); }
		Vector4D YXZY() { return Vector4D(Y, X, Z, Y); }
		Vector4D YXZZ() { return Vector4D(Y, X, Z, Z); }
		Vector4D YXZW() { return Vector4D(Y, X, Z, W); }
		Vector3D YXW() { return Vector3D(Y, X, W); }
		Vector4D YXWX() { return Vector4D(Y, X, W, X); }
		Vector4D YXWY() { return Vector4D(Y, X, W, Y); }
		Vector4D YXWZ() { return Vector4D(Y, X, W, Z); }
		Vector4D YXWW() { return Vector4D(Y, X, W, W); }
		Vector2D YY() { return Vector2D(Y, Y); }
		Vector3D YYX() { return Vector3D(Y, Y, X); }
		Vector4D YYXX() { return Vector4D(Y, Y, X, X); }
		Vector4D YYXY() { return Vector4D(Y, Y, X, Y); }
		Vector4D YYXZ() { return Vector4D(Y, Y, X, Z); }
		Vector4D YYXW() { return Vector4D(Y, Y, X, W); }
		Vector3D YYY() { return Vector3D(Y, Y, Y); }
		Vector4D YYYX() { return Vector4D(Y, Y, Y, X); }
		Vector4D YYYY() { return Vector4D(Y, Y, Y, Y); }
		Vector4D YYYZ() { return Vector4D(Y, Y, Y, Z); }
		Vector4D YYYW() { return Vector4D(Y, Y, Y, W); }
		Vector3D YYZ() { return Vector3D(Y, Y, Z); }
		Vector4D YYZX() { return Vector4D(Y, Y, Z, X); }
		Vector4D YYZY() { return Vector4D(Y, Y, Z, Y); }
		Vector4D YYZZ() { return Vector4D(Y, Y, Z, Z); }
		Vector4D YYZW() { return Vector4D(Y, Y, Z, W); }
		Vector3D YYW() { return Vector3D(Y, Y, W); }
		Vector4D YYWX() { return Vector4D(Y, Y, W, X); }
		Vector4D YYWY() { return Vector4D(Y, Y, W, Y); }
		Vector4D YYWZ() { return Vector4D(Y, Y, W, Z); }
		Vector4D YYWW() { return Vector4D(Y, Y, W, W); }
		Vector2D YZ() { return Vector2D(Y, Z); }
		Vector3D YZX() { return Vector3D(Y, Z, X); }
		Vector4D YZXX() { return Vector4D(Y, Z, X, X); }
		Vector4D YZXY() { return Vector4D(Y, Z, X, Y); }
		Vector4D YZXZ() { return Vector4D(Y, Z, X, Z); }
		Vector4D YZXW() { return Vector4D(Y, Z, X, W); }
		Vector3D YZY() { return Vector3D(Y, Z, Y); }
		Vector4D YZYX() { return Vector4D(Y, Z, Y, X); }
		Vector4D YZYY() { return Vector4D(Y, Z, Y, Y); }
		Vector4D YZYZ() { return Vector4D(Y, Z, Y, Z); }
		Vector4D YZYW() { return Vector4D(Y, Z, Y, W); }
		Vector3D YZZ() { return Vector3D(Y, Z, Z); }
		Vector4D YZZX() { return Vector4D(Y, Z, Z, X); }
		Vector4D YZZY() { return Vector4D(Y, Z, Z, Y); }
		Vector4D YZZZ() { return Vector4D(Y, Z, Z, Z); }
		Vector4D YZZW() { return Vector4D(Y, Z, Z, W); }
		//Vector3D YZW() { return Vector3D(Y, Z, W); }
		Vector4D YZWX() { return Vector4D(Y, Z, W, X); }
		Vector4D YZWY() { return Vector4D(Y, Z, W, Y); }
		Vector4D YZWZ() { return Vector4D(Y, Z, W, Z); }
		Vector4D YZWW() { return Vector4D(Y, Z, W, W); }
		Vector2D YW() { return Vector2D(Y, W); }
		Vector3D YWX() { return Vector3D(Y, W, X); }
		Vector4D YWXX() { return Vector4D(Y, W, X, X); }
		Vector4D YWXY() { return Vector4D(Y, W, X, Y); }
		Vector4D YWXZ() { return Vector4D(Y, W, X, Z); }
		Vector4D YWXW() { return Vector4D(Y, W, X, W); }
		Vector3D YWY() { return Vector3D(Y, W, Y); }
		Vector4D YWYX() { return Vector4D(Y, W, Y, X); }
		Vector4D YWYY() { return Vector4D(Y, W, Y, Y); }
		Vector4D YWYZ() { return Vector4D(Y, W, Y, Z); }
		Vector4D YWYW() { return Vector4D(Y, W, Y, W); }
		Vector3D YWZ() { return Vector3D(Y, W, Z); }
		Vector4D YWZX() { return Vector4D(Y, W, Z, X); }
		Vector4D YWZY() { return Vector4D(Y, W, Z, Y); }
		Vector4D YWZZ() { return Vector4D(Y, W, Z, Z); }
		Vector4D YWZW() { return Vector4D(Y, W, Z, W); }
		Vector3D YWW() { return Vector3D(Y, W, W); }
		Vector4D YWWX() { return Vector4D(Y, W, W, X); }
		Vector4D YWWY() { return Vector4D(Y, W, W, Y); }
		Vector4D YWWZ() { return Vector4D(Y, W, W, Z); }
		Vector4D YWWW() { return Vector4D(Y, W, W, W); }
		Vector2D ZX() { return Vector2D(Z, X); }
		Vector3D ZXX() { return Vector3D(Z, X, X); }
		Vector4D ZXXX() { return Vector4D(Z, X, X, X); }
		Vector4D ZXXY() { return Vector4D(Z, X, X, Y); }
		Vector4D ZXXZ() { return Vector4D(Z, X, X, Z); }
		Vector4D ZXXW() { return Vector4D(Z, X, X, W); }
		Vector3D ZXY() { return Vector3D(Z, X, Y); }
		Vector4D ZXYX() { return Vector4D(Z, X, Y, X); }
		Vector4D ZXYY() { return Vector4D(Z, X, Y, Y); }
		Vector4D ZXYZ() { return Vector4D(Z, X, Y, Z); }
		Vector4D ZXYW() { return Vector4D(Z, X, Y, W); }
		Vector3D ZXZ() { return Vector3D(Z, X, Z); }
		Vector4D ZXZX() { return Vector4D(Z, X, Z, X); }
		Vector4D ZXZY() { return Vector4D(Z, X, Z, Y); }
		Vector4D ZXZZ() { return Vector4D(Z, X, Z, Z); }
		Vector4D ZXZW() { return Vector4D(Z, X, Z, W); }
		Vector3D ZXW() { return Vector3D(Z, X, W); }
		Vector4D ZXWX() { return Vector4D(Z, X, W, X); }
		Vector4D ZXWY() { return Vector4D(Z, X, W, Y); }
		Vector4D ZXWZ() { return Vector4D(Z, X, W, Z); }
		Vector4D ZXWW() { return Vector4D(Z, X, W, W); }
		Vector2D ZY() { return Vector2D(Z, Y); }
		Vector3D ZYX() { return Vector3D(Z, Y, X); }
		Vector4D ZYXX() { return Vector4D(Z, Y, X, X); }
		Vector4D ZYXY() { return Vector4D(Z, Y, X, Y); }
		Vector4D ZYXZ() { return Vector4D(Z, Y, X, Z); }
		Vector4D ZYXW() { return Vector4D(Z, Y, X, W); }
		Vector3D ZYY() { return Vector3D(Z, Y, Y); }
		Vector4D ZYYX() { return Vector4D(Z, Y, Y, X); }
		Vector4D ZYYY() { return Vector4D(Z, Y, Y, Y); }
		Vector4D ZYYZ() { return Vector4D(Z, Y, Y, Z); }
		Vector4D ZYYW() { return Vector4D(Z, Y, Y, W); }
		Vector3D ZYZ() { return Vector3D(Z, Y, Z); }
		Vector4D ZYZX() { return Vector4D(Z, Y, Z, X); }
		Vector4D ZYZY() { return Vector4D(Z, Y, Z, Y); }
		Vector4D ZYZZ() { return Vector4D(Z, Y, Z, Z); }
		Vector4D ZYZW() { return Vector4D(Z, Y, Z, W); }
		Vector3D ZYW() { return Vector3D(Z, Y, W); }
		Vector4D ZYWX() { return Vector4D(Z, Y, W, X); }
		Vector4D ZYWY() { return Vector4D(Z, Y, W, Y); }
		Vector4D ZYWZ() { return Vector4D(Z, Y, W, Z); }
		Vector4D ZYWW() { return Vector4D(Z, Y, W, W); }
		Vector2D ZZ() { return Vector2D(Z, Z); }
		Vector3D ZZX() { return Vector3D(Z, Z, X); }
		Vector4D ZZXX() { return Vector4D(Z, Z, X, X); }
		Vector4D ZZXY() { return Vector4D(Z, Z, X, Y); }
		Vector4D ZZXZ() { return Vector4D(Z, Z, X, Z); }
		Vector4D ZZXW() { return Vector4D(Z, Z, X, W); }
		Vector3D ZZY() { return Vector3D(Z, Z, Y); }
		Vector4D ZZYX() { return Vector4D(Z, Z, Y, X); }
		Vector4D ZZYY() { return Vector4D(Z, Z, Y, Y); }
		Vector4D ZZYZ() { return Vector4D(Z, Z, Y, Z); }
		Vector4D ZZYW() { return Vector4D(Z, Z, Y, W); }
		Vector3D ZZZ() { return Vector3D(Z, Z, Z); }
		Vector4D ZZZX() { return Vector4D(Z, Z, Z, X); }
		Vector4D ZZZY() { return Vector4D(Z, Z, Z, Y); }
		Vector4D ZZZZ() { return Vector4D(Z, Z, Z, Z); }
		Vector4D ZZZW() { return Vector4D(Z, Z, Z, W); }
		Vector3D ZZW() { return Vector3D(Z, Z, W); }
		Vector4D ZZWX() { return Vector4D(Z, Z, W, X); }
		Vector4D ZZWY() { return Vector4D(Z, Z, W, Y); }
		Vector4D ZZWZ() { return Vector4D(Z, Z, W, Z); }
		Vector4D ZZWW() { return Vector4D(Z, Z, W, W); }
		//Vector2D ZW() { return Vector2D(Z, W); }
		Vector3D ZWX() { return Vector3D(Z, W, X); }
		Vector4D ZWXX() { return Vector4D(Z, W, X, X); }
		Vector4D ZWXY() { return Vector4D(Z, W, X, Y); }
		Vector4D ZWXZ() { return Vector4D(Z, W, X, Z); }
		Vector4D ZWXW() { return Vector4D(Z, W, X, W); }
		Vector3D ZWY() { return Vector3D(Z, W, Y); }
		Vector4D ZWYX() { return Vector4D(Z, W, Y, X); }
		Vector4D ZWYY() { return Vector4D(Z, W, Y, Y); }
		Vector4D ZWYZ() { return Vector4D(Z, W, Y, Z); }
		Vector4D ZWYW() { return Vector4D(Z, W, Y, W); }
		Vector3D ZWZ() { return Vector3D(Z, W, Z); }
		Vector4D ZWZX() { return Vector4D(Z, W, Z, X); }
		Vector4D ZWZY() { return Vector4D(Z, W, Z, Y); }
		Vector4D ZWZZ() { return Vector4D(Z, W, Z, Z); }
		Vector4D ZWZW() { return Vector4D(Z, W, Z, W); }
		Vector3D ZWW() { return Vector3D(Z, W, W); }
		Vector4D ZWWX() { return Vector4D(Z, W, W, X); }
		Vector4D ZWWY() { return Vector4D(Z, W, W, Y); }
		Vector4D ZWWZ() { return Vector4D(Z, W, W, Z); }
		Vector4D ZWWW() { return Vector4D(Z, W, W, W); }
		Vector2D WX() { return Vector2D(W, X); }
		Vector3D WXX() { return Vector3D(W, X, X); }
		Vector4D WXXX() { return Vector4D(W, X, X, X); }
		Vector4D WXXY() { return Vector4D(W, X, X, Y); }
		Vector4D WXXZ() { return Vector4D(W, X, X, Z); }
		Vector4D WXXW() { return Vector4D(W, X, X, W); }
		Vector3D WXY() { return Vector3D(W, X, Y); }
		Vector4D WXYX() { return Vector4D(W, X, Y, X); }
		Vector4D WXYY() { return Vector4D(W, X, Y, Y); }
		Vector4D WXYZ() { return Vector4D(W, X, Y, Z); }
		Vector4D WXYW() { return Vector4D(W, X, Y, W); }
		Vector3D WXZ() { return Vector3D(W, X, Z); }
		Vector4D WXZX() { return Vector4D(W, X, Z, X); }
		Vector4D WXZY() { return Vector4D(W, X, Z, Y); }
		Vector4D WXZZ() { return Vector4D(W, X, Z, Z); }
		Vector4D WXZW() { return Vector4D(W, X, Z, W); }
		Vector3D WXW() { return Vector3D(W, X, W); }
		Vector4D WXWX() { return Vector4D(W, X, W, X); }
		Vector4D WXWY() { return Vector4D(W, X, W, Y); }
		Vector4D WXWZ() { return Vector4D(W, X, W, Z); }
		Vector4D WXWW() { return Vector4D(W, X, W, W); }
		Vector2D WY() { return Vector2D(W, Y); }
		Vector3D WYX() { return Vector3D(W, Y, X); }
		Vector4D WYXX() { return Vector4D(W, Y, X, X); }
		Vector4D WYXY() { return Vector4D(W, Y, X, Y); }
		Vector4D WYXZ() { return Vector4D(W, Y, X, Z); }
		Vector4D WYXW() { return Vector4D(W, Y, X, W); }
		Vector3D WYY() { return Vector3D(W, Y, Y); }
		Vector4D WYYX() { return Vector4D(W, Y, Y, X); }
		Vector4D WYYY() { return Vector4D(W, Y, Y, Y); }
		Vector4D WYYZ() { return Vector4D(W, Y, Y, Z); }
		Vector4D WYYW() { return Vector4D(W, Y, Y, W); }
		Vector3D WYZ() { return Vector3D(W, Y, Z); }
		Vector4D WYZX() { return Vector4D(W, Y, Z, X); }
		Vector4D WYZY() { return Vector4D(W, Y, Z, Y); }
		Vector4D WYZZ() { return Vector4D(W, Y, Z, Z); }
		Vector4D WYZW() { return Vector4D(W, Y, Z, W); }
		Vector3D WYW() { return Vector3D(W, Y, W); }
		Vector4D WYWX() { return Vector4D(W, Y, W, X); }
		Vector4D WYWY() { return Vector4D(W, Y, W, Y); }
		Vector4D WYWZ() { return Vector4D(W, Y, W, Z); }
		Vector4D WYWW() { return Vector4D(W, Y, W, W); }
		Vector2D WZ() { return Vector2D(W, Z); }
		Vector3D WZX() { return Vector3D(W, Z, X); }
		Vector4D WZXX() { return Vector4D(W, Z, X, X); }
		Vector4D WZXY() { return Vector4D(W, Z, X, Y); }
		Vector4D WZXZ() { return Vector4D(W, Z, X, Z); }
		Vector4D WZXW() { return Vector4D(W, Z, X, W); }
		Vector3D WZY() { return Vector3D(W, Z, Y); }
		Vector4D WZYX() { return Vector4D(W, Z, Y, X); }
		Vector4D WZYY() { return Vector4D(W, Z, Y, Y); }
		Vector4D WZYZ() { return Vector4D(W, Z, Y, Z); }
		Vector4D WZYW() { return Vector4D(W, Z, Y, W); }
		Vector3D WZZ() { return Vector3D(W, Z, Z); }
		Vector4D WZZX() { return Vector4D(W, Z, Z, X); }
		Vector4D WZZY() { return Vector4D(W, Z, Z, Y); }
		Vector4D WZZZ() { return Vector4D(W, Z, Z, Z); }
		Vector4D WZZW() { return Vector4D(W, Z, Z, W); }
		Vector3D WZW() { return Vector3D(W, Z, W); }
		Vector4D WZWX() { return Vector4D(W, Z, W, X); }
		Vector4D WZWY() { return Vector4D(W, Z, W, Y); }
		Vector4D WZWZ() { return Vector4D(W, Z, W, Z); }
		Vector4D WZWW() { return Vector4D(W, Z, W, W); }
		Vector2D WW() { return Vector2D(W, W); }
		Vector3D WWX() { return Vector3D(W, W, X); }
		Vector4D WWXX() { return Vector4D(W, W, X, X); }
		Vector4D WWXY() { return Vector4D(W, W, X, Y); }
		Vector4D WWXZ() { return Vector4D(W, W, X, Z); }
		Vector4D WWXW() { return Vector4D(W, W, X, W); }
		Vector3D WWY() { return Vector3D(W, W, Y); }
		Vector4D WWYX() { return Vector4D(W, W, Y, X); }
		Vector4D WWYY() { return Vector4D(W, W, Y, Y); }
		Vector4D WWYZ() { return Vector4D(W, W, Y, Z); }
		Vector4D WWYW() { return Vector4D(W, W, Y, W); }
		Vector3D WWZ() { return Vector3D(W, W, Z); }
		Vector4D WWZX() { return Vector4D(W, W, Z, X); }
		Vector4D WWZY() { return Vector4D(W, W, Z, Y); }
		Vector4D WWZZ() { return Vector4D(W, W, Z, Z); }
		Vector4D WWZW() { return Vector4D(W, W, Z, W); }
		Vector3D WWW() { return Vector3D(W, W, W); }
		Vector4D WWWX() { return Vector4D(W, W, W, X); }
		Vector4D WWWY() { return Vector4D(W, W, W, Y); }
		Vector4D WWWZ() { return Vector4D(W, W, W, Z); }
		Vector4D WWWW() { return Vector4D(W, W, W, W); }


	};

	//	------------------------------------------------------------------------------------
	//		Global Vector4D functions
	//	------------------------------------------------------------------------------------


	PF_INLINE XYZ_API Vector4D PF_FASTCALL V4DXUnit();
	PF_INLINE XYZ_API Vector4D PF_FASTCALL V4DYUnit();
	PF_INLINE XYZ_API Vector4D PF_FASTCALL V4DZUnit();









}



#endif