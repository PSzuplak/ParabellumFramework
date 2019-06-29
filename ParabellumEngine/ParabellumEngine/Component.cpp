#include "Component.h"



//	------------------------------------------------------------
//		ComponentTag
//	------------------------------------------------------------

ParabellumEngine::Components::ComponentTag::ComponentTag(char* tag)
{
	m_tag = tag;
	m_hash = ParabellumUtils::Math::GenerateHash(tag);
}

ParabellumEngine::Components::ComponentTag::~ComponentTag()
{
}


bool 
ParabellumEngine::Components::ComponentTag::Is(char* tag)
{
	EUINT64 hash = ParabellumUtils::Math::GenerateHash(tag);

	if (hash == m_hash)
		return true;
	return false;
}

bool 
ParabellumEngine::Components::ComponentTag::Is(_IN_ ComponentTag* pCT)
{
	if(this->m_hash == pCT->m_hash)
		return true;
	return false;
}




//	------------------------------------------------------------
//		Component - Statics
//	------------------------------------------------------------
ParabellumFramework::EUINT32 ParabellumEngine::Components::Component::ComponentConstructorCalls = 0;

ParabellumFramework::EPTR ParabellumEngine::Components::Component::PointersToComponents[] = {0};


ParabellumFramework::EUINT32
ParabellumEngine::Components::Component::GetComponentsCount()
{
	return ComponentConstructorCalls;
}




//	------------------------------------------------------------
//		ComponentTag - Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::Component::Component()
{
	ComponentConstructorCalls += 1;
	m_id = ComponentConstructorCalls;
	PointersToComponents[m_id] = (EPTR)this;
}


ParabellumEngine::Components::Component::Component(Component&)
{
}

ParabellumEngine::Components::Component::~Component()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------
void
ParabellumEngine::Components::Component::AddTag(char* tag)
{
	for (ComponentTag& it : m_tags)
	{
		if (it.Is(tag))
			return;
	}
	
	m_tags.push_back( ComponentTag(tag) );
}


//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------


ParabellumFramework::EUINT32
ParabellumEngine::Components::Component::GetId()
{
	return m_id;
}