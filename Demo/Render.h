#pragma once
#include "stdafx.h"

using namespace vEngine;

namespace vEngine {
	class Keyboard;
	class Mouse;
	class XBoxGamePad;
	class FramesPerSecond;
	class FirstPersonCamera;
	class RenderStateHelper;
}

namespace Game {
	class TriangleDemo;
	class RenderingGame : public Engine {
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
		virtual ~RenderingGame();
	public:
		virtual void Initialize();
		virtual void Update(const EngineTime& engineTime);
		virtual void Draw(const EngineTime& engineTime);
	protected:
		virtual void Shutdown() override;
	private:
		static const XMVECTORF32 BackgroundColor;
	private:
		LPDIRECTINPUT8 m_DirectInput;
		Keyboard* m_Keyboard;
		Mouse* m_Mouse;
		XBoxGamePad* m_XBoxPad;
		FramesPerSecond* m_FrameRateView;
		FirstPersonCamera* m_FPSCamera;
		TriangleDemo* m_Demo;
	};
}