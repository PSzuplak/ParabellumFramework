#ifndef _PIVOT_H_
#define _PIVOT_H_

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"



//
// STDs
//
#include <memory>
#include <functional>
#include <list>

//
// ParabellumFramework
//
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/InputDevice.h"
#include "../ParabellumFramework/Model.h"
#include "../ParabellumFramework/MathHelperExtended.h"
#include "../ParabellumFramework/MathHelperPrimitives.h"

//
// ParabellumEngine
//
#include "EditorTypes.h"
#include "Component3D.h"
#include "MapSector.h"

#include "SelectionTool.h"
#include "SelectionTool_Box.h"


// UI
#include "SceneEditorUI.h"



namespace ParabellumEngine
{
	namespace Editor
	{

		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Graphics;
		using namespace ParabellumFramework::IO;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumEngine::Components;


		//
		// Part of a pivot - may be a "translation arrow" or "rotation circle"
		//
		class XYZ_API PivotPart
		{
		public:
			PivotPart()
			{
			}


			~PivotPart()
			{

			}


		//
		// Methods
		//
		public:

			void Update(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ ParabellumEngine::Components::Camera* pCamera, Vector2D& mouseUV)
			{
				Vector4D colorBorder, colorInterior, tmp, ndir;
				Matrix4x4 translate, scale, world, projected;
				Vector4D axisEnd;

				//	-------------------------------------------------------------------
				//		Calculate position
				//	-------------------------------------------------------------------

				EFLOAT32 pointerSize = Vector3D::Length(m_begin.XYZ - (*pCamera->GetPosition())) * (1.0f/16.0f);

				ndir = m_direction;
				ndir.Normalize();

				axisEnd = m_begin ;
				axisEnd += ndir * pointerSize;

				SetEnd(axisEnd);

				//
				// start position
				translate.MakeTranslation(&m_begin.XYZ);
				scale.MakeScale(1.0f, 1.0f, 1.0f);
				world = translate * scale;
				projected = (*pCamera->GetViewProjection()) * world;
				projected.GetTranslation(&tmp);
				tmp /= tmp.W;
				UnitHelper::ScreenSpaceToUv(tmp.XY, m_UVPositionStart);

				m_UVPositionStart = m_UVPositionStart;
				m_UVPositionStart = m_UVPositionStart;

				//
				// end position
				translate.MakeTranslation(&axisEnd.XYZ);
				scale.MakeScale(1.0f, 1.0f, 1.0f);
				world = translate * scale;
				projected = (*pCamera->GetViewProjection()) * world;
				projected.GetTranslation(&tmp);
				tmp /= tmp.W;
				UnitHelper::ScreenSpaceToUv(tmp.XY, m_UVPositionEnd);

			}


			void Draw(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ ParabellumEngine::Components::Camera* pCamera, Vector2D& mouseUV)
			{
				Vector4D color;
				Matrix4x4 translate, scale, world, projected;
				EFLOAT32 pointerSize = Vector3D::Length(m_begin.XYZ - (*pCamera->GetPosition())) * (1.0f/16.0f);

				// set common for all arrows shader parametrs
				translate.MakeTranslation(&m_begin.XYZ);
				scale.MakeScale(pointerSize, pointerSize, pointerSize);
				world = translate * scale;

				// Draw arrow
				if (m_isMarked == true)
					color.Set(1.0f, 1.0f, 0.0f, 1.0f);
				else
					color = m_color;

				pDevice->SetShader(shader);
				shader->SetParametrsBuffer(0, &world);
				shader->SetParametrsBuffer(1, pCamera->GetView());
				shader->SetParametrsBuffer(2, pCamera->GetProjection());
				shader->SetParametrsBuffer(3, &color);
				shader->SetParametrsBuffer(4, &color);

				pDevice->SetVertexBuffer(model->GetVertexBuffer(0));
				pDevice->SetIndexBuffer(model->GetIndexBuffer(0));

				pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);


				// draw line
				if (m_isMarked == true)
				{
					translate.MakeTranslation(&m_begin.XYZ);
					scale.MakeScale( m_direction.X * 1000.0f + 0.01f, m_direction.Y * 1000.0f + 0.01f, m_direction.Z * 1000.0f + 0.01f);
					world = translate * scale;

					shader->SetParametrsBuffer(0, &world);
					shader->SetParametrsBuffer(3, &color);
					shader->SetParametrsBuffer(4, &color);

					pDevice->SetVertexBuffer(PrimitivesFactory::Instance(pDevice).GetBoxVertexBuffer());
					pDevice->SetIndexBuffer(PrimitivesFactory::Instance(pDevice).GetBoxIndexBuffer());
					pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);
				}

			
			}



