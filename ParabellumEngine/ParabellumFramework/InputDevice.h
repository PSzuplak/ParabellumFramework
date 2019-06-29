#ifndef _INPUTDEVICE_H_
#define _INPUTDEVICE_H_

#include "SystemCodes.h"
#include "IOCodes.h"
#include "TypesC.h"
#include "FlagHelper.h"
#include "MathHelper.h"
#include "MathHelperExtended.h"
#include "DebugManager.h"
#include "IOTypes.h"
#include "Event.h"

namespace ParabellumFramework
{
	namespace IO
	{


		struct PFMouseState
		{
			union
			{
				struct
				{
					EUINT32 lX;
					EUINT32 lY;
					EUINT32 lZ;
					EBYTE rgbButtons[8];
					EBYTE reserved[8];
				};

				EBYTE p_bits[64];
			};
		};







		//
		// DirectX or OpenGL deivce shoud derive from this class
		//
		class XYZ_API InputDevice
		{
		private:

		public:
			InputDevice();
			InputDevice(InputDevice&);
			~InputDevice();

		public:

			virtual EUINT32 Initialize(void* hinstance, void* hwnd, int flag) = 0;
			virtual void Update() final;
			virtual void Release() = 0;

			virtual void* GetDevice() = 0;
			virtual void* GetKeyboard() = 0;

			//
			// Mouse functions
			//

			virtual EUINT32 GetCurrentMouseState() = 0;
			virtual EUINT32 GetPreviousMouseState() = 0;
			EUINT32 GetMouseX();
			EUINT32 GetMouseY();
			EINT32 GetMouseDeltaX();
			EINT32 GetMouseDeltaY();
			void GetMousePosition(_OUT_ EINT32& px, _OUT_ EINT32& py);
			void GetMousePositionNormalized(_OUT_ EFLOAT32& x, _OUT_ EFLOAT32& y);

			// if mouse is dragged - get the data
			EBOOL GetMouseDragged(_OUT_ EFLOAT32& pressedX, _OUT_ EFLOAT32& pressedY, _OUT_ EFLOAT32& currentX, _OUT_ EFLOAT32& currentY, _OUT_ EFLOAT32& deltaX, _OUT_ EFLOAT32& deltaY);


			EBOOL WasMouseMoved();

			EBOOL WasLeftButtonDown();
			EBOOL IsLeftButtonDown();
			EBOOL WasLeftButtonReleased();





	
			virtual EUINT8 GetKeyboardASCIIState(EUINT32 key) = 0;


			

		protected:
			unsigned char** GetAddressOfKeyboardState();

		public:
			EUINT8 GetCurrentKeyboardState(EUINT32 key);
			EUINT8 GetPreviousKeyboardState(EUINT32 key);

			EBOOL WasKeyReleased(EUINT32 key);
			EBOOL IsKeyHolded(EUINT32 key);

		private:


		virtual void Device_Update() = 0;

			
		public:
			// if true then copy to m_previousKeyboardState otherwise to m_currentKeyboardState
			EBOOL m_keyStateSwitch;

		protected:

			unsigned char m_currentKeyboardState[256];
			unsigned char m_previousKeyboardState[256];

		private:


			// stores history of pressed keys
			unsigned char m_history[1024];

			EUINT32 m_historyPosition;

		private:
			EUINT32 m_previousMouseX, m_previousMouseY;
			EUINT32 m_mouseX, m_mouseY;
			EINT32 m_mouseDeltaX, m_mouseDeltaY;

		protected:
			Vector2D m_mousePreviousNormalized;
			Vector2D m_mouseCurrentNormalized;
			Vector2D m_mouseDeltaNormalized;

			Vector2D m_leftButtonPressed;
			Vector2D m_leftButtonReleased;

			EBOOL m_isLeftButtonHolded;

			// if 'true' then cursor will move only by 'm_displacementVector'
			EBOOL m_isFicexDisplacement;

			// does not have to be normlaized
			Vector2D m_displacementVector;


			PFMouseState m_previousMouseState;
			PFMouseState m_currentMouseState;

		private:

			EFLOAT32 m_mouseSpeed;

		//
		// Gets And Sets
		// 
		public:

			void SetMousePosition(EUINT32 x, EUINT32 y);

			// move mouse position by (dx, dy)
			void MoveMousePosition(EUINT32 dx, EUINT32 dy);


			// 'displacementVector' does not have to be normlized - longer vector gives longer displacement
			void SetDisplacementVector(_IN_ Vector2D* displacementVector, EBOOL enabled);




		//
		// Events
		//
		public:

			// this Event is being generated whenever an user press and release a keyboard key
			// the parameter is key code (PFK_x)
			Event::Event<EBYTE> EventKeyReleased;

			// this Event is being generated whenever an user hold a key pressed
			// the parameter is key code (PFK_x)
			Event::Event<EBYTE> EventKeyHolded;


			// this Event is being generated whenever an user move a mouse
			// the parameters are screen coorinates (pixel coord X, pixel coord Y)
			Event::Event<EUINT32, EUINT32> EventMouseMoved;

			// check is left button was pressed (the event is sent only one time before released)
			// Parametrs are normalized (0.0f to 1.0f) mouse coordinates (x,y)
			Event::Event<EFLOAT32, EFLOAT32> EventMouseLeftPressed;

			// check is left button was released (first it had to be pressed). Parametrs are normalized (0.0f to 1.0f) mouse coordinates (x,y)
			Event::Event<EFLOAT32, EFLOAT32> EventMouseLeftReleased;

			// check is left button was pressed and dragged
			// 1st and 2nd parametr are mouse coordinates when a mouse left button was pressed first
			// 3rd and 4th parametr are the current mouse coordinates
			// 5th and 6th parametr are the delta coordinates from the last frame
			Event::Event<EFLOAT32, EFLOAT32, EFLOAT32, EFLOAT32, EFLOAT32, EFLOAT32> EventMouseLeftDragged;

			
		};
	}
}

#endif