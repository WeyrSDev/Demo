#include "stdafx.h"
#include "TriangleDemo.h"
namespace Game {
	RTTI_DEFINITIONS(TriangleDemo)

	TriangleDemo::TriangleDemo(Engine& engine, Camera& camera)
		: DrawableComponent(engine, camera),
		m_Effect(nullptr), m_Technique(nullptr), m_Pass(nullptr), m_WvpVariable(nullptr),
		m_InputLayout(nullptr), m_WorldMatrix(MatrixHelper::Identity), m_VertexBuffer(nullptr), m_Angle(0.0f)
	{
	}

	TriangleDemo::~TriangleDemo()
	{
		ReleaseObject(m_WvpVariable);
		ReleaseObject(m_Pass);
		ReleaseObject(m_Technique);
		ReleaseObject(m_Effect);
		ReleaseObject(m_InputLayout);
		ReleaseObject(m_VertexBuffer);
	}

	void TriangleDemo::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Compile the shader
		UINT shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* errorMessages = nullptr;
		HRESULT hr = D3DCompileFromFile(L"Content\\Effects\\BasicEffect.fx", nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
		if (FAILED(hr))
		{
			const char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
			EngineException ex(errorMessage, hr);
			ReleaseObject(errorMessages);

			throw ex;
		}

		// Create an effect object from the compiled shader
		hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_Engine->Direct3DDevice(), &m_Effect);
		if (FAILED(hr))
		{
			throw EngineException("D3DX11CreateEffectFromMemory() failed.", hr);
		}

		ReleaseObject(compiledShader);

		// Look up the technique, pass, and WVP variable from the effect
		m_Technique = m_Effect->GetTechniqueByName("main11");
		if (m_Technique == nullptr)
		{
			throw EngineException("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.", hr);
		}

		m_Pass = m_Technique->GetPassByName("p0");
		if (m_Pass == nullptr)
		{
			throw EngineException("ID3DX11EffectTechnique::GetPassByName() could not find the specified pass.", hr);
		}

		ID3DX11EffectVariable* variable = m_Effect->GetVariableByName("WorldViewProjection");
		if (variable == nullptr)
		{
			throw EngineException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
		}

		m_WvpVariable = variable->AsMatrix();
		if (m_WvpVariable->IsValid() == false)
		{
			throw EngineException("Invalid effect variable cast.");
		}

		// Create the input layout
		D3DX11_PASS_DESC passDesc;
		m_Pass->GetDesc(&passDesc);

		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED(hr = m_Engine->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_InputLayout)))
		{
			throw EngineException("ID3D11Device::CreateInputLayout() failed.", hr);
		}

		// Create the vertex buffer
		BasicEffectVertex vertices[] =
		{
			BasicEffectVertex(XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::Red))),
			BasicEffectVertex(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::Green))),
			BasicEffectVertex(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(reinterpret_cast<const float*>(&ColorHelper::Blue)))
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(BasicEffectVertex) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if (FAILED(m_Engine->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &m_VertexBuffer)))
		{
			throw EngineException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	void TriangleDemo::Update(const EngineTime& engineTime)
	{
		m_Angle += XM_PI * static_cast<float>(engineTime.ElapsedEngineTime());
		XMStoreFloat4x4(&m_WorldMatrix, XMMatrixRotationZ(m_Angle));
	}

	void TriangleDemo::Draw(const EngineTime& engineTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = m_Engine->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(m_InputLayout);

		UINT stride = sizeof(BasicEffectVertex);
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX wvp = worldMatrix * m_Camera->ViewMatrix() * m_Camera->ProjectionMatrix();
		m_WvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

		m_Pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->Draw(3, 0);
	}
}