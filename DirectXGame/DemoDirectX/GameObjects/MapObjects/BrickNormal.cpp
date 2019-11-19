#include "BrickNormal.h"

BrickNormal::BrickNormal(GVector3 position)
{
	this->init(position);
}

BrickNormal::~BrickNormal()
{
}


const char * BrickNormal::FileName()
{
    return "normalbrick";
}

int BrickNormal::TotalFrame()
{
    return 1;
}

int BrickNormal::Row()
{
    return 1;
}

int BrickNormal::Column()
{
    return 1;
}

float BrickNormal::SecondPerFrame()
{
    return 1.0f;
}