#ifndef _PGUIOBJECT_H_
#define _PGUIOBJECT_H_


//
// std
//
#include <string>
#include <list>
#include <functional>
#include <vector>

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"


//
//	Includes
//
#include "PUITypes.h"
#include "PUIEvent.h"


namespace ParabellumUserInterface
{
	using namespace ParabellumFramework;
	//
	//	Abstract object of ParabellumUserInterface
	//
	class XYZ_API PGUIObject
	{
	protected:
		PGUIObject();
		PGUIObject(const PGUIObject&) = delete;
		virtual ~PGUIObject();

	public:



	private:

		// uniqie id of PUI object
		EUINT32 m_id;

		// unique name of this object
		std::string m_name;

		// is object enabled - can be edit/draw/etc.
		EBOOL m_isEnabled;


		//
		// events
		//
		std::vector<PUIEvent> m_eventsOnKeyDown;
		std::vector<PUIEvent> m_eventsOnKeyUp;

		//
		// Gets And sets
		//

	protected:


		//
		// events
		//
	public:
		void AddEventKeyDown(PUIEvent&);
		void AddEventKeyUp(PUIEvent&);


		void InvokeKeyDown(_IN_ unsigned char* pStates);
		void InvokeKeyUp(_IN_ unsigned char* pStates);
	};

}








#endif