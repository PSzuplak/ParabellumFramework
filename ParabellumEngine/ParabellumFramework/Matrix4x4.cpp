#include "stdafx.h"
#include "Matrix4x4.h"



// ---------------------------------------------------------------------------
//	Constructors and destructors
// ---------------------------------------------------------------------------
ParabellumFramework::Matrix4x4::Matrix4x4()
{
}

ParabellumFramework::Matrix4x4::Matrix4x4(Matrix4x4& copy)
{
	memcpy(&this->DATA, &copy.DATA, sizeof(DATA));
}

ParabellumFramework::Matrix4x4::~Matrix4x4()
{
}


// ---------------------------------------------------------------------------
//	static Matrixes Constructors
// ---------------------------------------------------------------------------


//
//
//	Return pointer to new scale matrix
//
//
void 
ParabellumFramework::Matrix4x4::MakeIdentity()
{
	this->m11 = 1.0; this->m12 = 0.0; this->m13 = 0.0; this->m14 = 0.0;
	this->m21 = 0.0; this->m22 = 1.0; this->m23 = 0.0; this->m24 = 0.0;
	this->m31 = 0.0; this->m32 = 0.0; this->m33 = 1.0; this->m34 = 0.0;
	this->m41 = 0.0; this->m42 = 0.0; this->m43 = 0.0; this->m44 = 1.0;
}



//
//
//	Return pointer to new scale matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeIdentity(_OUT_ Matrix4x4* pOut)
{
	pOut->m11 = 1.0; pOut->m12 = 0.0; pOut->m13 = 0.0; pOut->m14 = 0.0;
	pOut->m21 = 0.0; pOut->m22 = 1.0; pOut->m23 = 0.0; pOut->m24 = 0.0;
	pOut->m31 = 0.0; pOut->m32 = 0.0; pOut->m33 = 1.0; pOut->m34 = 0.0;
	pOut->m41 = 0.0; pOut->m42 = 0.0; pOut->m43 = 0.0; pOut->m44 = 1.0;
}

//
//
//	Takes output matrix and makes its translation matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeTranslation(_OUT_ Matrix4x4* pOut, EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	pOut->m11 = 0.0; pOut->m12 = 0.0; pOut->m13 = 0.0; pOut->m14 = px;
	pOut->m21 = 0.0; pOut->m22 = 0.0; pOut->m23 = 0.0; pOut->m24 = py;
	pOut->m31 = 0.0; pOut->m32 = 0.0; pOut->m33 = 0.0; pOut->m34 = pz;
	pOut->m41 = 0.0; pOut->m42 = 0.0; pOut->m43 = 0.0; pOut->m44 = 1.0;
}


//
//
//	Takes output matrix and makes its rotation matrix over X axis using Right Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationXAtRH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle)
{
	pOut->Set(
		1, 0, 0, 0,
		0, ParabellumUtils::Math::FastCos(angle), -ParabellumUtils::Math::FastSin(angle), 0,
		0, ParabellumUtils::Math::FastSin(angle), ParabellumUtils::Math::FastCos(angle), 0,
		0, 0, 0, 1
		);
}

//
//
//	Takes output matrix and makes its rotation matrix over Y axis using Right Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationYAtRH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle)
{
	pOut->Set(
		ParabellumUtils::Math::FastCos(angle), 0, ParabellumUtils::Math::FastSin(angle), 0,
		0, 1, 0, 0,
		-ParabellumUtils::Math::FastSin(angle), 0, ParabellumUtils::Math::FastCos(angle), 0,
		0, 0, 0, 1
		);
}

//
//
//	Takes output matrix and makes its rotation matrix over Z axis using Right Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationZAtRH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle)
{
	pOut->Set(
		ParabellumUtils::Math::FastCos(angle), -ParabellumUtils::Math::FastSin(angle), 0, 0,
		ParabellumUtils::Math::FastSin(angle), ParabellumUtils::Math::FastCos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}

//
//
//	Takes output matrix and makes its rotation matrix over X axis using Left Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationXAtLH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle)
{
	pOut->Set(
		1, 0, 0, 0,
		0, ParabellumUtils::Math::FastCos(angle), -ParabellumUtils::Math::FastSin(angle), 0,
		0, ParabellumUtils::Math::FastSin(angle), ParabellumUtils::Math::FastCos(angle), 0,
		0, 0, 0, 1
		);
}

//
//
//	Takes output matrix and makes its rotation matrix over Y axis using Left Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationYAtLH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle)
{
	pOut->Set(
		ParabellumUtils::Math::FastCos(angle), 0, ParabellumUtils::Math::FastSin(angle), 0,
		0, 1, 0, 0,
		-ParabellumUtils::Math::FastSin(angle), 0, ParabellumUtils::Math::FastCos(angle), 0,
		0, 0, 0, 1
		);
}

//
//
//	Takes output matrix and makes its rotation matrix over Z axis using Left Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationZAtLH(_OUT_ Matrix4x4* pOut, EFLOAT32 angle)
{
	pOut->Set(
		ParabellumUtils::Math::FastCos(angle), ParabellumUtils::Math::FastSin(angle), 0, 0,
		-ParabellumUtils::Math::FastSin(angle), ParabellumUtils::Math::FastCos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}


//
//
//	Takes output matrix and makes its scale matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeScale(_OUT_ Matrix4x4* pOut, EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	pOut->m11 = px;	pOut->m12 = 0.0;	pOut->m13 = 0.0;	pOut->m14 = 0.0;
	pOut->m21 = 0.0;	pOut->m22 = py;	pOut->m23 = 0.0;	pOut->m24 = 0.0;
	pOut->m31 = 0.0;	pOut->m32 = 0.0;	pOut->m33 = pz;	pOut->m34 = 0.0;
	pOut->m41 = 0.0;	 pOut->m42 = 0.0;	pOut->m43 = 0.0;	pOut->m44 = 1.0;
}


//
//
//	Set matrix
//
//
void
ParabellumFramework::Matrix4x4::Set(
	EFLOAT32 i11, EFLOAT32 i12, EFLOAT32 i13, EFLOAT32 i14,
	EFLOAT32 i21, EFLOAT32 i22, EFLOAT32  i23, EFLOAT32 i24,
	EFLOAT32 i31, EFLOAT32 i32, EFLOAT32 i33, EFLOAT32 i34,
	EFLOAT32 i41, EFLOAT32 i42, EFLOAT32 i43, EFLOAT32 i44
	)
{
	m11 = i11; m21 = i21; m31 = i31; m41 = i41;
	m12 = i12; m22 = i22; m32 = i32; m42 = i42;
	m13 = i13; m23 = i23; m33 = i33; m43 = i43;
	m14 = i14; m24 = i24; m34 = i34; m44 = i44;

	//memcpy(m_matrix1D, &i11, 64);
}


//
//
//	Take matrix and makes it Perspective Field of View matrix using Left Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakePerspectiveFieldOfViewLH(
	_OUT_ Matrix4x4* pOut, EFLOAT32 FieldOfView, EFLOAT32 aspectRatio, EFLOAT32 zNear, EFLOAT32 zFar)
{
	pOut->m_DXMatrix64 = DirectX::XMMatrixPerspectiveFovLH(FieldOfView, aspectRatio, zNear, zFar);
}


//
//
//	Take matrix and makes it Perspective Field of View matrix using Right Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakePerspectiveFieldOfViewRH(
	_OUT_ Matrix4x4* pOut, EFLOAT32 FieldOfView, EFLOAT32 aspectRatio, EFLOAT32 zNear, EFLOAT32 zFar)
{
	//D3DXMatrixPerspectiveFovRH(&pOut->m_DXMatrix64, FieldOfView, aspectRatio, zNear, zFar);
}


