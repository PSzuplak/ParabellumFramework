//
// equations from http://docs.gimp.org/en/gimp-concepts-layer-modes.html
// and from http://pippin.gimp.org/image_processing/chap_point.html
// and some experiments...
 

//
// image - lower layer
// mask - upper layer
//
//
//




//	------------------------------------------------------------------------------
//
//		Gimpz
//
//	------------------------------------------------------------------------------

//
// from gimp/polar-coords.c
//
float2 GIMP_PolarCoordiantes(float2 uv, int imgWidth, int imgHeight, bool toRectangle, bool mapFromTop)
{
	float2 result = 0.0f;
	bool inside;
	double  phi, phi2;
	double  xx, xm, ym, yy;
	int     xdiff, ydiff;
	double  r;
	double  m;
	double  xmax, ymax, rmax;
	double  x_calc, y_calc;
	double  xi, yi;
	double  circle, angl, t, angle;
	int     x1, x2, y1, y2;
	double cen_x;
	double cen_y;

	double G_PI = 3.14159265359;

	double wx = uv.x * imgWidth;
	double wy = uv.y * imgHeight;

	phi = 0.0;
	x1 = 0;
	y1 = 0;
	x2 = imgWidth;
	y2 = imgHeight;
	xdiff = x2 - x1;
	ydiff = y2 - y1;
	xm = xdiff / 2.0;
	ym = ydiff / 2.0;
	circle = 100.0;
	angle = 0;
	angl = angle / 180.0 * G_PI;
	cen_x = 2048;
	cen_y = 2048;

	if (toRectangle == true)
	{
		phi = (2 * G_PI) * (wx - x1) / xdiff;

		phi = fmod(phi + angl, 2 * G_PI);

		if (phi >= 1.5 * G_PI)
			phi2 = 2 * G_PI - phi;
		else if (phi >= G_PI)
			phi2 = phi - G_PI;
		else if (phi >= 0.5 * G_PI)
			phi2 = G_PI - phi;
		else
			phi2 = phi;

		xx = tan(phi2);
		if (xx != 0)
			m = (double) 1.0 / xx;
		else
			m = 0;

		if (m <= ((double)(ydiff) / (double)(xdiff)))
		{
			if (phi2 == 0)
			{
				xmax = 0;
				ymax = ym - y1;
			}
			else
			{
				xmax = xm - x1;
				ymax = m * xmax;
			}
		}
		else
		{
			ymax = ym - y1;
			xmax = ymax / m;
		}

		rmax = sqrt((double)((xmax*xmax) + (ymax*ymax)));

		t = ((ym - y1) < (xm - x1)) ? (ym - y1) : (xm - x1);

		rmax = (rmax - t) / 100.0 * (100 - circle) + t;

		if (mapFromTop == true)
			r = rmax * (double)((wy - y1) / (double)(ydiff));
		else
			r = rmax * (double)((y2 - wy) / (double)(ydiff));

		xx = r * sin(phi2);
		yy = r * cos(phi2);

		if (phi >= 1.5 * G_PI)
		{
			x_calc = (double)xm - xx;
			y_calc = (double)ym - yy;
		}
		else if (phi >= G_PI)
		{
			x_calc = (double)xm - xx;
			y_calc = (double)ym + yy;
		}
		else if (phi >= 0.5 * G_PI)
		{
			x_calc = (double)xm + xx;
			y_calc = (double)ym + yy;
		}
		else
		{
			x_calc = (double)xm + xx;
			y_calc = (double)ym - yy;
		}


		xi = (int)(x_calc + 0.5);
		yi = (int)(y_calc + 0.5);

		result.x = xi;
		result.y = yi;
	}
	//	-------------------------------------------------------------------------------------
	// rect to polar
	else
	{
		if (wx >= cen_x)
		{
			if (wy > cen_y)
			{
				phi = G_PI - atan(((double)(wx - cen_x)) /
					((double)(wy - cen_y)));
			}
			else if (wy < cen_y)
			{
				phi = atan(((double)(wx - cen_x)) / ((double)(cen_y - wy)));
			}
			else
			{
				phi = G_PI / 2;
			}
		}
		else if (wx < cen_x)
		{
			if (wy < cen_y)
			{
				phi = 2 * G_PI - atan(((double)(cen_x - wx)) /
					((double)(cen_y - wy)));
			}
			else if (wy > cen_y)
			{
				phi = G_PI + atan(((double)(cen_x - wx)) /
					((double)(wy - cen_y)));
			}
			else
			{
				phi = 1.5 * G_PI;
			}
		}

		r = sqrt((wx - cen_x)*(wx - cen_x) + (wy - cen_y)*(wy - cen_y));

		if (wx != cen_x)
		{
			m = abs(((double)(wy - cen_y)) / ((double)(wx - cen_x)));
		}
		else
		{
			m = 0;
		}

		if (m <= ((double)(y2 - y1) / (double)(x2 - x1)))
		{
			if (wx == cen_x)
			{
				xmax = 0;
				ymax = cen_y - y1;
			}
			else
			{
				xmax = cen_x - x1;
				ymax = m * xmax;
			}
		}
		else
		{
			ymax = cen_y - y1;
			xmax = ymax / m;
		}

		rmax = sqrt((double)(xmax*xmax + ymax*ymax));

		t = ((cen_y - y1) < (cen_x - x1)) ? (cen_y - y1) : (cen_x - x1);
		rmax = (rmax - t) / 100 * (100 - circle) + t;

		phi = fmod(phi + angl, 2 * G_PI);

		//if (pcvals.backwards)
			x_calc = x2 - 1 - (x2 - x1 - 1) / (2 * G_PI) * phi;
		//else
		//	x_calc = (x2 - x1 - 1) / (2 * G_PI) * phi + x1;

		if (mapFromTop)
			y_calc = (y2 - y1) / rmax   * r + y1;
		else
			y_calc = y2 - (y2 - y1) / rmax * r;



		result.x = x_calc;
		result.y = y_calc;

	}


	

	result.x /= imgWidth;
	result.y /= imgHeight;

	return result;
}






