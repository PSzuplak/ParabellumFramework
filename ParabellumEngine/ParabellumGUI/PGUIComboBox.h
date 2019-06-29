#ifndef _PGUICOMBOBOX_H_
#define _PGUICOMBOBOX_H_


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
	//	ComboBox
	//
	class XYZ_API PGUIComboBox : public PGUIDrawableObject
	{


	public:

		PGUIComboBox(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIComboBox(const PGUIComboBox&) = delete;

		virtual ~PGUIComboBox();

	//
	// Methods
	//
	public:

		// Add GUI object - ComboBox can take ANY object - not only text
		void Add(_IN_ PGUIDrawableObject* pPUIObj);

		// Add text object
		void Add(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* str);

		// select index
		void SelectIndex(EUINT32 index);

		// show/hide content
		void Show(EBOOL val);

		// show/hide content
		void Turn();

	//
	// Members
	//
	private:

		// combobox button
		PGUIButton* m_boxBtn;

		// objects added to ComboBox
		std::vector<PGUIDrawableObject*> m_objects;

		// allocated objects by this combobox
		std::vector<PGUIDrawableObject*> m_allocatedObjects;

		// selected index starting from "0". "-1" is no selection
		EINT32 m_selectedIndex;


		EBOOL m_isExpanded;

	//
	// Gets And Sets
	//
	public:


		EINT32 GetSelectedIndex();



	};

}








#endif