#ifndef _SCENE_H_
#define _SCENE_H_


#include <list>

#include "../ParabellumFramework/SystemCodes.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Vector3D.h"
#include "../ParabellumFramework/Shader.h"

#include "Component3D.h"
#include "Camera.h"



using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumEngine::Components;



namespace ParabellumEngine
{
	namespace Core
	{

		namespace Scene
		{
			//
			//
			// Scene
			//
			//
			class Scene
			{
			public:
				Scene();
				~Scene();

			private:
				Scene(const Scene&);



			private:

			};
		}
	}
}


#endif