#include "stdafx.h"
#include "Resource.h"

ParabellumFramework::Resources::Resource::Resource()
{
	m_resourceSize = 0;

	m_isInitialized = false;
	m_isCreated = false;
}

ParabellumFramework::Resources::Resource::~Resource()
{
}


ParabellumFramework::EUINT32
ParabellumFramework::Resources::Resource::Load(const char* filename)
{
	EUINT32 result;

	if (m_isInitialized == false)
	{
		m_resourceName = filename;
		m_resourceNameW = (wchar_t*)filename;
		result = Parse(filename);
		m_isInitialized = true;

		return SystemCodes::SUCCESS;
	}
	else
	{
		return SystemCodes::ERROR_EXISTS_ALREADY;
	}

}





void ParabellumFramework::Resources::Resource::GetData(void* dest)
{
}

