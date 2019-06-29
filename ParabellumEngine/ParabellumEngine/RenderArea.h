#ifndef _RENDERAREA_H_
#define _RENDERAREA_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/Vector3D.h"


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;

namespace ParabellumEngine
{
	namespace Core
	{
		namespace Graphics
		{

			struct VertexTypeRenderArea
			{
				EFLOAT32 position[3];
				EFLOAT32 uv[2];
			};

			struct RenderProperties
			{
				Vector4D averageColor;

				// minimum value of view.Z of all pixels
				EFLOAT32 minDepth;	

				// maximum value of view.Z of all pixels
				EFLOAT32 maxDepth;		

				// if a model obey whole square this variable stores 
				// distance from camera position to center of bounding sphere - sphere ray
				// it can be used to remove models from papeline which are placed farer and their size is smaller than square (best to sort them first)
				EFLOAT32 obeyModelDistance; 

				bool isObeyed;
				bool isSolidObjects;
				bool isTransparentObjects;
				bool isParticleObjects;
				bool isLight;
				bool isShadow;
				bool isSSAO;
				bool isReflection;
				bool isRefraction;
				bool isEmissive;
				bool isGlow;
				bool isVolumetricLight;
				bool isParallaxMapping;
				bool isParallaxMapping1;
				bool isParallaxMapping2;
				bool isParallaxMapping3;
			};



			class EffectsApplied
			{
			public:
				EffectsApplied(EUINT32 size)
				{
					ReflectionCount = 0;
					SolidObjectsCount = 0;
					TransparentObjectsCount = 0;
					ParticleObjectsCount = 0;
					LightCount = 0;
					ShadowCount = 0;
					SSAOCount = 0;
					ReflectionCount = 0;
					RefractionCount = 0;
					EmissiveCount = 0;
					GlowCount = 0;
					VolumetricLightCount = 0;
					ParallaxMappingCount = 0;

					indicesOfReflection = new EUINT32[size];
					indicesOfSolidObjects = new EUINT32[size];
					indicesOfTransparentObjects = new EUINT32[size];
					indicesOfParticleObjects = new EUINT32[size];
					indicesOfLight = new EUINT32[size];
					indicesOfShadow = new EUINT32[size];
					indicesOfSSAO = new EUINT32[size];
					indicesOfReflection = new EUINT32[size];
					indicesOfRefraction = new EUINT32[size];
					indicesOfEmissive = new EUINT32[size];
					indicesOfGlow = new EUINT32[size];
					indicesOfVolumetricLight = new EUINT32[size];
					indicesOfParallaxMapping = new EUINT32[size];
				}

				~EffectsApplied()
				{

				}


			public:

				void ClearData()
				{
					ReflectionCount = 0;
					SolidObjectsCount = 0;
					TransparentObjectsCount = 0;
					ParticleObjectsCount = 0;
					LightCount = 0;
					ShadowCount = 0;
					SSAOCount = 0;
					ReflectionCount = 0;
					RefractionCount = 0;
					EmissiveCount = 0;
					GlowCount = 0;
					VolumetricLightCount = 0;
					ParallaxMappingCount = 0;
				}




				void AddAreaForParallaxMapping(EUINT32 no)
				{
					indicesOfParallaxMapping[ParallaxMappingCount] = no;
					ParallaxMappingCount++;
				}

			public:
				EUINT32* indicesOfSolidObjects;
				EUINT32* indicesOfTransparentObjects;
				EUINT32* indicesOfParticleObjects;
				EUINT32* indicesOfLight;
				EUINT32* indicesOfShadow;
				EUINT32* indicesOfSSAO;
				EUINT32* indicesOfReflection;
				EUINT32* indicesOfRefraction;
				EUINT32* indicesOfEmissive;
				EUINT32* indicesOfGlow;
				EUINT32* indicesOfVolumetricLight;
				EUINT32* indicesOfParallaxMapping;

				EUINT32 SolidObjectsCount;
				EUINT32 TransparentObjectsCount;
				EUINT32 ParticleObjectsCount;
				EUINT32 LightCount;
				EUINT32 ShadowCount;
				EUINT32 SSAOCount;
				EUINT32 ReflectionCount;
				EUINT32 RefractionCount;
				EUINT32 EmissiveCount;
				EUINT32 GlowCount;
				EUINT32 VolumetricLightCount;
				EUINT32 ParallaxMappingCount;
			};





			//
			//
			//
			//
			//
			class XYZ_API RenderArea
			{

			public:
				RenderArea();
				RenderArea(RenderArea&) = delete;
				~RenderArea();



			public:
				void Initialize(_IN_ GraphicsDevice* pDevice, EUINT32 countX, EUINT32 countY);
				void Release(_IN_ GraphicsDevice* pDevice);


			public:

				RenderProperties* Properties;
				
				EffectsApplied* Effects;


			public:
				VertexBuffer** m_vertexBuffers;
				IndexBuffer** m_indexBuffers;

				EUINT32 m_countX;
				EUINT32 m_countY;

				


			//
			// Gets and Sets
			//
			public:

				EUINT32 GetCount()
				{
					return m_countX*m_countY;
				}

				void ClearAllProperties()
				{
					//for (EUINT32 i = 0; i < GetCount(); ++i)
					{
						memset(&Properties[0], 0, sizeof(RenderProperties) * GetCount());
					}
				}

				void ClearProperties(EUINT32 nx, EUINT32 ny)
				{
					memset(&Properties[ny*m_countX + nx], 0, sizeof(RenderProperties));
				}



				






			};
		}
	}

}


#endif