//
//
//	Makes matrix Perspective Field of View matrix using Left Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakePerspectiveFieldOfViewLH(
	EFLOAT32 FieldOfView, EFLOAT32 aspectRatio, EFLOAT32 zn, EFLOAT32 zf)
{
	EFLOAT32 yScale = ParabellumUtils::Math::FastCot(FieldOfView * 0.5f);
	EFLOAT32 xScale = yScale / aspectRatio;

	m11 = xScale;	m21 = 0.0f;		m31 = 0.0f;					m41 = 0.0f;
	m12 = 0.0f;		m22 = yScale;	m32 = 0.0f;					m42 = 0.0f;
	m13 = 0.0f;		m23 = 0.0f;		m33 = zf / (zf - zn);		m43 = 1.0f;
	m14 = 0.0f;		m24 = 0.0f;		m34 = -zn*zf / (zf - zn);	m44 = 0.0f;
}


//
//
//	Makes matrix Perspective Field of View matrix using Right Handed coordinates
//
//
void
ParabellumFramework::Matrix4x4::MakePerspectiveFieldOfViewRH(
	EFLOAT32 FieldOfView, EFLOAT32 aspectRatio, EFLOAT32 zNear, EFLOAT32 zFar)
{
	m_DXMatrix64 = DirectX::XMMatrixPerspectiveFovRH(FieldOfView, aspectRatio, zNear, zFar);
}




//
//
//	Makes LookAt Matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeLookAtLH(_IN_ Vector3D* pPosition, _IN_ Vector3D* pLookAt, _IN_ Vector3D* pUp)
{

	Vector3D zaxis = (*pPosition) - (*pLookAt);    // forward
	zaxis.X = -zaxis.X;
	zaxis.Y = -zaxis.Y;
	zaxis.Z = -zaxis.Z;
	zaxis.Normalize();

	Vector3D xaxis;// = normal(cross(up, zaxis));// left
	Vector3D::Cross(&xaxis, pUp, &zaxis);
	xaxis.Normalize();


	Vector3D yaxis;// = cross(zaxis, xaxis);     // up
	Vector3D::Cross(&yaxis, &zaxis, &xaxis);
	yaxis.Normalize();

	EFLOAT32 dotXE = -Vector3D::Dot(&xaxis, pPosition);
	EFLOAT32 dotYE = -Vector3D::Dot(&yaxis, pPosition);
	EFLOAT32 dotZE = -Vector3D::Dot(&zaxis, pPosition);

	m11 = xaxis.X;		m21 = yaxis.X;		m31 = zaxis.X;		m41 = 0.0f;
	m12 = xaxis.Y;		m22 = yaxis.Y;		m32 = zaxis.Y;		m42 = 0.0f;
	m13 = xaxis.Z;		m23 = yaxis.Z;		m33 = zaxis.Z;		m43 = 0.0f;
	m14 = dotXE;		m24 = dotYE;		m34 = dotZE;		m44 = 1.0f;


	//D3DXMatrixLookAtLH(&m_DXMatrix64, &pPosition->DXVector3, &pLookAt->DXVector3, &pUp->DXVector3);
	//D3DXMatrixTranspose(&m_DXMatrix64, &m_DXMatrix64);
}


//
//
//	Makes Otographic matrix for Left Handed Coordiantes
//
//
void
ParabellumFramework::Matrix4x4::MakeOrthoLH(EFLOAT32 width, EFLOAT32 height, EFLOAT32 zn, EFLOAT32 zf)
{
	/*
	m11 = 0.5f*width;	m21 = 0.0f;			m31 = 0.0f;				m41 = 0.0f;
	m12 = 0.0f;			m22 = 0.5f*height;	m32 = 0.0f;				m42 = 0.0f;
	m13 = 0.0f;			m23 = 0.0f;			m33 = 1/(zf-zn);		m43 = 0.0f;
	m14 = 0.0f;			m24 = 0.0f;			m34 = -zn / (zf - zn);	m44 = 1.0f;
	*/

	m_DXMatrix64 = DirectX::XMMatrixOrthographicLH(width, height, zn, zf);
	m_DXMatrix64 = DirectX::XMMatrixTranspose(m_DXMatrix64);
}










//
//
//
//
//
void
ParabellumFramework::Matrix4x4::MakeView(_OUT_ Matrix4x4* pOut, Vector3D& eye, Vector3D& target, Vector3D& up)
{
	Vector3D zaxis;	// forward vector
	Vector3D xaxis;	// right vector
	Vector3D yaxis;	// up vector

	Matrix4x4 orientationMatrix;		// orientation and traslation matrices for calculate view matrix
	Matrix4x4 translationMatrix;

	zaxis = eye + target;
	zaxis.Normalize();

	Vector3D::Cross(&up, &zaxis, &xaxis);
	xaxis.Normalize();

	Vector3D::Cross(&zaxis, &xaxis, &yaxis);

	orientationMatrix.Set(
		xaxis.X, yaxis.X, zaxis.X, 0,
		xaxis.Y, yaxis.Y, zaxis.Y, 0,
		xaxis.Z, yaxis.Z, zaxis.Z, 0,
		0, 0, 0, 1
		);

	translationMatrix.Set(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.X, -eye.Y, -eye.Z, 1
		);

	MultiplicationSIMD(&orientationMatrix, &translationMatrix, pOut);
}
















//
//
//	Make translation matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeTranslation(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	this->m11 = 1.0; this->m12 = 0.0; this->m13 = 0.0; this->m14 = px;
	this->m21 = 0.0; this->m22 = 1.0; this->m23 = 0.0; this->m24 = py;
	this->m31 = 0.0; this->m32 = 0.0; this->m33 = 1.0; this->m34 = pz;
	this->m41 = 0.0; this->m42 = 0.0; this->m43 = 0.0; this->m44 = 1.0;

	//D3DXMatrixTranslation(&m_DXMatrix64, py, py, pz);

}


//
//
//	Make translation matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeTranslation(_IN_ Vector3D* input)
{
	this->m11 = 1.0; this->m12 = 0.0; this->m13 = 0.0; this->m14 = input->X;
	this->m21 = 0.0; this->m22 = 1.0; this->m23 = 0.0; this->m24 = input->Y;
	this->m31 = 0.0; this->m32 = 0.0; this->m33 = 1.0; this->m34 = input->Z;
	this->m41 = 0.0; this->m42 = 0.0; this->m43 = 0.0; this->m44 = 1.0;
}





//
//
//	Make scale matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeScale(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	this->m11 = px;	 this->m12 = 0.0; this->m13 = 0.0; this->m14 = 0.0;
	this->m21 = 0.0; this->m22 = py;  this->m23 = 0.0; this->m24 = 0.0;
	this->m31 = 0.0; this->m32 = 0.0; this->m33 = pz;  this->m34 = 0.0;
	this->m41 = 0.0; this->m42 = 0.0; this->m43 = 0.0; this->m44 = 1.0;
}



//
//
//	Make scale matrix
//
//
void
ParabellumFramework::Matrix4x4::MakeScale(_IN_ Vector3D* input)
{
	this->m11 = input->X;	this->m12 = 0.0;		this->m13 = 0.0;		this->m14 = 0.0;
	this->m21 = 0.0;		this->m22 = input->Y;	this->m23 = 0.0;		this->m24 = 0.0;
	this->m31 = 0.0;		this->m32 = 0.0;		this->m33 = input->Z;	this->m34 = 0.0;
	this->m41 = 0.0;		this->m42 = 0.0;		this->m43 = 0.0;		this->m44 = 1.0;
}





