#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"


#include "Component3D.h"
#include "BaseLight.h"
#include "Camera.h"

#include "TerrainVertexFormat.h"


namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumFramework::Graphics;

		//
		// Directinal Light
		//
		class XYZ_API DirectionalLight : public BaseLight
		{
		public:
			DirectionalLight();
			~DirectionalLight();

		private:
			DirectionalLight(const DirectionalLight&) = delete;



		//
		// Methods
		// 
		public:

			void Initialize(_IN_ Vector3D& lightDirection, _IN_ Vector3D& lightColor, EFLOAT32 lightIntensity);


			void Update();


			void CalculateLight3DData(_IN_ Camera* pCamera);


			void CreateLightViewProjectionMatrix(_IN_ EUINT32 no);

		//
		// Members
		//
		public:

			Vector3D m_lightDirection;
			Vector3D m_lightColor;
			EFLOAT32 m_intensity;

			//
			// data used in CSM
			//
			Vector3D m_position;
			Vector3D m_target;
			Vector3D m_up;


			Vector3D m_positions[9];
			Vector3D m_targets[9];
			Vector3D m_ups[9];

			Matrix4x4 m_view[9];
			Matrix4x4 m_viewTemp[9];

			Matrix4x4 m_projection[9];
			Matrix4x4 m_viewProjection[9];

			EFLOAT32 m_zNear[9];
			EFLOAT32 m_zFar[9];

			EFLOAT32 m_projectionsSizes[9];

			// sizes of part of bounding frustum
			// for example if m_siceSize[0] is 0.1 then the Top Left corner of
			// the first part is 10% of full frustum corner far from the camera
			EFLOAT32 m_sliceSize[9];	
										
									
			// AABB array obeys an area of all parts of 'sliced;' camera frustum
			// used do determine which depth map should be sampled
			BoundingBox m_CSMBoxes[9];

			// bounding frustums for light cameras
			BoundingFrustum m_frustums[9];	

			// how many CMS layers are displayed
			EUINT32 m_CSMCount;


		//
		// Gets and Sets
		//
		public:

			Vector3D* GetDirection();
			Vector3D* GetColor();
			EFLOAT32 GetIntensity();

			void SetDirection(Vector3D& val);
			void SetColor(Vector3D& val);
			void SetIntensity(EFLOAT32 val);


			BoundingBox* GetCSMBox(int no)
			{
				return &m_CSMBoxes[no];
			}


			BoundingFrustum* GetFrustum(int no)
			{
				return &m_frustums[no];
			}


			BoundingFrustum* GetFrustums()
			{
				return m_frustums;
			}
		};


	}
}
#endif