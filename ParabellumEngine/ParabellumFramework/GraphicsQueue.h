#ifndef _GRAPHICSQUEUE_H_
#define _GRAPHICSQUEUE_H_

//
//	Predefs
//
#include "DLLIE.h"

//
//	includes STD
//
#include <queue>

#include "TypesC.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderParametr.h"
#include "Texture2D.h"
#include "RenderTarget.h"
#include "GraphicsCommands.h"

namespace ParabellumFramework
{

	namespace Graphics
	{

		struct QueueParam
		{
			// from GraphicsCommands; PF_GRAPHICS_CMD_* values
			EUINT32 cmd;
			void* param0;
			void* param1;
			void* param2;
			void* param3;
		};



		class XYZ_API GraphicsQueue
		{
		public:

			GraphicsQueue()
			{
			}


		//
		// Methods
		//
		public:

			void Push(EUINT32 cmd, void* param0 = NULLPTR, void* param1 = NULLPTR, void* param2 = NULLPTR, void* param3 = NULLPTR)
			{
				m_queue.push({ cmd , param0, param1, param2, param3 });
			}

			QueueParam* Top()
			{
				return &m_queue.front();
			}

			void Pop()
			{
				m_queue.pop();
			}

			EUINT32 Size()
			{
				return m_queue.size();
			}


		//
		// Members
		//
		private:

			//QueueParam m_queue[256];
			//
			//EUINT32 m_queueIndexTop;
			//EUINT32 m_queueIndexCurrent;

			std::queue<QueueParam> m_queue;

		};







	}
}



#endif