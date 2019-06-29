#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "MemorySettings.h"
#include "SystemSettings.h"
#include "Convenctions.h"

#include <xmmintrin.h>	// for SIMD

#include "GraphicsDeviceHeaders.h"

#include "TypesC.h"
#include "MathHelper.h"
#include "Vector3D.h"
#include "Vector4D.h"



// preprocessors version of functions


#define PF_MATRIX4X4_Set(Matrix4x4_ref, i11,  i12,  i13,  i14, i21,  i22,   i23,  i24, i31,  i32,  i33,  i34, i41,  i42,  i43,  i44) Matrix4x4_ref.m11 = i11; Matrix4x4_ref.m21 = i21; Matrix4x4_ref.m31 = i31; Matrix4x4_ref.m41 = i41; Matrix4x4_ref.m12 = i12; Matrix4x4_ref.m22 = i22; Matrix4x4_ref.m32 = i32; Matrix4x4_ref.m42 = i42; Matrix4x4_ref.m13 = i13; Matrix4x4_ref.m23 = i23; Matrix4x4_ref.m33 = i33; Matrix4x4_ref.m43 = i43; Matrix4x4_ref.m14 = i14; Matrix4x4_ref.m24 = i24; Matrix4x4_ref.m34 = i34; Matrix4x4_ref.m44 = i44;

#define PF_MATRIX4X4_MakeIdentity(Matrix4x4_ref) Matrix4x4_ref.m11 = 0.0f; Matrix4x4_ref.m21 = 0.0f; Matrix4x4_ref.m31 = 0.0f; Matrix4x4_ref.m41 = 0.0f; Matrix4x4_ref.m12 = 0.0f; Matrix4x4_ref.m22 = 0.0f; Matrix4x4_ref.m32 = 0.0f; Matrix4x4_ref.m42 = 0.0f; Matrix4x4_ref.m13 = 0.0f; Matrix4x4_ref.m23 = 0.0f; Matrix4x4_ref.m33 = 0.0f; Matrix4x4_ref.m43 = 0.0f; Matrix4x4_ref.m14 = 0.0f; Matrix4x4_ref.m24 = 0.0f; Matrix4x4_ref.m34 = 0.0f; Matrix4x4_ref.m44 = 0.0f;


#define PF_MATRIX4X4_SetScaleFields(Matrix4x4_ref, x, y, z)						Matrix4x4_ref.m11 = x; Matrix4x4_ref.m22 = y; Matrix4x4_ref.m33 = z;
#define PF_MATRIX4X4_MakeRotationYawPitchRoll(Matrix4x4_ref, yaw, pitch, roll)	*(DirectX::XMMATRIX*)(&Matrix4x4_ref.DATA) = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll); Matrix4x4_ref.m_DXMatrix64 = DirectX::XMMatrixTranspose(Matrix4x4_ref.m_DXMatrix64);
#define PF_MATRIX4X4_SetPositionFields(Matrix4x4_ref, x, y, z)					Matrix4x4_ref.m14 = x; Matrix4x4_ref.m24 = y; Matrix4x4_ref.m34 = z;

// Multiply lhs and rhs and save in output
#define PF_MATRIX4X4_MULTIPLY(Matrix4x4_lhs, Matrix4x4_rhs, Matrix4x4_out) Matrix4x4_out.m_DXMatrix64 = DirectX::XMMatrixMultiply(Matrix4x4_rhs.m_DXMatrix64, Matrix4x4_lhs.m_DXMatrix64);



#define PF_MATRIX4X4_TRANSPONSE(Matrix4x4_in, Matrix4x4_out)



namespace ParabellumFramework
{
	//
	// Matrix4x4 - data in memory is placed row by row. Each component is 32bit (512 in total)
	// 
	class XYZ_API PF_ALLIGN(16) Matrix4x4
	{
	public:
		union
		{
			// 512bit DATA - for putting as shader parametr
			struct
			{
				EFLOAT32 m_fields[4][4];
			} DATA;


