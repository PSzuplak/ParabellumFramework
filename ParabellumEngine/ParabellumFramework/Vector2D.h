#ifndef _VECTROR2D_H_
#define _VECTROR2D_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "MemorySettings.h"
#include "SystemSettings.h"
#include "Convenctions.h"

#include "TypesC.h"
#include "MathHelper.h"


namespace ParabellumFramework
{
	class XYZ_API PF_ALLIGN(16) Vector2D
	{
	public:
		union
		{
			struct
			{
				EFLOAT32 X;
				EFLOAT32 Y;
			};

			struct
			{
				EFLOAT32 data[2];
			};

			struct
			{
				EFLOAT32 datac[2];
			} DATA;
		};


	public:

		Vector2D();
		Vector2D(EFLOAT32 px, EFLOAT32 py);
		Vector2D(const Vector2D& copy);

		void Set(EFLOAT32 px, EFLOAT32 py);
		void Set(const Vector2D& vec);

		static Vector2D* CreateVector2D(EFLOAT32 PX, EFLOAT32 PY);

		EFLOAT32 Length();

		void Normalize();
		static void Normalize(Vector2D* v1);

	
		void MakeOne();
		void MakeZero();

		void MakeUp();
		void MakeDown();
		void MakeLeft();
		void MakeRight();

		void MakeXUnit();
		void MakeYUnit();




		std::string ToString();


	public:

		PF_INLINE void operator=(Vector2D& rhs);


		PF_INLINE void operator +=(Vector2D& rhs)
		{
			this->X += rhs.X;
			this->Y += rhs.Y;
		}

		PF_INLINE void operator +=(EFLOAT32 rhs)
		{
			this->X += rhs;
			this->Y += rhs;
		}

		PF_INLINE void operator -=(Vector2D& rhs)
		{
			this->X -= rhs.X;
			this->Y -= rhs.Y;
		}

		PF_INLINE void operator -=(EFLOAT32 rhs)
		{
			this->X -= rhs;
			this->Y -= rhs;
		}

		PF_INLINE void operator *=(Vector2D& rhs)
		{
			this->X *= rhs.X;
			this->Y *= rhs.Y;
		}

		PF_INLINE void operator *=(EFLOAT32 rhs)
		{
			this->X *= rhs;
			this->Y *= rhs;
		}

		//
		// Operators left-right
		//
		PF_INLINE friend Vector2D operator+(Vector2D& lhs, Vector2D& rhs)
		{
			return Vector2D(lhs.X + rhs.X, lhs.Y + rhs.Y);
		}

		PF_INLINE friend Vector2D operator-(Vector2D& lhs, Vector2D& rhs)
		{
			return Vector2D(lhs.X - rhs.X, lhs.Y - rhs.Y);
		}

		PF_INLINE friend Vector2D operator*(Vector2D& lhs, Vector2D& rhs)
		{
			return Vector2D(lhs.X * rhs.X, lhs.Y * rhs.Y);
		}

		PF_INLINE friend Vector2D operator*(Vector2D& lhs, EFLOAT32 rhs)
		{
			return Vector2D(lhs.X * rhs, lhs.Y * rhs);
		}

		PF_INLINE friend Vector2D operator/(Vector2D& lhs, EFLOAT32 rhs)
		{
			return Vector2D(lhs.X / rhs, lhs.Y / rhs);
		}


	};




}



#endif