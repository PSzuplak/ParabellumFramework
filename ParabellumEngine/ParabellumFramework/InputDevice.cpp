#include "stdafx.h"
#include "InputDevice.h"

//	----------------------------------------------------
//		Constructors and destructors
//	----------------------------------------------------
ParabellumFramework::IO::InputDevice::InputDevice()
{
	m_keyStateSwitch = false;

	for (EUINT32 i = 0; i < 256; ++i)
	{
		m_currentKeyboardState[i] = 0;
		m_previousKeyboardState[i] = 0;
	}

	m_historyPosition = 0;


	m_mouseSpeed = 0.001f;
	m_isLeftButtonHolded = false;
	m_keyStateSwitch = false;

	m_previousMouseX = 0;
	m_previousMouseY = 0;
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseDeltaX = 0;
	m_mouseDeltaY = 0;

	m_mousePreviousNormalized.Set(0.0f, 0.0f);
	m_mouseCurrentNormalized.Set(0.0f, 0.0f);
	m_mouseDeltaNormalized.Set(0.0f, 0.0f);

	PFDLOG("PFII", "IODV", "InputDevice() : created");
}

ParabellumFramework::IO::InputDevice::InputDevice(InputDevice&)
{
}

ParabellumFramework::IO::InputDevice::~InputDevice()
{
}



//	----------------------------------------------------
//		Methods
//	----------------------------------------------------

void
ParabellumFramework::IO::InputDevice::Update()
{
	// save previous state before update
	m_previousMouseState = m_currentMouseState;

	// switch the state arrays
	m_keyStateSwitch = !m_keyStateSwitch;

	// update device
	Device_Update();

	// check the difference between mouse coordinates and generate an event
	if ( (m_previousMouseX != m_mouseX) || (m_previousMouseY != m_mouseY) )
	{
		EventMouseMoved(m_mouseX, m_mouseY);
	}


	for (EUINT32 i = 0; i < 256; ++i)
	{
		if (WasKeyReleased(i)) EventKeyReleased((EBYTE)i);
		if (IsKeyHolded(i)) EventKeyHolded((EBYTE)i);
	}


	// calculate selectin area for Left Button
	if ( (IsLeftButtonDown() == true) && (m_isLeftButtonHolded == false) )
	{
		m_leftButtonPressed = m_mouseCurrentNormalized;
		m_isLeftButtonHolded = true;

		EventMouseLeftPressed(m_mouseCurrentNormalized.X, m_mouseCurrentNormalized.Y);
	}

	if ((IsLeftButtonDown() == false) && (m_isLeftButtonHolded == true))
	{
		m_leftButtonReleased = m_mouseCurrentNormalized;
		m_isLeftButtonHolded = false;

		EventMouseLeftReleased(m_mouseCurrentNormalized.X, m_mouseCurrentNormalized.Y);
	}

	// generate an event when a left button was pressed and dragged
	if ((m_isLeftButtonHolded == true) && (WasMouseMoved() == true))
	{
		EventMouseLeftDragged(m_leftButtonPressed.X, m_leftButtonPressed.Y, m_mouseCurrentNormalized.X, m_mouseCurrentNormalized.Y, m_mouseDeltaNormalized.X, m_mouseDeltaNormalized.Y);
	}
		
}


//	----------------------------------------------------
//		Gets and Sets
//	----------------------------------------------------

unsigned char**
ParabellumFramework::IO::InputDevice::GetAddressOfKeyboardState()
{
	if (m_keyStateSwitch == false)
		return (unsigned char**)&m_currentKeyboardState;
	else
		return (unsigned char**)&m_previousKeyboardState;
}



unsigned char
ParabellumFramework::IO::InputDevice::GetCurrentKeyboardState(EUINT32 key)
{
	if (m_keyStateSwitch == false)
		return m_currentKeyboardState[key];
	else
		return m_previousKeyboardState[key];
}

unsigned char
ParabellumFramework::IO::InputDevice::GetPreviousKeyboardState(EUINT32 key)
{
	if (m_keyStateSwitch == true)
		return m_currentKeyboardState[key];
	else
		return m_previousKeyboardState[key];
}


ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::WasKeyReleased(EUINT32 key)
{
	if ((GetPreviousKeyboardState(key) != 0) && (GetCurrentKeyboardState(key) == 0))
		return true;
	return false;
}


ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::IsKeyHolded(EUINT32 key)
{
	if ((GetPreviousKeyboardState(key) != 0) && (GetCurrentKeyboardState(key) != 0))
		return true;
	return false;
}


ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::WasMouseMoved()
{
	if ((m_previousMouseX - m_mouseX) != 0)
		return true;

	if ((m_previousMouseY - m_mouseY) != 0)
		return true;

	return false;
}


ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::WasLeftButtonDown()
{
	if (PF_FLAG_ISSET(GetPreviousMouseState(), PFM_LEFT_DOWN))
		return true;
	return false;
}

ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::IsLeftButtonDown()
{
	if (PF_FLAG_ISSET(GetCurrentMouseState(), PFM_LEFT_DOWN))
		return true;
	return false;
}

ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::WasLeftButtonReleased()
{
	if (
		(PF_FLAG_ISSET(GetCurrentMouseState(), PFM_LEFT_DOWN) == false) &&
		(PF_FLAG_ISSET(GetPreviousMouseState(), PFM_LEFT_DOWN) == true)
		)
		return true;

	return false;
}


