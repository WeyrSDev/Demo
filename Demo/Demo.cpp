// Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo.h"
#include "Render.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace vEngine;
using namespace Game;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::unique_ptr<RenderingGame> game(new RenderingGame(instance, L"RenderingClass", L"Rendering", showCommand));

	try
	{
		game->Run();
	}
	catch (EngineException ex)
	{
		MessageBox(game->WindowHandle(), ex.WString().c_str(), game->WindowTitle().c_str(), MB_ABORTRETRYIGNORE);
	}

	return 0;
}