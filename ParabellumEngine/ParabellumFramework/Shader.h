#ifndef _SHADER_H_
#define _SHADER_H_


//
//	Predefs
//
#include "DLLIE.h"

//
//	Includes
//
#include "TypesC.h"
#include "MathHelper.h"
#include "GraphicsTypes.h"
#include "Matrix4x4.h"

#include <memory>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>

#include "Resource.h"

#include "DebugManager.h"

//#include <d3d11.h>

#include "ShaderLayout.h"
#include "ShaderParametr.h"
#include "ShaderSamplerState.h"



//
//	Macros
//


// should be used with shader objects f.ex. : shader1->SETH("worldMatrix")
#ifdef SPARAMETR
	#define "SPARAMETR ALREADY DEFINIED"
#else
	#define SPARAMETR(str) Parametrs<C$(str)>()
#endif




namespace ParabellumFramework
{

	namespace Graphics
	{

		//
		//	Shader - can be created only in GraphicsDevice and ResourceManager
		//
		class XYZ_API Shader : public ParabellumFramework::Resources::Resource
		{
			// friendship
			friend class XYZ_API ParabellumFramework::Graphics::GraphicsDevice;
			friend class XYZ_API ParabellumFramework::Resources::ResourceManager;

			//
			// forwards declaration
			class ShaderCBuffer;
		
		private:

			// Can be created only from GraphicsDevice
			Shader();
			
			Shader(const Shader&) = delete;

			// Can be called only from GraphicsDevice
			~Shader();

		private:


			// initialize CBuffers, Resources and Parametrs. Should be executed after Parse(...)
			EUINT32 InitializeData();

			EUINT32 Release();
			EUINT32 Parse(const char* filePath);

			
		//
		// operators
		//
		private:

			Shader& operator=(const Shader& rhs) = delete;

		//
		// Members
		//
		public:

			std::string m_shaderFilename;

			// value seted by GraphicsDevice
			void* m_vertexShaderBuffer;

			// value seted by GraphicsDevice
			void* m_pixelShaderBuffer;

			// value seted by GraphicsDevice
			void* m_geometryShaderBuffer;

			// value seted by GraphicsDevice
			void* m_instanceBuffer;


			// name of the compiled vertex shader function
			std::string m_vertexShaderFunctionName;		

			// name of the compiled pixel shader function
			std::string m_pixelShaderFunctionName;					

			// name of the compiled vertex shader function
			std::string m_geometryShaderFunctionName;

			// name of the compiled pixel shader function
			std::string m_tesselShaderFunctionName;


			// number of fileds in structure used as vertex input
			EUINT32 m_vsLayoutCount;

			// array wit shader layouts
			ShaderLayout* m_vertexShaderLayout;
			
			void* m_inputLayout;									// pointer to layout stored on graphics card

			//	-----------------------------------------------------------------------------------------
			ShaderSamplerState** m_samplerStates;					// array of samplerstates
			EUINT32 m_samplersCount;								// number of sampler states
							
			// globals variables in shader program for ex. world matrix, texture
			// note that each (at least for parameters not resources)  ShaderParametr* points to some location from the same buffer which is m_bufferShaderParametrs
			std::map<EUINT64, ShaderParametr*> m_parametrs;		

			std::map<std::string, ShaderParametr*> m_umParams;	// pointers from m_parametrs placed in hash map


			// pointers which are stored in m_parametrs. The are sorted by the position in Shader Code - same as m_globalVariablesNames for passing to ShaderInterface
			ShaderParametr** m_sortedPointersToParametrs;	


			// buffer for all Shader parametrs - continious memory block 
			// all pointers from m_parametrs should point to this buffer
			// ShaderParametr objects are sorted from the lowest key to the bigest
			void* m_bufferShaderParametrs;


			// buffer for all generated keys
			EUINT64* m_bufferKeys;



			// pointer to area where all parametrs are stored
			// use it for fastest memcpy in setting GBuffer
			// parametr location is stored in m_parametrsBuffer + m_parametrsOffsets[c][paramNo]
			// where 'c' is the number of CBffer 
			char* m_parametrsBuffer;

			// size of m_parametrsBuffer in Bytes
			// 
			EUINT32 m_parametrsBufferSize;

			// array with offsets for each data stored in m_parametrsBuffer
			// f.e. First Matrix4x4 has offset 0, second float4 has offset 64, third Matrix4x4 has offset 64+16 (sizeof first + sizeof second)
			// first [] is no of cbuffer - second [] contains offsetss for parametrs
			EUINT32** m_parametrsOffsets;

