#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "DLLIE.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Event.h"

#include "Component3D.h"

using namespace ParabellumFramework;

namespace ParabellumEngine
{

	namespace Components
	{
		//
		//
		// Component which stand for 
		//
		//
		class XYZ_API PhysicsComponent : public Component3D
		{
		public:
			PhysicsComponent();
			PhysicsComponent(PhysicsComponent&);
			~PhysicsComponent();


		//
		// Methods
		//
		public:

		//
		// Members
		//
		public:


		//
		// Events
		//
		public:

			// event triggered when 2 physics objects collide
			Event::Event<PhysicsComponent*, PhysicsComponent*> EventOnCollision;

			// event triggered when an object collides with aground/terrain
			Event::Event<PhysicsComponent*> EventOnGroundCollision;


		//
		// Gets and Sets
		//
		public:


		};
	}
}


#endif