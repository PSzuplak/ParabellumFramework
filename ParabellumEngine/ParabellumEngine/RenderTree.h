#ifndef _RENDERTREE_H_
#define _RENDERTREE_H_

//
//	Predefs
//
#include "DLLIE.h"

#include <thread>

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/Vector3D.h"
#include "../ParabellumFramework/Deck.h"

#include "Camera.h"




//
// Defines
//
#define PE_RENDER_SETTINGS_TRANSPARENCY				(1 << 3)		// are some transparent models in this region


#define PE_RENDER_SETTINGS_DIRECTIONALLIGHTBASIC	(1 << 12)		// basic directinal light Phong model - no shadows, translucency and others
#define PE_RENDER_SETTINGS_DIRECTIONALLIGHT			(1 << 13)		// directional light with all its features except shadomapping
#define PE_RENDER_SETTINGS_SHADOWMAPPING			(1 << 14)		// directional light with all its features including shadowmapping
#define PE_RENDER_SETTINGS_PARALLAXMAPPING			(1 << 15)
#define PE_RENDER_SETTINGS_SSAO						(1 << 16)
#define PE_RENDER_SETTINGS_REFLECTIONS				(1 << 17)
#define PE_RENDER_SETTINGS_REFRACIONS				(1 << 18)
#define PE_RENDER_SETTINGS_HDR						(1 << 19)		// High Dynamic Range
#define PE_RENDER_SETTINGS_VOLUMETRICLIGHT			(1 << 20)		// Volumetric Light

#define PE_RENDER_SETTINGS_FXAA						(1 << 26)		// FX Anti Aliasing
#define PE_RENDER_SETTINGS_MSAA						(1 << 27)		// MSAA
#define PE_RENDER_SETTINGS_EAA						(1 << 28)		// Edge AA






using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumEngine::Components;

namespace ParabellumEngine
{
	namespace Core
	{
		namespace Graphics
		{

			struct VertexTypeRenderTree
			{
				EFLOAT32 position[3];
				EFLOAT32 uv[2];
			};

			struct PropertiesRenderTree
			{
				// flags are set if this node is FULLY (all its children flags are set to 1) - lighted, parallaxed etc
				EUINT32 flags;

				// flags are set if one of the child node is partially (one of the child of child of child etc. flags are set to 1) - lighted, parallaxed etc
				// added to minimize recursive Render() calls count
				EUINT32 childrenProperties;

				// minimum value of depth
				EFLOAT32 minDepth;

				// maximum value of depth
				EFLOAT32 maxDepth;

				// average color of the scene
				Vector4D averageColor;

				// its basically averageColor.X * 0.3f + averageColor.Y * 0.4f + averageColor.Z * 0.3f
				EFLOAT32 averageLuminance;


				EFLOAT32 luminanceMin;

				EFLOAT32 luminanceMax;

				bool isBlindSpot; // blind spot is an area where is no any model component.

				PropertiesRenderTree()
				{
					flags = 0;
					childrenProperties = 0;
					minDepth = +99999.0f;
					maxDepth = -99999.0f;
					averageColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
					averageLuminance = 0.0f;
					luminanceMin = 0.0f;
					luminanceMax = 0.0f;
					isBlindSpot = false;
				}

				~PropertiesRenderTree()
				{

				}

				void Clear()
				{
					flags = 0;
					childrenProperties = 0;
					minDepth = +99999.0f;
					maxDepth = -99999.0f;
					averageColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
					averageLuminance = 0.0f;
					luminanceMin = 0.0f;
					luminanceMax = 0.0f;
					isBlindSpot = false;
				}
			};


			//
			//
			// RenderTree - better than render quad and renderArea :)
			//
			//
			class XYZ_API RenderTree
			{
			private:

				// VertexBuffers should point to m_vertexBuffer of all nodes
				static VertexBuffer** VertexBuffers;

				// VertexBuffers should point to m_vertexBuffer of all nodes
				static IndexBuffer** IndexBuffers;

			public:
				RenderTree();
				RenderTree(RenderTree&) = delete;
				~RenderTree();


			public:

				// depth 5 means 32 quads in row and 32 in cols,
				// depth 6 means 64 quads in row and 64 in cols
				void Initialize(_IN_ GraphicsDevice* pDevice, EUINT32 depth);

			private:

				void Initialize(
					_IN_ GraphicsDevice* pDevice, _IN_ _KEEP_ RenderTree* pParent,
					EUINT32 countX, EUINT32 countY, Vector4D position, EUINT32 treeDepth, EUINT32 depth
					);

			public:
				void Release(_IN_ GraphicsDevice* pDevice);



				//
				// Render RenderTree which nodes matches to options in flagsp parametr
				// averageColor will be set in pShader in paramter pShader->GetParametr(paramNo)->(averagecolor)
				// if pShader is NULL no average color will be copied
				// 
				void Render(_IN_ GraphicsDevice* pDevice, _IN_ Shader* pShader, EUINT32 paramNo, EUINT32 flagsp, EUINT32& no);

				EUINT32 Setup(PropertiesRenderTree* properties, EUINT32 width, EUINT32 height);

				
				// clear PropertiesFlags and childrens properties
				void ClearChild(EUINT32 childNo);

				// clear PropertiesFlags and childrens properties
				void Clear();



				// 
				// check if nodes covers whole bounding box - using minZ and maxZ from properties
				//
				IntersectCodes Intersects(_IN_ Camera* pCamera, _IN_ BoundingBox* pBox);


				IntersectCodes Intersects(_IN_ Camera* pCamera, _IN_ BoundingSphere* pSphere);



				// 
				// check if area in Screen Coordinates intersects with RenderTree
				//
				EBOOL Covers(Vector4D& area, EFLOAT32 areaDepth);


				void AddToDeck(EUINT32 componentId)
				{
					m_deck[m_deckFront].Add(componentId);
				}

				void ClearDeck()
				{
					m_isDeckReady = false;
					m_deck[m_deckFront].CleanUp();
				}

				void SwapDeck()
				{
					EUINT32 tmpu = m_deckFront;
					m_deckFront = m_deckBack;
					m_deckBack = tmpu;
				}

				Deck* GetDeckFront()
				{
					return &m_deck[m_deckFront];
				}

				Deck* GetDeckBack()
				{
					return &m_deck[m_deckBack];
				}


			public:

				// 32bits of properties  - flag is set only if RenderTree node is FULLY lighted/parallex etc - it means that ALL its cildren must be FULLY lighted
				//EUINT32 PropertiesFlags;

				PropertiesRenderTree Properties;

				// is RenderTree processing Setup function
				EBOOL m_isProcessing;

			private:

				Vector4D m_screenPosition;

				EUINT32 m_treeDepth;
				EUINT32 m_nodeDepth;

				RenderTree* m_parent;

				RenderTree* m_children[4];

				RenderTree* m_childTL;
				RenderTree* m_childTR;
				RenderTree* m_childBR;
				RenderTree* m_childBL;


				VertexBuffer* m_vertexBuffer;
				IndexBuffer* m_indexBuffer;

				EUINT32 m_countX;
				EUINT32 m_countY;

				EUINT32 m_indexX;
				EUINT32 m_indexY;

				EUINT32 m_depth;

			private:
				Deck m_deck[2];	// Models visible in the current frame

				EUINT32 m_deckFront;
				EUINT32 m_deckBack;


				bool m_isDeckReady;






			//
			// Gets and Sets
			//
			public:

				EUINT32 GetCount();


			};
		}
	}

}


#endif