#ifndef _DEFERREDRENDERER_H_
#define _DEFERREDRENDERER_H_


//
// defines
//
#define DR_MAX_MODELPARTS 2048

#include <iostream>
#include <mutex>

//
// includes
//
#include "../ParabellumFramework/GraphicsDeviceTight.h"

#include "../ParabellumFramework/Profiler.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/ResourceManager.h"

#include "RenderQuad.h"
#include "RenderArea.h"
#include "RenderTree.h"

#include "Camera.h"

#include "../ParabellumFramework/ModelPart.h"
#include  "../ParabellumFramework/Deck.h"

#include "EModel.h"
#include "EModelPart.h"

#include "SceneCart.h"

#include "Grass.h"
#include "TerrainCell.h"
#include "Terrain.h"

#include "Skydome.h"

#include "Particles.h"

#include "PrimitivesFactory.h"

#include "FrustumOutput.h"



// extensions
#include "ExDRLight.h"
#include "ExDRSSAO.h"
#include "ExDRHDR.h"
#include "ExDRParallaxMapping.h"
#include "ExDRVolumetricLight.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::Resources;
using namespace ParabellumEngine::Components;

namespace ParabellumEngine
{
	namespace Core
	{
		namespace Graphics
		{

			using namespace ParabellumFramework;
			using namespace ParabellumFramework::Graphics;
			using namespace ParabellumEngine::Core::Utiles;

			//
			//
			// Deferred Renderer
			//
			//
			class XYZ_API DeferredRenderer
			{

			public:
				ParabellumEngine::Core::Graphics::DeferredRenderer::DeferredRenderer();
				ParabellumEngine::Core::Graphics::DeferredRenderer::~DeferredRenderer();

			private:
				ParabellumEngine::Core::Graphics::DeferredRenderer::DeferredRenderer(DeferredRenderer&) = delete;
				
				Vector4D* ccc;


			//
			// Settings
			//
			public:

				EUINT32 Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				EUINT32 Release();


			//
			// Rendering
			//
			public:

				EUINT32 xxxxdrawcalls;

				EUINT32 xxxxdrawcallsA;
				EUINT32 xxxxdrawcallsB;
				
				// MUST be called at the beginning of each frame
				void PrepareToDraw(_IN_ GraphicsDevice* pDevice);


				// Set default values for GBuffer Solid
				void PrepareAndSetGBufferSolid(_IN_ GraphicsDevice* pDevice);



				// Draw models solid nearest the camera. After this GBufferSolidGenerateMipMaps should be called
				void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawSolidPhase1(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ Utiles::FrustumOutput* pFrustumOutput
					);


				void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawSolidBack(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);



				// draw terrain cells
				void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawTerrainCells(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);


				// draw terrain cells
				void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawExtensions(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ Utiles::FrustumOutput* pFrustumOutput);


				// merge solid with particles
				void ParabellumEngine::Core::Graphics::DeferredRenderer::MergeSolidNParticles(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);



				void ParabellumEngine::Core::Graphics::DeferredRenderer::MergeGBufferSolidWithLightMap(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);


				// Display buffer
				void GBufferSolidGenerateMipMaps(_IN_ GraphicsDevice* pDevice);

				// Display buffer
				void DisplayBuffer(_IN_ GraphicsDevice* pDevice, int no);


				// Display buffer
				void DisplayBuffer(_IN_ GraphicsDevice* pDevice, _IN_ RenderTarget* pRT);


			//
			// Threads
			//
			private:

				std::thread m_threadGBufferSolidGenerateMipMaps;

			//
			// Members
			//
			public:

				EUINT32 m_sliceCount = 8;
				EUINT32 m_sliceNo = 0;
				EUINT32 m_sliceSize = 64;

				//
				//	Screen info
				//
				EUINT32 m_screenWidth;
				EUINT32 m_screenHeight;

				//
				//	Screen Quads
				//
				RenderQuad m_fullScreenQuad;		// render quad from (0,0) to (1,1) in projection cooridnates

				//RenderArea m_renderArea;

				RenderTree m_renderTree;

				//
				//	Render Targets
				//

				// GBuffers Render Targets - each of them are R32G32B32A32 format
				// so :
				// 

				// RT for Material properties
				// R - packed color (Alpha channel for Solid Models is translucency) as Float4 to Float, (assuming color depth is 32bits - 8bits per each channel)
				// G - packed Reflection, Height Factor, RESERVED, RESERVED as Float4 to Float
				// B - Model/Component ID
				// A - position view Z
				RenderTarget* m_RTGBufferSolid0;


				// RT for Light Material properties
				// R - Normal bumped XY World Space (each X,Y as Float16)
				// G - Normal bumped Z  World Space, RESERVED  (each X,Y as Float16)
				// B - packed Specular intensty, Specular power, Roughtness, Opacity(translucency) as Float4 to Float
				// A - packed Thickness/Translucency, Glow, Emissive, RESERVED as Float4 to Float
				RenderTarget* m_RTGBufferSolid1;	

