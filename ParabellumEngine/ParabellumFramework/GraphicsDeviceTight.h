/*
*
*	Contains Macros and optimizations for GraphicsDevice.h
*	Reducing function calls
*
*
*
*
*/


#ifndef _GRAPHICSDEVICETIGHT_H_
#define _GRAPHICSDEVICETIGHT_H_

#include "GraphicsDevice.h"


//

#define PFGD_RENDER(pDevice, pVertexBuffer, m_currentShader) \
		pDevice->Device_SetCBuffer(m_currentShader->GetCBuffer(0)->GetBuffer(), m_currentShader->m_parametrsBuffer, m_currentShader->m_parametrsBufferSize);\
		pDevice->Device_Render(m_currentIndexBuffer->GetIndicesCount());
	


#endif