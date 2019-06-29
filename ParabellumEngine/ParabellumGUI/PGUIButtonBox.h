#ifndef _PGUIBUTTONBOX_H_
#define _PGUIBUTTONBOX_H_


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
	//	Deck -
	//
	class XYZ_API PGUIButtonBox : public PGUIDrawableObject
	{
	public:

		PGUIButtonBox(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);
		PGUIButtonBox(const PGUIButtonBox&) = delete;

		virtual ~PGUIButtonBox();

	//
	// Methods
	//
	public:


		void AddButton(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, Vector4D skinUV);

		void Add(EUINT32 containerNo, _IN_ PGUIDrawableObject* pGUIObj);

		void SelectIndex(EINT32 index);

	//
	// Members
	//
	private:

		std::vector<PGUIButton*> m_buttons;
		PGUIGroupBox* m_buttonsContainer;

		std::vector<PGUIGroupBox*> m_content;


		EINT32 m_selectedIndex;

	//
	// Gets And Sets
	//
	public:



	};

}








#endif