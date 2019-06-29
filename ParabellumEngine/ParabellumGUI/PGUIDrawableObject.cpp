#include "stdafx.h"
#include "PGUIDrawableObject.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

ParabellumFramework::Event::TaskManager 
ParabellumUserInterface::PGUIDrawableObject::taskManager;

ParabellumFramework::Graphics::VertexBuffer*
ParabellumUserInterface::PGUIDrawableObject::m_vertexBuffer = NULLPTR;

ParabellumFramework::Graphics::IndexBuffer*
ParabellumUserInterface::PGUIDrawableObject::m_indexBuffer = NULLPTR;

namespace ParabellumUserInterface
{

	ParabellumUserInterface::PGUIUnitData operator"" _uif(long double val)
	{
		ParabellumUserInterface::PGUIUnitData output;

		output.Value = (EFLOAT32)val;
		output.Unit = ParabellumUserInterface::PGUIUnit::Float;

		return output;
	}

	// UI pertentage
	ParabellumUserInterface::PGUIUnitData operator"" _uip(long double val)
	{
		ParabellumUserInterface::PGUIUnitData output;

		output.Value = (EFLOAT32)val;
		output.Unit = ParabellumUserInterface::PGUIUnit::Percentage;

		return output;
	}

}



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIDrawableObject::PGUIDrawableObject(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)

{
	m_parent = NULLPTR;

	m_isUpdatePositionNeeded = true;
	m_isUpdateSizeNeeded = true;

	layout.bgUV.Set(0.0f, 0.0f, 0.20f, 0.05f);
	layout.fgUV.Set(0.0f, 0.0f, 0.20f, 0.05f);

	SetSkingBackgroundCoords(Vector4D(0.03515625f, 0.720703125f, 0.056640625f, 0.744140625f));
	SetSkinForegroundCoords(Vector4D(0.03515625f, 0.720703125f, 0.056640625f, 0.744140625f));

	//layout.bgUV.Set(0.0390625f, 0.7265625f, 0.04296875f, 0.73046875f);
	//layout.fgUV.Set(0.0390625f, 0.7265625f, 0.04296875f, 0.73046875f);

	layout.positionAspect = PGUIPositionAspect::Relative;
	layout.placementType = PGUIPlacementType::TopLeft;

	layout.calculatedPosition.Set(1.0f, 2.0f, 3.0f, 0.50f);

	layout.rotation.Set(0.0f, 0.0f, 0.0f, 0.0f);

	layout.positionUnits[0] = PGUIUnit::Float;
	layout.positionUnits[1] = PGUIUnit::Float;
	layout.positionUnits[2] = PGUIUnit::Float;
	layout.positionUnits[3] = PGUIUnit::Float;

	layout.padding.Set(0.0f, 0.0f, 0.0f, 0.0f);
	layout.margin.Set(0.0f, 0.0f, 0.0f, 0.0f);

	m_isVisible = true;
	m_isDisplayed = true;

	layout.color.Set(1.0f, 1.0f, 1.0f, 0.0f);

	//layout.borderSize.Set(0.0f, 0.0f, 0.0f, 0.0f);
	layout.borderSize.Set(0.001f, 0.001f, 0.001f, 0.001f);
	layout.borderColor.Set(1.0f, 1.0f, 1.0f, 0.5f);


	// initialize viewport values
	viewport.Set(false);
	viewport.area.Set(0.0f, 0.0f, 1.0f, 1.0f);


	// Initialize Graphics Resources


	// create Vertex nad Index Buffers
	if (m_vertexBuffer == NULLPTR)
	{
		PUIVertexFormat vertices[4];
		EUINT32 indices[6] = { 0,1,2,0,3,2 };

		// first triangle
		vertices[0].Position[0] = 0.0f;
		vertices[0].Position[1] = 0.0f;
		vertices[0].Position[2] = 0.0f;
		vertices[0].UV[0] = 0.0f;
		vertices[0].UV[1] = 0.0f;

		vertices[1].Position[0] = 1.0f;
		vertices[1].Position[1] = 0.0f;
		vertices[1].Position[2] = 0.0f;
		vertices[1].UV[0] = 1.0f;
		vertices[1].UV[1] = 0.0f;

		vertices[2].Position[0] = 1.0f;
		vertices[2].Position[1] = 1.0f;
		vertices[2].Position[2] = 0.0f;
		vertices[2].UV[0] = 1.0f;
		vertices[2].UV[1] = 1.0f;

		vertices[3].Position[0] = 0.0f;
		vertices[3].Position[1] = 1.0f;
		vertices[3].Position[2] = 0.0f;
		vertices[3].UV[0] = 0.0f;
		vertices[3].UV[1] = 1.0f;

		m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(PUIVertexFormat), 4);
		m_indexBuffer = pDevice->CreateIndexBuffer(indices, 6);

		taskManager.Run();
	}

	// load shader
	m_shader = pResourceManager->Load<Shader>("Resources/Shaders/GUIShader.psx");

	// load texture
	layout.t2Dbackground = pResourceManager->Load<Texture2D>("Resources/GUI/GUI_01.pft");



}