			// 1D array
			struct
			{
				EFLOAT32 m_matrix1D[16];
			};

			struct
			{
				EFLOAT32 m_matrix[4][4];
			};

			// for SIMD
			struct
			{
				__m128 m1, m2, m3, m4;
			};

			struct
			{
				__m128 m_row[4];
			};

			//
			struct
			{
				EFLOAT32 m11, m12, m13, m14;
				EFLOAT32 m21, m22, m23, m24;
				EFLOAT32 m31, m32, m33, m34;
				EFLOAT32 m41, m42, m43, m44;
			};

			// for DirectX Functions
			struct
			{
				DirectX::XMMATRIX m_DXMatrix64;
			};

			// Vector4D
			struct
			{
				Vector4D row0;
				Vector4D row1;
				Vector4D row2;
				Vector4D row3;
			};

		};

		//
		//	Constructors and destructors
		//
	public:
		Matrix4x4();
		~Matrix4x4();

	private:
		Matrix4x4(Matrix4x4& copy);		// copy constructor forbiden

	public:

		// set matrix fields
		PF_INLINE void PF_FASTCALL Set(
			EFLOAT32 i11, EFLOAT32 i12, EFLOAT32 i13, EFLOAT32 i14,
			EFLOAT32 i21, EFLOAT32 i22, EFLOAT32  i23, EFLOAT32 i24,
			EFLOAT32 i31, EFLOAT32 i32, EFLOAT32 i33, EFLOAT32 i34,
			EFLOAT32 i41, EFLOAT32 i42, EFLOAT32 i43, EFLOAT32 i44
			);


		PF_INLINE void MakeIdentity();
		PF_INLINE static void PF_FASTCALL MakeIdentity(_OUT_ Matrix4x4* pOut);

		PF_INLINE static void PF_FASTCALL MakePerspectiveFieldOfViewLH(_OUT_ Matrix4x4* pOut, EFLOAT32 fov, EFLOAT32 aspect, EFLOAT32 zNear, EFLOAT32 zFar);
		PF_INLINE static void PF_FASTCALL MakePerspectiveFieldOfViewRH(_OUT_ Matrix4x4* pOut, EFLOAT32 fov, EFLOAT32 aspect, EFLOAT32 zNear, EFLOAT32 zFar);
		PF_INLINE void PF_FASTCALL MakePerspectiveFieldOfViewLH(EFLOAT32 fov, EFLOAT32 aspect, EFLOAT32 zNear, EFLOAT32 zFar);
		PF_INLINE void PF_FASTCALL MakePerspectiveFieldOfViewRH(EFLOAT32 fov, EFLOAT32 aspect, EFLOAT32 zNear, EFLOAT32 zFar);

		PF_INLINE void PF_FASTCALL MakeLookAtLH(_IN_ Vector3D* pPosition, _IN_ Vector3D* pLookAt, _IN_ Vector3D* pUp);
		PF_INLINE void PF_FASTCALL MakeOrthoLH(EFLOAT32 width, EFLOAT32 height, EFLOAT32 zNear, EFLOAT32 zFar);



		PF_INLINE static void PF_FASTCALL MakeView(_OUT_ Matrix4x4* pOut, Vector3D& eye, Vector3D& target, Vector3D& up);
		PF_INLINE static void PF_FASTCALL MakeOrtographic(_OUT_ Matrix4x4* pOut);
		PF_INLINE static void PF_FASTCALL MakeTranslation(_OUT_ Matrix4x4* pOut, EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);


		PF_INLINE void PF_FASTCALL MakeTranslation(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);
		PF_INLINE void PF_FASTCALL MakeTranslation(_IN_ Vector3D* input);

		PF_INLINE void PF_FASTCALL MakeScale(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);
		PF_INLINE void PF_FASTCALL MakeScale(_IN_ Vector3D* input);

