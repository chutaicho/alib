//
//  aType.h
//
//  Created by Takashi Aoki on 11/13/14.
//  (c) takashiaoki.com
//
#pragma once

#include <iostream>

namespace at
{
#pragma mark -
#pragma mark point
    
    template<class T>
    class Vec2
    {
    public:
        Vec2(T _x=0.f,T _y=0.f):x(_x),y(_y){};
        void set(T _x, T _y)
        {
            x = _x; y = _y;
        };
        T x,y;
    };
    
    template<class T>
    class Vec3
    {
    public:
        Vec3(T _x=0.f,T _y=0.f,T _z=0.f):x(_x),y(_y),z(_z){};
        void set(T _x, T _y, T _z)
        {
            x = _x; y = _y; z = _z;
        };
        T x,y,z;
    };

    template<class T>
    class Rect
    {
    public:
        Rect():x(0.f),y(0.f),w(0.f),h(0.f){};
        void set(T _x, T _y, T _w, T _h)
        {
            x = _x; y = _y; w = _w; h = _h;
        };
        T x,y,w,h;
    };
};