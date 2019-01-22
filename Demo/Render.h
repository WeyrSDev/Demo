#pragma once
#include "Engine.h"

using namespace vEngine;

namespace Game {
	class RenderingGame : public Engine {
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
		virtual ~RenderingGame();
	public:
		virtual void Initialize();
		virtual void Update(const EngineTime& engineTime);
		virtual void Draw(const EngineTime& engineTime);
	private:
		static const XMVECTORF32 BackgroundColor;
	};
}