//	------------------------------------------------------------------------------
//
//		Operators
//
//	------------------------------------------------------------------------------

//
//
// change color to grey
//
//
float LFGreyScale(float4 image)
{
	float grey = (image.r + image.g + image.b) * 0.3333f;
	return grey;
}





//
// GIMP :
// Thresholding an image is the process of making all pixels above a certain threshold level white, others black
//
//
float4 LFTreshold(float4 image, float value)
{
	float grey = LFGreyScale(image);
	
	if (grey > value)
		return float4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
}



//
// GIMP :
// When changing the brightness of an image, a constant is added or subtracted from the luminnance 
// of all sample values.This is equivalent to shifting the contents of the histogram 
// left(subtraction) or right(addition).
//
float4 LFBrightness(float4 image, float value)
{
	return image + value;
}




//
// GIMP :
// Changing the contrast of an image, changes the range of luminance values present.
// Visualized in the histogram it is equivalent to expanding or compressing the histogram around 
// the midpoint value.Mathematically it is expressed as :
//
float4 LFContrast(float4 image, float value)
{
	return ((image - 0.5f) * value) + 0.5f;
}




//
// GIMP :
// 
// Inverting the sample values in the image, produces the same image that would be found in a film negative
//
float4 LFInvert(float4 image)
{
	return (1.0f - image);
}



//
// GIMP :
// 
//
float4 LFGamma(float4 image, float value)
{
	return pow(image, value);
}


//
// GIMP :
// 
// The levels tool found in many image processing packages is in 
// its simplest form just a different way to adjust brightness/ contrast. 
//
float4 LFLevels(float4 image, float in_min, float gamma, float in_max, float out_min, float out_max)
{
	float4 result;

	result = image - in_min;
	result = pow(result, gamma);
	result = result * (out_max - out_min) + out_min;


	return result;
}


//
// GIMP :
// 
// Brightness + Contrast
//
float4 LFBrigCon(float4 image, float brightness, float contrast)
{
	return ((image - 0.5f) * contrast) + 0.5f + brightness;
}





//
// GIMP : Color to Alpha
// output : modiefied 'image' : new alpha stored in Alpha channel
// input : , color value, range
// 
//
float4 ColorToAlpha(float4 image, float4 color, float range)
{
	float4 output;

	float4 bottom = image - color * range;
	float4 up = image + color * range;

	return output;
}




//	------------------------------------------------------------------------------
//
//		Layer Modes
//
//	------------------------------------------------------------------------------

//
//
//
//
//
float4 LFTransparency(float4 image, float4 mask, float maskTransparency)
{
	return (image * maskTransparency) + (mask * (1.0f - maskTransparency));
}



//
//
//
//
//
float4 LFMultiplication(float4 image, float4 mask)
{
	return image * mask;
}

float4 LFScreen(float4 image, float4 mask)
{
	return 1.0f - (1.0f - image) * (1.0f - mask);
}








//
//
//
//
//
float4 LFDivide(float4 image, float4 mask)
{
	return image / (mask + 0.0001f);
}


//
//
//
//
//
float4 LFOverlay(float4 image, float4 mask)
{
	return image * (image + (2.0*mask) * image );
}



//
//
//
//
//
float4 LFSoftLight(float4 image, float4 mask)
{
	// "screen"
	float4 rs = 1.0f - ((1.0f - mask) * (1.0f - image));
	return ( (1.0f - image) * mask + rs ) * image;
}



float4 LFHardLight(float4 image, float4 mask)
{
	float4 rs = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (mask.r > 0.5f)
		rs.r = 1.0f - (1.0f - 2.0f * (mask.r - 0.5f)) * (1.0f - image.r);
	else
		rs.r = 2.0f * image.r * mask.r;

	if (mask.g > 0.5f)
		rs.g = 1.0f - (1.0f - 2.0f * (mask.g - 0.5f)) * (1.0f - image.g);
	else
		rs.g = 2.0f * image.g * mask.g;

	if (mask.b > 0.5f)
		rs.b = 1.0f - (1.0f - 2.0f * (mask.b - 0.5f)) * (1.0f - image.b);
	else
		rs.b = 2.0f * image.b * mask.b;

	if (mask.a > 0.5f)
		rs.a = 1.0f - (1.0f - 2.0f * (mask.a - 0.5f)) * (1.0f - image.a);
	else
		rs.a = 2.0f * image.a * mask.a;

	return rs;
}







//	------------------------------------------------------------------------------
//
//		Color Modes
//
//	------------------------------------------------------------------------------
