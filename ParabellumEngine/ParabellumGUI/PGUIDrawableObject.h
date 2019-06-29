#ifndef _PGUIDRAWABLEOBJECT_H_
#define _PGUIDRAWABLEOBJECT_H_


//
// 
//
#include <string>
#include <list>


//
// Devices
// 
#include "../ParabellumFramework/IOCodes.h"
#include "../ParabellumFramework/TaskManager.h"
//
//	ParabellumGUI dependieces
//
#include "PGUIObject.h"
#include "PUITypes.h"
#include "PUIEventMouse.h"

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/UnitHelper.h"

#include "../ParabellumFramework/VertexBuffer.h"
#include "../ParabellumFramework/IndexBuffer.h"

#include "../ParabellumFramework/Vector2D.h"
#include "../ParabellumFramework/Vector3D.h"

#include "../ParabellumFramework/Event.h"

#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/Texture2D.h"
//
//	Includes
//


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;
using namespace ParabellumFramework::Math;

namespace ParabellumUserInterface
{



	// ------------------------------------------------------------------------------------------------
	//
	//	Raw Drawable PUI object
	//
	class XYZ_API PGUIDrawableObject : public PGUIObject
	{
	public:

		static ParabellumFramework::Event::TaskManager taskManager;

	public:

		PGUIDrawableObject(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);

		// disallow copy constructor
		PGUIDrawableObject(const PGUIDrawableObject&) = delete;

		virtual ~PGUIDrawableObject();

	//
	// Methods
	//
	public:

		// Draw GUI object
		virtual void Draw(_IN_ GraphicsDevice* pDevice);



	private:
		// force to recalculate its position and position if its children
		void ForceUpdate();

	public:
		EBOOL CheckCollision(EFLOAT32 x, EFLOAT32 y);
		EBOOL CheckCollision(EFLOAT32 x1, EFLOAT32 y1, EFLOAT32 x2, EFLOAT32 y2);
		EBOOL CheckCollision(_IN_ Vector4D& v4d);



	//
	// Parent-Cildren relation
	public:

		//
		// parent-child relation
		//
		void SetParent(_IN_ PGUIDrawableObject* pParent);
		void UnsetParent();

		// return pointer to a child which collides with (x,y) point
		// there can be one child like that because we must not execute events for other objects
		PGUIDrawableObject* GetDepest(EFLOAT32 x, EFLOAT32 y);

	private:
		void AddChild(_IN_ PGUIDrawableObject* pChild);
		void RemoveChild(_IN_ PGUIDrawableObject* pChild);



		// find child position
		// pChild must be child of this object (stored in m_children)
		void FindChildPosition(PGUIDrawableObject* pChild, EFLOAT32& outX, EFLOAT32& outY);


	public:

		// return pointer to its parent
		PGUIDrawableObject* GetParent();

		// check if object is related to some other
		EBOOL IsRelatedClose(_IN_ PGUIDrawableObject* pObj);

		// is pObj parent/grandpa/UP of this object
		EBOOL IsRelatedUp(_IN_ PGUIDrawableObject* pObj);

		// is pObj son/down of this object
		EBOOL IsRelatedDown(_IN_ PGUIDrawableObject* pObj);

		EBOOL IsSibling(_IN_ PGUIDrawableObject* pObj);

	//
	//
	//
	public:

		// move object by (dx,dy)
		void Move(EFLOAT32 dx, EFLOAT32 dy);

		// resize object by (dx,dy)
		void Resize(EFLOAT32 dx, EFLOAT32 dy);

		//  move all children which have position Relative by (dx,dy)
		void MoveChildren(EFLOAT32 dx, EFLOAT32 dy);


	//
	//
	//
	private:

		void UpdateForm();

	//
	// Members
	//
	public:

		//
		// Graphics
		//

		// VertexBuffer
		static VertexBuffer* m_vertexBuffer;

		// IndexBuffer
		static IndexBuffer* m_indexBuffer;

		// shader
		Shader* m_shader;


		//
		// Parent-child relation
		//

		// parent of this object
		PGUIDrawableObject* m_parent;

		// children of this object
		std::list<PGUIDrawableObject*> m_children;


		//
		//
		EBOOL m_isUpdatePositionNeeded;
		EBOOL m_isUpdateSizeNeeded;

		//
		// Position data

		struct Layout
		{

			// position on display - using normalized coordinates (0,0) to (1,1)
			// basically it stores position of Top Left corner
			// X,Y - screen coordinates
			// Z - object with bigger depth covers object with smaller depth value
			// W - unused
			Vector4D absolutePosition;

			// calculated position using all tricks
			Vector4D calculatedPosition;

			// units for position
			PGUIUnit positionUnits[4];

			//
			// PositionAspect
			PGUIPositionAspect positionAspect;

			//
			// PositionPoint
			PGUIPlacementType placementType;

			// If some children are aligned,relative or auto - then they will be moved by padding value
			// X - padding top, Y - padding right, Z - padding bottom, W - padding left
			// If object covers padding boundaries it will be cut during drawing!
			Vector4D padding;


			// If PositionType is auto this component will be moved by margin values from the other objects. 
			Vector4D margin;



			//
			// Rotation data
			Vector4D rotation;


			//
			// Size data

			// X - width of the object - using normalized coordinates (0,0) to (1,1)
			// Y - height of the object - using normalized coordinates (0,0) to (1,1)
			// Z,W - unused
			Vector4D absoluteSize;

			// calculated size using all tricks
			Vector4D calculatedSize;


			// units for position
			PGUIUnit sizeUnits[4];

			// size type
			PUISizeType sizeType;

