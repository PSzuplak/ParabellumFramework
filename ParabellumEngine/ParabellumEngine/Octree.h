#ifndef _OCTREE_H_
#define _OCTREE_H_


#include "DLLIE.h"

#include <list>
#include <map>
#include <queue>

#include "../ParabellumFramework/SystemCodes.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Narray.h"
#include "../ParabellumFramework/Vector3D.h"
#include "../ParabellumFramework/Shader.h"

#include "Component3D.h"
#include "EModel.h"
#include "Camera.h"

#include "RenderArea.h"
#include "RenderTree.h"

#include "PlaceHolder.h"
#include "SceneCart.h"


#define PE_CORE_SCENE_MAXMODELSINOCTREENODE		32
#define PE_CORE_SCENE_MAXTREESINOCTREE			30
#define PE_CORE_SCENE_MAXLIGHTSINOCTREE			5
#define PE_CORE_SCENE_MAXLGRASSPACKSINOCTREE	5

#define PE_CORE_SCENE_MINMODELSNODESIZE			2.0f
#define PE_CORE_SCENE_MINTREESNODESIZE			20.0f
#define PE_CORE_SCENE_MINLIGHTSNODESIZE			10.0f
#define PE_CORE_SCENE_MINGRASSPACKSNODESIZE		10.0f

#define PE_CORE_SCENE_MAXELEMENTARRAYSIZE 100

#define PE_CORE_SCENE_DISTANCELEVELS	4		// how many list for models are stored in octree

#define PE_CORE_SCENE_DISTANCELEVELVAL0 50.0f
#define PE_CORE_SCENE_DISTANCELEVELVAL1 200.0f
#define PE_CORE_SCENE_DISTANCELEVELVAL2 500.0f
#define PE_CORE_SCENE_DISTANCELEVELVAL3 1000.0f


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumEngine::Components;
using namespace ParabellumEngine::Core::Graphics;


namespace ParabellumEngine
{
	namespace Core
	{

		namespace Scene
		{
			//
			//
			// Store Components in 3D Octree
			//
			//
			class XYZ_API Octree
			{
	
			//
			// Construstors n destructors
			//
			public:
				Octree();
				Octree(Octree&) = delete;
				~Octree();

			//
			//
			//
			public:
	
				// root node initialization
				EUINT32 Initialize(_IN_ Vector3D& min, Vector3D& max);

				// release octree and all its children
				void Release();

				// draw octree
				void DrawRecursive(_IN_ GraphicsDevice* pDevice, _IN_ Shader* pShader,
					_IN_ VertexBuffer* pVB, _IN_ IndexBuffer* pIB, _IN_ Camera* pCamera);

				// draw octree - this draw ONLY visible nodes
				void DrawVisible(_IN_ GraphicsDevice* pDevice, _IN_ Shader* pShader,
					_IN_ VertexBuffer* pVB, _IN_ IndexBuffer* pIB, _IN_ Camera* pCamera, _IN_ RenderTree* pRenderTree);


			private:

				// Add extra nodes to the octree
				// depth - how many octree levels of nodes should be added
				EUINT32 CreateFull(EUINT32 depth);

				// initialize children
				EUINT32 Initialize(_IN_ Octree* pParent, _IN_ Vector3D& min, Vector3D& max);

				// create children for the octree
				EUINT32 CreateChildren();


			public:

				// pComponent MUST derive from Component3D! For better performance it does NOT check class derivation or if object is already in octree!
				template<typename T>
				void Add(_IN_ T* pComponent)
				{					
					if (m_children == NULLPTR)
					{
						m_placedComponentsCount++;
						GetList<T>()->push_back(pComponent);
					}
					// if children exist
					else
					{
						// add to child if child is big enought (child AABB contains model AABB)
						EUINT32 ires = IntersectCodes::DISJOINT;
						bool isFound = false;

						for (int i = 0; i < 8; ++i)
						{
							ires = m_children[i].GetBoundingBox()->Intersects(((Component3D*)pComponent)->GetTransformedBoundingBox());

							if (ires == IntersectCodes::CONTAIN)
							{
								m_children[i].Add<T>(pComponent);
								isFound = true;
								break;
							}
						}

						// add to this node if children are too small
						if (isFound == false)
						{
							m_placedComponentsCount++;
							GetList<T>()->push_back(pComponent);
						}
						
					}
				}



