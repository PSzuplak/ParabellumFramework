#ifndef _SELECTIONTOOL_H_
#define _SELECTIONTOOL_H_

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
// ParabellumFramework
//
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/Model.h"
#include "../ParabellumFramework/BoundingVolumes.h"


//
// ParabellumEngine
//
#include "EditorTypes.h"
#include "Component3D.h"
#include "PrimitivesFactory.h"
#include "MapSector.h"
#include "Octree.h"

namespace ParabellumEngine
{
	namespace Editor
	{

		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Graphics;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumEngine::Core::Graphics;
		using namespace ParabellumEngine::Core::Scene;
		//
		// Selection tool on Component3D
		//
		class XYZ_API SelectionTool
		{

		//
		// Constructors and destructors
		//
		protected:

			SelectionTool();
			~SelectionTool();

		//
		// Methods
		//
		public:

			virtual void Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera) = 0;
			virtual void SelectComponents(_OUT_ std::list<Component3D*>* pComponents, _IN_ Octree* pOctree, SelectionType selectionType) = 0;

		};
	}
}






#endif