ParabellumUserInterface::PGUIDrawableObject::~PGUIDrawableObject()
{
	// set new parent for children
	if (m_parent != NULLPTR)
	{
		PGUIDrawableObject* parentPtr = m_parent;

		UnsetParent();

		for (auto it : m_children)
			it->SetParent(parentPtr);
	}
	// remove parent and children
	else
	{
		for (auto it : m_children)
			it->UnsetParent();
	}

}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------


void 
ParabellumUserInterface::PGUIDrawableObject::Draw(_IN_ GraphicsDevice* pDevice)
{
	Vector4D position, size, boundingRect, parentDisplayRect, borderSize;
	ViewPort parentViewPort;

	//parentViewPort.area.Set(1.0f, 1.0f, 0.0f, 0.0f);

	if (IsDisplayed() == true)
	{

		// get vieport
		if (m_parent != NULLPTR)
		{
			m_parent->GetViewPort(&parentViewPort);
		}
		else
			parentViewPort.area.Set(0.0f, 0.0f, 1.0f, 1.0f);

		//printf("\nArea : %s", parentViewPort.area.ToString().c_str());

		// draw object if visible
		if (IsVisible() == true)
		{
			GetPosition(position);
			GetSize(size);	

			pDevice->SetVertexBuffer(m_vertexBuffer);
			pDevice->SetIndexBuffer(m_indexBuffer);

			pDevice->SetShader(m_shader);

			m_shader->GetParametr(0)->Set(&position);
			m_shader->GetParametr(1)->Set(&layout.rotation);
			m_shader->GetParametr(2)->Set(&size);
			m_shader->GetParametr(3)->Set(&layout.bgUV);
			m_shader->GetParametr(4)->Set(&layout.fgUV);
			m_shader->GetParametr(5)->Set(&parentViewPort.area);
			m_shader->GetParametr(6)->Set(&layout.color);
			m_shader->GetParametr(7)->Set(&layout.borderSize);
			m_shader->GetParametr(8)->Set(&layout.borderColor);

			m_shader->GetResource(0)->Set<Texture2D>(layout.t2Dbackground);

			pDevice->Render();
		}


		// draw recursivelly children last due to depth comparsion
		for (auto it : m_children)
			it->Draw(pDevice);
	}
}