//
//
//	Make rotation matrix from Yaw/Pitch/Roll
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationYawPitchRoll(EFLOAT32 yaw, EFLOAT32 pitch, EFLOAT32 roll)
{
	
	//EFLOAT32 sinY = Math::FastCos(yaw);
	//EFLOAT32 sinP = Math::FastCos(pitch);
	//EFLOAT32 sinR = Math::FastCos(roll);
	//
	//EFLOAT32 cosY = Math::FastCos(yaw);
	//EFLOAT32 cosP = Math::FastCos(pitch);
	//EFLOAT32 cosR = Math::FastCos(roll);
	//
	//m11 = cosY*cosP;	m12 = cosY*sinP*sinR - sinY*cosR;	m13 = cosY*sinP*cosR + sinY*sinR;	m14 = 0.0f;
	//m21 = sinY*cosP;	m22 = sinY*sinP*sinR + cosY*cosR;	m23 = sinY*sinP*cosR - cosY*sinR;	m24 = 0.0f;
	//m31 = -sinY;		m32 = cosP*sinR;					m33 = cosP*cosR;					m34 = 0.0f;
	//m41 = 0.0f;		m42 = 0.0f;							m43 = 0.0f;							m44 = 1.0f;

	m_DXMatrix64 = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	m_DXMatrix64 = DirectX::XMMatrixTranspose(m_DXMatrix64);
}



//
//
//	Make rotation matrix from Yaw/Pitch/Roll
//
//
void
ParabellumFramework::Matrix4x4::MakeRotationYawPitchRoll(_IN_ Vector3D* input)
{
	m_DXMatrix64 = DirectX::XMMatrixRotationRollPitchYaw(input->Y, input->X, input->Z);
}


void
ParabellumFramework::Matrix4x4::MakeScaleTranslation(_IN_ Vector3D* position, _IN_ Vector3D* scale)
{
	m11 = scale->X;		m12 = 0.0f;			m13 = 0.0f;			m14 = position->X;
	m21 = 0.0f;			m22 = scale->Y;		m23 = 0.0f;			m24 = position->Y;
	m31 = 0.0f;			m32 = 0.0f;			m33 = scale->Z;		m34 = position->Z;
	m41 = 0.0f;			m42 = 0.0f;			m43 = 0.0f;			m44 = 1.0f;
}



void 
ParabellumFramework::Matrix4x4::Multiply(_OUT_ Vector3D* pOut, _IN_ Vector3D* lhs, _IN_ Matrix4x4* rhs)
{
	EFLOAT32 result[4];

	for (int i = 0; i < 4; ++i)
		result[i] = lhs->data[0] * rhs->m_matrix[0][i] + lhs->data[1] * rhs->m_matrix[1][i] + lhs->data[2] + rhs->m_matrix[2][i] + lhs->data[3] * rhs->m_matrix[3][i];

	pOut->data[0] = result[0] / result[3];
	pOut->data[1] = result[1] / result[3];
	pOut->data[2] = result[2] / result[3];
}


void 
ParabellumFramework::Matrix4x4::Multiply(_IO_ Vector3D* lhs, _IN_ Matrix4x4* rhs)
{
	EFLOAT32 result[3];

	result[0] = lhs->X;
	result[1] = lhs->Y;
	result[2] = lhs->Z;

	lhs->X = (result[0] * rhs->m11) + (result[1] * rhs->m21) + (result[2] * rhs->m31) + rhs->m41;
	lhs->Y = (result[0] * rhs->m12) + (result[1] * rhs->m22) + (result[2] * rhs->m32) + rhs->m42;
	lhs->Z = (result[0] * rhs->m13) + (result[1] * rhs->m23) + (result[2] * rhs->m33) + rhs->m43;
}


void
ParabellumFramework::Matrix4x4::Multiply(_IO_ Matrix4x4* lhs, _IN_ Vector3D* rhs)
{
	Vector3D sv;

	sv.X = lhs->m11 * rhs->X + lhs->m12 * rhs->Y + lhs->m13 * rhs->Z;
	sv.Y = lhs->m21 * rhs->X + lhs->m22 * rhs->Y + lhs->m23 * rhs->Z;
	sv.Z = lhs->m31 * rhs->X + lhs->m32 * rhs->Y + lhs->m33 * rhs->Z;

	*rhs = sv;
}


//
//
//	Matrix multiplication using SIMD
//
//
void
ParabellumFramework::Matrix4x4::Multiply(_IN_ Matrix4x4* rhs)
{
	this->m_DXMatrix64 = DirectX::XMMatrixMultiply(this->m_DXMatrix64, rhs->m_DXMatrix64);
}



//
//
//	Matrix multiplication using SIMD
//
//
void
ParabellumFramework::Matrix4x4::Multiply(_OUT_ Matrix4x4* pOut, _IN_ Matrix4x4* lhs, _IN_ Matrix4x4* rhs)
{
	pOut->m_DXMatrix64 = DirectX::XMMatrixMultiply(lhs->m_DXMatrix64, rhs->m_DXMatrix64);
}






//
//
//	Matrix multiplication using SIMD
//
//
void
ParabellumFramework::Matrix4x4::MultiplicationSIMD(_IN_ Matrix4x4* lhs, _IN_ Matrix4x4* rhs, _OUT_ Matrix4x4* out)
{
	__m128 a = rhs->m1;
	__m128 b = rhs->m2;
	__m128 c = rhs->m3;
	__m128 d = rhs->m4;

	__m128 t1, t2;

	t1 = _mm_set1_ps(lhs->m11);
	t2 = _mm_mul_ps(a, t1);
	t1 = _mm_set1_ps(lhs->m12);
	t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
	t1 = _mm_set1_ps(lhs->m13);
	t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
	t1 = _mm_set1_ps(lhs->m14);
	t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

	//_mm_store_ps(&out->m_matrix1D[0], t2);
	out->m1 = t2;

	t1 = _mm_set1_ps(lhs->m21);
	t2 = _mm_mul_ps(a, t1);
	t1 = _mm_set1_ps(lhs->m22);
	t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
	t1 = _mm_set1_ps(lhs->m23);
	t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
	t1 = _mm_set1_ps(lhs->m24);
	t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

	//_mm_store_ps(&out->m_matrix1D[4], t2);
	out->m2 = t2;

	t1 = _mm_set1_ps(lhs->m31);
	t2 = _mm_mul_ps(a, t1);
	t1 = _mm_set1_ps(lhs->m32);
	t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
	t1 = _mm_set1_ps(lhs->m33);
	t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
	t1 = _mm_set1_ps(lhs->m34);
	t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

	//_mm_store_ps(&out->m_matrix1D[8], t2);
	out->m3 = t2;

	t1 = _mm_set1_ps(lhs->m41);
	t2 = _mm_mul_ps(a, t1);
	t1 = _mm_set1_ps(lhs->m42);
	t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
	t1 = _mm_set1_ps(lhs->m43);
	t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
	t1 = _mm_set1_ps(lhs->m44);
	t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

	//_mm_store_ps(&out->m_matrix1D[12], t2);
	out->m4 = t2;
}


// ---------------------------------------------------------------------------
//	Matrix operations
// ---------------------------------------------------------------------------

