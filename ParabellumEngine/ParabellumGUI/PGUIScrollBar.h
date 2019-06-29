#ifndef _PGUISCROLLBAR_H_
#define _PGUISCROLLBAR_H_


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
	class XYZ_API PGUIScrollBar : public PGUIDrawableObject
	{
	public:

		PGUIScrollBar(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIScrollBar(const PGUIScrollBar&) = delete;

		virtual ~PGUIScrollBar();

	//
	// Methods
	//
	public:

	//
	// Members
	//
	private:

		PGUIDrawableObject* m_bar;

		PGUIDrawableObject* m_pointer;

		PGUIDrawableObject* m_back;

		PGUIDrawableObject* m_next;

		PGUIOrientation m_orientation;

		// percentage - from 0.0f to 1.0f
		EFLOAT32 m_pointerSize;

		// percentage - from 0.0f to 1.0f
		EFLOAT32 m_pointerPosition;


	//
	// Gets And Sets
	//
	public:


		// move pointer by delta (dd, 0) which can be from -1.0f to 1.0f but 
		// sum will be cut to (0.0f, 1.0f)
		void MovePointer(EFLOAT32 dd);

		// set pointer position - rate is a percentage position form 0.0f to 1.0f
		void SetPointer(EFLOAT32 dd);




	};

}








#endif