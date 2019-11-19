#include "GameTime.h"

GameTime* GameTime::mInstance = NULL;

GameTime::GameTime()
{
}

GameTime::~GameTime()
{
}

GameTime* GameTime::GetInstance()
{
    if (!mInstance)
        mInstance = new GameTime();

    return mInstance;
}

void GameTime::StartCounter()
{
    if (!QueryPerformanceFrequency(&this->mClockRate))
		return;

    QueryPerformanceCounter(&this->mStartTime);
    
}

float GameTime::GetCouter()
{
    QueryPerformanceCounter(&this->mEndTime);
	this->mDelta.QuadPart = this->mEndTime.QuadPart - this->mStartTime.QuadPart;
	return float(this->mDelta.QuadPart / this->mClockRate.QuadPart);
}