//
//
//	Multiple some row by some number
//
//
void
ParabellumFramework::Matrix4x4::MultipleRow(EUINT rowNo, EFLOAT32 factor)
{
	m_matrix[0][rowNo] *= factor;
	m_matrix[1][rowNo] *= factor;
	m_matrix[2][rowNo] *= factor;
	m_matrix[3][rowNo] *= factor;
}


//
//
//	Sub some row from another row
//
//
void
ParabellumFramework::Matrix4x4::SubRows(int noRowToDelete, int noChoosedRow)
{
	m_matrix[0][noRowToDelete] -= m_matrix[0][noChoosedRow];
	m_matrix[1][noRowToDelete] -= m_matrix[1][noChoosedRow];
	m_matrix[2][noRowToDelete] -= m_matrix[2][noChoosedRow];
	m_matrix[3][noRowToDelete] -= m_matrix[3][noChoosedRow];
}


//
//
//	Swap 2 rows
//
//
//
void
ParabellumFramework::Matrix4x4::SwapRows(int row1, int row2)
{
	EFLOAT32 tmp;

	tmp = m_matrix[0][row1];
	m_matrix[0][row1] = m_matrix[0][row2];
	m_matrix[0][row2] = tmp;

	tmp = m_matrix[1][row1];
	m_matrix[1][row1] = m_matrix[1][row2];
	m_matrix[1][row2] = tmp;

	tmp = m_matrix[2][row1];
	m_matrix[2][row1] = m_matrix[2][row2];
	m_matrix[2][row2] = tmp;

	tmp = m_matrix[3][row1];
	m_matrix[3][row1] = m_matrix[3][row2];
	m_matrix[3][row2] = tmp;
}




//
//
//	Transponse Matrix
//
//
ParabellumFramework::Matrix4x4
ParabellumFramework::Matrix4x4::Transponse(_IN_ Matrix4x4& input)
{
	Matrix4x4 sm;

	sm.m11 = input.m11;	sm.m12 = input.m21;	sm.m13 = input.m31;	sm.m14 = input.m41;
	sm.m21 = input.m12;	sm.m22 = input.m22;	sm.m23 = input.m32;	sm.m24 = input.m42;
	sm.m31 = input.m13;	sm.m32 = input.m23;	sm.m33 = input.m33;	sm.m34 = input.m43;
	sm.m41 = input.m14;	sm.m42 = input.m24;	sm.m43 = input.m34;	sm.m44 = input.m44;

	return sm;
}


void
ParabellumFramework::Matrix4x4::Transponse(_IN_ Matrix4x4& input, _OUT_ Matrix4x4& output)
{
	output.m11 = input.m11;	output.m12 = input.m21;	output.m13 = input.m31;	output.m14 = input.m41;
	output.m21 = input.m12;	output.m22 = input.m22;	output.m23 = input.m32;	output.m24 = input.m42;
	output.m31 = input.m13;	output.m32 = input.m23;	output.m33 = input.m33;	output.m34 = input.m43;
	output.m41 = input.m14;	output.m42 = input.m24;	output.m43 = input.m34;	output.m44 = input.m44;
}


//
//
//	Invert Matrix
//
//
ParabellumFramework::Matrix4x4
ParabellumFramework::Matrix4x4::Invert(_IN_ Matrix4x4& input)
{
	Matrix4x4 sm;

	sm.m_matrix1D[0] = input.m_matrix1D[5] * input.m_matrix1D[10] * input.m_matrix1D[15] -
		input.m_matrix1D[5] * input.m_matrix1D[11] * input.m_matrix1D[14] -
		input.m_matrix1D[9] * input.m_matrix1D[6] * input.m_matrix1D[15] +
		input.m_matrix1D[9] * input.m_matrix1D[7] * input.m_matrix1D[14] +
		input.m_matrix1D[13] * input.m_matrix1D[6] * input.m_matrix1D[11] -
		input.m_matrix1D[13] * input.m_matrix1D[7] * input.m_matrix1D[10];

	sm.m_matrix1D[4] = -input.m_matrix1D[4] * input.m_matrix1D[10] * input.m_matrix1D[15] +
		input.m_matrix1D[4] * input.m_matrix1D[11] * input.m_matrix1D[14] +
		input.m_matrix1D[8] * input.m_matrix1D[6] * input.m_matrix1D[15] -
		input.m_matrix1D[8] * input.m_matrix1D[7] * input.m_matrix1D[14] -
		input.m_matrix1D[12] * input.m_matrix1D[6] * input.m_matrix1D[11] +
		input.m_matrix1D[12] * input.m_matrix1D[7] * input.m_matrix1D[10];

	sm.m_matrix1D[8] = input.m_matrix1D[4] * input.m_matrix1D[9] * input.m_matrix1D[15] -
		input.m_matrix1D[4] * input.m_matrix1D[11] * input.m_matrix1D[13] -
		input.m_matrix1D[8] * input.m_matrix1D[5] * input.m_matrix1D[15] +
		input.m_matrix1D[8] * input.m_matrix1D[7] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[5] * input.m_matrix1D[11] -
		input.m_matrix1D[12] * input.m_matrix1D[7] * input.m_matrix1D[9];

	sm.m_matrix1D[12] = -input.m_matrix1D[4] * input.m_matrix1D[9] * input.m_matrix1D[14] +
		input.m_matrix1D[4] * input.m_matrix1D[10] * input.m_matrix1D[13] +
		input.m_matrix1D[8] * input.m_matrix1D[5] * input.m_matrix1D[14] -
		input.m_matrix1D[8] * input.m_matrix1D[6] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[5] * input.m_matrix1D[10] +
		input.m_matrix1D[12] * input.m_matrix1D[6] * input.m_matrix1D[9];

	sm.m_matrix1D[1] = -input.m_matrix1D[1] * input.m_matrix1D[10] * input.m_matrix1D[15] +
		input.m_matrix1D[1] * input.m_matrix1D[11] * input.m_matrix1D[14] +
		input.m_matrix1D[9] * input.m_matrix1D[2] * input.m_matrix1D[15] -
		input.m_matrix1D[9] * input.m_matrix1D[3] * input.m_matrix1D[14] -
		input.m_matrix1D[13] * input.m_matrix1D[2] * input.m_matrix1D[11] +
		input.m_matrix1D[13] * input.m_matrix1D[3] * input.m_matrix1D[10];

	sm.m_matrix1D[5] = input.m_matrix1D[0] * input.m_matrix1D[10] * input.m_matrix1D[15] -
		input.m_matrix1D[0] * input.m_matrix1D[11] * input.m_matrix1D[14] -
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[15] +
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[14] +
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[11] -
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[10];

	sm.m_matrix1D[9] = -input.m_matrix1D[0] * input.m_matrix1D[9] * input.m_matrix1D[15] +
		input.m_matrix1D[0] * input.m_matrix1D[11] * input.m_matrix1D[13] +
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[15] -
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[11] +
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[9];

	sm.m_matrix1D[13] = input.m_matrix1D[0] * input.m_matrix1D[9] * input.m_matrix1D[14] -
		input.m_matrix1D[0] * input.m_matrix1D[10] * input.m_matrix1D[13] -
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[14] +
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[10] -
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[9];

	sm.m_matrix1D[2] = input.m_matrix1D[1] * input.m_matrix1D[6] * input.m_matrix1D[15] -
		input.m_matrix1D[1] * input.m_matrix1D[7] * input.m_matrix1D[14] -
		input.m_matrix1D[5] * input.m_matrix1D[2] * input.m_matrix1D[15] +
		input.m_matrix1D[5] * input.m_matrix1D[3] * input.m_matrix1D[14] +
		input.m_matrix1D[13] * input.m_matrix1D[2] * input.m_matrix1D[7] -
		input.m_matrix1D[13] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[6] = -input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[15] +
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[14] +
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[15] -
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[14] -
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[7] +
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[10] = input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[15] -
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[13] -
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[15] +
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[7] -
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[5];

	sm.m_matrix1D[14] = -input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[14] +
		input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[13] +
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[14] -
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[6] +
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[5];

	sm.m_matrix1D[3] = -input.m_matrix1D[1] * input.m_matrix1D[6] * input.m_matrix1D[11] +
		input.m_matrix1D[1] * input.m_matrix1D[7] * input.m_matrix1D[10] +
		input.m_matrix1D[5] * input.m_matrix1D[2] * input.m_matrix1D[11] -
		input.m_matrix1D[5] * input.m_matrix1D[3] * input.m_matrix1D[10] -
		input.m_matrix1D[9] * input.m_matrix1D[2] * input.m_matrix1D[7] +
		input.m_matrix1D[9] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[7] = input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[11] -
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[10] -
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[11] +
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[10] +
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[7] -
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[11] = -input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[11] +
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[9] +
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[11] -
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[9] -
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[7] +
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[5];

	sm.m_matrix1D[15] = input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[10] -
		input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[9] -
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[10] +
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[9] +
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[6] -
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[5];

	EFLOAT det = input.m_matrix1D[0] * sm.m_matrix1D[0] + input.m_matrix1D[1] * sm.m_matrix1D[4] + input.m_matrix1D[2] * sm.m_matrix1D[8] + input.m_matrix1D[3] * sm.m_matrix1D[12];
	det = 1.0f / det;

	#ifdef PF_MATH_SIMD 
		sm.m1 = _mm_mul_ps(sm.m1, _mm_set1_ps(det));
		sm.m2 = _mm_mul_ps(sm.m2, _mm_set1_ps(det));
		sm.m3 = _mm_mul_ps(sm.m3, _mm_set1_ps(det));
		sm.m4 = _mm_mul_ps(sm.m4, _mm_set1_ps(det));
	#else
		for (EUINT32 i = 0; i < 16; i++)
			sm.m_matrix1D[i] = sm.m_matrix1D[i] * det;
	#endif


	return sm;
}


