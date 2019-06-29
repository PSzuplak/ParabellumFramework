#ifndef _PGUIEXTDRAWABLEOBJECT_H_
#define _PGUIEXTDRAWABLEOBJECT_H_


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

//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

#include "PGUIDrawableObject.h"
#include "PGUISimple.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	Extended DrawableObject. Contains more features like image borders
	//
	class XYZ_API PGUIExtDrawableObject : public PGUIDrawableObject
	{


	public:

		PGUIExtDrawableObject(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIExtDrawableObject(const PGUIExtDrawableObject&) = delete;

		virtual ~PGUIExtDrawableObject();

	//
	// Methods
	//
	public:

		// show/hide image border
		void DisplayBorder(EBOOL val);

	//
	// Members
	//
	private:

		//
		// Button is sorrounded by "borders"

		PGUISimple* m_borderTL;	// top left corner
		PGUISimple* m_borderT;	// top corner
		PGUISimple* m_borderTR;	// top right corner
		PGUISimple* m_borderR;	// right corner
		PGUISimple* m_borderBR;	// bottom corner
		PGUISimple* m_borderB;	// bottom corner
		PGUISimple* m_borderBL;	// bottom left corner
		PGUISimple* m_borderL;	// left corner


	//
	// Gets And Sets
	//
	public:

		//
		// Set border size
		void SetBorderSize(EFLOAT32 val);







	};

}








#endif