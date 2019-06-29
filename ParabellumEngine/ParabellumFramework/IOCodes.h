#ifndef _IOCODES_H_
#define _IOCODES_H_

#include "TypesC.h"

//
// Keyboard codes
//
#define PFK_ESCAPE          0x01
#define PFK_1               0x02
#define PFK_2               0x03
#define PFK_3               0x04
#define PFK_4               0x05
#define PFK_5               0x06
#define PFK_6               0x07
#define PFK_7               0x08
#define PFK_8               0x09
#define PFK_9               0x0A
#define PFK_0               0x0B
#define PFK_MINUS           0x0C 
#define PFK_EQUALS          0x0D
#define PFK_BACK            0x0E 
#define PFK_TAB             0x0F
#define PFK_Q               0x10
#define PFK_W               0x11
#define PFK_E               0x12
#define PFK_R               0x13
#define PFK_T               0x14
#define PFK_Y               0x15
#define PFK_U               0x16
#define PFK_I               0x17
#define PFK_O               0x18
#define PFK_P               0x19
#define PFK_LBRACKET        0x1A
#define PFK_RBRACKET        0x1B
#define PFK_RETURN          0x1C 
#define PFK_LCONTROL        0x1D
#define PFK_A               0x1E
#define PFK_S               0x1F
#define PFK_D               0x20
#define PFK_F               0x21
#define PFK_G               0x22
#define PFK_H               0x23
#define PFK_J               0x24
#define PFK_K               0x25
#define PFK_L               0x26
#define PFK_SEMICOLON       0x27
#define PFK_APOSTROPHE      0x28
#define PFK_GRAVE           0x29   
#define PFK_LSHIFT          0x2A
#define PFK_BACKSLASH       0x2B
#define PFK_Z               0x2C
#define PFK_X               0x2D
#define PFK_C               0x2E
#define PFK_V               0x2F
#define PFK_B               0x30
#define PFK_N               0x31
#define PFK_M               0x32
#define PFK_COMMA           0x33
#define PFK_PERIOD          0x34   
#define PFK_SLASH           0x35   
#define PFK_RSHIFT          0x36
#define PFK_MULTIPLY        0x37   
#define PFK_LMENU           0x38   
#define PFK_SPACE           0x39
#define PFK_CAPITAL         0x3A
#define PFK_F1              0x3B
#define PFK_F2              0x3C
#define PFK_F3              0x3D
#define PFK_F4              0x3E
#define PFK_F5              0x3F
#define PFK_F6              0x40
#define PFK_F7              0x41
#define PFK_F8              0x42
#define PFK_F9              0x43
#define PFK_F10             0x44
#define PFK_NUMLOCK         0x45
#define PFK_SCROLL          0x46   
#define PFK_NUMPAD7         0x47
#define PFK_NUMPAD8         0x48
#define PFK_NUMPAD9         0x49
#define PFK_SUBTRACT        0x4A   
#define PFK_NUMPAD4         0x4B
#define PFK_NUMPAD5         0x4C
#define PFK_NUMPAD6         0x4D
#define PFK_ADD             0x4E   
#define PFK_NUMPAD1         0x4F
#define PFK_NUMPAD2         0x50
#define PFK_NUMPAD3         0x51
#define PFK_NUMPAD0         0x52
#define PFK_DECIMAL         0x53   
#define PFK_OEM_102         0x56   
#define PFK_F11             0x57
#define PFK_F12             0x58
#define PFK_F13             0x64   
#define PFK_F14             0x65   
#define PFK_F15             0x66   
#define PFK_KANA            0x70   
#define PFK_ABNT_C1         0x73   
#define PFK_CONVERT         0x79   
#define PFK_NOCONVERT       0x7B   
#define PFK_YEN             0x7D   
#define PFK_ABNT_C2         0x7E   
#define PFK_NUMPADEQUALS    0x8D   
#define PFK_PREVTRACK       0x90   
#define PFK_AT              0x91   
#define PFK_COLON           0x92   
#define PFK_UNDERLINE       0x93   
#define PFK_KANJI           0x94   
#define PFK_STOP            0x95   
#define PFK_AX              0x96   
#define PFK_UNLABELED       0x97   
#define PFK_NEXTTRACK       0x99   
#define PFK_NUMPADENTER     0x9C   
#define PFK_RCONTROL        0x9D
#define PFK_MUTE            0xA0   
#define PFK_CALCULATOR      0xA1   
#define PFK_PLAYPAUSE       0xA2   
#define PFK_MEDIASTOP       0xA4   
#define PFK_VOLUMEDOWN      0xAE   
#define PFK_VOLUMEUP        0xB0 
#define PFK_WEBHOME         0xB2 
#define PFK_NUMPADCOMMA     0xB3 
#define PFK_DIVIDE          0xB5 
#define PFK_SYSRQ           0xB7
#define PFK_RMENU           0xB8 
#define PFK_PAUSE           0xC5 
#define PFK_HOME            0xC7 
#define PFK_UP              0xC8 
#define PFK_PRIOR           0xC9 
#define PFK_LEFT            0xCB 
#define PFK_RIGHT           0xCD 
#define PFK_END             0xCF 
#define PFK_DOWN            0xD0 
#define PFK_NEXT            0xD1 
#define PFK_INSERT          0xD2 
#define PFK_DELETE          0xD3 
#define PFK_LWIN            0xDB 
#define PFK_RWIN            0xDC 
#define PFK_APPS            0xDD 
#define PFK_POWER           0xDE 
#define PFK_SLEEP           0xDF 
#define PFK_WAKE            0xE3 
#define PFK_WEBSEARCH       0xE5 
#define PFK_WEBFAVORITES    0xE6 
#define PFK_WEBREFRESH      0xE7 
#define PFK_WEBSTOP         0xE8 
#define PFK_WEBFORWARD      0xE9 
#define PFK_WEBBACK         0xEA 
#define PFK_MYCOMPUTER      0xEB 
#define PFK_MAIL            0xEC 
#define PFK_MEDIASELECT     0xED 

#define PFK_BACKSPACE       PFK_BACK     
#define PFK_NUMPADSTAR      PFK_MULTIPLY 
#define PFK_LALT            PFK_LMENU    
#define PFK_CAPSLOCK        PFK_CAPITAL  
#define PFK_NUMPADMINUS     PFK_SUBTRACT 
#define PFK_NUMPADPLUS      PFK_ADD      
#define PFK_NUMPADPERIOD    PFK_DECIMAL  
#define PFK_NUMPADSLASH     PFK_DIVIDE   
#define PFK_RALT            PFK_RMENU    
#define PFK_UPARROW         PFK_UP       
#define PFK_PGUP            PFK_PRIOR    
#define PFK_LEFTARROW       PFK_LEFT     
#define PFK_RIGHTARROW      PFK_RIGHT    
#define PFK_DOWNARROW       PFK_DOWN     
#define PFK_PGDN            PFK_NEXT     


//
// Mouse Codes
//
#define PFM_NO_ACTION		EBYTENO_0
#define PFM_LEFT_DOWN		EBYTENO_1
#define PFM_MIDDLE_DOWN		EBYTENO_3
#define PFM_RIGHT_DOWN		EBYTENO_5

namespace ParabellumFramework
{
	namespace IO
	{

		enum class MouseState : ParabellumFramework::EUINT32
		{
			NO_ACTION = 0,
			LEFT_DOWN = 1,
			LEFT_UP,
			MIDDLE_DOWN,
			MIDDLE_UP,
			RIGHT_DOWN,
			RIGHT_UP,
		};

	}

}

#endif