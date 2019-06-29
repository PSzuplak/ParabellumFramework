#ifndef _PGUIMANAGER_H_
#define _PGUIMANAGER_H_


//
// 
//
#include <string>
#include <list>


//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Event.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/InputDevice.h"
#include "../ParabellumFramework/Profiler.h"
//
//	PUI
//

#include "PGUIDrawableObject.h"
#include "PGUIExtDrawableObject.h"
#include "PGUIExpandBar.h"
#include "PGUIExpander.h"
#include "PGUIScrollBar.h"
#include "PGUIComboBox.h"
#include "PGUIComboPage.h"
#include "PGUIPageTab.h"
#include "PGUIProgressBar.h"
#include "PGUISlider.h"
#include "PGUIColorPicker.h"
#include "PGUIButtonBox.h"

#include "PGUICursor.h"
#include "PGUIText.h"
#include "PGUIHintable.h"

#include "PUIEvent.h"
#include "PUIEventMouse.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::IO;

namespace ParabellumUserInterface
{

	//
	//	GUI Window - manages gui objects
	//
	class XYZ_API PGUIManager : public Event::EventHandler
	{
	public:
		PGUIManager(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput, _IN_ ResourceManager* pResourceManager);
		PGUIManager(const PGUIManager&) = delete;
		virtual ~PGUIManager();



	public:

		// draw elemens which belong to this window
		virtual void Draw(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput) final;

		virtual void Update(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput) final;

		

		virtual void Add(_IN_ _KEEP_ PGUIDrawableObject* pObj) final;


		//
		// if 'enabled' == true then cursor will move only by 'pDisplacement'
		//
		void SetCursosFixedDisplacement(_IN_ Vector3D* pDisplacement, bool enabled)
		{
			cursor.isFixedDisplacement = enabled;
			cursor.displacementVector = *pDisplacement;
		}


	//
	// Events Handlers
	//
	public:

		void HandlerMouseClick(EFLOAT32 mouseX, EFLOAT32 mouseY);
		void HandlerMouseDrag(EFLOAT32 beginX, EFLOAT32 beginY, EFLOAT32 currentX, EFLOAT32 currentY, EFLOAT32 deltaX, EFLOAT32 deltaY);
	//
	// Members
	//
	private:

		PGUICursor* m_puiCursor;
		std::list<PGUIDrawableObject*> m_objects;

		// pointer to selected object
		PGUIDrawableObject* m_selectedObject;	

		std::list<PGUIDrawableObject*> m_objectsMouseOver;

		struct Cursor
		{
			bool isFixedDisplacement;
			Vector3D displacementVector;

		} cursor;
	

	//
	// Gets and sets
	//
	public:

		// get object which collide with (x,y) point
		PGUIDrawableObject* GetGUIObject(EFLOAT32 x, EFLOAT32 y);

	};

}








#endif