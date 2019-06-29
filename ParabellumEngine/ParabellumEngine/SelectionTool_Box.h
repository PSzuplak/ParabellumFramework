#ifndef _SELECTIONTOOL_BOX_H_
#define _SELECTIONTOOL_BOX_H_

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
#include "SelectionTool.h"
#include "Component3D.h"
#include "Octree.h"

using namespace ParabellumEngine::Core::Scene;

namespace ParabellumEngine
{
	namespace Editor
	{
		

		//
		// Selection tool on Component3D
		//
		class XYZ_API SelectionTool_Box : public SelectionTool
		{

		//
		// 
		//
		public:

			SelectionTool_Box(_IN_ GraphicsDevice* pDevice, _IN_ ParabellumFramework::Resources::ResourceManager* pResource);
			~SelectionTool_Box();


		//
		// Methods
		//
		public:

			void Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);
			void SelectComponents(_OUT_ std::list<Component3D*>* pComponents, _IN_ Octree* pOctree, SelectionType selectionType);

		//
		// Memebers
		//
		private:

			Vector3D m_selectedPointMin;
			Vector3D m_selectedPointMax;

			BoundingBox m_selectedRegionBox;

			Shader* m_shader;


			Narray<Octree*>* m_visibleOctreeNodes;

		//
		// Gets and Sets
		//
		public:

			void SetSelectionRegion(Vector3D v1, Vector3D v2);

		};
	}
}






#endif