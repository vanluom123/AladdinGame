#ifndef __TEXT_H__
#define __TEXT_H__

#include <d3dx9core.h>
#include <string>
#include "GameGlobal.h"

class Text
{
private:
	LP_SPRITE spriteHandler; //SpriteHandler ho tro ve hinh

	std::wstring string;
	LPCWSTR fontName;
	int width, height, weight;
	bool isItalic;
	DWORD alignment;//DT_CENTER, DT_LEFT, DT_RIGHT, DT_WORDBREAK, DT_EXPANDTABS, DT_NOCLIP
	D3DCOLOR color;

	LP_FONT font;
public:
	Text();
	Text(std::wstring Str, int Width, int Height, int Weight, bool IsItalic, DWORD Alignment, D3DCOLOR Color, LPCWSTR FontName);
	~Text();

	void Draw(GVector3 position);

	std::wstring GetString();
	void SetString(std::wstring value);
	void SetString(int value);
};

#endif // __TEXT_H__
