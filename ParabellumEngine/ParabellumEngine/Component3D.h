#ifndef _COMPONENT3D_H_
#define _COMPONENT3D_H_

#include "DLLIE.h"

#include "../ParabellumFramework/Vector3D.h"
#include "../ParabellumFramework/Matrix4x4.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/Volume.h"
#include "../ParabellumFramework/Event.h"

#include "Component.h"


namespace ParabellumEngine
{

	namespace Components
	{
		//
		//
		// Represents object in 3D space
		//
		//
		class XYZ_API Component3D : public Component, public ParabellumFramework::Event::EventHandler
		{
		private:

		public:
			Component3D();
			Component3D(Component3D&);
			virtual ~Component3D();

		// override
		public:
			virtual void Initialize();
			virtual void Release();
			virtual void Draw();
			virtual void Update();


		private:

			// update world matrix
			void UpdateWorldMatrix();		

			// update bounding sphere and bounding box
			void UpdateBoundingVolumes();				

		private:

			// matrices created after change position/rotation/scale
			Matrix4x4 m_world;
			Matrix4x4 m_translationMatrix;
			Matrix4x4 m_rotationMatrix;
			Matrix4x4 m_scaleMatrix;

			Vector4D m_position;		// position of component in world coordinates
			Vector4D m_rotation;		// object rotation angles - vector x component is value of angle on x axis
			Vector4D m_scale;			// object scale

			// parent-chid relation
			Component3D* m_parent;				
			std::list<Component3D*> m_children;

			

		protected:
			BoundingBox m_boundingBox;
			BoundingSphere m_boundingSphere;
			

		private:
			BoundingBox m_boundingBoxTransformed;
			BoundingSphere m_boundingSphereTransformed;
			OrientedBoundingBox m_boundingBoxExtendedTransformed;
			Volume* m_boundingConvex;

		private:

			// TRUE if  World Matrix needs an update
			bool m_isWorldNeedsUpdate;

			// if TRUE then m_collisionVolume is going to be updated when 
			bool m_isBoundingConvexNeedUpdate;

		//
		// gets and sets
		//
		public:

			PF_INLINE Vector3D* GetPosition();
			PF_INLINE Vector3D* GetRotation();
			PF_INLINE Vector3D* GetScale();

			PF_INLINE void GetPosition(_OUT_ Vector3D& out);
			PF_INLINE void GetRotation(_OUT_ Vector3D& out);
			PF_INLINE void GetScale(_OUT_ Vector3D& out);

			// get absolute (parent position is NOT added) position 
			PF_INLINE void GetPosition(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z);

			// get absolute (parent rotation is NOT added) rotation 
			PF_INLINE void GetRotation(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z);

			// get absolute (parent scale is NOT added) scale 
			PF_INLINE void GetScale(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z);


		public:
			// return position relative to components parent
			PF_INLINE void GetRelativePosition(_OUT_ Vector3D& out);

			// return rotation relative to components parent
			PF_INLINE void GetRelativeRotation(_OUT_ Vector3D& out);

			// return scale relative to components parent
			PF_INLINE void GetRelativeScale(_OUT_ Vector3D& out);

		public:
			PF_INLINE void SetPosition(_IN_ Vector3D& out);
			PF_INLINE void SetRotation(_IN_ Vector3D& out);
			PF_INLINE void SetScale(_IN_ Vector3D& out);

			PF_INLINE void SetPosition(EFLOAT32 x, EFLOAT32 y, EFLOAT32 z);
			PF_INLINE void SetRotation(EFLOAT32 x, EFLOAT32 y, EFLOAT32 z);
			PF_INLINE void SetScale(EFLOAT32 x, EFLOAT32 y, EFLOAT32 z);


		//
		// Transform matrices methods
		//
		private:
			PF_INLINE Matrix4x4* GetTranslationMatrix();
			PF_INLINE Matrix4x4* GetRotationMatrix();
			PF_INLINE Matrix4x4* GetScaleMatrix();

		public:
			PF_INLINE Matrix4x4* GetWorldMatrix();

			//
			// Return pointer to bounding volumes which are already transformed
			// - should be used for static mesh (bounding volumes doesnt change)
			// - transformed bounding volumes are update each time when Component 
			//	 changes position, so it should be used EVERY time
			//
			PF_INLINE BoundingBox* GetTransformedBoundingBox();
			PF_INLINE BoundingSphere* GetTransformedBoundingSphere();
			PF_INLINE OrientedBoundingBox* GetTransformedOrientedBoundingBox();

			// An 'Volume' transformation is very time consuming, so the 'Volume' will be tranformated by a world matrix
			// only when THIS method is called
			PF_INLINE Volume* GetBoundingConvex();

			//
			// Get Bounding Volumes in components Local Coordinates
			//
			PF_INLINE BoundingBox* GetLocalBoundingBox();
			PF_INLINE BoundingSphere* GetLocalBoundingSphere();


			//
			// Transform bounding volumes to World Space and then copy it to pointers
			// - actual there is no reason to use these
			//


			// get PRIMAL bounding boxes - this is not scaled or moved by component members
			// use GetTransformedBoundingBox()
			PF_INLINE virtual void GetBoundingBox(_OUT_ BoundingBox* box);


			// get PRIMAL bounding boxes - this is not scaled or moved by component members
			// use GetTransformedBoundingSphere()
			PF_INLINE virtual void GetBoundingSphere(_OUT_ BoundingSphere* sphere);


			//
			// parent-child reation
			//
			void SetParent(_IN_ Component3D* pParent);
			Component3D* GetParent();

		private:
			void AddChild(_IN_ Component3D* pChild);
			void RemoveChild(_IN_ Component3D* pChild);


		private:

			// return true if needs to update world matrix
			PF_INLINE bool IsUpdateNeeded();


			// force to update all children
			void ForceUpdateChildren();


		//
		// Events 
		//
		public:


			struct InternalEvents
			{
				// this event is being called whenever the Component changes its position
				// as parametrs the Component return its address
				ParabellumFramework::Event::Event<Component3D*> EventOnMove;

			} Events;

			


			



		};
	}

}


#endif