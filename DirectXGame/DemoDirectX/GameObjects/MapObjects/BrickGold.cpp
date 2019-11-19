#include "BrickGold.h"

BrickGold::BrickGold(GVector3 position)
{
	this->init(position);
}

BrickGold::~BrickGold()
{
}

const char * BrickGold::FileName()
{
    return "goldbrick";
}

int BrickGold::TotalFrame()
{
    return 4;
}

int BrickGold::Row()
{
    return 1;
}

int BrickGold::Column()
{
    return 4;
}

float BrickGold::SecondPerFrame()
{
    return 0.25f;
}