void
ParabellumFramework::Matrix4x4::Invert(_IN_ Matrix4x4& input, _OUT_ Matrix4x4& output)
{
	output.m_matrix1D[0] = input.m_matrix1D[5] * input.m_matrix1D[10] * input.m_matrix1D[15] -
		input.m_matrix1D[5] * input.m_matrix1D[11] * input.m_matrix1D[14] -
		input.m_matrix1D[9] * input.m_matrix1D[6] * input.m_matrix1D[15] +
		input.m_matrix1D[9] * input.m_matrix1D[7] * input.m_matrix1D[14] +
		input.m_matrix1D[13] * input.m_matrix1D[6] * input.m_matrix1D[11] -
		input.m_matrix1D[13] * input.m_matrix1D[7] * input.m_matrix1D[10];

	output.m_matrix1D[4] = -input.m_matrix1D[4] * input.m_matrix1D[10] * input.m_matrix1D[15] +
		input.m_matrix1D[4] * input.m_matrix1D[11] * input.m_matrix1D[14] +
		input.m_matrix1D[8] * input.m_matrix1D[6] * input.m_matrix1D[15] -
		input.m_matrix1D[8] * input.m_matrix1D[7] * input.m_matrix1D[14] -
		input.m_matrix1D[12] * input.m_matrix1D[6] * input.m_matrix1D[11] +
		input.m_matrix1D[12] * input.m_matrix1D[7] * input.m_matrix1D[10];

	output.m_matrix1D[8] = input.m_matrix1D[4] * input.m_matrix1D[9] * input.m_matrix1D[15] -
		input.m_matrix1D[4] * input.m_matrix1D[11] * input.m_matrix1D[13] -
		input.m_matrix1D[8] * input.m_matrix1D[5] * input.m_matrix1D[15] +
		input.m_matrix1D[8] * input.m_matrix1D[7] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[5] * input.m_matrix1D[11] -
		input.m_matrix1D[12] * input.m_matrix1D[7] * input.m_matrix1D[9];

	output.m_matrix1D[12] = -input.m_matrix1D[4] * input.m_matrix1D[9] * input.m_matrix1D[14] +
		input.m_matrix1D[4] * input.m_matrix1D[10] * input.m_matrix1D[13] +
		input.m_matrix1D[8] * input.m_matrix1D[5] * input.m_matrix1D[14] -
		input.m_matrix1D[8] * input.m_matrix1D[6] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[5] * input.m_matrix1D[10] +
		input.m_matrix1D[12] * input.m_matrix1D[6] * input.m_matrix1D[9];

	output.m_matrix1D[1] = -input.m_matrix1D[1] * input.m_matrix1D[10] * input.m_matrix1D[15] +
		input.m_matrix1D[1] * input.m_matrix1D[11] * input.m_matrix1D[14] +
		input.m_matrix1D[9] * input.m_matrix1D[2] * input.m_matrix1D[15] -
		input.m_matrix1D[9] * input.m_matrix1D[3] * input.m_matrix1D[14] -
		input.m_matrix1D[13] * input.m_matrix1D[2] * input.m_matrix1D[11] +
		input.m_matrix1D[13] * input.m_matrix1D[3] * input.m_matrix1D[10];

	output.m_matrix1D[5] = input.m_matrix1D[0] * input.m_matrix1D[10] * input.m_matrix1D[15] -
		input.m_matrix1D[0] * input.m_matrix1D[11] * input.m_matrix1D[14] -
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[15] +
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[14] +
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[11] -
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[10];

	output.m_matrix1D[9] = -input.m_matrix1D[0] * input.m_matrix1D[9] * input.m_matrix1D[15] +
		input.m_matrix1D[0] * input.m_matrix1D[11] * input.m_matrix1D[13] +
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[15] -
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[11] +
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[9];

	output.m_matrix1D[13] = input.m_matrix1D[0] * input.m_matrix1D[9] * input.m_matrix1D[14] -
		input.m_matrix1D[0] * input.m_matrix1D[10] * input.m_matrix1D[13] -
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[14] +
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[10] -
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[9];

	output.m_matrix1D[2] = input.m_matrix1D[1] * input.m_matrix1D[6] * input.m_matrix1D[15] -
		input.m_matrix1D[1] * input.m_matrix1D[7] * input.m_matrix1D[14] -
		input.m_matrix1D[5] * input.m_matrix1D[2] * input.m_matrix1D[15] +
		input.m_matrix1D[5] * input.m_matrix1D[3] * input.m_matrix1D[14] +
		input.m_matrix1D[13] * input.m_matrix1D[2] * input.m_matrix1D[7] -
		input.m_matrix1D[13] * input.m_matrix1D[3] * input.m_matrix1D[6];

	output.m_matrix1D[6] = -input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[15] +
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[14] +
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[15] -
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[14] -
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[7] +
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[6];

	output.m_matrix1D[10] = input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[15] -
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[13] -
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[15] +
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[7] -
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[5];

	output.m_matrix1D[14] = -input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[14] +
		input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[13] +
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[14] -
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[6] +
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[5];

	output.m_matrix1D[3] = -input.m_matrix1D[1] * input.m_matrix1D[6] * input.m_matrix1D[11] +
		input.m_matrix1D[1] * input.m_matrix1D[7] * input.m_matrix1D[10] +
		input.m_matrix1D[5] * input.m_matrix1D[2] * input.m_matrix1D[11] -
		input.m_matrix1D[5] * input.m_matrix1D[3] * input.m_matrix1D[10] -
		input.m_matrix1D[9] * input.m_matrix1D[2] * input.m_matrix1D[7] +
		input.m_matrix1D[9] * input.m_matrix1D[3] * input.m_matrix1D[6];

	output.m_matrix1D[7] = input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[11] -
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[10] -
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[11] +
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[10] +
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[7] -
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[6];

	output.m_matrix1D[11] = -input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[11] +
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[9] +
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[11] -
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[9] -
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[7] +
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[5];

	output.m_matrix1D[15] = input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[10] -
		input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[9] -
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[10] +
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[9] +
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[6] -
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[5];

	EFLOAT32 det = input.m_matrix1D[0] * output.m_matrix1D[0] + input.m_matrix1D[1] * output.m_matrix1D[4] + input.m_matrix1D[2] * output.m_matrix1D[8] + input.m_matrix1D[3] * output.m_matrix1D[12];
	det = 1.0f / det;

	#ifdef PF_MATH_SIMD 
		output.m1 = _mm_mul_ps(output.m1, _mm_set1_ps(det));
		output.m2 = _mm_mul_ps(output.m2, _mm_set1_ps(det));
		output.m3 = _mm_mul_ps(output.m3, _mm_set1_ps(det));
		output.m4 = _mm_mul_ps(output.m4, _mm_set1_ps(det));
	#else
		for (EUINT32 i = 0; i < 16; i++)
			output.m_matrix1D[i] = output.m_matrix1D[i] * det;
	#endif

}