				// RT for extensin properties
				// R - Normal XY View Space (each X,Y as Float16)
				// G - Normal Z View Space, Reserved  (each X,Y as Float16)
				// B - Tangent XY View Space (each X,Y as Float16)
				// A - Tangent Z View Space, HeightFactor  (each X,Y as Float16)
				RenderTarget* m_RTGBufferSolid2;	


				// RT position of hidden vertices
				RenderTarget* m_RTGBufferSolidBack0;
				RenderTarget* m_RTGBufferSolidBack1;
				RenderTarget* m_RTGBufferSolidBack2;
				RenderTarget* m_RTGBufferSolidBack3;



				//
				// Rt for mered GBuffer with lightmap
				RenderTarget* m_RTMergeGBufferLightMap;


				//
				// RT contains some solid buffer properties and Volumetric lighting
				// This one should be used for MipMaps
				//
				RenderTarget* m_RTMergeGBufferWithEffects;


				//
				// RT for shrinked m_RTGBufferSolid1 - 7 RTs
				// 4x4 is used as histogram - MipMaps should be connected with lightmap first
				RenderTarget* m_RTGBufferSolidMipMaps[8];

				RenderTarget* m_RTGBufferProperties;			// properties in float from m_RTGBufferSolidMipMaps
				RenderTarget* m_RTGBufferPropertiesHisogram;	// histogram from m_RTGBufferSolidMipMaps

				//
				// GBUffer for particles
				RenderTarget* m_RTGBufferParticlesFront0;	
				RenderTarget* m_RTGBufferParticlesFront1;
				RenderTarget* m_RTGBufferParticlesBack0;

				RenderTarget* m_RTParticleLightMap;

				// merged
				RenderTarget* m_RTGBufferMergeBasics;

				Texture2D* TEX01;



				// depth buffer based on sphere which center is placed at camera position
				// it should be used for Volumetric Light and Global Illumination
				//RenderTarget* m_RTVolumeDepthMap;



				//
				// Shaders
				//
				Shader* m_ShaderCreateGBufferSolid;
				Shader* m_ShaderCreateGBufferSolidFar;
				Shader* m_ShaderCreateGBufferSolidInstanced2;
				Shader* m_ShaderCreateGBufferSolidInstanced8;

				Shader* m_ShaderCreateGBufferSolid_GPUCalc;
				Shader* m_ShaderCreateGBufferSolid_GPUCalcI8;


				Shader* m_SMergeGBufferSolidLightMap;





				Shader* m_ShaderDisplayQuad;

				Shader* m_SParticles;

				Shader* m_SMergeSolidNParticles;


				Shader* m_SMergeGBufferWithEffects;

				Shader* m_SShrinkMaterialProperties;


				Shader* m_SCopyGBufferProperties;		// copyies properties from m_RTGBufferSolidMipMaps to m_RTGBufferProperties
				Shader* m_SCopyGBufferHistogram;		// copyies histogram from m_RTGBufferSolidMipMaps to m_RTGBufferPropertiesHisogram



				Shader* SDrawOctree;


				Shader* m_SParticlesDirectionalLight;





				//
				// Extenstions
				// 
				ExDRLight m_ExDRLight;
				ExDRSSAO m_ExDRSSAO;
				ExDRHDR m_ExDRHDR;

				ExDRParallaxMapping m_ExDRParallaxMapping;

				ExDRVolumetricLight m_ExDRVolumetricLight;



				bool m_isFeatureParallaxMaping;



				//
				// Current frame data
				//

				std::vector<TerrainCell*> m_terrainCells;


				//
				// environment objects
				//
				Grass m_grass;
				Shader* m_ShaderDrawGrass;

				TerrainCell m_terrain;
				Shader* m_ShaderDrawTerrain;

				Camera* cam;



				Terrain* m_terrainz;


				Particles m_particleSmoke;


				//
				// Optimization
				//

				


				//
				// Settings
				//

				// max depth of object for parallax mapping effect
				EFLOAT32 m_settingsMaxParallaxDepth;


				EUINT32 m_settingsRenderTreeFrameSkip;


				//
				// debug data
				//

				// transparent models draw calls
				EUINT32 m_debugDrawCallsTransparent;

				// draw calls for shadowing effects
				EUINT32 m_debugDrawCallsShadows;	

				// how many terrain parts were rendered
				EUINT32 m_debugTerrainPartsInView;		


				EUINT32 m_frameRate;
				EUINT32 m_frameRateLast;

				EUINT32 m_frameCounter;

				std::clock_t m_timeBegin;
				std::clock_t m_timeEnd;




			//
			// Gets And Sets
			//
			public:

				RenderTarget* GetRenderTarget(EUINT32 no)
				{
					if (no == 0)
					{
						return m_RTGBufferSolid0;
					}

					return NULLPTR;
				}

			};

		}
	}
}



#endif