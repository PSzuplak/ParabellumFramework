#ifndef _PGUITEXT_H_
#define _PGUITEXT_H_


//
// 
//
#include <string>
#include <list>

//
//	ParabellumGUI dependieces
//
#include "PGUIDrawableObject.h"
#include "PGUIFont.h"

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


	struct PGUITextAreaFormat
	{
		EFLOAT32 DistanceBetweenChars;

		EFLOAT32 DistanceBetweenLines;

	};

	//
	//	Text - contains FORMATED text to draw - using some HTML
	//
	//	<br> - new line
	//	<b>/<u>/<i> 
	//
	//
	//
	//
	class XYZ_API PGUIText : public PGUIDrawableObject
	{

	//
	// Structs
	//
	private:

		struct TextFormat
		{
			EBOOL newLine;
		};


	public:
		PGUIText(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);
		PGUIText(const PGUIText&) = delete;
		virtual ~PGUIText();


	public:

		virtual void Draw(_IN_ GraphicsDevice* pDevice);

		//
		// Members
		//
	private:

		std::vector<PGUIFont*> m_fonts;

		PGUIFont* m_font;

		// text format
		PGUITextFormat m_textFormat;

		// string
		std::string m_uStr;
		std::vector<EUINT32> m_str;
		std::vector<char> m_str8bit;

		EUINT32 m_length;

		EUINT32 m_formatedLength;

		//PGUITextAreaFormat m_textSettings;

		TextFormat m_textFormat2;

	//
	// Gets and Sets
	//
	public:

		// set ASCII string
		void SetText(_IN_ const char* str);

		const char* GetText();

	//
	// Inside
	//
	private:

		// p1 should be '<' and p2 should be '>' in m_str
		PUIFormatString FormatString(EUINT32 p1, EUINT32 p2);

		PGUITextFormat FormatString2(EUINT32 p1, EUINT32 p2);
	};

}








#endif