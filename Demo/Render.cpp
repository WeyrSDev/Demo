#include "stdafx.h"
#include "Render.h"
#include "TriangleDemo.h"
#include "CubeDemo.h"
namespace Game {
	const XMVECTORF32 RenderingGame::BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand) :
		Engine(instance, windowClass, windowTitle, showCommand), m_FrameRateView(nullptr), m_Grid(nullptr), m_DirectInput(nullptr), m_Keyboard(nullptr),
		m_Mouse(nullptr), m_XBoxPad(nullptr), m_RenderStateHelper(nullptr), m_Demo(nullptr)
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

		m_XBoxPad = new XBoxGamePad(*this);
		m_EngineComponents.push_back(m_XBoxPad);
		m_Services.AddService(XBoxGamePad::TypeIdClass(), m_XBoxPad);

		m_FPSCamera = new FirstPersonCamera(*this);
		m_EngineComponents.push_back(m_FPSCamera);
		m_Services.AddService(Camera::TypeIdClass(), m_FPSCamera);
//#if defined(DEBUG) || defined(_DEBUG)
		m_FrameRateView = new FramesPerSecond(*this);
		m_EngineComponents.push_back(m_FrameRateView);
//#endif
		m_Grid = new Grid(*this, *m_FPSCamera);
		m_EngineComponents.push_back(m_Grid);

		m_Demo = new CubeDemo(*this, *m_FPSCamera);
		m_EngineComponents.push_back(m_Demo);

		m_RenderStateHelper = new RenderStateHelper(*this);

		Engine::Initialize();
		m_FPSCamera->SetPosition(0.0f, 0.0f, 10.0f);
	}
	void RenderingGame::Update(const EngineTime& engineTime)
	{
		m_FrameRateView->Update(engineTime);
		if (m_Keyboard->WasKeyPressedThisFrame(DIK_ESCAPE) || m_XBoxPad->WasButtonPressedThisFrame(PadButtons::Back))
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

		m_RenderStateHelper->SaveAll();
		m_FrameRateView->Draw(engineTime);
		m_RenderStateHelper->RestoreAll();

		HRESULT hr = m_SwapChain->Present(0, 0);

		if (FAILED(hr))
		{
			throw EngineException("ID3D11SwapChain::Present() failed...ShitBags", hr);
		}
	}
	void RenderingGame::Shutdown()
	{
		DeleteObject(m_Demo);
		DeleteObject(m_FrameRateView);
		DeleteObject(m_Keyboard);
		DeleteObject(m_Mouse);
		DeleteObject(m_XBoxPad);
		DeleteObject(m_FPSCamera);
		DeleteObject(m_Grid);
		ReleaseObject(m_DirectInput);
		Engine::Shutdown();
	}
}