		PF_INLINE void PF_FASTCALL MakeRotationYawPitchRoll(EFLOAT32 yaw, EFLOAT32 pitch, EFLOAT32 roll);
		PF_INLINE void PF_FASTCALL MakeRotationYawPitchRoll(_IN_ Vector3D* input);

		PF_INLINE void PF_FASTCALL MakeScaleTranslation(_IN_ Vector3D* position, _IN_ Vector3D* scale);


		PF_INLINE static void PF_FASTCALL MakeRotationXAtRH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle);
		PF_INLINE static void PF_FASTCALL MakeRotationYAtRH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle);
		PF_INLINE static void PF_FASTCALL MakeRotationZAtRH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle);
		PF_INLINE static void PF_FASTCALL MakeRotationXAtLH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle);
		PF_INLINE static void PF_FASTCALL MakeRotationYAtLH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle);
		PF_INLINE static void PF_FASTCALL MakeRotationZAtLH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle);

		PF_INLINE static void PF_FASTCALL MakeScale(_OUT_ Matrix4x4* pOut, EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);

		// multiply vector "left" by matrix "right"ans save it in vector "pOut"
		PF_INLINE static void PF_FASTCALL Multiply(_OUT_ Vector3D* pOut, _IN_ Vector3D* left, _IN_ Matrix4x4* rhs);

		// multiply vector "left" by matrix "right"ans save it in vector "left"
		PF_INLINE static void PF_FASTCALL Multiply(_IO_ Vector3D* lhs, _IN_ Matrix4x4* rhs);

		// multiply vector "right" by matrix "left"ans save it in vector "rigt"
		PF_INLINE static void PF_FASTCALL Multiply(_IO_ Matrix4x4* lhs, _IN_ Vector3D* rhs);

		// multiply matrix by another matrix
		PF_INLINE void PF_FASTCALL Multiply(_IN_ Matrix4x4* rhs);

		PF_INLINE static void PF_FASTCALL Multiply(_OUT_ Matrix4x4* pOut, _IN_ Matrix4x4* lhs, _IN_ Matrix4x4* rhs);

		PF_INLINE static void PF_FASTCALL MultiplicationSIMD(_IN_ Matrix4x4* lhs, _IN_ Matrix4x4* rhs, Matrix4x4* _OUT_ pOut);


		PF_INLINE void PF_FASTCALL MultipleRow(EUINT rowNo, EFLOAT32 factor);
		PF_INLINE void PF_FASTCALL SubRows(int noRowToDelete, int noChoosedRow);
		PF_INLINE void PF_FASTCALL SwapRows(int row1, int row2);


		PF_INLINE static Matrix4x4 PF_FASTCALL Transponse(_IN_ Matrix4x4& input);
		PF_INLINE static void PF_FASTCALL Transponse(_IN_ Matrix4x4& input, _OUT_ Matrix4x4& output);

		PF_INLINE static Matrix4x4 PF_FASTCALL Invert(_IN_ Matrix4x4& input);
		PF_INLINE static void PF_FASTCALL Invert(_IN_ Matrix4x4& input, _OUT_ Matrix4x4& output);

		PF_INLINE EFLOAT32 PF_FASTCALL Determinant();
		PF_INLINE static EFLOAT32 PF_FASTCALL Determinant(_IN_ Matrix4x4& input);

		static void PF_FASTCALL Transform(_OUT_ Vector3D* position, _IN_ Matrix4x4& matrix);


		//
		// Gets and Sets
		//
	public:
		PF_INLINE void PF_FASTCALL GetTranslation(_OUT_ Vector3D* pOut);

		// get Position and W component
		PF_INLINE void PF_FASTCALL GetTranslation(_OUT_ Vector4D* pOut);

		PF_INLINE void PF_FASTCALL GetRotation(_OUT_ Vector3D* pOut);
		PF_INLINE void PF_FASTCALL GetScale(_OUT_ Vector3D* pOut);

		PF_INLINE void PF_FASTCALL SetPositionFields(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);
		PF_INLINE void PF_FASTCALL SetPositionFields(_IN_ Vector3D* pVec);

		PF_INLINE void PF_FASTCALL SetScaleFields(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz);
		PF_INLINE void PF_FASTCALL SetScalePositionFields(_IN_ Vector3D* pVec);


		//
		//	ToString
		//
	public:
		std::string ToString();



	//
	//	Operators
	//
	public:

		PF_INLINE _THREAD_ATOMIC_ void operator=(const Matrix4x4& rhs);

		PF_INLINE friend _THREAD_ATOMIC_ Matrix4x4 operator*(Matrix4x4& lhs, Matrix4x4& rhs)
		{
			Matrix4x4 result;
		
			#ifdef PF_MATH_SIMD 
				//result.m_DXMatrix64 = DirectX::XMMatrixMultiply(lhs.m_DXMatrix64, rhs.m_DXMatrix64);
				Matrix4x4::MultiplicationSIMD(&lhs, &rhs, &result);
			#else
				result.m_DXMatrix64 = DirectX::XMMatrixMultiply(lhs.m_DXMatrix64, rhs.m_DXMatrix64);
			#endif
		
			return result;
		}

		// __ATOMIC__
		PF_INLINE friend _THREAD_ATOMIC_ Vector4D operator*(Matrix4x4& lhs, Vector4D& rhs)
		{
			Vector4D result;

			#ifdef PF_MATH_SIMD 
				result.m128 = _mm_mul_ps(lhs.m1, rhs.m128);
				result.m128 = _mm_mul_ps(lhs.m2, rhs.m128);
				result.m128 = _mm_mul_ps(lhs.m3, rhs.m128);
				result.m128 = _mm_mul_ps(lhs.m4, rhs.m128);
			#else
				result.X = lhs.m11 * rhs.X + lhs.m12 * rhs.Y + lhs.m13 * rhs.Z + lhs.m14 * rhs.W;
				result.Y = lhs.m21 * rhs.X + lhs.m22 * rhs.Y + lhs.m23 * rhs.Z + lhs.m24 * rhs.W;
				result.Z = lhs.m31 * rhs.X + lhs.m32 * rhs.Y + lhs.m33 * rhs.Z + lhs.m34 * rhs.W;
				result.W = lhs.m41 * rhs.X + lhs.m42 * rhs.Y + lhs.m43 * rhs.Z + lhs.m44 * rhs.W;
			#endif

			return result;
		}

		// __ATOMIC__
		PF_INLINE friend _THREAD_ATOMIC_ Vector3D operator*(Matrix4x4& lhs, Vector3D& rhs)
		{
			#ifdef PF_MATH_SIMD 
				Vector4D result;
				__m128 mt = _mm_set_ps(rhs.Z, rhs.Z, rhs.Y, rhs.X);
				result.m128 = _mm_mul_ps(lhs.m1, mt);
				result.m128 = _mm_mul_ps(lhs.m2, mt);
				result.m128 = _mm_mul_ps(lhs.m3, mt);
				result.m128 = _mm_mul_ps(lhs.m4, mt);
				return result.XYZ;
			#else
				Vector3D result;
				result.X = lhs.m11 * rhs.X + lhs.m12 * rhs.Y + lhs.m13 * rhs.Z + lhs.m14 * 1.0f;
				result.Y = lhs.m21 * rhs.X + lhs.m22 * rhs.Y + lhs.m23 * rhs.Z + lhs.m24 * 1.0f;
				result.Z = lhs.m31 * rhs.X + lhs.m32 * rhs.Y + lhs.m33 * rhs.Z + lhs.m34 * 1.0f;
				return result;
			#endif

		}


	};


}



#endif