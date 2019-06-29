//	------------------------------------------------------------------------------------------------
//
//	ResourceManager manages resources
//	
//
//	Example :
//		Shader* file1 = resourceManager->Load<Shader>("ColorShadervs.hlsl");
//
//	------------------------------------------------------------------------------------------------

#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_


//
//	Predefs
//
#include "DLLIE.h"

//
//	libs
//
#include <typeinfo>
#include <string>
#include <map>
#include <unordered_map>
#include <list>

#include "TypesC.h"
#include "MemoryManager.h"
#include "Resource.h"
#include "GraphicsDevice.h"
#include "MathHelper.h"
#include "DebugManager.h"
#include "Profiler.h"

#include "Model.h"

namespace ParabellumFramework
{
	namespace Resources
	{


		// Manage ParabellumFramework resources (Shader,Texture, Model)
		class XYZ_API ResourceManager
		{
		private:

			// loaded resources map - it contains pointer to Resource assigned to some number
			std::map<EUINT64, Resource*> m_resources;		

			// how many Load() function was called for resource located in m_resources at "key" (first = unique key, second - number of Load)
			std::map<EUINT64, EUINT32> m_referencesCount;	

			// unordered map which store pointers to resources
			//std::unordered_map<std::string, Resource*> m_umResources;	// disabled

			// use to create graphics resources
			ParabellumFramework::Graphics::GraphicsDevice* m_graphics;


		public:
			ResourceManager();
			~ResourceManager();

			EUINT32 Initialize(_IN_ ParabellumFramework::Graphics::GraphicsDevice* graphics);
			EUINT32 Release();


		//
		// Resource managment functions
		//
		public:



			//
			// Load external file using typename 'T' - ParseFile method
			//
			template<typename T>
			T* Load(char* filename)
			{
				EUINT64 key = GenerateHash(filename);

				// allocate memory for shader parametr
				if (m_resources[key] == NULLPTR)
				{
					m_resources[key] = new T();
					m_resources[key]->SetResourceEnvironment(key, filename);
					m_resources[key]->Load(filename);								// parse file
					m_referencesCount[key] = 0;
					m_resources[key]->Initialize();
				}

				// increase number of tries how many times resource was loaded
				// ofcourse resource should be loaded only 1 time. For the rest - return exist pointer
				m_referencesCount[key] += 1;

				return (T*)m_resources[key];
			}



			template<>
			ParabellumFramework::Graphics::Shader*
			Load(char* filename);

			template<>
			ParabellumFramework::Graphics::Texture2D*
			Load(char* filename);

			template<>
			ParabellumFramework::Graphics::Model*
			Load(char* filename);
		
			
			template<typename T>
			T* 
			Load(const std::string& filename)
			{
				return Load<T>(filename.c_str());
			}

		};
	}

}


#endif