ParabellumFramework::EUINT32
ParabellumFramework::IO::InputDevice::GetMouseX()
{
	return m_mouseX;
}

ParabellumFramework::EUINT32
ParabellumFramework::IO::InputDevice::GetMouseY()
{
	return m_mouseY;
}

ParabellumFramework::EINT32
ParabellumFramework::IO::InputDevice::GetMouseDeltaX()
{
	return m_mouseDeltaX;
}

ParabellumFramework::EINT32
ParabellumFramework::IO::InputDevice::GetMouseDeltaY()
{
	return m_mouseDeltaY;
}

void
ParabellumFramework::IO::InputDevice::GetMousePosition(EINT32& px, EINT32& py)
{
	px = m_mouseX;
	py = m_mouseY;
}

void
ParabellumFramework::IO::InputDevice::GetMousePositionNormalized(_OUT_ EFLOAT32& x, _OUT_ EFLOAT32& y)
{
	x = m_mouseCurrentNormalized.X;
	y = m_mouseCurrentNormalized.Y;
}


ParabellumFramework::EBOOL
ParabellumFramework::IO::InputDevice::GetMouseDragged(
	_OUT_  EFLOAT32& pressedX, _OUT_ EFLOAT32& pressedY, 
	_OUT_  EFLOAT32& currentX, _OUT_ EFLOAT32& currentY, 
	_OUT_  EFLOAT32& deltaX, _OUT_ EFLOAT32& deltaY)
{
	if ((m_isLeftButtonHolded == true) && (WasMouseMoved() == true))
	{
		pressedX = m_leftButtonPressed.X;
		pressedY = m_leftButtonPressed.Y;
		currentX = m_mouseCurrentNormalized.X;
		currentY = m_mouseCurrentNormalized.Y;
		deltaX = m_mouseDeltaNormalized.X;
		deltaY = m_mouseDeltaNormalized.Y;

		return true;
	}

	return false;
}


void
ParabellumFramework::IO::InputDevice::SetMousePosition(EUINT32 x, EUINT32 y)
{
	// save previous values
	m_previousMouseX = m_mouseX;
	m_previousMouseY = m_mouseY;

	m_mousePreviousNormalized = m_mouseCurrentNormalized;

	m_mouseDeltaX = x - m_mouseX;
	m_mouseDeltaY = y - m_mouseY;

	if (m_isFicexDisplacement == false)
	{
		m_mouseX += m_mouseDeltaX;
		m_mouseY += m_mouseDeltaY;
	}
	else
	{
		Vector2D fixVec;
		fixVec = Math::Projection( Vector2D((EFLOAT32)m_mouseDeltaX, (EFLOAT32)m_mouseDeltaY), m_displacementVector);
		
		m_mouseX += fixVec.X;
		m_mouseY += fixVec.Y;
	}

	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;
	if (m_mouseX > 1650) m_mouseX = 1650;
	if (m_mouseY > 1020) m_mouseY = 1020;

	m_mouseCurrentNormalized.X = (EFLOAT32)m_mouseX / 1650.0f;
	m_mouseCurrentNormalized.Y = (EFLOAT32)m_mouseY / 1020.0f;

	m_mouseCurrentNormalized = Math::Clamp(m_mouseCurrentNormalized, 0.0f, 1.0f);

	m_mouseDeltaNormalized = m_mouseCurrentNormalized - m_mousePreviousNormalized;
	m_mouseDeltaNormalized = Math::Clamp(m_mouseDeltaNormalized, -1.0f, 1.0f);

}

void
ParabellumFramework::IO::InputDevice::MoveMousePosition(EUINT32 dx, EUINT32 dy)
{
	SetMousePosition(m_mouseX + dx, m_mouseY + dy);
}


void 
ParabellumFramework::IO::InputDevice::SetDisplacementVector(_IN_ Vector2D* displacementVector, EBOOL enabled)
{
	m_isFicexDisplacement = enabled;

	if(m_isFicexDisplacement)
		m_displacementVector = *displacementVector;
}