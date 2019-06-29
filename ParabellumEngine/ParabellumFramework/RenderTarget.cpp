#include "stdafx.h"
#include "RenderTarget.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::RenderTarget::RenderTarget()
{
	PFDLOG("PFGI", "PGRT", "RenderTarget[" << (EADDR)this << "]->RenderTarget() : Constructor called");
	PFDLOG("PFGI", "PGRT", "AddressOf(m_rtBufferTexture) : " << &m_rtBufferTexture);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_rtBufferFormat) : " << &m_rtBufferFormat);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_rtBufferResource) : " << &m_rtBufferResource);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_depthBuffer) : " << &m_depthBuffer);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_depthBufferFormat) : " << &m_depthBufferFormat);
}


ParabellumFramework::Graphics::RenderTarget::~RenderTarget()
{
	PFDLOG("PFGI", "PGRT", "RenderTarget[" << (EADDR)this << "]->~RenderTarget() : Destructor called");
}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------




//	--------------------------------------------------------------------
//		Gets And Sets
//	--------------------------------------------------------------------


void* 
ParabellumFramework::Graphics::RenderTarget::GetRTBufferResource()
{
	return m_rtBufferResource;
}

ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::RenderTarget::GetWidth()
{
	return m_width;
}

ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::RenderTarget::GetHeight()
{
	return m_height;
}
