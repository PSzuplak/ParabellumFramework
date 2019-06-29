#include "stdafx.h"
#include "PGUIText.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIText::PGUIText(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	m_length = 0;
	m_formatedLength = 0;

	m_font = new PGUIFont(pDevice, pResourceManager);
	m_font->SetPosition(0.0_uif, 0.0_uif);
	m_font->SetSize(0.01_uif, 0.02_uif);
	m_font->SetPlacementType(PGUIPlacementType::TopLeft);
	m_font->SetPositionAspect(PGUIPositionAspect::Relative);
	
	m_textFormat2.newLine = false;

	layout.borderSize.Set(0.0f, 0.0f, 0.0f, 0.0f);
	layout.borderColor.Set(1.0f, 1.0f, 1.0f, 0.5f);

	SetVisibility(false);
}



ParabellumUserInterface::PGUIText::~PGUIText()
{
	delete m_font;
}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void
ParabellumUserInterface::PGUIText::Draw(_IN_ GraphicsDevice* pDevice)
{
	for (EUINT32 i = 0; i < m_length; ++i)
	{
		m_fonts[i]->Draw(pDevice);
	}
}

//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------


void 
ParabellumUserInterface::PGUIText::SetText(_IN_ const char* str)
{
	EUINT32 i = 0;

	m_uStr = str;

	m_formatedLength = strlen(str);

	if (m_formatedLength == 0)
		return;


	if (m_str.size() < m_formatedLength)
	{
		m_str.resize(m_formatedLength, 0);
		m_str8bit.resize(m_formatedLength + 1, 0);
		m_str8bit[m_formatedLength] = 0;
	}

	for (EUINT32 i = 0; i < m_formatedLength; ++i)
	{
		m_str[i] = (EUINT32)str[i];
		m_str8bit[i] = (char)str[i];
	}

	// add more font object
	if (m_fonts.size() < m_formatedLength)
	{
		EUINT32 range = m_formatedLength - m_fonts.size();

		for (EUINT32 i = 0; i < range; ++i)
		{
			PGUIFont* font = (PGUIFont*)malloc(sizeof(PGUIFont)); // ... temporary :)
			memcpy(font, m_font, sizeof(PGUIFont));

			font->SetParent(this);
			font->SetPosition(0.0_uif, 0.0_uif);
			font->SetSize(0.01_uif, 0.02_uif);
			font->SetPlacementType(PGUIPlacementType::TopLeft);
			font->SetPositionAspect(PGUIPositionAspect::Relative);

			m_fonts.push_back(font);
		}
	}

	
	//
	// Set fonts 
	Vector4D thisPos, charPos, color;

	GetPosition(thisPos);
	thisPos.Set(0.0f, 0.0f, 0.0f, 0.0f);
	charPos = thisPos;

	color.Set(m_textFormat.Color[0], m_textFormat.Color[1], m_textFormat.Color[2], m_textFormat.Color[3]);


	EUINT32 letterNo = 0;

	for (EUINT32 i = 0; i < m_formatedLength; ++i)
	{

		// format string
		if (m_str[i] == '<')
		{
			EUINT32 p1, p2;
			p1 = i;

			for (EUINT32 n = i; n < m_formatedLength; ++n)
				if (m_str[n] == '>')
				{
					p2 = n;
					i = n + 1;
					break;
				}

			FormatString(p1, p2);
		}

		if (m_textFormat2.newLine == true)
		{
			charPos.X = thisPos.X;
			charPos.Y += m_textFormat.LineHeight;
			m_textFormat2.newLine = false;
		}

		if ((char)m_str[i] == ' ')
		{
			charPos.X += m_textFormat.WordSpacing;
			continue;
		}
		

		m_fonts[letterNo]->SetSign(m_str[i]);

		PGUIUnitData newPosX, newPosY, newWidth, newHeight;
		Vector4D fontUVCoords;
		EFLOAT32 whFactor;

		newPosX.Unit = PGUIUnit::Float;
		newPosX.Value = charPos.X;
		newPosY.Unit = PGUIUnit::Float;
		newPosY.Value = charPos.Y;

		newWidth.Unit = PGUIUnit::Float;
		newWidth.Value = m_textFormat.FontSize * 0.3f;
		newHeight.Unit = PGUIUnit::Float;
		newHeight.Value = m_textFormat.FontSize;

		m_fonts[letterNo]->SetPosition(newPosX, newPosY);
		m_fonts[letterNo]->SetColor(color);
		m_fonts[letterNo]->SetSize(newWidth, newHeight);

		// next letter
		charPos.X += m_textFormat.LetterSpacing;

		letterNo++;
	}

	//
	// set length
	m_length = letterNo;

	PGUIUnitData newWidth, newHeight;
	newWidth.Unit = PGUIUnit::Float;
	newWidth.Value = charPos.X;
	newHeight.Unit = PGUIUnit::Float;
	newHeight.Value = 0.01f;

	SetWidth(newWidth);
	SetHeight(newHeight);
}



const char* 
ParabellumUserInterface::PGUIText::GetText()
{
	return &m_str8bit[0];
}



//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------

ParabellumUserInterface::PUIFormatString
ParabellumUserInterface::PGUIText::FormatString(EUINT32 p1, EUINT32 p2)
{
	std::string attrib, val;

	for (EUINT32 i = p1+1; i < p2; ++i)
	{
		attrib += (char)m_str[i];
	}


	// br
	if (attrib.compare("br") == 0)
		m_textFormat2.newLine = true;


	// b

	// i

	// u

	return PUIFormatString::PUI_NONE;
}



ParabellumUserInterface::PGUITextFormat
ParabellumUserInterface::PGUIText::FormatString2(EUINT32 p1, EUINT32 p2)
{
	return PGUITextFormat();
}