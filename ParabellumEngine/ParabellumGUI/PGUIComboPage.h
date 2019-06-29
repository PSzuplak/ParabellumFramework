#ifndef _PGUICOMBOPAGE_H_
#define _PGUICOMBOPAGE_H_


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
#include "PGUIComboBox.h"
#include "PGUIGroupBox.h"
#include "PGUIScrollBar.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	ComboPage - similar to PageTab - page (group box) selection from combo box
	//
	class XYZ_API PGUIComboPage : public PGUIDrawableObject
	{


	public:

		PGUIComboPage(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIComboPage(const PGUIComboPage&) = delete;

		virtual ~PGUIComboPage();

	//
	// Methods
	//
	public:

		void AddPage(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* str);

		// add object to a page with index "no"
		void AddToPage(EUINT32 no, PGUIDrawableObject* pPUIObj);

		void SelectPage(EINT32 no);

	//
	// Members
	//
	private:

		PGUIComboBox* m_selector;

		std::vector<PGUIGroupBox*> m_pages;

		std::vector<PGUIScrollBar*> m_verticalScrolls;

		EINT32 m_selectedPage;

	//
	// Gets And Sets
	//
	public:






	};

}








#endif