		//
		// Members
		//
		private:

			Vector4D m_direction;

		public:
			Vector4D m_begin;
			Vector4D m_end;
			

			bool m_isMarked;

			Vector4D m_color;

			Vector2D m_UVPositionStart;
			Vector2D m_UVPositionEnd;

		public:
			Model* model;
			Shader* shader;
		//
		// Gets And Sets
		//
		public:

			void SetBegin(_IN_ Vector4D& input)
			{
				m_end += input - m_begin;
				m_begin = input;
				m_direction = m_end - m_begin;
			}

			void Move(_IN_ Vector4D& input)
			{
				m_begin += input;
				m_end += input;
			}

			void SetEnd(_IN_ Vector4D& input)
			{
				m_end = input;
				m_direction = m_end - m_begin;
			}

			void SetColor(_IN_ Vector4D& color)
			{
				m_color = color;
			}

			void SetMark(bool val)
			{
				m_isMarked = val;
			}



			Vector4D GetBegin()
			{
				return m_begin;
			}

			Vector4D GetDirection()
			{
				return m_direction;
			}

			Vector4D GetEnd()
			{
				return m_end;
			}

		};






		//
		//
		//
		class XYZ_API Pivot
		{

		public:

			Pivot(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);
			~Pivot();

		//
		// Methods
		//
		public:

			void Update(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ ParabellumEngine::Components::Camera* pCamera, Vector2D& mouseUV);
			void Draw(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ ParabellumEngine::Components::Camera* pCamera, Vector2D& mouseUV);
			

			void MarkAxis(PE_AxisType axisType)
			{
				markedAxis = axisType;

				axisX.SetMark(false);
				axisY.SetMark(false);
				axisZ.SetMark(false);

				if (axisType != PE_AxisType::None)
					axis[axisType].SetMark(true);			
			}

			void MarkAxis(_IN_ Vector2D& mouseUV)
			{
				EFLOAT32 distX = DistanceFixed(axisX.m_UVPositionStart, axisX.m_UVPositionEnd, mouseUV);
				EFLOAT32 distY = DistanceFixed(axisY.m_UVPositionStart, axisY.m_UVPositionEnd, mouseUV);
				EFLOAT32 distZ = DistanceFixed(axisZ.m_UVPositionStart, axisZ.m_UVPositionEnd, mouseUV);

				if ((distX < 0.02f) && (distX > 0.0f))
					MarkAxis(PE_AxisType::Axis_X);
				else if ((distY < 0.02f) && (distY > 0.0f))
					MarkAxis(PE_AxisType::Axis_Y);
				else if ((distZ < 0.02f) && (distZ > 0.0f))
					MarkAxis(PE_AxisType::Axis_Z);
				else
					MarkAxis(PE_AxisType::None);
			}

		//
		// Memebers
		//
		public:

			


			Vector4D m_positionBegin;

			union
			{
				struct
				{
					PivotPart axisX;
					PivotPart axisY;
					PivotPart axisZ;
				};

				struct
				{
					PivotPart axis[3];
				};
			};



			// mark axis as marked if mouse is over arrow
			PE_AxisType markedAxis;

			EFLOAT32 pointerSize;
	
			struct PointerData
			{	
			};


			struct PivotGeneral
			{
				PivotGeneral() {}
				~PivotGeneral() {}

				union
				{
					struct
					{
						PointerData axisX;
						PointerData axisY;
						PointerData axisZ;
					};

					struct
					{
						PointerData axis[3];
					};
				};


			


				// size on the screen
				EFLOAT32 scale;


				




			} pivotData;

		//
		// Gets and sets
		//
		public:


			void SetPosition(_IN_ Vector4D& input);

			void Move(_IN_ Vector4D& input);




			EBOOL IsSelected();


			PivotPart& MarkedAxis()
			{
				return axis[markedAxis];
			}

			PivotPart& MarkedAxis2()
			{
				return axis[markedAxis];
			}


		};
	}
}






#endif