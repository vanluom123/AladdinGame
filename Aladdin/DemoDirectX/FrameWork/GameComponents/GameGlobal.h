#ifndef __GAME_GLOBAL__
#define __GAME_GLOBAL__
#include "../GameDefines/define.h"

class GameGlobal
{
public:
	static HINSTANCE GetCurrentHINSTANCE();
	static HWND getCurrentHWND();

	static void SetCurrentHINSTANCE(HINSTANCE hInstance);
	static void SetCurrentHWND(HWND hWnd);

	static void SetCurrentSpriteHandler(LPD3DXSPRITE spriteHandler);
	static LPD3DXSPRITE GetCurrentSpriteHandler();

	static void SetCurrentDevice(LPDIRECT3DDEVICE9 device);
	static LPDIRECT3DDEVICE9 GetCurrentDevice();

	static void SetWidth(int width);
	static int GetWidth();

	static void SetHeight(int height);
	static int GetHeight();

	static LPDIRECT3DSURFACE9 backSurface;

	static bool isGameRunning;
	
private:
	
	static HINSTANCE mHInstance;
	static HWND mHwnd;
	static LPD3DXSPRITE mSpriteHandler;
	static int mWidth, mHeight;
	static LPDIRECT3DDEVICE9 mDevice;

};

#endif