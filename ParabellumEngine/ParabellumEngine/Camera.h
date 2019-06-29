#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/DebugManager.h"

#include "Component3D.h"


namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;


		class XYZ_API Camera : public Component3D
		{


		//
		// Coinstructors
		// 
		public:
			Camera();
			~Camera();
		private:
			Camera(const Camera&);

		public:
			EUINT32 Initialize(_IN_ Vector3D& position, _IN_ Vector3D& target, _IN_ Vector3D& up,
				EUINT32 screenWidth, EUINT32 screenHeight, EFLOAT32 fov, EFLOAT32 nearPlane, EFLOAT32 farPlane);

			void Update();


			IntersectCodes Intersects(_IN_ BoundingSphere* pVolume);
			IntersectCodes Intersects(_IN_ BoundingBox* pVolume);
			IntersectCodes Intersects(_IN_ Vector3D* pVolume);
			IntersectCodes Intersects(_IN_ Component3D* pC3D);

		private:
			void UpdateMatrices();			// update set of camera matrices


		//
		// Members
		//
		public:
			//Vector3D m_position;
			Vector3D m_direction;
			Vector3D m_up;



			// projection matrix info
			EFLOAT32 m_nearPlane;
			EFLOAT32 m_farPlane;
			EFLOAT32 m_fov;
			EFLOAT32 m_aspectRatio;

			EUINT32 m_screenWidth;
			EUINT32 m_screenHeight;

			// camera properties
			EFLOAT32 m_speed;		// camera movement speed


									// camera matrices
			Matrix4x4 m_view;
			Matrix4x4 m_projection;
			Matrix4x4 m_viewProjection;
			Matrix4x4 m_inverseView;
			Matrix4x4 m_inverseProjection;
			Matrix4x4 m_inverseViewProjection;
			Matrix4x4 m_transponseInverseProjection;


			// transponsed matrices


			// bounding frustum for camera
			//BoundingSphere m_boundingSphere;
			BoundingFrustum m_frustum;


			Vector4D m_ctl;
			Vector4D m_ctr;
			Vector4D m_cbr;
			Vector4D m_cbl;

			// camera to top left frustum corner To camera to top right frustum corner
			Vector4D m_ctl2ctr;

			// camera to top left frustum corner To camera to botoom left frustum corner
			Vector4D m_ctl2cbl;

			
			Vector4D m_nctl;	// near corner top left
			Vector4D m_nctr;	// near corner top left
			Vector4D m_ncbr;	// near corner top left
			Vector4D m_ncbl;	// near corner top left






			// difference of angle between m_direction from last and current frame (Update())
			Vector3D m_directionPrevious;

			// has angle changed since last Update
			EBOOL m_angleChanged;

			// difference of position between m_direction from last and current frame (Update())
			Vector3D m_positionPrevious;

			// has position changed since last Update
			EBOOL m_positionChanged;


		//
		// gets and sets
		//
		public:
			void GetDirection(_OUT_ Vector3D&);
			Vector3D* GetDirection();
			void GetDirection(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z);

			void GetUp(_OUT_ Vector3D&);
			Vector3D* GetUp();


			EFLOAT32 GetZNear();
			EFLOAT32 GetZFar();
			EFLOAT32 GetFOV();


			void GetCorners(_OUT_ Vector3D* pVectors);




			void GetView(_OUT_ Matrix4x4&);
			Matrix4x4* GetView();

			void GetProjection(_OUT_ Matrix4x4&);
			Matrix4x4* GetProjection();

			void GetViewProjection(_OUT_ Matrix4x4&);
			Matrix4x4* GetViewProjection();

			void GetInverseView(_OUT_ Matrix4x4&);
			Matrix4x4* GetInverseView();

			void GetInverseProjection(_OUT_ Matrix4x4&);
			Matrix4x4* GetInverseProjection();

			void GetInverseViewProjection(_OUT_ Matrix4x4&);
			Matrix4x4* GetInverseViewProjection();

			void GetTransponseInverseProjection(_OUT_ Matrix4x4&);
			Matrix4x4* GetTransponseInverseProjection();

			// Get camera frustum
			BoundingFrustum* GetFrustum();

			//
			// Transform projected (screen) position to world position
			// sScreenPosition.XY - UV of the screen Render target -> for (-1,-1) its (0,0) for (1,1) its (1,1)
			// sScreenPosition.Z - depth from 0.0f - 1.0f
			//
			void WorldPositionFromLinearDepth(_OUT_ Vector3D* pWorld, _IN_ Vector3D* pScreenPosition);

			Vector3D GetWorldFromFrustumCorners(Vector2D UV, EFLOAT32 depth);

			//
			// Get Projected cooridinates of bounding box - put them in pTopLeft and pBottomRight
			//
			void GetProjected(_IN_ BoundingBox* pBox, _OUT_ Vector3D* pTopLeft, _OUT_ Vector3D* pBottomRight);


			Vector4D* GetCTL();

			Vector4D* GetCTL2CTR();

			Vector4D* GetCTL2CBL();


			// fast tranforming BoundingBox world cooridnates to screen position - output will be save to 'out'
			// 'out' contains 2 corners of rectangle which obey BoundingBox
			// function returns distance from the camera in normalized cooridanates (distance DIV farPlane)
			void ToScreenPosition(_IN_ BoundingBox* pBox, _OUT_ Vector4D* out, _OUT_ EFLOAT32* outNDist)
			{
				Vector3D center;
				Vector3D corners[8];
				Vector3D camPos;
				Vector4D vec;

				pBox->GetCenter(&center);
				pBox->GetCorners(corners);
				GetPosition(camPos);

				Vector4D area(+2.0f, -2.0f, -2.0f, +2.0f);
				EFLOAT32 areaDepht = 0.0f;
				areaDepht = (camPos - center).Length() / 1000.0f;

				for (int i = 0; i < 8; ++i)
				{
					vec.X = corners[i].X;
					vec.Y = corners[i].Y;
					vec.Z = corners[i].Z;
					vec.W = 1.0f;

					vec = (*GetViewProjection()) * vec;
					vec /= vec.W;

					if (area.X > vec.X) area.X = vec.X;
					if (area.Y < vec.Y) area.Y = vec.Y;
					if (area.Z < vec.X) area.Z = vec.X;
					if (area.W > vec.Y) area.W = vec.Y;
				}


				if (area.X < -1.0f) area.X = -1.0f;
				if (area.X > +1.0f) area.X = +1.0f;
				if (area.Y < -1.0f) area.Y = -1.0f;
				if (area.Y > +1.0f) area.Y = +1.0f;
				

				*out = area;

				*outNDist = areaDepht;
			}


			// fast tranforming BoundingSphere world cooridnates to screen position - output will be save to 'out'
			// 'out' contains 2 corners of rectangle which obey BoundingBox
			// function returns distance from the camera in normalized cooridanates (distance DIV farPlane)
			void ToScreenPosition(_IN_ BoundingSphere* pSphere, _IN_ Vector4D* out, _IN_ EFLOAT32* outNDist)
			{
				Vector3D center, centerUpRay;
				Vector3D corners[8];
				Vector3D camPos;
				Vector4D vec;

				Vector4D screenCenter, screenRay;

				center = *pSphere->GetCenter();

				centerUpRay = center;
				centerUpRay += pSphere->GetRayLength();

				GetPosition(camPos);

				Vector4D area(+2.0f, -2.0f, -2.0f, +2.0f);
				EFLOAT32 areaDepht = 0.0f;

				areaDepht = (camPos - center).Length() / 1000.0f;

				// projected vectors
				screenCenter = (*GetViewProjection()) * center;
				screenCenter /= screenCenter.W;

				screenRay = (*GetViewProjection()) * centerUpRay;
				screenRay /= screenRay.W;

				EFLOAT32 dist = screenRay.Y - screenCenter.Y;

				area.X = screenCenter.X - dist;
				area.Y = screenCenter.Y + dist;

				area.Z = screenCenter.X + dist;
				area.W = screenCenter.Y - dist;

				if (area.X < -1.0f) area.X = -1.0f;
				if (area.X > +1.0f) area.X = +1.0f;
				if (area.Y < -1.0f) area.Y = -1.0f;
				if (area.Y > +1.0f) area.Y = +1.0f;


				*out = area;

				*outNDist = areaDepht;
			}



			// get squared distance from the plane no 'planeNo'
			// 0 - left plane
			// 1 - right plane
			// 2 - top plane
			// 3 - bottom plane
			// 4 - near plane
			// 5 - far plane
			EFLOAT32 GetDistanceFromNearPlane(_IN_ Vector3D* pVolume)
			{
				return m_frustum.GetSquareDistance(1, pVolume);
			}



			void GetFastProjection(_IN_ _ARRAY_ Vector3D* pIn, _OUT_ _ARRAY_ Vector3D* pOut)
			{
				//pOut->X = m_frustum.m_planes[0].GetDistance(&cp2m) * dist;
				pOut->Y = 1.0f;
				pOut->Z = 1.0f;
			}



			//
			// operators
			//
			Camera& operator =(Camera& rhs)
			{
				if (&rhs != this)
				{
					this->Initialize(*rhs.GetPosition(), *rhs.GetDirection(), rhs.m_up, rhs.m_screenWidth, rhs.m_screenHeight, rhs.m_fov, rhs.m_nearPlane, rhs.m_farPlane);
					this->Update();
				}

				return *this;
			}




		};
	}
}
#endif