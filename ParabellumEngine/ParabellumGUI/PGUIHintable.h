#ifndef _PGUIHINTABLE_H_
#define _PGUIHINTABLE_H_


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
#include "PGUIButton.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	Hintable - show hint whenever mouse us put over the object
	//
	class XYZ_API PGUIHintable : public PGUIDrawableObject
	{
	public:

		PGUIHintable(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIHintable(const PGUIHintable&) = delete;

		virtual ~PGUIHintable();

	//
	// Methods
	//
	public:

		void DisplayHint(EBOOL val);

	//
	// Members
	//
	private:

		PGUIButton* m_hintArea;


	//
	// Gets And Sets
	//
	public:

		void SetHint(const char* str);

		void SetHintPosition(EFLOAT32 x, EFLOAT32 y);


	};

}








#endif