#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "../GameDefines/define.h"

class Camera
{
public:
    Camera(int width, int height);
	~Camera();

    //center of camera
    void SetPosition(float x, float y);
    void SetPosition(GVector3 pos);

    int GetWidth();
    int GetHeight();

    GVector3 GetPosition();    
    RECT GetBound();

private:
	int mWidth;
	int mHeight;

    GVector3 mPosition;
};
#endif // __VIEWPORT_H__