#ifndef _VECTROR3D_H_
#define _VECTROR3D_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "MemorySettings.h"
#include "SystemSettings.h"
#include "Convenctions.h"

#include <string>
#include "TypesC.h"
#include "MathHelper.h"
#include "Vector2D.h"



//	----------------------------------------------------
//	Optimized PF_INLINE version of some functions
//	----------------------------------------------------


#define PF_VECTOR3D_Set(Vector3D_ref, x, y, z) Vector3D_ref.X = x; Vector3D_ref.Y = y; Vector3D_ref.Z = z; 

#define PF_VECTOR3D_MakeZero(Vector3D_ref) Vector3D_ref.X = 0.0f; Vector3D_ref.Y = 0.0f; Vector3D_ref.Z = 0.0f; 

#define PF_VECTOR3D_Distance(Vector3D_first, Vector3D_second, distance_out) \
		{ \
			EFLOAT32 tx, ty, tz; \
			tx = Vector3D_second->X - Vector3D_first->X; \
			ty = Vector3D_second->Y - Vector3D_first->Y; \
			tz = Vector3D_second->Z - Vector3D_first->Z; \
			distance_out = ParabellumUtils::Math::FastSqrt(tx*tx + ty*ty + tz*tz); \
		}


namespace ParabellumFramework
{

	class XYZ_API PF_ALLIGN(16) Vector3D
	{
		// common used vectors
	public:
		static const Vector3D& Up();
		static const Vector3D& Down();
		static const Vector3D& Left();
		static const Vector3D& Right();
		static const Vector3D& Forward();
		static const Vector3D& Backward();
		static const Vector3D& Zero();
		static const Vector3D& UnitX();
		static const Vector3D& UnitY();
		static const Vector3D& UnitZ();

		//
		//	Vector3D - 12byte data
		//
	public:
		union
		{
			struct
			{
				EFLOAT32 X;
				EFLOAT32 Y;
				EFLOAT32 Z;
			};

			struct
			{
				EFLOAT32 data[3];
			};

			// 12 Bytes variable - for shaders parametrs purpose
			struct
			{
				EFLOAT32 datac[3];
			} DATA;

			struct
			{
				Vector2D XY;
				EFLOAT32 Z;
			};

			struct
			{
				EFLOAT32 X;
				Vector2D YZ;
			};

		};

		//
		// Constructors and Descrutors
		//
	public:
		Vector3D();
		Vector3D(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);
		Vector3D(const Vector3D& copy);
		~Vector3D();

	public:

		PF_INLINE void PF_FASTCALL Set(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);
		PF_INLINE void PF_FASTCALL Set(const Vector3D& src);

		PF_INLINE EFLOAT32 Length();							// get vector length
		PF_INLINE static EFLOAT32 PF_FASTCALL Length(_IN_ Vector3D& vec);

		// calculate distance between 2 vectors
		PF_INLINE static EFLOAT32 PF_FASTCALL Distance(_IN_ Vector3D* vec1, _IN_ Vector3D* vec2);


		PF_INLINE EFLOAT32 PF_FASTCALL SquareLength();
		PF_INLINE static EFLOAT32 PF_FASTCALL SquareLength(_IN_ Vector3D& vec);

		PF_INLINE void PF_FASTCALL Normalize();			// normalize vector
		PF_INLINE static void PF_FASTCALL Normalize(_IO_ Vector3D* v3);

		// angle between 2 vectors in Radians
		PF_INLINE static EFLOAT32 PF_FASTCALL Angle(Vector3D v1, Vector3D v2);

		// dot product
		PF_INLINE static EFLOAT32 PF_FASTCALL Dot(_IN_ Vector3D* pV1, _IN_ Vector3D* pV2);


		PF_INLINE static void PF_FASTCALL Cross(_OUT_ Vector3D* pOut, _IN_ Vector3D* pIn1, _IN_ Vector3D* pIn2);

		// reflect vector against normal vector
		// pOut can NOT be the same vector as pIn
		PF_INLINE static void PF_FASTCALL Reflect(_OUT_ Vector3D* pOut, _IN_ Vector3D* pIn, _IN_ Vector3D* pNormal);


		// angle in radians
		PF_INLINE void PF_FASTCALL RotateX(EFLOAT32 angle);

		// angle in radians
		PF_INLINE void PF_FASTCALL RotateY(EFLOAT32 angle);

		// angle in radians
		PF_INLINE void PF_FASTCALL RotateZ(EFLOAT32 angle);



		PF_INLINE EFLOAT32 GetMax();			// get the biggest from X, Y and Z
		PF_INLINE EFLOAT32 GetMin();			// get the lowest form X, U and Z

		PF_INLINE void Get(EFLOAT32& px, EFLOAT32& py, EFLOAT32& pz);


		PF_INLINE void CopyTo(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z);

		// cast X,Y,Z to integer
		PF_INLINE void CastToInt();

		// X,Y,Z truncate
		PF_INLINE void Trunc();


		// cange Vector to one of the commons ones
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



		Vector2D XX() { return Vector2D(X, X); }
		Vector2D YY() { return Vector2D(Y, Y); }
		Vector2D ZZ() { return Vector2D(Z, Z); }
		//Vector2D XY() { return Vector2D(X, Y); }
		Vector2D YX() { return Vector2D(Y, X); }
		Vector2D XZ() { return Vector2D(X, Z); }
		Vector2D ZX() { return Vector2D(Z, X); }
		Vector2D ZY() { return Vector2D(Z, Y); }
		//Vector2D YZ() { return Vector2D(Y, Z); }

