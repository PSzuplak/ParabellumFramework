#ifndef _CLOUD_H_
#define _CLOUD_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/UnitHelper.h"
#include "../ParabellumFramework/RenderTarget.h"

#include "Component3D.h"
#include "Camera.h"
#include "RenderQuad.h"
#include "PrimitivesFactory.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::Resources;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumEngine::Core::Graphics;


namespace ParabellumEngine
{
	namespace Components
	{

		struct CloudVertexFormat
		{
			EFLOAT32 position[3];
			EFLOAT32 coords[2];
			EFLOAT32 properties[2];
		};



		//
		// Cloud
		//
		class XYZ_API Cloud : public Component3D
		{
		public:
			Cloud(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);
			Cloud(Cloud&) = delete;
			~Cloud();



		//
		// Members
		//
		public:

			void DrawMerged(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);

		//
		// Members
		//
		public:

			VertexBuffer* m_vertexBuffer;
			IndexBuffer* m_indexBuffer;

			RenderQuad m_renderQuad;

			RenderQuad m_placementQuad;

			Model* m_MSphereModel;

			RenderTarget* m_RTClouds;
			RenderTarget* m_RTCloudsBluredVertical;
			RenderTarget* m_RTCloudsBluredHorizontal;
			RenderTarget* m_RTFinal;

			Shader* m_SDrawCloud;
			Shader* m_SDrawCloudSphere;
			Shader* m_SBlurVertical;

			Texture2D* m_T2DCloud;
			Texture2D* m_T2DCloudSphere;

			BoundingSphere* m_cloudSpheres;
			EUINT32 m_cloudSpheresCount;


		Texture2D* tex;
		//
		// Gets and sets
		//
		public:





		};


	}
}
#endif