#pragma once
#include "stdafx.h"

using namespace vEngine;

namespace vEngine {
	class FramesPerSecond;
}

namespace Game {
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
		FramesPerSecond* m_FrameRateView;
	};
}