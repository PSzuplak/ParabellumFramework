#include "stdafx.h"
#include "UnitHelper.h"






ParabellumFramework::Math::UnitHelper::UnitHelper::UnitHelper()
{
}

ParabellumFramework::Math::UnitHelper::UnitHelper::~UnitHelper()
{
}




void
ParabellumFramework::Math::UnitHelper::UnitHelper::U32toV4N(EUINT32 in, Vector4D& out)
{
	out.X = (EFLOAT32)((in >> 24)) / 255.0f;
	out.Y = (EFLOAT32)((in >> 16) & 0x000000FF) / 255.0f;
	out.Z = (EFLOAT32)((in >> 8) & 0x000000FF) / 255.0f;
	out.W = (EFLOAT32)(in & 0x000000FF) / 255.0f;
}


void
ParabellumFramework::Math::UnitHelper::UnitHelper::V4NtoU32(Vector4D& in, EUINT32& out)
{
	out =	(((EUINT32)(in.X * 255.0f) & 0x000000FF) << 24) |
			(((EUINT32)(in.Y * 255.0f) & 0x000000FF) << 16) |
			(((EUINT32)(in.Z * 255.0f) & 0x000000FF) << 8)  |
			(((EUINT32)(in.W * 255.0f) & 0x000000FF));
}

ParabellumFramework::EUINT32 
ParabellumFramework::Math::UnitHelper::UnitHelper::U8toU32(EBYTE r, EBYTE g, EBYTE b, EBYTE a)
{
	EUINT32 result = 0;

	result = ((EUINT32)r << 24) | ((EUINT32)g << 16) | ((EUINT32)b << 8) | (EUINT32)a;

	return result;
}

void 
ParabellumFramework::Math::UnitHelper::UnitHelper::U4toV4(
	_OUT_ Vector4D* input, _IN_ EUINT32* outX, _IN_ EUINT32* outY, _IN_ EUINT32* outZ, _IN_ EUINT32* outW)
{
	input->X = *((EFLOAT*)outX);
	input->Y = *((EFLOAT*)outY);
	input->Z = *((EFLOAT*)outZ);
	input->W = *((EFLOAT*)outW);
}

void 
ParabellumFramework::Math::UnitHelper::UnitHelper::V4toU4(
	_IN_ Vector4D* input, _OUT_ EUINT32* outX, _OUT_ EUINT32*outY, _OUT_ EUINT32* outZ, _OUT_ EUINT32* outW)
{
	*outX = *((EUINT32*)(&input->X));
	*outY = *((EUINT32*)(&input->Y));
	*outZ = *((EUINT32*)(&input->Z));
	*outW = *((EUINT32*)(&input->W));
}

ParabellumFramework::EFLOAT32
ParabellumFramework::Math::UnitHelper::UnitHelper::HUEtoRGB(Vector3D& hue)
{
	if (hue.Z < 0.0f)
		hue.Z += 1.0f;

	if (hue.Z > 1.0f)
		hue.Z -= 1.0f;

	if ((6.0f * hue.Z) < 1.0f)
		return (hue.X + (hue.Y - hue.X) * 6.0f * hue.Z);

	if ((2.0f * hue.Z) < 1.0f)
		return hue.Y;

	if ((3.0f * hue.Z) < 2.0f)
		return (hue.X + (hue.Y - hue.X) * ((2.0f / 3.0f) - hue.Z) * 6.0f);

	return hue.X;
}






void
ParabellumFramework::Math::UnitHelper::UnitHelper::ScreenSpaceToUv(Vector2D ss, Vector2D& uv)
{
	uv.X = (ss.X + 1.0f) * 0.5f;
	uv.Y = (-ss.Y + 1.0f) * 0.5f;
}


void
ParabellumFramework::Math::UnitHelper::UnitHelper::UVToScreenSpace(Vector2D uv, Vector2D& ss)
{
	ss.X = uv.X * 2.0f - 1.0f;
	ss.Y = -(uv.Y * 2.0f - 1.0f);
}


void
ParabellumFramework::Math::UnitHelper::UnitHelper::HSLtoRGB(Vector3D& hsl, Vector3D& rgb)
{
	EFLOAT32 hue = hsl.X / 360.0f;

	EFLOAT32 f1, f2;


	if (hsl.Z < 0.5f)
		f1 = hsl.Z * (1.0f + hsl.Y);
	else
		f2 = (hsl.Z + hsl.Y) - (hsl.Z * hsl.Y);

	f1 = (2.0f * hsl.Z) - f2;

	Vector3D tmp;
	tmp.Set(f1, f2, 0.0f);

	tmp.Z = hue + 0.3333f;
	rgb.X = HUEtoRGB(tmp);

	tmp.Z = hue;
	rgb.Y =  HUEtoRGB(tmp);

	tmp.Z = hue - 0.3333f;
	rgb.Z = HUEtoRGB(tmp);
}



void
ParabellumFramework::Math::UnitHelper::UnitHelper::CartesianToPolar2D(Vector2D& cart, Vector2D& polar)
{
	EFLOAT32 angle = 0.0f;

	polar.X = cart.Length();

	if ((cart.X > 0.0f) && (cart.Y >= 0.0f))
	{
		polar.Y = FastAtan(cart.Y/ cart.X);
	}
	else if ((cart.X > 0.0f) && (cart.Y < 0.0f))
	{
		polar.Y = FastAtan(cart.Y / cart.X) + PI2;
	}
	else if (cart.X < 0.0f)
	{
		polar.Y = FastAtan(cart.Y / cart.X) + PI;
	}
	else if (cart.Y > 0.0f)
	{
		polar.Y = PIO2;
	}
	else if (cart.Y < 0.0f)
	{
		polar.Y = PI * 1.5f;
	}
}


void
ParabellumFramework::Math::UnitHelper::UnitHelper::SphericalToCartesian(
	_IN_ EFLOAT32 p, _IN_ EFLOAT32 ay, _IN_ EFLOAT32 az,
	_OUT_ EFLOAT32& cx, _OUT_ EFLOAT32& cy, _OUT_ EFLOAT32& cz)
{
	EFLOAT32 sinY = FastSin(ay);

	cx = p * sinY * FastCos(az);
	cy = p * sinY * FastSin(az);
	cz = p * FastCos(ay);
}
