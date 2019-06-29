#ifndef _SHADERPARAMETR_H_
#define _SHADERPARAMETR_H_


//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//

#include "TypesC.h"
#include "SystemCodes.h"

#include "DebugManager.h"

#include <string>

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"

#include "Texture2D.h"
#include "RenderTarget.h"

namespace ParabellumFramework
{

	namespace Graphics
	{

	//
	//
	//
	class XYZ_API ShaderParametr
	{

		private:

			void* m_parametr;

		public:
			// points to some place in Shader::m_parametrsBuffer
			void* m_shaderBufferLocation;



			EUINT32 m_parametrSize;

		private:
			ShaderParametr(ShaderParametr&);

		public:

			ShaderParametr();
			~ShaderParametr();

			// Initialize Shader Parametr
			EUINT32 Initialize();

			// Release Shader Parametr
			EUINT32 Release();

		public:


			inline EUINT32 GetSize();

			inline void SetParametr(void* ptr);

			inline void SetSize(EUINT32 value);


			// templates
		public:



			//
			// Get parametr of the shader.  'm_parametr' should be cast on ID3D11Buffer or other DX/OGL type
			// if it is a resource
			//
			template<typename T>
			T* Get()
			{
				return (T*)m_parametr;
			}

			//
			// Set m_parametr pointer
			//
			//void Set(void* ptr)
			//{
			//	m_parametr = ptr;
			//}

			// Set shader parametr/resource.
			template<typename T>
			inline void Set(T* ptr)
			{
				//PFDPRINTNL("Set incorrect data on the shader!");
				//assert(!"Set incorrect data on the shader!");
				//m_parametrSize = sizeof(T);
				m_parametr = ptr;
				//*((T*)m_shaderBufferLocation) = *ptr;
				memcpy(m_shaderBufferLocation, ptr, sizeof(T));
			}

			template<>
			inline void Set(Texture2D* ptr);

			template<>
			inline void Set(RenderTarget* ptr);


			template<typename T>
			inline void operator=(_IN_ T& rhs)
			{
				*((T*)m_shaderBufferLocation) = rhs;
			}


			inline void operator=(_IN_ Texture2D* rhs)
			{
				//m_parametr = &rhs;
				m_parametr = *((char**)rhs->GetAddressOBufferType());
			}

			inline void operator=(_IN_ RenderTarget* ptr)
			{
				m_parametr = ptr->m_rtBufferResource;
			}



		};





	}
}








#endif