//
//
//
//
//
ParabellumFramework::EFLOAT32
ParabellumFramework::Matrix4x4::Determinant()
{
	Matrix4x4 sm;

	sm.m_matrix1D[0] = m_matrix1D[5] * m_matrix1D[10] * m_matrix1D[15] -
		m_matrix1D[5] * m_matrix1D[11] * m_matrix1D[14] -
		m_matrix1D[9] * m_matrix1D[6] * m_matrix1D[15] +
		m_matrix1D[9] * m_matrix1D[7] * m_matrix1D[14] +
		m_matrix1D[13] * m_matrix1D[6] * m_matrix1D[11] -
		m_matrix1D[13] * m_matrix1D[7] * m_matrix1D[10];

	sm.m_matrix1D[4] = -m_matrix1D[4] * m_matrix1D[10] * m_matrix1D[15] +
		m_matrix1D[4] * m_matrix1D[11] * m_matrix1D[14] +
		m_matrix1D[8] * m_matrix1D[6] * m_matrix1D[15] -
		m_matrix1D[8] * m_matrix1D[7] * m_matrix1D[14] -
		m_matrix1D[12] * m_matrix1D[6] * m_matrix1D[11] +
		m_matrix1D[12] * m_matrix1D[7] * m_matrix1D[10];

	sm.m_matrix1D[8] = m_matrix1D[4] * m_matrix1D[9] * m_matrix1D[15] -
		m_matrix1D[4] * m_matrix1D[11] * m_matrix1D[13] -
		m_matrix1D[8] * m_matrix1D[5] * m_matrix1D[15] +
		m_matrix1D[8] * m_matrix1D[7] * m_matrix1D[13] +
		m_matrix1D[12] * m_matrix1D[5] * m_matrix1D[11] -
		m_matrix1D[12] * m_matrix1D[7] * m_matrix1D[9];

	sm.m_matrix1D[12] = -m_matrix1D[4] * m_matrix1D[9] * m_matrix1D[14] +
		m_matrix1D[4] * m_matrix1D[10] * m_matrix1D[13] +
		m_matrix1D[8] * m_matrix1D[5] * m_matrix1D[14] -
		m_matrix1D[8] * m_matrix1D[6] * m_matrix1D[13] -
		m_matrix1D[12] * m_matrix1D[5] * m_matrix1D[10] +
		m_matrix1D[12] * m_matrix1D[6] * m_matrix1D[9];

	sm.m_matrix1D[1] = -m_matrix1D[1] * m_matrix1D[10] * m_matrix1D[15] +
		m_matrix1D[1] * m_matrix1D[11] * m_matrix1D[14] +
		m_matrix1D[9] * m_matrix1D[2] * m_matrix1D[15] -
		m_matrix1D[9] * m_matrix1D[3] * m_matrix1D[14] -
		m_matrix1D[13] * m_matrix1D[2] * m_matrix1D[11] +
		m_matrix1D[13] * m_matrix1D[3] * m_matrix1D[10];

	sm.m_matrix1D[5] = m_matrix1D[0] * m_matrix1D[10] * m_matrix1D[15] -
		m_matrix1D[0] * m_matrix1D[11] * m_matrix1D[14] -
		m_matrix1D[8] * m_matrix1D[2] * m_matrix1D[15] +
		m_matrix1D[8] * m_matrix1D[3] * m_matrix1D[14] +
		m_matrix1D[12] * m_matrix1D[2] * m_matrix1D[11] -
		m_matrix1D[12] * m_matrix1D[3] * m_matrix1D[10];

	sm.m_matrix1D[9] = -m_matrix1D[0] * m_matrix1D[9] * m_matrix1D[15] +
		m_matrix1D[0] * m_matrix1D[11] * m_matrix1D[13] +
		m_matrix1D[8] * m_matrix1D[1] * m_matrix1D[15] -
		m_matrix1D[8] * m_matrix1D[3] * m_matrix1D[13] -
		m_matrix1D[12] * m_matrix1D[1] * m_matrix1D[11] +
		m_matrix1D[12] * m_matrix1D[3] * m_matrix1D[9];

	sm.m_matrix1D[13] = m_matrix1D[0] * m_matrix1D[9] * m_matrix1D[14] -
		m_matrix1D[0] * m_matrix1D[10] * m_matrix1D[13] -
		m_matrix1D[8] * m_matrix1D[1] * m_matrix1D[14] +
		m_matrix1D[8] * m_matrix1D[2] * m_matrix1D[13] +
		m_matrix1D[12] * m_matrix1D[1] * m_matrix1D[10] -
		m_matrix1D[12] * m_matrix1D[2] * m_matrix1D[9];

	sm.m_matrix1D[2] = m_matrix1D[1] * m_matrix1D[6] * m_matrix1D[15] -
		m_matrix1D[1] * m_matrix1D[7] * m_matrix1D[14] -
		m_matrix1D[5] * m_matrix1D[2] * m_matrix1D[15] +
		m_matrix1D[5] * m_matrix1D[3] * m_matrix1D[14] +
		m_matrix1D[13] * m_matrix1D[2] * m_matrix1D[7] -
		m_matrix1D[13] * m_matrix1D[3] * m_matrix1D[6];

	sm.m_matrix1D[6] = -m_matrix1D[0] * m_matrix1D[6] * m_matrix1D[15] +
		m_matrix1D[0] * m_matrix1D[7] * m_matrix1D[14] +
		m_matrix1D[4] * m_matrix1D[2] * m_matrix1D[15] -
		m_matrix1D[4] * m_matrix1D[3] * m_matrix1D[14] -
		m_matrix1D[12] * m_matrix1D[2] * m_matrix1D[7] +
		m_matrix1D[12] * m_matrix1D[3] * m_matrix1D[6];

	sm.m_matrix1D[10] = m_matrix1D[0] * m_matrix1D[5] * m_matrix1D[15] -
		m_matrix1D[0] * m_matrix1D[7] * m_matrix1D[13] -
		m_matrix1D[4] * m_matrix1D[1] * m_matrix1D[15] +
		m_matrix1D[4] * m_matrix1D[3] * m_matrix1D[13] +
		m_matrix1D[12] * m_matrix1D[1] * m_matrix1D[7] -
		m_matrix1D[12] * m_matrix1D[3] * m_matrix1D[5];

	sm.m_matrix1D[14] = -m_matrix1D[0] * m_matrix1D[5] * m_matrix1D[14] +
		m_matrix1D[0] * m_matrix1D[6] * m_matrix1D[13] +
		m_matrix1D[4] * m_matrix1D[1] * m_matrix1D[14] -
		m_matrix1D[4] * m_matrix1D[2] * m_matrix1D[13] -
		m_matrix1D[12] * m_matrix1D[1] * m_matrix1D[6] +
		m_matrix1D[12] * m_matrix1D[2] * m_matrix1D[5];

	sm.m_matrix1D[3] = -m_matrix1D[1] * m_matrix1D[6] * m_matrix1D[11] +
		m_matrix1D[1] * m_matrix1D[7] * m_matrix1D[10] +
		m_matrix1D[5] * m_matrix1D[2] * m_matrix1D[11] -
		m_matrix1D[5] * m_matrix1D[3] * m_matrix1D[10] -
		m_matrix1D[9] * m_matrix1D[2] * m_matrix1D[7] +
		m_matrix1D[9] * m_matrix1D[3] * m_matrix1D[6];

	sm.m_matrix1D[7] = m_matrix1D[0] * m_matrix1D[6] * m_matrix1D[11] -
		m_matrix1D[0] * m_matrix1D[7] * m_matrix1D[10] -
		m_matrix1D[4] * m_matrix1D[2] * m_matrix1D[11] +
		m_matrix1D[4] * m_matrix1D[3] * m_matrix1D[10] +
		m_matrix1D[8] * m_matrix1D[2] * m_matrix1D[7] -
		m_matrix1D[8] * m_matrix1D[3] * m_matrix1D[6];

	sm.m_matrix1D[11] = -m_matrix1D[0] * m_matrix1D[5] * m_matrix1D[11] +
		m_matrix1D[0] * m_matrix1D[7] * m_matrix1D[9] +
		m_matrix1D[4] * m_matrix1D[1] * m_matrix1D[11] -
		m_matrix1D[4] * m_matrix1D[3] * m_matrix1D[9] -
		m_matrix1D[8] * m_matrix1D[1] * m_matrix1D[7] +
		m_matrix1D[8] * m_matrix1D[3] * m_matrix1D[5];

	sm.m_matrix1D[15] = m_matrix1D[0] * m_matrix1D[5] * m_matrix1D[10] -
		m_matrix1D[0] * m_matrix1D[6] * m_matrix1D[9] -
		m_matrix1D[4] * m_matrix1D[1] * m_matrix1D[10] +
		m_matrix1D[4] * m_matrix1D[2] * m_matrix1D[9] +
		m_matrix1D[8] * m_matrix1D[1] * m_matrix1D[6] -
		m_matrix1D[8] * m_matrix1D[2] * m_matrix1D[5];

	EFLOAT det = m_matrix1D[0] * sm.m_matrix1D[0] + m_matrix1D[1] * sm.m_matrix1D[4] + m_matrix1D[2] * sm.m_matrix1D[8] + m_matrix1D[3] * sm.m_matrix1D[12];
	det = 1.0 / det;

	return det;
}