void
ParabellumUserInterface::PGUIDrawableObject::ForceUpdate()
{
	m_isUpdatePositionNeeded = true;
	m_isUpdateSizeNeeded = true;

	for (auto it : m_children)
		it->ForceUpdate();
}



ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::CheckCollision(EFLOAT32 x, EFLOAT32 y)
{
	Vector4D position, size, dispayRect;

	// do not check not displayed object
	if (IsDisplayed() == false)
		return false;

	GetPosition(position);
	GetSize(size);

	if ((x > position.X) && (x < (position.X + size.X)) &&
		(y > position.Y) && (y < (position.Y + size.Y))
		)
	{
		if (m_parent != NULLPTR)
		{
			if (IsInsideViewPort(x, y) == false)
				return false;
			else
				return true;
		}
		else
			return true;
		
		return true;
	}

	return false;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::CheckCollision(EFLOAT32 x1, EFLOAT32 y1, EFLOAT32 x2, EFLOAT32 y2)
{
	Vector4D thisPosition;
	GetPosition(thisPosition);

	if (
		(thisPosition.X < x2) &&
		(thisPosition.X + layout.absoluteSize.X > x1) &&
		(thisPosition.Y < y2) &&
		(thisPosition.Y + layout.absoluteSize.Y) > y1
		)
	{
		return true;
	}

	return false;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::CheckCollision(_IN_ Vector4D& v4d)
{
	return CheckCollision(v4d.X, v4d.Y, v4d.Z, v4d.W);
}



void
ParabellumUserInterface::PGUIDrawableObject::SetParent(_IN_ PGUIDrawableObject* pParent)
{
	// quit if tries to set the same parent
	if ((m_parent == pParent) || (pParent == NULLPTR) || (this == pParent))
		return;

	if (m_parent != NULLPTR)
	{
		m_parent->RemoveChild(this);
		m_parent = NULLPTR;
	}

	// remove parent by setting NULL
	m_parent = pParent;
	m_parent->AddChild(this);

	ForceUpdate();
}

void
ParabellumUserInterface::PGUIDrawableObject::UnsetParent()
{
	if (m_parent != NULLPTR)
	{
		m_parent->RemoveChild(this);
		m_parent = NULLPTR;
		ForceUpdate();
	}
}


ParabellumUserInterface::PGUIDrawableObject*
ParabellumUserInterface::PGUIDrawableObject::GetDepest(EFLOAT32 x, EFLOAT32 y)
{
	PGUIDrawableObject* result;
	result = this;

	if (CheckCollision(x, y) == false)
		return NULLPTR;

	for (auto it : m_children)
	{
		result = it->GetDepest(x, y);

		if (result != NULLPTR)
			return result;
	}

	return this;
}

void
ParabellumUserInterface::PGUIDrawableObject::AddChild(_IN_ PGUIDrawableObject* pChild)
{
	for (auto it : m_children)
		if (it == pChild)
			return;

	m_children.push_front(pChild);
}



void
ParabellumUserInterface::PGUIDrawableObject::RemoveChild(_IN_ PGUIDrawableObject* pChild)
{
	if (m_children.size() > 0)
		for (auto it : m_children)
			if (it == pChild)
			{
				m_children.remove(it);
				it->m_parent = NULLPTR;

				return;
			}
}



// find child position
// pChild must be child of this object (stored in m_children)
void 
ParabellumUserInterface::PGUIDrawableObject::FindChildPosition(PGUIDrawableObject* pChild, EFLOAT32& outX, EFLOAT32& outY)
{
	outX = 0.0f;
	outY = 0.0f;
}



ParabellumUserInterface::PGUIDrawableObject*
ParabellumUserInterface::PGUIDrawableObject::GetParent()
{
	return m_parent;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsRelatedClose(_IN_ PGUIDrawableObject* pObj)
{
	return IsRelatedUp(pObj) | IsRelatedDown(pObj) | IsSibling(pObj);
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsRelatedUp(_IN_ PGUIDrawableObject* pObj)
{
	return pObj->IsRelatedDown(this);
}

ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsRelatedDown(_IN_ PGUIDrawableObject* pObj)
{
	bool isRelated = false;

	if (GetParent() == NULLPTR)
		return false;

	if (GetParent() == pObj)
		return true;

	isRelated = isRelated | GetParent()->IsRelatedDown(pObj);

	return isRelated;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsSibling(_IN_ PGUIDrawableObject* pObj)
{
	if (GetParent() == NULLPTR)
		return false;

	for (auto it : GetParent()->m_children)
	{
		if (it == pObj)
			return true;
	}

	return false;
}







void 
ParabellumUserInterface::PGUIDrawableObject::Move(EFLOAT32 dx, EFLOAT32 dy)
{
	layout.absolutePosition.X += dx;
	layout.absolutePosition.Y += dy;

	ForceUpdate();
}

void
ParabellumUserInterface::PGUIDrawableObject::Resize(EFLOAT32 dx, EFLOAT32 dy)
{
	layout.absoluteSize.X += dx;
	layout.absoluteSize.Y += dy;

	ForceUpdate();
}



void
ParabellumUserInterface::PGUIDrawableObject::MoveChildren(EFLOAT32 dx, EFLOAT32 dy)
{
	for (auto it : m_children)
	{
		if (it->layout.positionAspect == PGUIPositionAspect::Relative)
		{
			it->Move(dx, dy);
		}
	}
}



void
ParabellumUserInterface::PGUIDrawableObject::UpdateForm()
{
	// calculate Viewport
	Vector4D vpPosition, vpSize;

	//GetPosition(vpPosition);
	//GetSize(vpSize);

	if (m_parent != NULLPTR)
	{
		viewport.area.X = m_parent->layout.calculatedPosition.X;
		viewport.area.Y = m_parent->layout.calculatedPosition.Y;
		viewport.area.Z = m_parent->layout.calculatedPosition.X + m_parent->layout.calculatedSize.X;
		viewport.area.W = m_parent->layout.calculatedPosition.Y + m_parent->layout.calculatedSize.Y;

		//printf("\n Viewport : %s", viewport.area.ToString().c_str() );
	}


}


//	--------------------------------------------------------------------
//		Events
//	--------------------------------------------------------------------


void
ParabellumUserInterface::PGUIDrawableObject::AddEventMouseClick(PUIEvent func)
{
	m_eventsOnMouseClick.push_back(func);
}

void
ParabellumUserInterface::PGUIDrawableObject::AddEventMouseHold(PUIEvent func)
{
	m_eventsOnMouseHold.push_back(func);
}

void
ParabellumUserInterface::PGUIDrawableObject::AddEventMouseOver(PUIEvent func)
{
	m_eventsOnMouseOver.push_back(func);
}


void
ParabellumUserInterface::PGUIDrawableObject::AddEventMouseOut(PUIEvent func)
{
	m_eventsOnMouseOut.push_back(func);
}

void
ParabellumUserInterface::PGUIDrawableObject::AddEventMouseMove(PUIEvent func)
{
	m_eventsOnMouseMove.push_back(func);
}

void
ParabellumUserInterface::PGUIDrawableObject::AddEventMouseDrag(PUIEvent func)
{
	m_eventsOnMouseDrag.push_back(func);
}


void ParabellumUserInterface::PGUIDrawableObject::AddEventChange(PUIEvent func)
{
	m_eventsOnChange.push_back(func);
}




ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::InvokeMouseClick(EFLOAT32 x, EFLOAT32 y)
{
	EBOOL wasInvoked = false;

	if (IsDisplayed() == false)
		return false;

	// do not invoke function if mouse position is lying outside a BoundingRect
	if (IsInsideViewPort(x, y) == false)
		return false;


	for (auto child : m_children)
	{
		wasInvoked |= child->InvokeMouseClick(x, y);
	}

	if ( (wasInvoked == false) && (CheckCollision(x, y) == true) )
	{
		for (auto it : m_eventsOnMouseClick)
		{
			PUIEventMouse e;
			e.MouseX = x;
			e.MouseY = y;
			e.MouseState = IO::MouseState::NO_ACTION;

			(it)((void*)this, (void*)&e);

			wasInvoked = true;
		}
	}

	return wasInvoked;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::InvokeMouseHold(EFLOAT32 x, EFLOAT32 y, IO::MouseState state)
{
	EBOOL wasInvoked = false;

	return wasInvoked;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::InvokeMouseOver(EFLOAT32 x, EFLOAT32 y)
{
	EBOOL wasInvoked = false;

	for (auto it : m_eventsOnMouseOver)
	{
		PUIEventMouse e;
		e.MouseX = x;
		e.MouseY = y;
		e.MouseState = IO::MouseState::NO_ACTION;

		(it)((void*)this, (void*)&e);

		wasInvoked = true;
	}

	for (auto child : m_children)
		child->InvokeMouseOver(x, y);

	return wasInvoked;
}


void
ParabellumUserInterface::PGUIDrawableObject::InvokeMouseOut(EFLOAT32 x, EFLOAT32 y)
{
	for (auto it : m_eventsOnMouseOut)
	{
		PUIEventMouse e;
		e.MouseX = x;
		e.MouseY = y;
		e.MouseState = IO::MouseState::NO_ACTION;

		(it)((void*)this, (void*)&e);
	}

	for (auto child : m_children)
		child->InvokeMouseOut(x, y);
}

void
ParabellumUserInterface::PGUIDrawableObject::InvokeMouseMove(EFLOAT32 x, EFLOAT32 y)
{
	//
	//
	if (IsDisplayed() == false)
		return;

	//
	// do not invoke function if mouse position is lying outside a BoundingRect
	if (IsInsideViewPort(x, y) == false)
		return;

	//
	//invoke for children
	for (auto child : m_children)
		child->InvokeMouseMove(x, y);

	// exit
	if (CheckCollision(x, y) == true)
	{
		if (m_eventsOnMouseMove.size() > 0)
			for (EUINT32 i = 0; i < m_eventsOnMouseMove.size(); ++i)
			{
				PUIEventMouse e;
				e.MouseX = x;
				e.MouseY = y;
				e.MouseState = IO::MouseState::NO_ACTION;

				(m_eventsOnMouseMove[i])(this, &e);
			}
	}
}


void
ParabellumUserInterface::PGUIDrawableObject::InvokeMouseDrag(EFLOAT32 beginX, EFLOAT32 beginY, EFLOAT32 currentX, EFLOAT32 currentY, EFLOAT32 deltaX, EFLOAT32 deltaY)
{
	if (CheckCollision(currentX, currentY))
	{
		Move(deltaX, deltaY);
	}
}




void
ParabellumUserInterface::PGUIDrawableObject::InvokeOnChange()
{
	if (m_eventsOnChange.size() > 0)
		for (EUINT32 i = 0; i<m_eventsOnChange.size(); ++i)
		{			
			PUIEventMouse e;
			e.MouseX = 0.0f;
			e.MouseY = 0.0f;
			e.MouseState = IO::MouseState::NO_ACTION;

			(m_eventsOnChange[i])(this, &e);			
		}
}


//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------


void
ParabellumUserInterface::PGUIDrawableObject::GetChildInlinePosition(_IN_ PGUIDrawableObject* pChild, _OUT_ Vector4D& out)
{
	Vector4D thisPos, thisSize, thisBoundingRect, itSize;
	GetPosition(thisPos);
	GetSize(thisSize);

	thisBoundingRect.X = thisPos.X + layout.padding.X;
	thisBoundingRect.Y = thisPos.Y + layout.padding.Y;
	thisBoundingRect.Z = thisPos.X + thisSize.X - layout.padding.Z;
	thisBoundingRect.W = thisPos.Y + thisSize.Y - layout.padding.W;
	
	EFLOAT32 cx = thisBoundingRect.X;
	EFLOAT32 cy = thisBoundingRect.Y;

	EFLOAT32 cMaxh = 0.0f;

	EFLOAT32 maxMarginTop = 0.0f;
	EFLOAT32 maxMarginBottom = 0.0f;

	EUINT32 lineNo = 1;

	for (std::list<PGUIDrawableObject*>::reverse_iterator it = m_children.rbegin(); it != m_children.rend(); ++it)
	{
		// exit if reached child
		// exit if it is the child
		if (*it == pChild)
		{
			out.X = cx + pChild->layout.margin.X;
			out.Y = cy;
			return;
		}
		else
		if ((*it)->layout.positionAspect == PGUIPositionAspect::Inline)
		{
			(*it)->GetSize(itSize);

			if (cMaxh < itSize.Y) 
				cMaxh = itSize.Y;

			cx = cx + (*it)->layout.margin.X + itSize.X;

			if (cx > thisBoundingRect.Z - itSize.X - (*it)->layout.margin.Z)
			{
				maxMarginTop = 0.0f;
				maxMarginBottom = 0.0f;

				// set pos X to beginning
				cx = thisBoundingRect.X + (*it)->layout.margin.X;
				cy = thisBoundingRect.Y + cMaxh * lineNo;

				lineNo++;
			}

		} // end if inline

	} // end for it

}




void
ParabellumUserInterface::PGUIDrawableObject::GetHookPoint(Vector4D& val)
{
	Vector4D size;
	GetSize(size);

	val.Set(0.0f, 0.0f, 0.0f, 0.0f);

	if (layout.placementType == PGUIPlacementType::TopLeft)
	{
	}
	else if (layout.placementType == PGUIPlacementType::TopRight)
	{
		val.X -= size.X;
	}
	else if (layout.placementType == PGUIPlacementType::BottomRight)
	{
		val.X -= size.X;
		val.Y -= size.Y;
	}
	else if (layout.placementType == PGUIPlacementType::BottomLeft)
	{
		val.Y -= size.Y;
	}
	else if (layout.placementType == PGUIPlacementType::CenterX)
	{
		val.X -= size.X / 2.0f;
	}
	else if (layout.placementType == PGUIPlacementType::CenterY)
	{
		val.Y -= size.Y / 2.0f;
	}
	else if (layout.placementType == PGUIPlacementType::CenterXY)
	{
		val.X -= size.X / 2.0f;
		val.Y -= size.Y / 2.0f;
	}

}

void 
ParabellumUserInterface::PGUIDrawableObject::GetPosition(Vector4D& val)
{
	// return calculated position if object and its parent were not changed
	// otherwise recalculate the position
	if (m_isUpdatePositionNeeded == false)
	{
		val = layout.calculatedPosition;
		return;
	}


	// if object does not hav a parent we treat it as Absolute - Does not matter the PositionType, just PlacementType
	// Units also does not need to be changed because...
	if (m_parent == NULLPTR)
	{
		//GetHookPoint(layout.calculatedPosition);
		layout.calculatedPosition = layout.absolutePosition;

		val = layout.calculatedPosition;
		m_isUpdatePositionNeeded = false;
		return;
	}



	//
	// If object has parrent
	if (m_parent != NULLPTR)
	{
		Vector4D parentPosition, parentSize, parentPadding, parrentArea;
		Vector4D size, position, hookPoint;


		// do not calculate position if Absolute
		if (layout.positionAspect == PGUIPositionAspect::Absolute)
		{
			GetHookPoint(layout.calculatedPosition);
			layout.calculatedPosition += layout.absolutePosition;

			m_isUpdatePositionNeeded = false;
			val = layout.calculatedPosition;
			return;
		}

		
		m_parent->GetPosition(parentPosition);
		m_parent->GetSize(parentSize);
		m_parent->GetPadding(parentPadding);


		this->GetHookPoint(hookPoint);

		this->GetSize(size);

		

		//
		// RELATIVE
		if (layout.positionAspect == PGUIPositionAspect::Relative)
		{
			// Coord X
			if (layout.positionUnits[0] == PGUIUnit::Float)
			{
				layout.calculatedPosition.X = parentPosition.X + layout.absolutePosition.X + hookPoint.X;
			}
			else if(layout.positionUnits[0] == PGUIUnit::Percentage)
			{
				layout.calculatedPosition.X = parentPosition.X + (layout.absolutePosition.X * parentSize.X) + hookPoint.X;
			}

			// Coord Y
			if (layout.positionUnits[1] == PGUIUnit::Float)
			{
				layout.calculatedPosition.Y = parentPosition.Y + layout.absolutePosition.Y + hookPoint.Y;
			}
			else if (layout.positionUnits[1] == PGUIUnit::Percentage)
			{
				layout.calculatedPosition.Y = parentPosition.Y + (layout.absolutePosition.Y * parentSize.Y) + hookPoint.Y;
			}

			m_isUpdatePositionNeeded = false;
			val = layout.calculatedPosition;
			return;
		}
		//
		// Fixed
		else if (layout.positionAspect == PGUIPositionAspect::Fixed)
		{
			// Coord X
			if (layout.positionUnits[0] == PGUIUnit::Float)
			{
				layout.calculatedPosition.X = parentPosition.X + layout.absolutePosition.X + hookPoint.X;
			}
			else if (layout.positionUnits[0] == PGUIUnit::Percentage)
			{
				layout.calculatedPosition.X = parentPosition.X + (layout.absolutePosition.X * parentSize.X) + hookPoint.X;
			}

			// Coord Y
			if (layout.positionUnits[1] == PGUIUnit::Float)
			{
				layout.calculatedPosition.Y = parentPosition.Y + layout.absolutePosition.Y + hookPoint.Y;
			}
			else if (layout.positionUnits[1] == PGUIUnit::Percentage)
			{
				layout.calculatedPosition.Y = parentPosition.Y + (layout.absolutePosition.Y * parentSize.Y) + hookPoint.Y;
			}

			m_isUpdatePositionNeeded = false;
			val = layout.calculatedPosition;
			return;
		}
		//
		// Inline
		else if (layout.positionAspect == PGUIPositionAspect::Inline)
		{
			m_parent->GetChildInlinePosition(this, layout.calculatedPosition);

			m_isUpdatePositionNeeded = false;
			val = layout.calculatedPosition;

			return;
		}


		m_isUpdatePositionNeeded = false;
		val = layout.calculatedPosition;

		// update viewport
		UpdateForm();

	}




	
}



ParabellumUserInterface::PGUIPositionAspect
ParabellumUserInterface::PGUIDrawableObject::GetPositionAspect()
{
	return layout.positionAspect;
}



void 
ParabellumUserInterface::PGUIDrawableObject::GetSize(Vector4D& val)
{

	// return calculated size if object and its parent were not changed
	// otherwise recalculate the position
	if (m_isUpdateSizeNeeded == false)
	{
		val = layout.calculatedSize;
		return;
	}

	//
	//
	if (m_parent == NULLPTR)
	{
		layout.calculatedSize = layout.absoluteSize;

		m_isUpdateSizeNeeded = false;
		val = layout.calculatedSize;
		return;
	}


	//
	//
	if (m_parent != NULLPTR)
	{
		Vector4D parentPosition, parentSize, parentPadding, parrentArea;
		Vector4D size, position, hookPoint;

		m_parent->GetSize(parentSize);

		// Coord X
		if (layout.sizeUnits[0] == PGUIUnit::Float)
		{
			layout.calculatedSize.X = layout.absoluteSize.X;
		}
		else if (layout.sizeUnits[0] == PGUIUnit::Percentage)
		{
			layout.calculatedSize.X = parentSize.X * layout.absoluteSize.X;
		}

		// Coord Y
		if (layout.sizeUnits[1] == PGUIUnit::Float)
		{
			layout.calculatedSize.Y = layout.absoluteSize.Y;
		}
		else if (layout.sizeUnits[1] == PGUIUnit::Percentage)
		{
			layout.calculatedSize.Y = parentSize.Y * layout.absoluteSize.Y;
		}

		m_isUpdateSizeNeeded = false;
		val = layout.calculatedSize;
		
		// update viewport
		UpdateForm();
	}
}


void 
ParabellumUserInterface::PGUIDrawableObject::GetPadding(Vector4D& val)
{
	val = layout.padding;
}

ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsVisible()
{
	return m_isVisible;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsDisplayed()
{
	if (m_parent != NULLPTR)
		return m_isDisplayed && m_parent->IsDisplayed();
	else
		return m_isDisplayed;
}

void 
ParabellumUserInterface::PGUIDrawableObject::GetSkinForegroundCoords(_IN_ Vector4D& v4d)
{
	v4d = layout.bgUV;
}

void 
ParabellumUserInterface::PGUIDrawableObject::GetSkinBackgroundCoords(_IN_ Vector4D& v4d)
{
	v4d = layout.fgUV;
}


ParabellumFramework::EBOOL
ParabellumUserInterface::PGUIDrawableObject::IsInsideViewPort(EFLOAT32 x, EFLOAT32 y)
{
	ViewPort parentViewPort;
	
	if (m_parent != NULLPTR)
	{
		m_parent->GetViewPort(&parentViewPort);

		if (parentViewPort.isEnabled == false)
			parentViewPort.area.Set(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
		parentViewPort.area.Set(0.0f, 0.0f, 1.0f, 1.0f);


	if (
		(x < parentViewPort.area.X) ||
		(y < parentViewPort.area.Y) ||
		(x > parentViewPort.area.Z) ||
		(y > parentViewPort.area.W)
		)
		return false;

	return true;
}


void
ParabellumUserInterface::PGUIDrawableObject::GetViewPort(_OUT_ ViewPort* pViewPort)
{
	if (this->viewport.isEnabled == true)
	{
		pViewPort->area.X = ParabellumUtils::Math::Max(pViewPort->area.X, this->viewport.area.X);
		pViewPort->area.Y = ParabellumUtils::Math::Max(pViewPort->area.Y, this->viewport.area.Y);
		pViewPort->area.Z = ParabellumUtils::Math::Min(pViewPort->area.Z, this->viewport.area.Z);
		pViewPort->area.W = ParabellumUtils::Math::Min(pViewPort->area.W, this->viewport.area.W);
	}

	if (m_parent != NULLPTR)
		m_parent->GetViewPort(pViewPort);

}




void
ParabellumUserInterface::PGUIDrawableObject::SetPosition(PGUIUnitData x, PGUIUnitData y)
{
	layout.absolutePosition.X = x.Value;
	layout.positionUnits[0] = x.Unit;
	layout.absolutePosition.Y = y.Value;
	layout.positionUnits[1] = y.Unit;

	ForceUpdate();
}

void
ParabellumUserInterface::PGUIDrawableObject::SetPositionAspect(PGUIPositionAspect positionAspect)
{
	layout.positionAspect = positionAspect;

	ForceUpdate();
}

void
ParabellumUserInterface::PGUIDrawableObject::SetPlacementType(PGUIPlacementType placementType)
{
	layout.placementType = placementType;

	ForceUpdate();
}


void
ParabellumUserInterface::PGUIDrawableObject::SetMargin(Vector4D v4d)
{
	layout.margin = v4d;
}

void
ParabellumUserInterface::PGUIDrawableObject::SetPadding(Vector4D v4d)
{
	layout.padding = v4d;
}


void
ParabellumUserInterface::PGUIDrawableObject::SetSize(PGUIUnitData width, PGUIUnitData height)
{
	layout.absoluteSize.X = width.Value;
	layout.sizeUnits[0] = width.Unit;
	layout.absoluteSize.Y = height.Value;
	layout.sizeUnits[1] = height.Unit;

	ForceUpdate();
}

void
ParabellumUserInterface::PGUIDrawableObject::SetWidth(PGUIUnitData width)
{
	layout.absoluteSize.X = width.Value;
	layout.sizeUnits[0] = width.Unit;

	ForceUpdate();
}


void
ParabellumUserInterface::PGUIDrawableObject::SetHeight(PGUIUnitData height)
{
	layout.absoluteSize.Y = height.Value;
	layout.sizeUnits[1] = height.Unit;

	ForceUpdate();
}

void 
ParabellumUserInterface::PGUIDrawableObject::SetVisibility(EBOOL val)
{
	m_isVisible = val;

	ForceUpdate();
}

void
ParabellumUserInterface::PGUIDrawableObject::SetDisplayable(EBOOL val)
{
	m_isDisplayed = val;

	ForceUpdate();
}


void
ParabellumUserInterface::PGUIDrawableObject::SetSkinForegroundCoords(Vector4D v4d)
{
	layout.fgUV = v4d;
}


void
ParabellumUserInterface::PGUIDrawableObject::SetSkingBackgroundCoords(Vector4D v4d)
{
	layout.bgUV = v4d;
}


void
ParabellumUserInterface::PGUIDrawableObject::SetShader(_IN_ _KEEP_ Shader* pShader)
{
	m_shader = pShader;
}

void
ParabellumUserInterface::PGUIDrawableObject::SetSkinBackground(_IN_ _KEEP_ Texture2D* pT2D)
{
	layout.t2Dbackground = pT2D;
}

void
ParabellumUserInterface::PGUIDrawableObject::SetColor(Vector4D color)
{
	layout.color = color;
}
