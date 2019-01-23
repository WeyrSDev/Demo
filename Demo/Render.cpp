#include "stdafx.h"
#include "Render.h"

namespace Game {
	const XMVECTORF32 RenderingGame::BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand) :
		Engine(instance, windowClass, windowTitle, showCommand), m_FrameRateView(nullptr), m_DirectInput(nullptr), m_Keyboard(nullptr), m_Mouse(nullptr)
	{
		m_DepthStencilBufferEnabled = true;
		m_MultiSamplingEnabled = true;
	}
	RenderingGame::~RenderingGame()
	{
		
	}
	void RenderingGame::Initialize()
	{
		if (FAILED(DirectInput8Create(m_Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_DirectInput, nullptr)))
		{
			throw EngineException("DirectInput8Create() failed");
	
		}

		m_Keyboard = new Keyboard(*this, m_DirectInput);
		m_EngineComponents.push_back(m_Keyboard);
		m_Services.AddService(Keyboard::TypeIdClass(), m_Keyboard);

		m_Mouse = new Mouse(*this, m_DirectInput);
		m_EngineComponents.push_back(m_Mouse);
		m_Services.AddService(Mouse::TypeIdClass(), m_Mouse);

//#if defined(DEBUG) || defined(_DEBUG)
		m_FrameRateView = new FramesPerSecond(*this);
		m_EngineComponents.push_back(m_FrameRateView);
//#endif
		Engine::Initialize();
	}
	void RenderingGame::Update(const EngineTime& engineTime)
	{
		if (m_Keyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}

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
	void RenderingGame::Shutdown()
	{
		DeleteObject(m_FrameRateView);
		DeleteObject(m_Keyboard);
		DeleteObject(m_Mouse);
		ReleaseObject(m_DirectInput);
		Engine::Shutdown();
	}
}