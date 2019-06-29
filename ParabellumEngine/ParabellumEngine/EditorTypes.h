#ifndef _EDITORTYPES_H_
#define _EDITORTYPES_H_

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
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/MathHelperExtended.h"
#include "../ParabellumFramework/MathHelperPrimitives.h"

namespace ParabellumEngine
{
	namespace Editor
	{

		enum PE_AxisType : ParabellumFramework::EUINT32
		{
			Axis_X = 0,
			Axis_Y = 1,
			Axis_Z = 2,
			None = 3,
		};


		enum class SelectionType : ParabellumFramework::EUINT32
		{
			Replace = 0,
			Add,
			Substract,
			Intersect,
		};

		enum class PE_ManipulationType : ParabellumFramework::EUINT32
		{
			None = 0,
			Translation,
			Rotation,
			Scale
		};


		enum class PE_ManipulationHook : ParabellumFramework::EUINT32
		{
			Free = 0,

			// object positon Y will be automatically fit to the ground
			AlignToTerrain,

			// object will be automatically set near the other objects
			AlignToObjects,

		};

	}

}






#endif