				void AddEModel(_IN_ EModel* pComponent)
				{
					//
					// check if can be added to child
					if (m_children != NULLPTR)
					{
						IntersectCodes ires = IntersectCodes::DISJOINT;
						Vector3D pos;

						for (EUINT32 i = 0; i < 8; ++i)
						{
							//ires = m_children[i].GetBoundingBox()->Intersects(pComponent->GetTransformedBoundingBox());
							pComponent->GetRelativePosition(pos);

							ires = m_children[i].GetBoundingBox()->Intersects(&pos);

							if (ires == IntersectCodes::CONTAIN)
							{
								m_children[i].AddEModel(pComponent);
								break;
							}
						}

						if (ires == IntersectCodes::DISJOINT)
						{
							//printf("\n\nCOS SIE ZJEBALO :");
							//printf("\n   comp pos = %s ", pos.ToString().c_str());

							for (EUINT32 i = 0; i < 8; ++i)
							{
								//printf("\nbox[%u] ", i);
								//printf(" min = %s", m_children[i].GetBoundingBox()->GetMin()->ToString().c_str());
								//printf(" max = %s", m_children[i].GetBoundingBox()->GetMax()->ToString().c_str());
							}
						}

					}
					// try to add to this node
					else
					{

						//
						// add component to the node
						m_objectsEModel[m_totalComponentsCount] = pComponent;
						m_placedComponentsCount++;
						m_totalComponentsCount++;
						m_componentsEModel.push_back(pComponent);

						
						m_placeHolder.Add(pComponent);


						//
						// check if we have achieved the limit and redistrubude it again
						if (m_placedComponentsCount > PE_CORE_SCENE_MAXMODELSINOCTREENODE)
						{
							//printf("\n\nredist");

							CreateChildren();

							for (auto it : m_componentsEModel)
								AddEModel(it);

							m_componentsEModel.clear();

							m_totalComponentsCount = 0;

							m_placeHolder.ClearAll();
						}

						
					}

					/*
					if (m_children == NULLPTR)
					{

						if (m_placedComponentsCount > PE_CORE_SCENE_MAXMODELSINOCTREE)
						{
							CreateChildren();
							AddEModel(pComponent);
							return;
						}
						else
						{
							m_placedComponentsCount++;
							m_componentsEModel.push_back(pComponent);
							printf("\n add1 to %u ; placed = %u", (EADDR)this, m_placedComponentsCount);
						}

						
					}
					// if children exist
					else
					{
						// add to child if child is big enought (child AABB contains model AABB)
						EUINT32 ires = IntersectCodes::DISJOINT;
						bool isFound = false;

						for (int i = 0; i < 8; ++i)
						{
							//Vector3D pos;
							//pComponent->GetRelativePosition(pos);

							ires = m_children[i].GetBoundingBox()->Intersects(pComponent->GetTransformedBoundingBox());

							//ires = m_children[i].GetBoundingBox()->Intersects(&pos);

							if (ires == IntersectCodes::CONTAIN)
							{
								m_children[i].AddEModel(pComponent);
								isFound = true;
								break;
							}
						}

						// add to this node if children are too small
						if (isFound == false)
						{
							m_placedComponentsCount++;
							m_componentsEModel.push_back(pComponent);
							printf("\n add2 to %u", (EADDR)this);
						}

					}
					*/
				}




