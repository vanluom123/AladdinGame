#include "GameGlobal.h"

HINSTANCE GameGlobal::mHInstance = NULL;
HWND GameGlobal::mHwnd = NULL;
LP_SPRITE GameGlobal::mSpriteHandler = NULL;
int GameGlobal::mWidth = 320; 
int GameGlobal::mHeight = 224;
LP_3DDEVICE GameGlobal::mDevice = nullptr;
bool GameGlobal::isGameRunning = true;
IDirect3DSurface9* GameGlobal::backSurface = nullptr;


void GameGlobal::SetCurrentDevice(LP_3DDEVICE device)
{
	mDevice = device;
}

LP_3DDEVICE GameGlobal::GetCurrentDevice()
{
	return mDevice;
}

HINSTANCE GameGlobal::GetCurrentHINSTANCE()
{
	return mHInstance;
}

HWND GameGlobal::getCurrentHWND()
{
	return mHwnd;
}

void GameGlobal::SetCurrentHINSTANCE(HINSTANCE hInstance)
{
	mHInstance = hInstance;
}

void GameGlobal::SetCurrentHWND(HWND hWnd)
{
	mHwnd = hWnd;
}

void GameGlobal::SetCurrentSpriteHandler(LP_SPRITE spriteHandler)
{
	mSpriteHandler = spriteHandler;
}

LP_SPRITE GameGlobal::GetCurrentSpriteHandler()
{
	return mSpriteHandler;
}

void GameGlobal::SetWidth(int width)
{
	mWidth = width;
}

int GameGlobal::GetWidth()
{
	return mWidth;
}

void GameGlobal::SetHeight(int height)
{
	mHeight = height;
}

int GameGlobal::GetHeight()
{
	return mHeight;
}


