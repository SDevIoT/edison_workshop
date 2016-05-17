
#ifndef _RADEG_H
#define _RADEG_H

#define M_PI 3.14159265 

float radians(float degrees1)
{
    float radians1 = degrees1*M_PI/180;
    return radians1;
}

float degrees(float radians1)
{
    float degrees1 = radians1*180/M_PI;
    return degrees1;
}

#endif
