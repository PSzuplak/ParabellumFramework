#ifndef _SHADERSAMPLERSTATE_H_
#define _SHADERSAMPLERSTATE_H_

//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//

#include "TypesC.h"
#include "SystemCodes.h"
#include "GraphicsTypes.h"

#include <string>

namespace ParabellumFramework
{

	namespace Graphics
	{
		class XYZ_API ShaderSamplerState
		{
			// firned zone
			friend class XYZ_API Shader;
			friend class XYZ_API ShaderInstance;
			friend class GraphicsDevice;

		private:
			ShaderSamplerState();
			ShaderSamplerState(const ShaderSamplerState&);
			~ShaderSamplerState();

		public:
			EUINT32 Initialize(SamplerFilter filter,
				SamplerAddress addressU,
				SamplerAddress addressV,
				SamplerAddress addressW,
				EFLOAT32 mipLODBias,
				EUINT32 maxAnisotropy,
				SamplerComparsion comparisonFunc);


			EUINT32 Initialize(SamplerFilter filter,
				SamplerAddress addressU,
				SamplerAddress addressV,
				SamplerAddress addressW,
				EFLOAT32 mipLODBias,
				EUINT32 maxAnisotropy,
				SamplerComparsion comparisonFunc,
				EUINT32 borderColor0, EUINT32 borderColor1, EUINT32 borderColor2, EUINT32 borderColor3,
				EUINT32 minLOD,
				EUINT32 maxLOD);

		private:

			// pointer to sampler. Usuale set by graphics EFLOAT32
			// in DirectX seted by CreateSamplerState
			void* m_buffer;


			SamplerFilter m_filter;
			SamplerAddress m_addressU;
			SamplerAddress m_addressV;
			SamplerAddress m_addressW;
			EFLOAT32 m_mipLODBias;
			EUINT32 m_maxAnisotropy;
			SamplerComparsion m_comparisonFunc;
			EUINT32 m_borderColor[4];
			EUINT32 m_minLOD;
			EUINT32 m_maxLOD;

		private:
			// get reference to pointer to the sampler thate buffer
			void* GetAddressOfBuffer();


		};

	}
}








#endif