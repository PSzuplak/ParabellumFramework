#ifndef _PGUIPAGETAB_H_
#define _PGUIPAGETAB_H_


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
#include "PGUIButton.h"
#include "PGUIGroupBox.h"


//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	PageTab
	//
	class XYZ_API PGUIPageTab : public PGUIDrawableObject
	{


	public:

		PGUIPageTab(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIPageTab(const PGUIPageTab&) = delete;

		virtual ~PGUIPageTab();

	//
	// Methods
	//
	public:

		// 
		void AddTab(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* str);

		//
		void AddToPage(EUINT32 no, _IN_ PGUIDrawableObject* pGUIObj);

		void SelectPage(EINT32 no);

	//
	// Members
	//
	private:

		std::vector<PGUIButton*> m_selectors;

		std::vector<PGUIGroupBox*> m_pages;

		EINT32 m_selectedPage;

	//
	// Gets And Sets
	//
	public:






	};

}








#endif