//
//
//
//
//
ParabellumFramework::EFLOAT32
ParabellumFramework::Matrix4x4::Determinant(_IN_ Matrix4x4& input)
{
	Matrix4x4 sm;

	sm.m_matrix1D[0] = input.m_matrix1D[5] * input.m_matrix1D[10] * input.m_matrix1D[15] -
		input.m_matrix1D[5] * input.m_matrix1D[11] * input.m_matrix1D[14] -
		input.m_matrix1D[9] * input.m_matrix1D[6] * input.m_matrix1D[15] +
		input.m_matrix1D[9] * input.m_matrix1D[7] * input.m_matrix1D[14] +
		input.m_matrix1D[13] * input.m_matrix1D[6] * input.m_matrix1D[11] -
		input.m_matrix1D[13] * input.m_matrix1D[7] * input.m_matrix1D[10];

	sm.m_matrix1D[4] = -input.m_matrix1D[4] * input.m_matrix1D[10] * input.m_matrix1D[15] +
		input.m_matrix1D[4] * input.m_matrix1D[11] * input.m_matrix1D[14] +
		input.m_matrix1D[8] * input.m_matrix1D[6] * input.m_matrix1D[15] -
		input.m_matrix1D[8] * input.m_matrix1D[7] * input.m_matrix1D[14] -
		input.m_matrix1D[12] * input.m_matrix1D[6] * input.m_matrix1D[11] +
		input.m_matrix1D[12] * input.m_matrix1D[7] * input.m_matrix1D[10];

	sm.m_matrix1D[8] = input.m_matrix1D[4] * input.m_matrix1D[9] * input.m_matrix1D[15] -
		input.m_matrix1D[4] * input.m_matrix1D[11] * input.m_matrix1D[13] -
		input.m_matrix1D[8] * input.m_matrix1D[5] * input.m_matrix1D[15] +
		input.m_matrix1D[8] * input.m_matrix1D[7] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[5] * input.m_matrix1D[11] -
		input.m_matrix1D[12] * input.m_matrix1D[7] * input.m_matrix1D[9];

	sm.m_matrix1D[12] = -input.m_matrix1D[4] * input.m_matrix1D[9] * input.m_matrix1D[14] +
		input.m_matrix1D[4] * input.m_matrix1D[10] * input.m_matrix1D[13] +
		input.m_matrix1D[8] * input.m_matrix1D[5] * input.m_matrix1D[14] -
		input.m_matrix1D[8] * input.m_matrix1D[6] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[5] * input.m_matrix1D[10] +
		input.m_matrix1D[12] * input.m_matrix1D[6] * input.m_matrix1D[9];

	sm.m_matrix1D[1] = -input.m_matrix1D[1] * input.m_matrix1D[10] * input.m_matrix1D[15] +
		input.m_matrix1D[1] * input.m_matrix1D[11] * input.m_matrix1D[14] +
		input.m_matrix1D[9] * input.m_matrix1D[2] * input.m_matrix1D[15] -
		input.m_matrix1D[9] * input.m_matrix1D[3] * input.m_matrix1D[14] -
		input.m_matrix1D[13] * input.m_matrix1D[2] * input.m_matrix1D[11] +
		input.m_matrix1D[13] * input.m_matrix1D[3] * input.m_matrix1D[10];

	sm.m_matrix1D[5] = input.m_matrix1D[0] * input.m_matrix1D[10] * input.m_matrix1D[15] -
		input.m_matrix1D[0] * input.m_matrix1D[11] * input.m_matrix1D[14] -
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[15] +
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[14] +
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[11] -
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[10];

	sm.m_matrix1D[9] = -input.m_matrix1D[0] * input.m_matrix1D[9] * input.m_matrix1D[15] +
		input.m_matrix1D[0] * input.m_matrix1D[11] * input.m_matrix1D[13] +
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[15] -
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[11] +
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[9];

	sm.m_matrix1D[13] = input.m_matrix1D[0] * input.m_matrix1D[9] * input.m_matrix1D[14] -
		input.m_matrix1D[0] * input.m_matrix1D[10] * input.m_matrix1D[13] -
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[14] +
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[10] -
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[9];

	sm.m_matrix1D[2] = input.m_matrix1D[1] * input.m_matrix1D[6] * input.m_matrix1D[15] -
		input.m_matrix1D[1] * input.m_matrix1D[7] * input.m_matrix1D[14] -
		input.m_matrix1D[5] * input.m_matrix1D[2] * input.m_matrix1D[15] +
		input.m_matrix1D[5] * input.m_matrix1D[3] * input.m_matrix1D[14] +
		input.m_matrix1D[13] * input.m_matrix1D[2] * input.m_matrix1D[7] -
		input.m_matrix1D[13] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[6] = -input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[15] +
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[14] +
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[15] -
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[14] -
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[7] +
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[10] = input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[15] -
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[13] -
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[15] +
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[13] +
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[7] -
		input.m_matrix1D[12] * input.m_matrix1D[3] * input.m_matrix1D[5];

	sm.m_matrix1D[14] = -input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[14] +
		input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[13] +
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[14] -
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[13] -
		input.m_matrix1D[12] * input.m_matrix1D[1] * input.m_matrix1D[6] +
		input.m_matrix1D[12] * input.m_matrix1D[2] * input.m_matrix1D[5];

	sm.m_matrix1D[3] = -input.m_matrix1D[1] * input.m_matrix1D[6] * input.m_matrix1D[11] +
		input.m_matrix1D[1] * input.m_matrix1D[7] * input.m_matrix1D[10] +
		input.m_matrix1D[5] * input.m_matrix1D[2] * input.m_matrix1D[11] -
		input.m_matrix1D[5] * input.m_matrix1D[3] * input.m_matrix1D[10] -
		input.m_matrix1D[9] * input.m_matrix1D[2] * input.m_matrix1D[7] +
		input.m_matrix1D[9] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[7] = input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[11] -
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[10] -
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[11] +
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[10] +
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[7] -
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[6];

	sm.m_matrix1D[11] = -input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[11] +
		input.m_matrix1D[0] * input.m_matrix1D[7] * input.m_matrix1D[9] +
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[11] -
		input.m_matrix1D[4] * input.m_matrix1D[3] * input.m_matrix1D[9] -
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[7] +
		input.m_matrix1D[8] * input.m_matrix1D[3] * input.m_matrix1D[5];

	sm.m_matrix1D[15] = input.m_matrix1D[0] * input.m_matrix1D[5] * input.m_matrix1D[10] -
		input.m_matrix1D[0] * input.m_matrix1D[6] * input.m_matrix1D[9] -
		input.m_matrix1D[4] * input.m_matrix1D[1] * input.m_matrix1D[10] +
		input.m_matrix1D[4] * input.m_matrix1D[2] * input.m_matrix1D[9] +
		input.m_matrix1D[8] * input.m_matrix1D[1] * input.m_matrix1D[6] -
		input.m_matrix1D[8] * input.m_matrix1D[2] * input.m_matrix1D[5];

	EFLOAT det = input.m_matrix1D[0] * sm.m_matrix1D[0] + input.m_matrix1D[1] * sm.m_matrix1D[4] + input.m_matrix1D[2] * sm.m_matrix1D[8] + input.m_matrix1D[3] * sm.m_matrix1D[12];
	det = 1.0 / det;

	return det;
}


