#ifndef _TRANSFORMTOOL_H_
#define _TRANSFORMTOOL_H_

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"



//
// STDs
//
#include <memory>
#include <functional>
#include <list>


//
// ParabellumEngine
//
#include "EditorTypes.h"
#include "Component3D.h"

namespace ParabellumEngine
{
	namespace Editor
	{

		//
		// Transform tool on Component3D
		//
		class XYZ_API TransformTool
		{
		protected:

			TransformTool();
			~TransformTool();


		//
		// Methods
		//
		public:
			
			void Change(_IN_ ParabellumEngine::Components::Component3D* pC3D);

		//
		// Memebers
		//
		public:







		};
	}
}






#endif