			// offsets to m_parametrsBuffer which poins to the beginnig of cbuffer
			// for example m_parametrsBuffer[m_parametrsBuffersOffsets[1]] points to the second cbuffer
			// one cbuffer starts at m_parametrsBuffer[m_parametrsBuffersOffsets[XXX]] and ends at m_parametrsBuffer[m_parametrsBuffersOffsets[XXX]] + m_parametrsBuffersSizes[XXX]
			EUINT32* m_parametrsBuffersOffsets;

			EUINT32* m_parametrsBuffersSizes;
																
	
			// parametrs count for all variables in all CBuffers. Now used as tere is only 1 GLOBAL CBuffer - its iterated in Parse() method
			EUINT32 m_parametrsCount;							

			ShaderParametr** m_sortedPointersToResources;		// pointers to resourceswhich are stored in m_parametrs. They are sorted
																// by position in Shader Code - same as m_shaderResourcesNames
																// for passing to ShaderInterface

			EUINT32	m_shaderResourcesPointersCount;

			std::vector<EUINT32> m_resourcesTypes;				// types of used shader resources
			std::vector<std::string> m_resourcesNames;
			std::vector<EUINT64> m_resourcesKeys;				// keys reffered to m_parametrs. They are sorted by
																// position in Shader Code

																//	-----------------------------------------------------------------------------------------
			std::vector<std::string> m_shaderSamplersNames;		// names of the Samplers States


																//	-----------------------------------------------------------------------------------------
			std::vector<std::string> m_functionsTypes;			// functions types in shader code
			std::vector<std::string> m_functionsNames;			// functions names in shader code

																//	-----------------------------------------------------------------------------------------
																//	cbuffers
			ShaderCBuffer** m_cbuffers;							// array of cbuffers		
			EUINT32	m_cbuffersCount;							// number of cbuffers


			
			

			//
			// External buffers
			// 
			

			void** m_externalBuffers;

			// you may choose to use external bufer for shader variables
			// it means that during rendering GraphicsDevice will copy
			// the buffer instead of internal buffer.
			bool m_isExternalBufferUsed;



		private:
			std::string& GetShaderFilename();

			void* GetVertexBuffer();

			void* GetPixelBuffer();


			std::string& GetVSFunctionName();

			std::string& GetPSFunctionName();


			EUINT32 GetCBuffersCount();

			ShaderCBuffer* GetCBuffer(EUINT32 no);


			EUINT32 GetLayoutDataCount();
			ShaderLayout* GetLayout();

			// return reference to input layout pointer
			void* GetInputLayout();

			// templates
			public:

				// using m_parametrsBuffer to set parametrs
				// a way faster than ShaderParametr::SetParametr, because some matrices (Camera View etc) will be copied only once per frame :)
				//inline void SetParametrsBuffer(_IN_ void* pIn, EUINT32 paramNo, EUINT32 size)
				//{
				//	memcpy(m_parametrsBuffer + m_parametrsOffsets[paramNo], pIn, size);
				//}


				inline void SetParametrsBuffer(EUINT32 paramNo, _IN_ Matrix4x4* pIn)
				{
					//PFDLOG("PFGI", "SHCL", "SetParametrsBuffer(" << paramNo << ", " << (void*)pIn << ") : " << (void*)(m_parametrsBuffer + m_parametrsOffsets[0][paramNo]) );
					memcpy((void*)(m_parametrsBuffer + m_parametrsOffsets[0][paramNo]), pIn, sizeof(Matrix4x4));
				}

				inline void SetParametrsBuffer(EUINT32 paramNo, _IN_ Vector4D* pIn)
				{
					memcpy(m_parametrsBuffer + m_parametrsOffsets[0][paramNo], pIn, sizeof(Vector4D));
				}


				// Set external buffer - this buffer will be copied on Graphics Card instead
				// cbufferNo - number of CBuffer
				// pBuffer - pointer to memory with shader parametrs
				inline void SetExternalBuffer(EUINT32 cbufferNo, void* pBuffer)
				{
					m_isExternalBufferUsed = true;
					m_externalBuffers[cbufferNo] = pBuffer;
				}

				
				//
				// return Smart object so operator= can be used
				// binary search for Map m_parametrs will be used only at first try of setting parametr which matches to 'key',
				// so it works "quite good" during drawing models
				// key MUST be generated by GenerateHash
				//template<ParabellumFramework::EUINT64 key>
				//ShaderParametr& Parametrs()
				//{	
				//	static Shader* currentShader = NULLPTR;
				//	static ShaderParametr* smarty;
				//	
				//	if (currentShader != this)
				//	{
				//		currentShader = this;
				//		smarty = this->m_parametrs[key];
				//	}
				//
				//	return *smarty;		
				//}