void 
ParabellumFramework::Matrix4x4::Transform(_OUT_ Vector3D* position, _IN_ Matrix4x4& matrix)
{
	EFLOAT32 posx = position->X;
	EFLOAT32 posy = position->Y;
	EFLOAT32 posz = position->Z;

	position->X = (posx * matrix.m11) + (posy * matrix.m21) + (posz * matrix.m31) + matrix.m41;
	position->Y = (posx * matrix.m12) + (posy * matrix.m22) + (posz * matrix.m32) + matrix.m42;
	position->Z = (posx * matrix.m13) + (posy * matrix.m23) + (posz * matrix.m33) + matrix.m43;
}


// ---------------------------------------------------------------------------
//	Gets
// ---------------------------------------------------------------------------


//
//
//	Get Translation fields from matrix
//
//
void
ParabellumFramework::Matrix4x4::GetTranslation(_OUT_ Vector3D* pOut)
{
	pOut->X = m14;
	pOut->Y = m24;
	pOut->Z = m34;
}

//
//
//	Get Translation fields from matrix
//
//
void
ParabellumFramework::Matrix4x4::GetTranslation(_OUT_ Vector4D* pOut)
{
	pOut->X = m14;
	pOut->Y = m24;
	pOut->Z = m34;
	pOut->W = m44;
}


//
//
//	Get Yaw,Pitch, Roll from matrix
//
//
void
ParabellumFramework::Matrix4x4::GetRotation(_OUT_ Vector3D* pOut)
{
	if (m11 > 0.9999f && m11 < 1.0001f)
	{
		pOut->X = atan2f(m13, m34);
		pOut->Y = 0.0f;
		pOut->Z = 0.0f;

	}
	else if (m11 < -0.9999f && m11 > -1.0001f)
	{
		pOut->X = atan2f(m13, m34);
		pOut->Y = 0;
		pOut->Z = 0;
	}
	else
	{
		pOut->X = atan2(-m31, m11);
		pOut->Y = asin(m21);
		pOut->Z = atan2(-m23, m22);
	}
}


//
//
//	Set scale from matrix
//
//
void
ParabellumFramework::Matrix4x4::GetScale(_OUT_ Vector3D* pOut)
{
	pOut->X = m11;
	pOut->Y = m22;
	pOut->Z = m33;
}







//
//
//	Set scale from matrix
//
//
void
ParabellumFramework::Matrix4x4::SetPositionFields(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	this->m14 = px;
	this->m24 = py;
	this->m34 = pz;
}

//
//
//	Set scale from matrix
//
//
void
ParabellumFramework::Matrix4x4::SetPositionFields(_IN_ Vector3D* pVec)
{
	this->m14 = pVec->X;
	this->m24 = pVec->Y;
	this->m34 = pVec->Z;
}



//
//
//	Set scale from matrix
//
//
void
ParabellumFramework::Matrix4x4::SetScaleFields(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	this->m11 = px;
	this->m22 = py;
	this->m33 = pz;
}


//
//
//	Set scale from matrix
//
//
void
ParabellumFramework::Matrix4x4::SetScalePositionFields(_IN_ Vector3D* pVec)
{
	this->m11 = pVec->X;
	this->m22 = pVec->Y;
	this->m33 = pVec->Z;
}








// ---------------------------------------------------------------------------
//	ToString
// ---------------------------------------------------------------------------


std::string ParabellumFramework::Matrix4x4::ToString()
{
	static std::string str;
	str = "\n" + std::to_string(m11) + "," + std::to_string(m12) + "," + std::to_string(m13) + "," + std::to_string(m14);
	str += "\n" + std::to_string(m21) + "," + std::to_string(m22) + "," + std::to_string(m23) + "," + std::to_string(m24);
	str += "\n" + std::to_string(m31) + "," + std::to_string(m32) + "," + std::to_string(m33) + "," + std::to_string(m34);
	str += "\n" + std::to_string(m41) + "," + std::to_string(m42) + "," + std::to_string(m43) + "," + std::to_string(m44);
	return str;
}



// ---------------------------------------------------------------------------
//	Operators
// ---------------------------------------------------------------------------

void ParabellumFramework::Matrix4x4::operator=(const Matrix4x4& rhs)
{
	//memcpy(&this->DATA, &right.DATA, sizeof(DATA));
	this->m1 = rhs.m1;
	this->m2 = rhs.m2;
	this->m3 = rhs.m3;
	this->m4 = rhs.m4;
}
