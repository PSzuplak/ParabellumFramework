#ifndef _SHADERLAYOUT_H_
#define _SHADERLAYOUT_H_

//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//

#include "TypesC.h"
#include "SystemCodes.h"

#include <string>

namespace ParabellumFramework
{

	namespace Graphics
	{
	 class XYZ_API ShaderLayout
		{
			// firned zone
			friend class XYZ_API Shader;
			friend class XYZ_API ShaderInstance;
			
		private:
			ShaderLayout();
			ShaderLayout(const ShaderLayout&);
			~ShaderLayout();
			
			
			EUINT32 Initialize(std::string& semanticName, EUINT32 semanticType, EUINT32 index, EUINT32 format,
				EUINT32 inputSlot, EUINT32 allignetByteOffset,
				EUINT32 inputSlottClass, EUINT32 instanceDataStepRate);

			void Release();

		private:
			void* m_buffer;
			std::string m_semanticName;
			EUINT32 m_semanticType;
			EUINT32 m_semanticIndex;
			EUINT32 m_format;
			EUINT32 m_inputSlot;
			EUINT32 m_alignedByteOffset;
			EUINT32 m_inputSlotClass;
			EUINT32 m_instanceDataStepRate;

		public:

			void* GetBuffer();
			std::string& GetSemanticName();
			EUINT32 GetSemanticType();
			EUINT32 GetSemanticIndex();
			EUINT32 GetFormat();
			EUINT32 GetInputSlot();
			EUINT32 GetAlignedByteOffset();
			EUINT32 GetInputSlotClass();
			EUINT32 GetInstanceDataStepRate();
		};

	}
}








#endif