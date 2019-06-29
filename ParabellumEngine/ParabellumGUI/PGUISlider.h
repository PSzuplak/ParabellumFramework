#ifndef _PGUISLIDER_H_
#define _PGUISLIDER_H_


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
#include "PGUIProgressBar.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	GroupBox - group UI objects
	//
	class XYZ_API PGUISlider : public PGUIProgressBar
	{
	public:

		PGUISlider(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUISlider(const PGUISlider&) = delete;

		virtual ~PGUISlider();

	//
	// Methods
	//
	public:

	//
	// Members
	//
	private:

		void SetValue(EFLOAT32 mx, EFLOAT32 my);

	//
	// Gets And Sets
	//
	public:



	};

}








#endif