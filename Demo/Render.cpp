#include "stdafx.h"
#include "Render.h"

namespace Game {
	const XMVECTORF32 RenderingGame::BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand) :
		Engine(instance, windowClass, windowTitle, showCommand)
	{
		m_DepthStencilBufferEnabled = true;
		m_MultiSamplingEnabled = true;
	}
	RenderingGame::~RenderingGame()
	{
	}
	void RenderingGame::Initialize()
	{
		Engine::Initialize();
	}
	void RenderingGame::Update(const EngineTime& engineTime)
	{
		Engine::Update(engineTime);
	}
	void RenderingGame::Draw(const EngineTime& engineTime)
	{
		m_Direct3DDeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
		m_Direct3DDeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Engine::Draw(engineTime);

		HRESULT hr = m_SwapChain->Present(0, 0);

		if (FAILED(hr))
		{
			throw EngineException("ID3D11SwapChain::Present() failed...ShitBags", hr);
		}
	}
}