#ifndef _PGUICOLORPICKER_H_
#define _PGUICOLORPICKER_H_


//
// 
//
#include <string>
#include <list>

//
//	ParabellumGUI dependieces
//
#include "PUITypes.h"
#include "PUIEventMouse.h"

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

#include "PGUIExtDrawableObject.h"
#include "PGUISlider.h"


//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	TrianglePicker - pick Vector3D ala colors from triangle color picker
	//
	class XYZ_API PGUIColorPicker : public PGUIDrawableObject
	{
	public:

		PGUIColorPicker(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIColorPicker(const PGUIColorPicker&) = delete;

		virtual ~PGUIColorPicker();

	//
	// Methods
	//
	private:

		void CalculateValue(EFLOAT32 mx, EFLOAT32 my);

	//
	// Members
	//
	public:

		Vector3D m_hsl;
		Vector3D m_rgb;


	//
	// Gets And Sets
	//
	public:


	};

}








#endif