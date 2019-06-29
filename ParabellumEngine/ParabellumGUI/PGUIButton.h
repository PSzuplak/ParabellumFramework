#ifndef _PGUIButton_H_
#define _PGUIButton_H_


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
#include "PGUIText.h"

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
	class XYZ_API PGUIButton : public PGUIDrawableObject
	{


	public:

		PGUIButton(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIButton(const PGUIButton&) = delete;

		virtual ~PGUIButton();

	//
	// Methods
	//
	public:

		

	//
	// Members
	//
	private:

		PGUIText* m_label;



	//
	// Gets And Sets
	//
	public:


		void SetLabel(const char* str);

		const char* GetLabel();

	};

}








#endif