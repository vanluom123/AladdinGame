#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>
using namespace std;

#pragma warning(disable : 26812)


#define GVector2 D3DXVECTOR2
#define GVector3 D3DXVECTOR3
#define LP_3DTEXTURE LPDIRECT3DTEXTURE9
#define LP_SPRITE LPD3DXSPRITE
#define IMAGE_INFO D3DXIMAGE_INFO
#define MATRIX D3DXMATRIX
#define LP_3DDEVICE LPDIRECT3DDEVICE9

#define GVec2ZERO GVector2(0.0f, 0.0f)
#define GVec3ZERO GVector3(0.0f, 0.0f, 0.0f)


// Color defines
// ARGB numbers range from 0 through 255
// a = Alpha channel (transparency where 255 is opaque)
// r = Red, g = Green, b = Blue
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

namespace graphicsNS
{
	// Some common colors
	// ARGB numbers range from 0 through 255
	// A = Alpha channel (transparency where 255 is opaque)
	// R = Red, G = Green, B = Blue
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game
	const COLOR_ARGB TRANSCOLOR = MAGENTA;                      // transparent color

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

enum eDirection
{
	NONE = 1 << 0,
	LEFT = 1 << 1,
	TOP = 1 << 2,
	RIGHT = 1 << 3,
	BOTTOM = 1 << 4,
	ALL = (LEFT | TOP | RIGHT | BOTTOM),
};


enum eID
{
	UNKNOWN,
	PLAYER,
	BRICK,
	BRICK_GOLD_NORMAL,
	BRICK_NORMAL,
	WALL
};

namespace Define
{
	const float PLAYER_MAX_JUMP_VELOCITY = 480.0f; //van toc nhay lon nhat
	const float PLAYER_MIN_JUMP_VELOCITY = -480.0f; //van toc nhay thap nhat
	const float PLAYER_MAX_RUNNING_SPEED = 450.0f; //toc do chay nhanh nhat cua player
	const float PLAYER_BOTTOM_RANGE_FALLING = 6.0f; // do dai va cham voi bottom neu nhu va cham bottom nho hon thi player se bi roi xuong
}
