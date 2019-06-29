#ifndef _PUITYPES_H_
#define _PUITYPES_H_

//
//
//
#include <memory>
#include <functional>

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

namespace ParabellumUserInterface
{

	//
	// Typedefs for events
	//

	typedef std::function<void(void*, void*)> PUIEvent;

	//typedef std::shared_ptr< std::function<void(void*, void*)> > PUIEvent;

	//
	// struct for ParabellumFramework VertexBuffer
	//
	struct PUIVertexFormat
	{
		ParabellumFramework::EFLOAT32 Position[3];
		ParabellumFramework::EFLOAT32 UV[2];
	};


	enum class PGUIEventType
	{
		Unknown = 0,
		Mouse_Pressed,
		Mouse_Holded,
		Mouse_Released,
		Mouse_Moved,
		Key_Pressed,
		Key_Holded,
		Key_Released,
	};

	enum PUICodes
	{
		PUI_SUCCESS = 0,
		PUI_FAIL
	};


	// position type
	enum class PUIPositionType
	{
		// parent of object does not influence position
		PUI_ABSOLUTE = 0,

		// position is relative to m_contentArea 
		PUI_RELATIVE = 1,

		// position is relative to m_position 
		PUI_FIXED = 2,

		// Always Aligned to the parent - object position is ignored
		PUI_ALIGNED = 3,

		// position of the object depends only on other FLOATing objects
		// position is calclated depends on position in children list
		PUI_FLOAT = 4,
	};


	// where the object 
	enum class PUIAligmentSide
	{
		PUI_INSIDE = 0,

		// does not work - collides with IsVisible() - object must be inside parent - someday
		PUI_OUTSIDE = 1
	};

	// if PUIPositionType::PUI_ALIGNED is set then PUIAligmentPosition will be used to place the element
	enum class PUIAligmentPosition
	{
		PUI_NOALIGMENT = 0,

		// position is calculated from top left corner
		PUI_TOPLEFT = 1,

		// position is calculated from top right corner
		PUI_TOPRIGHT = 2,

		// position is calculated from bottom right corner
		PUI_BOTTOMRIGHT = 3,

		// position is calculated from bottom left corner
		PUI_BOTTOMLEFT = 4,

		// centered on both axis
		PUI_CENTER = 5,

		// centered on X axis
		PUI_CENTER_X = 6,

		// centered on Y axis
		PUI_CENTER_Y = 7,

	};

	// 
	enum class PUISizeType
	{
		// no stretch
		PUI_NORMAL = 0,

		// width will be equal to parent width
		PUI_STRETCHED_WIDTH = 1,

		// height will be equal to parent height
		PUI_STRETCHED_HEIGHT = 2,

		// width and height will be equal to parent height
		//PUI_STRETCHED_ALL = 3,
	};


	// texturing
	enum class PUITexturing
	{
		PUI_REPEAT = 0,
		PUI_WRAP = 1,
	};



	enum class PUIFormatString
	{
		PUI_NONE = 0,
		PUI_NEWLINE = 1,
		UI_BOLD = 2,
		UI_ITALIC = 3,
		UI_UNDERLINE = 4,
	};


	// where a menu item should be displayed
	enum class PUIMenuItemSide
	{
		PUI_LEFT = 0,
		PUI_UP = 1,
		PUI_RIGHT = 2,
		PUI_DOWN = 3,
	};




	//
	// RawDrawableObject
	//

	enum class PGUIPositionAspect
	{
		// Position is calculated from Screen (0,0) position
		// parents position does not have any impact
		Absolute = 0,

		// Position is calculated as (0,0) point start from its parent position
		Relative,

		// NOT FULLY IMPLEMENTED
		// objects will be displayed one after another until reach Parents width/height
		// adding childrent matters - influences the order
		// also matters parents padding and child margin (distance from othe childrens)
		Inline,


		// Fixed position is calculated the same way as Relative,
		// the only difference is that ScrollBars etc. will not affect this
		Fixed,
	};


	// Which point should be use for positioning
	// F.ex if CenterXY is marked and Aspest is Relative
	// coordinates = obj.position + (obj.size / 2) + (parent.position + parent.padding) 
	enum class PGUIPlacementType
	{
		TopLeft = 0,
		TopRight,
		BottomRight,
		BottomLeft,
		CenterX,
		CenterY,
		CenterXY,
	};


	// working with PGUIPositionAspect and PGUIPlacementType
	// f.e if Outside
	enum class PGUIPlacementSide
	{
		// ...
		Outside = 0,

		// position will be calculated with relation to Parent
		// f.ex. if Child has PGUIPlacementType::BottomRight and position (0,0) then
		// it will be displayed in Parents bottom right corner
		Inside = 1,
	};



	enum class PGUISizeAspect
	{
		Normal = 0,
		Screthed_Width = 1,
		Screthed_Height = 2,
		Screthed_Both = 3,
	};


	enum class PGUIOrientation
	{
		Horizontal = 0,
		Vertical = 1
	};



	enum class PGUIUnit
	{
		// same as GraphicDevice ViewPort
		Float = 0,

		// pixels - NOT IMPLEMENTED
		Pixel,

		// percentage
		Percentage

	};


	struct PGUITextFormat
	{

		PGUITextFormat()
		{
			Color[0] = 1.0f;
			Color[1] = 1.0f;
			Color[2] = 1.0f;
			Color[3] = 1.0f;

			FontSize = 0.009f;

			TextAlign = 0;

			LetterSpacing = 0.005f;

			LineHeight = 0.1f;

			WordSpacing = 0.010f;
		}

		~PGUITextFormat()
		{

		}

		ParabellumFramework::EFLOAT32 Color[4];

		ParabellumFramework::EFLOAT32 FontSize;

		ParabellumFramework::EUINT32 TextAlign;
		ParabellumFramework::EFLOAT32 LetterSpacing;
		ParabellumFramework::EFLOAT32 LineHeight;
		ParabellumFramework::EFLOAT32 WordSpacing;
	};


	//
	//
	// Unit data for PGUI
	class PGUIUnitData
	{
	public:

		PGUIUnitData()
		{
			Value = 0.0f;
			Unit = PGUIUnit::Float;
		}

		PGUIUnitData(ParabellumFramework::EFLOAT32 floatVal)
		{
			Value = floatVal;
			Unit = PGUIUnit::Float;
		}

		PGUIUnitData(ParabellumFramework::EFLOAT32 val, PGUIUnit unit)
		{
			Value = val;
			Unit = PGUIUnit::Float;
		}

		~PGUIUnitData()
		{
		}

	//
	// members
	//
	public:
		ParabellumFramework::EFLOAT32 Value;
		PGUIUnit Unit;


	private:

	//
	// Gets and sets
	//

		PGUIUnitData Get()
		{
			return *this;
		}

	//
	// operators
	//
	public:
		inline PGUIUnitData operator=(const PGUIUnitData& rhs)
		{
			Value = rhs.Value;
			Unit = rhs.Unit;

			return *this;
		}

		inline friend PGUIUnitData operator-(const PGUIUnitData& rhs)
		{
			PGUIUnitData sv;

			sv.Value = -rhs.Value;
			sv.Unit = rhs.Unit;

			return sv;
		}


		inline friend PGUIUnitData operator+(const PGUIUnitData& rhs)
		{
			return rhs;
		}


	};


	// UI Float
	PGUIUnitData XYZ_API operator"" _uif(long double val);

	// UI pertentage
	PGUIUnitData XYZ_API operator"" _uip(long double val);







}








#endif