#pragma once
#include "stdafx.h"

using namespace vEngine;

namespace vEngine {
	class Keyboard;
}

namespace Game {
	class CubeDemo : public DrawableComponent {
		RTTI_DECLARATIONS(CubeDemo, DrawableComponent)
	public:
		CubeDemo(Engine& engine, Camera& camera);
		~CubeDemo();
		//
		virtual void Initialize() override;
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
		CubeDemo();
		CubeDemo(const CubeDemo& rhs);
		CubeDemo& operator=(const CubeDemo& rhs);
	private:
		ID3DX11Effect* m_Effect;
		ID3DX11EffectTechnique* m_Technique;
		ID3DX11EffectPass* m_Pass;
		ID3DX11EffectMatrixVariable* m_WvpVariable;
		ID3D11InputLayout* m_InputLayout;
		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;
		XMFLOAT4X4 m_WorldMatrix;
	};
}