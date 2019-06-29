#ifndef _PGUIFONT_H_
#define _PGUIFONT_H_


//
// 
//
#include <string>
#include <list>

//
//	ParabellumGUI dependieces
//
#include "PGUIDrawableObject.h"

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"


//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	Drawable PUI object
	//
	class XYZ_API PGUIFont : public PGUIDrawableObject
	{
	private:
		static EFLOAT32 Chars[1024];

	public:
		PGUIFont(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);
		PGUIFont(const PGUIFont&) = delete;
		virtual ~PGUIFont();

	//
	// Methods
	//
	public:

	//
	// Members
	//
	private:

		// 32bit char value
		EUINT32 m_sign;

		EUINT32	m_fontTextureWidth;
		EUINT32 m_fontTextureHeight;

	public:

		EUINT32 SetSign(EUINT32 value);


	};

}








#endif