				template<ParabellumFramework::EUINT64 key>
				ShaderParametr& Parametrs()
				{
					if( m_bufferKeys[0] == key )
						return *((ShaderParametr*)(m_bufferShaderParametrs)+0);
					else if (m_bufferKeys[1] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+1);
					else if (m_bufferKeys[2] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+2);
					else if (m_bufferKeys[3] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+3);
					else if (m_bufferKeys[4] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+4);
					else if (m_bufferKeys[5] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+5);
					else if (m_bufferKeys[6] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+6);
					else if (m_bufferKeys[7] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+7);
					else if (m_bufferKeys[8] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+8);
					else if (m_bufferKeys[9] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+9);
					else if (m_bufferKeys[10] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+10);
					else if (m_bufferKeys[11] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+11);
					else if (m_bufferKeys[12] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+12);
					else if (m_bufferKeys[13] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+13);
					else if (m_bufferKeys[14] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+14);
					else if (m_bufferKeys[15] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+15);
					else if (m_bufferKeys[16] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+16);
					else if (m_bufferKeys[17] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+17);
					else if (m_bufferKeys[18] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+18);
					else if (m_bufferKeys[19] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+19);
					else if (m_bufferKeys[20] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+20);
					else if (m_bufferKeys[21] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+21);
					else if (m_bufferKeys[22] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+22);
					else if (m_bufferKeys[23] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+23);

					assert(!"ShaderParametr& Parametrs() : something thih key or too many variables");

					return *((ShaderParametr*)(m_bufferShaderParametrs)+0);
				}


				//
				// Note that Struct12<"g_cameraPositionFar"_hash>::value should be used except only '_hash' because VS does not handle it well
				// 
				ShaderParametr& Parametrs(EUINT64 key)
				{
					if (m_bufferKeys[0] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+0);
					else if (m_bufferKeys[1] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+1);
					else if (m_bufferKeys[2] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+2);
					else if (m_bufferKeys[3] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+3);
					else if (m_bufferKeys[4] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+4);
					else if (m_bufferKeys[5] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+5);
					else if (m_bufferKeys[6] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+6);
					else if (m_bufferKeys[7] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+7);
					else if (m_bufferKeys[8] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+8);
					else if (m_bufferKeys[9] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+9);
					else if (m_bufferKeys[10] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+10);
					else if (m_bufferKeys[11] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+11);
					else if (m_bufferKeys[12] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+12);
					else if (m_bufferKeys[13] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+13);
					else if (m_bufferKeys[14] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+14);
					else if (m_bufferKeys[15] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+15);
					else if (m_bufferKeys[16] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+16);
					else if (m_bufferKeys[17] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+17);
					else if (m_bufferKeys[18] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+18);
					else if (m_bufferKeys[19] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+19);
					else if (m_bufferKeys[20] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+20);
					else if (m_bufferKeys[21] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+21);
					else if (m_bufferKeys[22] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+22);
					else if (m_bufferKeys[23] == key)
						return *((ShaderParametr*)(m_bufferShaderParametrs)+23);

					assert(!"ShaderParametr& Parametrs() : something thih key or too many variables");

					return *((ShaderParametr*)(m_bufferShaderParametrs)+0);
				}
				

				//
				// Get access to memory where shader parametr is stored
				// cbufferNo - no of cbuffer
				// paramNo - no of parametr
				template<typename T>
				inline T* GetParametrsBuffer(EUINT32 cbufferNo, EUINT32 paramNo)
				{
					return (T*)(m_parametrsBuffer + m_parametrsOffsets[cbufferNo][paramNo]);
				}


				// Get address on GraphicsCard of place where buffer is stored
				inline void* GetAddressOfCBuffer(EUINT32 cbufferNo)
				{
					return GetCBuffer(cbufferNo)->GetAddressOfBuffer();
				}

				// Get pointer do buffer with parametrs
				inline void* GetCBufferBuffer(EUINT32 cbufferNo)
				{
					return m_parametrsBuffer + m_parametrsOffsets[cbufferNo][0];
				}


				inline EUINT32 GetCBufferSize(EUINT32 cbufferNo)
				{
					return m_parametrsBuffersSizes[cbufferNo];
				}




				//
				// Return parametr stored in m_parametrs 
				// key is a hash generated from parametr name
				//
				inline ShaderParametr& operator[](EUINT64 key);

				//
				// index is the number of parametr 
				//
				inline ShaderParametr* operator()(EUINT32 index);


