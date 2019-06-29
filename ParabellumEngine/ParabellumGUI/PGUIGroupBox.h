#ifndef _PGUIGROUPBOX_H_
#define _PGUIGROUPBOX_H_


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
	//	GroupBox - group UI objects
	//
	class XYZ_API PGUIGroupBox : public PGUIExtDrawableObject
	{
	public:

		PGUIGroupBox(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIGroupBox(const PGUIGroupBox&) = delete;

		virtual ~PGUIGroupBox();

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