		Vector3D XXX() { return Vector3D(X ,X, X); }
		Vector3D YYY() { return Vector3D(Y, Y, Y); }
		Vector3D ZZZ() { return Vector3D(Z, Z, Z); }
		Vector3D YXX() { return Vector3D(Y, X, X); }
		Vector3D XYX() { return Vector3D(X, Y, X); }
		Vector3D XXY() { return Vector3D(X, X, Y); }
		Vector3D YYX() { return Vector3D(Y, Y, X); }
		Vector3D YXY() { return Vector3D(Y, X, Y); }
		Vector3D YZZ() { return Vector3D(Y, Z, Z); }
		Vector3D ZYZ() { return Vector3D(Z, Y, Z); }
		Vector3D ZZY() { return Vector3D(Z, Z, Y); }
		Vector3D YYZ() { return Vector3D(Y, Y, Z); }
		Vector3D YZY() { return Vector3D(Y, Z, Y); }
		Vector3D ZXX() { return Vector3D(Z, X, X); }
		Vector3D XZX() { return Vector3D(X, Z, X); }
		Vector3D XXZ() { return Vector3D(X, X, Z); }
		Vector3D ZZX() { return Vector3D(Z, Z, X); }
		Vector3D ZXZ() { return Vector3D(Z, X, Z); }
		Vector3D ZYY() { return Vector3D(Z, Y, Y); }
		Vector3D XYZ() { return Vector3D(X, Y, Z); }
		Vector3D XZY() { return Vector3D(X, Z, Y); }
		Vector3D YXZ() { return Vector3D(Y, X, Z); }
		Vector3D ZXY() { return Vector3D(Z, X, Y); }
		Vector3D YZX() { return Vector3D(Y, Z, X); }
		Vector3D ZYX() { return Vector3D(Z, Y, X); }

		//
		//	Get and Sets
		//
	public:



		//
		// To string
		//
	public:
		std::string ToString();






	//
	//	Operators
	//
	public:
		PF_INLINE void operator=(const Vector3D& rhs);

		PF_INLINE void operator=(EFLOAT32 rhs);

		PF_INLINE void operator +=(const Vector3D& rhs);

		PF_INLINE void operator +=(const EFLOAT32 rhs);

		PF_INLINE void operator -=(const Vector3D& rhs);

		PF_INLINE void operator -=(const EFLOAT32 rhs);

		PF_INLINE void operator *=(const EFLOAT32 rhs);

		PF_INLINE void operator *=(const Vector3D& rhs);

		PF_INLINE void operator /=(const EFLOAT32 rhs);


		//
		// Operators left-right
		//
		PF_INLINE friend Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
		{
			Vector3D result;

			#ifdef PF_MATH_SIMD 
				result.X = lhs.X + rhs.X;
				result.Y = lhs.Y + rhs.Y;
				result.Z = lhs.Z + rhs.Z;
			#else
				result.X = lhs.X + rhs.X;
				result.Y = lhs.Y + rhs.Y;
				result.Z = lhs.Z + rhs.Z;
			#endif

			return result;
		}


		PF_INLINE friend Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
		{
			Vector3D result;

			#ifdef PF_MATH_SIMD 
				result.X = lhs.X - rhs.X;
				result.Y = lhs.Y - rhs.Y;
				result.Z = lhs.Z - rhs.Z;
			#else
				result.X = lhs.X - rhs.X;
				result.Y = lhs.Y - rhs.Y;
				result.Z = lhs.Z - rhs.Z;
			#endif


			return result;
		}

		PF_INLINE friend Vector3D operator-(const Vector3D& rhs)
		{
			Vector3D result;

			#ifdef PF_MATH_SIMD 
				result.X = -rhs.X;
				result.Y = -rhs.Y;
				result.Z = -rhs.Z;
			#else
				result.X = -rhs.X;
				result.Y = -rhs.Y;
				result.Z = -rhs.Z;
			#endif

			return result;
		}

		PF_INLINE friend Vector3D operator*(Vector3D& lhs, Vector3D& rhs)
		{
			Vector3D result;

			result.X = lhs.X * rhs.X;
			result.Y = lhs.Y * rhs.Y;
			result.Z = lhs.Z * rhs.Z;

			return result;
		}

		PF_INLINE friend Vector3D operator*(const Vector3D& lhs, const EFLOAT32 rhs)
		{
			Vector3D result;

			#ifdef PF_MATH_SIMD
				result.X = lhs.X * rhs;
				result.Y = lhs.Y * rhs;
				result.Z = lhs.Z * rhs;
			#else
				result.X = lhs.X * rhs;
				result.Y = lhs.Y * rhs;
				result.Z = lhs.Z * rhs;
			#endif

			return result;
		}

		PF_INLINE friend Vector3D operator/(const Vector3D& lhs, const EFLOAT32 rhs)
		{
			Vector3D result;

			#ifdef PF_MATH_SIMD
				EFLOAT32 factor = 1.0f / rhs;
				result.X = lhs.X * factor;
				result.Y = lhs.Y * factor;
				result.Z = lhs.Z * factor;
			#else
				EFLOAT32 factor = 1.0f / rhs;
				result.X = lhs.X * factor;
				result.Y = lhs.Y * factor;
				result.Z = lhs.Z * factor;
			#endif

			return result;
		}

	};



}



#endif