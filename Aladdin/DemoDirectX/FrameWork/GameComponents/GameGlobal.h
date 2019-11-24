#ifndef __GAME_GLOBAL__
#define __GAME_GLOBAL__
#include <Windows.h>
#include "../GameDefines/define.h"

class GameGlobal
{
public:
	static HINSTANCE GetCurrentHINSTANCE();
	static HWND getCurrentHWND();

	static void SetCurrentHINSTANCE(HINSTANCE hInstance);
	static void SetCurrentHWND(HWND hWnd);

	static void SetCurrentSpriteHandler(LP_SPRITE spriteHandler);
	static LP_SPRITE GetCurrentSpriteHandler();

	static void SetCurrentDevice(LP_3DDEVICE device);
	static LP_3DDEVICE GetCurrentDevice();

	static void SetWidth(int width);
	static int GetWidth();

	static void SetHeight(int height);
	static int GetHeight();

	static LP_3DSURFACE backSurface;

	static bool isGameRunning;
	
private:
	static HINSTANCE mHInstance;
	static HWND mHwnd;
	static LP_SPRITE mSpriteHandler;
	static int mWidth, mHeight;
	static LP_3DDEVICE mDevice;

};

#endif