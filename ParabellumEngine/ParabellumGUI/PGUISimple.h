#ifndef _PGUISIMPLE_H_
#define _PGUISIMPLE_H_


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

#include "PGUIDrawableObject.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	Raw Drawable PUI object
	//
	class XYZ_API PGUISimple : public PGUIDrawableObject
	{


	public:

		PGUISimple(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUISimple(const PGUISimple&) = delete;

		virtual ~PGUISimple();

		//
		// Methods
		//
	public:


		//
		// Members
		//
	private:


		//
		// Gets And Sets
		//
	public:










	};

}








#endif