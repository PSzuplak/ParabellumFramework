#ifndef _PGUIPROGRESSBAR_H_
#define _PGUIPROGRESSBAR_H_


//
// 
//
#include <string>
#include <list>

//
//	ParabellumGUI dependieces
//
#include "PUITypes.h"
#include "PUIEventMouse.h"

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

#include "PGUIExtDrawableObject.h"
#include "PGUIText.h"

//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumUserInterface
{

	//
	//	GroupBox - group UI objects
	//
	class XYZ_API PGUIProgressBar : public PGUIExtDrawableObject
	{
	public:

		PGUIProgressBar(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIProgressBar(const PGUIProgressBar&) = delete;

		virtual ~PGUIProgressBar();

	//
	// Methods
	//
	public:

	//
	// Members
	//
	private:

		EFLOAT32 m_minValue;

		EFLOAT32 m_maxValue;

		EFLOAT32 m_currentValue;

		PGUIDrawableObject* m_progress;

		PGUIText* m_label;

	//
	// Gets And Sets
	//
	public:


		EFLOAT32 GetPercentageValue();
		EFLOAT32 GetAbsoluteValue();

		EFLOAT32 GetMin();
		EFLOAT32 GetMax();

		// val is 0.0f to 1.0f which means 0% to 100%
		void SetPercentageValue(EFLOAT32 val);

		// val shoud be from m_minValue to m_maxValue
		void SetAbsoluteValue(EFLOAT32 val);


		void SetMin(EFLOAT32 val);
		void SetMax(EFLOAT32 val);

	};

}








#endif