				// Get OctreeNodes visible by Camera
				void GetNodes(
					ParabellumFramework::Narray<Octree*>* nodes,
					_IN_ Camera* pCamera
					)
				{
					// ignore nodes without components
					if (m_placedComponentsCount == 0)
						return;

					// ignore the node if its bounding sphere is not seen in camera
					IntersectCodes intersect_OctreeSphereWithCamera = pCamera->Intersects(&m_boundingSphere);

					//if (intersect_OctreeSphereWithCamera == IntersectCodes::DISJOINT)
					//	return;

					// ignore the node if its box is not seen in camera
					//if (pCamera->Intersects(&m_box) == IntersectCodes::DISJOINT)
					//	return;

					if (m_placeHolder.m_EModelsCount > 0)
					{
						nodes->Add(this);
					}

					//
					// add its children recursively
					if (m_children != NULLPTR)
						for (EUINT32 i = 0; i < 8; ++i)
						{
							m_children[i].GetNodes(nodes, pCamera);
						}

				}





				// Get OctreeNodes which fit to BoundingBox
				void GetNodes(
					ParabellumFramework::Narray<Octree*>* nodes,
					_IN_ BoundingBox* pBoundingBox
					)
				{
					// ignore nodes without components
					if (m_placedComponentsCount == 0)
						return;

					// ignore the node if its bounding sphere is not seen in camera
					IntersectCodes intersection = pBoundingBox->Intersects(&m_boundingSphere);

					if (intersection == IntersectCodes::DISJOINT)
						return;

					if (m_placeHolder.m_EModelsCount > 0)
					{
						nodes->Add(this);
					}

					//
					// add its children recursively
					if (m_children != NULLPTR)
						for (EUINT32 i = 0; i < 8; ++i)
						{
							m_children[i].GetNodes(nodes, pBoundingBox);
						}

				}


			//
			// Members
			//
			public:

				Vector3D m_center;
				Vector3D m_min;
				Vector3D m_max;

				// parent node
				Octree* m_parent;


				// children
				Octree* m_children;

				Octree* m_childUFL; // points to m_children[0]
				Octree* m_childUFR; // points to m_children[1]
				Octree* m_childUBL; // points to m_children[2]
				Octree* m_childUBR; // points to m_children[3]
				Octree* m_childDFL; // points to m_children[4]
				Octree* m_childDFR; // points to m_children[5]
				Octree* m_childDBL; // points to m_children[6]
				Octree* m_childDBR; // points to m_children[7]


				BoundingBox m_box;
				BoundingSphere m_boundingSphere;


				// placeholder for node objects
				PlaceHolder m_placeHolder;


				// list with models -it isslow as shit, so m_objectsEModel should be used
				// durung paring the nodes
				std::list<EModel*> m_componentsEModel;


				// array with const size - for better CPU performance
				// working with m_placedComponentsCount
				EModel* m_objectsEModel[PE_CORE_SCENE_MAXMODELSINOCTREENODE];


				// how many models can be added to node
				EUINT32 m_maxModelsCount;			

				// total number of components placed ONLY in this node
				EUINT32 m_placedComponentsCount;

				// total number of components placed in this node and in the children nodes
				EUINT32 m_totalComponentsCount;

				// level of the node
				EUINT32 m_depth;


				// has something with component in this node changed (added/emoved, changed position etc)
				EBOOL m_hasContentChanged;



			//
			// Gets and sets
			// 
			public:

				BoundingBox* GetBoundingBox();
				BoundingSphere* GetBoundingSphere();

			public:


				// return a pointer to the list for a specific class. List stores pointers to object of that class
				template<typename T>
				std::list<T*>* GetList()
				{
					static std::map<unsigned int, std::list<T*> > lst;

					return &lst[(unsigned int)this];
				}


			//
			// Get Components
			//
			public:

				// return this node and children nodes components of the specific class
				template<typename T>
				void GetOfType(std::list<T*>* list)
				{			
					for (auto it : *GetList<T>())
						list->push_back(it);

					if (m_children != NULLPTR)
						for (int i = 0; i < 8; ++i)
							m_children[i].GetOfType<T>(list);
					
				}

				// return level of the node
				EUINT32 GetDepth()
				{
					return m_depth;
				}


			//
			// Operators
			//
			public:




			};
		}
	}
}


#endif