#ifndef _PGUIEXPANER_H_
#define _PGUIEXPANER_H_


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

#include "PGUIButton.h"
#include "PGUIGroupBox.h"
#include "PGUIExpandBar.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	ExpandBar - Show/Hide some GroupBox
	//
	class XYZ_API PGUIExpander : public PGUIDrawableObject
	{


	public:

		PGUIExpander(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIExpander(const PGUIExpander&) = delete;

		virtual ~PGUIExpander();

		//
		// Methods
		//
	public:

		// add 1 more expand bar
		void AddExpandBar(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* label);

		// add object to content
		void AddObject(EUINT32 no, PGUIDrawableObject* pPGUIObject);


		void SetContentVisibility(EUINT32 no, EBOOL val);
	//
	// Members
	//
	private:


		std::vector<PGUIExpandBar*> m_expandBars;

		// if set then it is possible to expand many expandbars
		EBOOL m_isMultiselecting;

	//
	// Gets And Sets
	//
	public:






	};

}








#endif