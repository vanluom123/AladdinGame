#include "SpriteSheet.h"
#include "GameComponents/GameGlobal.h"

SpriteSheet::SpriteSheet(const char* filePath, D3DCOLOR color) :
	Position(this),
	PositionX(this),
	PositionY(this),
	Width(this),
	Height(this),
	Rect(this),
	Scale(this),
	ScaleX(this),
	ScaleY(this),
	FlipX(this),
	FlipY(this),
	Rotate(this),
	Translation(this),
	TranslationX(this),
	TranslationY(this)
{
	this->init(filePath, color);
}

SpriteSheet::~SpriteSheet()
{
	this->m_Texture->Release();
}

void SpriteSheet::init(const char* filePath, D3DCOLOR color)
{
	D3DXGetImageInfoFromFile(filePath, &this->m_ImageInfo);

	this->m_pos = GVec2ZERO;
	this->m_width = this->m_ImageInfo.Width;
	this->m_Height = this->m_ImageInfo.Height;
	this->m_srect = { 0, 0, this->m_width, this->m_Height };
	this->m_Scaling = GVector2(1.0f, 1.0f);
	this->m_IsFlipX = false;
	this->m_IsFlipY = false;
	this->m_Rotation = 0.0f;
	this->m_Translation = GVec2ZERO;
	this->m_Texture = NULL;
	this->m_ImageInfo = D3DXIMAGE_INFO();

	// LOAD TEXTURE
	D3DXCreateTextureFromFileEx(
		GameGlobal::GetCurrentDevice(),
		filePath,
		this->m_ImageInfo.Width,
		this->m_ImageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		color,
		&this->m_ImageInfo,
		nullptr,
		&this->m_Texture
	);
}

void SpriteSheet::render(D3DCOLOR color)
{
	GVector2 scalingCenter = GVector2(float((int)this->m_pos.x), float((int)this->m_pos.y));
	GVector2 rotationCenter = GVector2(float((int)this->m_pos.x), float((int)this->m_pos.y));
	GVector2 scaling = GVector2(this->m_Scaling.x, this->m_Scaling.y);
	GVector2 translation = GVector2(float((int)this->m_Translation.x), float((int)this->m_Translation.y));

	D3DXMatrixTransformation2D(
		&this->m_Matrix,
		&scalingCenter,
		0,
		&scaling,
		&rotationCenter,
		float(this->m_Rotation),
		&translation);
	D3DXMATRIX oldMatrix;

	GameGlobal::GetCurrentSpriteHandler()->GetTransform(&oldMatrix);
	GameGlobal::GetCurrentSpriteHandler()->SetTransform(&this->m_Matrix);

	RECT rectangle;
	rectangle.left = this->m_srect.left;
	rectangle.top = this->m_srect.top;
	rectangle.right = this->m_srect.right;
	rectangle.bottom = this->m_srect.bottom;

	GVector3 position = GVector3(float((int)this->m_pos.x), float((int)this->m_pos.y), 0.0f);
	GVector3 center = GVector3(float(this->m_width) / 2.0f, float(this->m_Height) / 2.0f, 0.0f);

	GameGlobal::GetCurrentSpriteHandler()->Draw(
		this->m_Texture,
		&rectangle,
		&center,
		&position,
		color);

	GameGlobal::GetCurrentSpriteHandler()->SetTransform(&oldMatrix);
}