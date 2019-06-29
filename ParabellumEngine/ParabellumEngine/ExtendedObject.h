#ifndef _EXTENDEDOBJECT_H_
#define _EXTENDEDOBJECT_H_

#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"


#include "Component3D.h"

#include "EModelPart.h"


#include <string>

namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;

		//
		// ExtendedObject is composited from models, lights, particles and so on
		// We can have complex animated character which hangles candle which generates smoke particles
		//
		class XYZ_API ExtendedObject : public Component3D
		{
		public:
			ExtendedObject();
			~ExtendedObject();

		private:
			ExtendedObject(const ExtendedObject&) = delete;


		public:

		

		private:

			

		//
		// Gets and Sets
		//
		public:

		};
	}
}
#endif