			// foreground texture
			Texture2D* t2Dbackground;

			// begin and end UV coords of background texture
			Vector4D bgUV;

			// begin and end UV coords of foreground texture
			Vector4D fgUV;

			// this value will be multiplied by texture
			Vector4D color;

			// border size
			Vector4D borderSize;

			// bordercolor
			Vector4D borderColor;

			// opacity level 0.0f to 1.0f
			EFLOAT32 opacity;

			// blur factor 0.0f - nor blur, 1.0f max blur
			EFLOAT32 bgBlurFactor;


		} layout;

		
	public:

		// is object can be drawed - object can be visible if its parents are not visible
		// objects position and size has still an affect -
		// it is not the same as m_isDisplayed
		EBOOL m_isVisible;

		// if true this object is taken as it does not exist
		// it is being skiped in layout
		EBOOL m_isDisplayed;

		// draw rect - area where children can be drawed 
		// if children is bigger then it will be cut
		Vector4D m_displayRect;
		Vector4D m_calculatedDisplayRect;


		//
		// Sprites setting
		EBOOL m_isSpriteable;
		std::vector<Vector4D> m_spriteCoords;


	public:


		class ViewPort
		{
		public:
			ViewPort()
			{
				area.Set(0.0f, 0.0f, 1.0f, 1.0f);
			}

			void Set(bool enabled)
			{
				isEnabled = enabled;
			}

		public:
			// if viewport is enabled then all object children will be cut to the area definied by 'ViewPort::area'
			EBOOL isEnabled;

			// coordinates are from (0.0f, 0.0f) to (1.0f, 1.0f)
			Vector4D area;

		} viewport;





	//
	// Events
	//
	private:

		std::vector<PUIEvent> m_eventsOnMouseClick;
		std::vector<PUIEvent> m_eventsOnMouseHold;
		std::vector<PUIEvent> m_eventsOnMouseOver;
		std::vector<PUIEvent> m_eventsOnMouseOut;
		std::vector<PUIEvent> m_eventsOnMouseMove;
		std::vector<PUIEvent> m_eventsOnMouseDrag;
		std::vector<PUIEvent> m_eventsOnChange;

	public:

		void AddEventMouseClick(PUIEvent);
		void AddEventMouseHold(PUIEvent);
		void AddEventMouseOver(PUIEvent);
		void AddEventMouseOut(PUIEvent);
		void AddEventMouseMove(PUIEvent);
		void AddEventMouseDrag(PUIEvent);
		void AddEventChange(PUIEvent);

		// invoke mouce click events - return true if any was invoked
		// not that if child object invoked some funcion then parrent will not invoke anyting
		EBOOL InvokeMouseClick(EFLOAT32 x, EFLOAT32 y);
		EBOOL InvokeMouseHold(EFLOAT32 x, EFLOAT32 y, IO::MouseState state);

		EBOOL InvokeMouseOver(EFLOAT32 x, EFLOAT32 y);
		void InvokeMouseOut(EFLOAT32 x, EFLOAT32 y);
		void InvokeMouseMove(EFLOAT32 x, EFLOAT32 y);
		void InvokeMouseDrag(EFLOAT32 beginX, EFLOAT32 beginY, EFLOAT32 currentX, EFLOAT32 currentY, EFLOAT32 deltaX, EFLOAT32 deltaY);

		void InvokeOnChange();




	//
	// Gets And Sets
	//
	private:

		// get screen position of child which is set to Inline
		void GetChildInlinePosition(_IN_ PGUIDrawableObject* pChild, _OUT_ Vector4D& out);

	public:
		// Hook point is a point which will be used to calculate position
		// if PlacemenType is Center then center will be interpretated as (0,0)
		// so HookPoint will have x = size.x/2 and y = size.y/2
		void GetHookPoint(Vector4D& val);


		// calculate object position and save it in parametr val
		// val contains SCREEN (-1.0f, 1.0f)-(1.0f, -1.0f) coordiantes
		void GetPosition(Vector4D& val);

		PGUIPositionAspect GetPositionAspect();


	public:
		// calculate object size and save it in parametr val
		void GetSize(Vector4D& val);

		// Get padding
		void GetPadding(Vector4D& val);

		EBOOL IsVisible();

		EBOOL IsDisplayed();

		void GetSkinForegroundCoords(_IN_ Vector4D& v4d);
		void GetSkinBackgroundCoords(_IN_ Vector4D& v4d);


		EBOOL IsInsideViewPort(EFLOAT32 x, EFLOAT32 y);


		// calculate viewport which will be used to drawn this object.
		// then the function will also be cut to ALL its grandparents
		void GetViewPort(_OUT_ ViewPort* pViewPort);





		void SetPosition(PGUIUnitData x, PGUIUnitData y);


		void SetPositionAspect(PGUIPositionAspect positionAspect);
		void SetPlacementType(PGUIPlacementType placementType);

		void SetMargin(Vector4D v4d);
		void SetPadding(Vector4D v4d);


		void SetSize(PGUIUnitData width, PGUIUnitData height);
		void SetWidth(PGUIUnitData width);
		void SetHeight(PGUIUnitData height);

		void SetVisibility(EBOOL val);
		void SetDisplayable(EBOOL val);

		void SetSkinForegroundCoords(Vector4D v4d);
		void SetSkingBackgroundCoords(Vector4D v4d);

		void SetShader(_IN_ _KEEP_ Shader* pShader);

		void SetSkinBackground(_IN_ _KEEP_ Texture2D* pT2D);

		void SetColor(Vector4D color);



	//
	// operators
	//
	private:


	};

}








#endif