				//
				// Get shader parametr from m_sortedPointersToParametrs. The fastest way
				//
				inline ShaderParametr* GetParametr(EUINT32 index);


				//
				// Get shader resource from m_sortedPointersToParametrs. The fastest way
				//
				inline ShaderParametr* GetResource(EUINT32 index);

				//
				// Get array to resources
				//
				inline ShaderParametr** GetResources();

				//
				// Get  resources count
				//
				inline EUINT32 GetResourcesCount();



			//------------------------------------------------------------------------------
			// nested ShaderCBuffer
			//------------------------------------------------------------------------------

			


		//
		//	ShaderVariable - this class XYZ_API represents one of the variable in shader code(HLSL/GLS). Initialy it was part of CBuffer in DirectX
		//	In other GraphicsDevice can be created only 1 CBuffer for all variables.
		//	Musnt be used outside CBuffer
		//
		class XYZ_API ShaderVariable
		{

		private:
			ShaderVariable(ShaderVariable&);

		public:
			ShaderVariable();

			void Release();
			void Initialize(std::string& name, EUINT32 type, EUINT32 size);
			~ShaderVariable();

		private:
			
			// can be used to store "key" value for ShaderParametr
			EUINT64 m_key;

			// variable name
			std::string m_name;

			// variable type
			EUINT32 m_type;

			// variable size
			EUINT32 m_size;


		public:

			inline EUINT32 GetType();

			inline EUINT32 GetSize();

			inline EUINT64 GetKey();

			inline std::string& GetName();
		};


			
		//
		//
		//
		class XYZ_API ShaderCBuffer
		{
		public:
			ShaderCBuffer();
			~ShaderCBuffer();
			ShaderCBuffer(const ShaderCBuffer&);

			EUINT32 Initialize(_IN_ ShaderVariable* variables, EUINT32 variablesCount, EUINT32 bufferNo)
			{
				m_variablesCount = variablesCount;
				m_bufferNo = bufferNo;
				m_variables = new ShaderVariable[m_variablesCount];

				for (EUINT32 i = 0; i < m_variablesCount; ++i)
				{
					m_variables[i].Initialize(variables[i].GetName(), variables[i].GetType(), variables[i].GetSize());
				}

				//set cbuffer size - align to 16bytes
				m_size = 0;
				for (EUINT32 i = 0; i < m_variablesCount; ++i)
				{
					m_size += m_variables[i].GetSize();
				}

				if (m_size % 16 == 0)
					m_size = m_size;
				else
					m_size = (m_size + (16 - (m_size % 16)));

				

				// create array of pointer to store sorted variables
				m_sortedPointersToResources = new ShaderParametr*[m_variablesCount];

				return SystemCodes::SUCCESS;
			}


			private:

				// pointer to buffer used by GrpahicsDevice
				void* m_buffer;

				// size of cbuffer in BYTES
				EUINT32 m_size;

				// number of variables stored in cbuffer
				EUINT32 m_variablesCount;

				// shader variables which are stored in cbuffer
				ShaderVariable* m_variables;

				// number of variables stored in cbuffer
				EUINT32 m_resourcesCount;

				// number of cbuffer in shader (start counting from top)
				EUINT32 m_bufferNo;

			public:
				// pointers to resources which are stored in m_parametrs. They are sorted
				// by position in Shader Code - same as m_shaderResourcesNames
				// for passing to ShaderInterface
				ShaderParametr** m_sortedPointersToResources;	
																
																

			public:




				// get reference to buffer for cbuffer
				inline void* GetAddressOfBuffer()
				{
					return &m_buffer;
				}

				// getsizeof the cbuffer in BYTES
				inline EUINT32 GetSize()
				{
					return m_size;
				}

				//get variables count
				inline EUINT32 GetVariablesCount()
				{
					return m_variablesCount;
				}

				//
				// return pointer to ShaderVariable at position 'no' ('no' depends on position in shader file)
				//
				inline ShaderVariable* GetVariable(EUINT32 no)
				{
					return &m_variables[no];
				}


				inline EUINT32 GetResourcesCount()
				{
					return m_resourcesCount;
				}



				//
				// return Shader Parametrs
				//
				inline ShaderParametr** GetShaderParametrs()
				{
					return m_sortedPointersToResources;
				}

			};





			//	----------------------------------
			//		Struct forloading file
			//	----------------------------------

			void ReadString(std::string& str, FILE* stream)
			{
				EUINT32 len = 0;

				char filename[4096] = { 0 };

				fread(&len, sizeof(EUINT32), 1, stream);
				fread(&filename, sizeof(char), len, stream);

				str = filename;
			}



		};







	}
}








#endif