//
//  Copyright (c) 2014 Takashi Aoki
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//
//  Usage:
//	float t = ofGetElapsedTimef();
//  float d = ofMap(t, 0, DURATION(sec), 0.f, 1.f);
//	_py = at::easeInOutSine(d) * TARGET_POSITION(float);
//

#pragma once
#include <cmath>

namespace at
{
	//! Easing equation for a simple linear tweening with no easing.
	inline float easeNone( float t )
	{
		return t;
	};

#pragma mark -
#pragma mark Sine

	inline float easeInSine(float t )
	{
		return -1.0 * cos(t*(M_PI*.5)) + 1.0;
	};
	inline float easeOutSine(float t)
	{
		return sin(t*M_PI*.5);
	};
	inline float easeInOutSine(float t)
	{
		return -0.5 * (cos(M_PI*t) - 1.0);
	};

#pragma mark -
#pragma mark Cubic

	inline float easeInCubic( float t )
	{
		return t*t*t;
	};
	inline float easeOutCubic( float t )
	{
		t -= 1;
		return t*t*t + 1;
	};
	inline float easeInOutCubic( float t )
	{
		t *= 2;
		if( t < 1 )
			return 0.5f * t*t*t;
		t -= 2;
		return 0.5f*(t*t*t + 2);
	};

#pragma mark -
#pragma mark Quint

	inline float easeInQuint( float t )
	{
		return t*t*t*t*t;
	};
	inline float easeOutQuint( float t )
	{
		t -= 1;
		return t*t*t*t*t + 1;
	};
	inline float easeInOutQuint( float t )
	{
		t *= 2;
		if( t < 1 ) return 0.5f*t*t*t*t*t;
		else {
			t -= 2;
			return 0.5f*(t*t*t*t*t + 2);
		}
	};

#pragma mark -
#pragma mark Quadratic

	inline float easeInQuad(float t)
	{
		return t*t;
	};
	inline float easeOutQuad(float t )
	{
		return -t * ( t - 2 );
	};
	inline float easeInOutQuad(float t )
	{
		t *= 2;
		if( t < 1 ) return 0.5 * t * t;
		
		t -= 1;
		return -0.5 * ((t)*(t-2) - 1);
	};
	
#pragma mark -
#pragma mark Quartic

	inline float easeInQuart(float t )
	{
		return t*t*t*t;
	};
	inline float easeOutQuart(float t )
	{
		t -= 1;
		return -(t*t*t*t - 1);
	};
	inline float easeInOutQuart(float t )
	{
		t *= 2;
		if( t < 1 ) return 0.5*t*t*t*t;
		else {
			t -= 2;
			return -0.5 * (t*t*t*t - 2);
		}
	};

#pragma mark -
#pragma mark Back
	
	inline float easeInBack(float t, float s=1.70158 )
	{
		return t * t * ((s+1)*t - s);
	};
	inline float easeOutBack(float t, float s=1.70158)
	{
		t -= 1;
		return (t*t*((s+1)*t + s) + 1);
	};
	inline float easeInOutBack(float t,float s=1.70158 )
	{
		t *= 2;
		if( t < 1 ) {
			s *= 1.525;
			return 0.5*(t*t*((s+1)*t - s));
		}
		else {
			t -= 2;
			s *= 1.525;
			return 0.5*(t*t*((s+1)*t+ s) + 2);
		}
	};
}