#ifndef _PGUICURSOR_H_
#define _PGUICURSOR_H_


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


//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	Cursor
	//
	class XYZ_API PGUICursor : public PGUIDrawableObject
	{


	public:

		PGUICursor(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUICursor(const PGUICursor&) = delete;

		virtual ~PGUICursor();

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