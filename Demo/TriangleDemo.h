#pragma once
#include "stdafx.h"

using namespace vEngine;

namespace Game {
	class TriangleDemo : public DrawableComponent {
		RTTI_DECLARATIONS(TriangleDemo, DrawableComponent)
	public:
		TriangleDemo(Engine& engine, Camera& camera);
		~TriangleDemo();
		//
		virtual void Initialize() override;
		virtual void Update(const EngineTime& engineTime) override;
		virtual void Draw(const EngineTime& engineTime) override;
	private:
		typedef struct _BasicEffectVertex
		{
			XMFLOAT4 Position;
			XMFLOAT4 Color;

			_BasicEffectVertex() { }
			_BasicEffectVertex(XMFLOAT4 position, XMFLOAT4 color)
				: Position(position), Color(color) { }
		} BasicEffectVertex;
	private:
		TriangleDemo();
		TriangleDemo(const TriangleDemo& rhs);
		TriangleDemo& operator=(const TriangleDemo& rhs);
	private:
		ID3DX11Effect* m_Effect;
		ID3DX11EffectTechnique* m_Technique;
		ID3DX11EffectPass* m_Pass;
		ID3DX11EffectMatrixVariable* m_WvpVariable;
		ID3D11InputLayout* m_InputLayout;
		ID3D11Buffer* m_VertexBuffer;
		XMFLOAT4X4 m_WorldMatrix;
		float m_Angle;
	};
}