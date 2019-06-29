#ifndef _PGUIEXPANDBAR_H_
#define _PGUIEXPANDBAR_H_


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
	class XYZ_API PGUIExpandBar : public PGUIButton
	{


	public:

		PGUIExpandBar(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIExpandBar(const PGUIExpandBar&) = delete;

		virtual ~PGUIExpandBar();

	//
	// Methods
	//
	public:

		void Expand2(int h);

		void Expand();

		// expand true/false
		void Expand(EBOOL val);

		void ChangeContentVisibility();

		// add object to content
		void AddObject(PGUIDrawableObject* pPGUIObject);

	//
	// Members
	//
	private:


		PGUIDrawableObject* m_selector;

		PGUIGroupBox* m_content;

		EBOOL m_isExpanded;

		Vector4D m_savedSize;

		Vector4D m_tempSize;

	//
	// Gets And Sets
	//
	public:


		EBOOL IsExpanded();



	};

}








#endif