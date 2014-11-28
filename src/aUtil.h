//
//  Copyright (c) 2014 Takashi Aoki
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "ofMain.h"

#define ALIB_VERSION 2.0

namespace at
{
#pragma mark -
#pragma mark Math
    
    inline void randomSeed()
    {
        srand((unsigned int)time(NULL));
    };
    inline int getRandom(float max)
    {
        return (int)(max * rand() / (RAND_MAX + 1.0f));
    };
    inline float getRandomf(float min, float max)
    {
        return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
    };
    inline int getRandomEX(int max, int exception)
    {
        int res = getRandom(max);
        return (res == exception)? getRandomEX(max,exception) : res;
    };
    inline int fastabs(int a)
    {
        int mask = a >> 31;
        return (a ^ mask) - mask;
    };
    inline int fastmax(int a, int b)
    {
        int t = (a-b);
        return a - (t & (t >> 31));
    };
    inline int fastmin(int a, int b)
    {
        int t = (a-b);
        return b + (t & (t >> 31));
    };
    inline float fastmap(float value, float inputMin, float inputMax, float outputMin, float outputMax) 
    {
        return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
    };
    inline float getRadian(float y2, float y1, float x2, float x1)
    {
        return atan2(y2-y1, x2-x1);
    };
    inline float getDegree(float y2, float y1, float x2, float x1)
    {
        return atan2(y2-y1, x2-x1) * (180.0/M_PI);
    };
    // ToDo: ceil to nearest multiple value of numDevices
//    inline int getNearestM(int value, int m)
//    {
//        return (value + m - 1) / m * m;
//    };
	
#pragma mark -
#pragma mark string utilities
	
	inline std::string intToString(int num)
	{
		std::ostringstream ss;
		ss << num;
		return ss.str();
	};
	inline int stringToInt(std::string str)
	{
		int num;
		std::istringstream iss(str);
		iss >> num;
		return num;
	};
    inline int getNumWords(std::string str)
    {
        int word_count(0);
        std::stringstream ss(str);
        std::string word;
        while(ss >> word) ++word_count;
        return word_count;
    };
    template<class T>
    inline std::string vecToString(const std::vector<T>& values)
    {
        std::stringstream out;
        int n = values.size();
        out << "[";
        if(n > 0)
        {
            for(int i = 0; i < n - 1; i++) out << values[i] << ",";
            out << values[n - 1];
        }
        out << "]";
        return out.str();
    };
    inline std::string getFileExtention(std::string& str)
    {
        return str.substr(str.find_last_of("."));
    };

#pragma mark -
#pragma mark Color

    template<class T>
    class RGB
    {
        template <class> friend class RGB;
    public:
        RGB(T _r=0,T _g=0,T _b=0):r(_r),g(_g),b(_b){};
        void set(T _r, T _g, T _b)
        {
            r = _r; g= _g; b = _b;
        };
        T r,g,b;
    };

    // RGB value 0.0 - 1.0
    class RGBf
    {
    public:
        RGBf():r(0.f),g(0.f),b(0.f){};
        void set(float _r, float _g, float _b)
        {
            r = _r; g= _g; b = _b;
        };
        float r,g,b;
    };
    // RGB value 0 - 255
    class RGBi
    {
    public:
        RGBi():r(0),g(0),b(0){};
        void set(int _r, int _g, int _b)
        {
            r = _r; g= _g; b = _b;
        };
        int r,g,b;
    };

    inline RGB<int> hexToRGBi(int hex)
    {
        RGB<int> res;  
        res.r = (hex >> 16) & 0xff;
        res.g = (hex >> 8) & 0xff;
        res.b = (hex >> 0) & 0xff;
        return res;
    };

    // 0.0 - 1.0
    inline int rgbToHex(const float r, const float g, const float b)
    {
    	return 0;
    };
    // 0 - 255
    inline int rgbToHex(const int r, const int g, const int b)
    {
    	return 0;
    }
    inline int rgbToHex(const RGBf rgb)
    {
    	return 0;
    }
    inline int rgbToHex(const RGBi rgb)
    {
    	return 0;
    }
    
    // convert color name to hex - e.g. #FFFFFF -> 0xFFFFFF
    inline int getHexFromColorName(std::string colorName)
    {
        return ofHexToInt(colorName.replace(0, 1, "0x"));
    };
    
#pragma mark -
#pragma mark Utilities - debug

    // oFLog with time
    template<class T>
    inline void log(ofLogLevel level, const T& value)
    {
        std::ostringstream ss;
        ss << value;
        ofLog(level,ofGetTimestampString("%H:%M:%S") + " - " + ss.str());
    };
	inline void trace(std::string str)
	{
#ifdef TARGET_OSX
		std::cout << str << std::endl;
#endif
	};
	
	inline void errorLog(std::string scope, std::string message)
	{
		std::cout << "ERROR @ " << scope << " : " << message << std::endl;
	};
	
	inline std::string floatToString(float f, int digits)
	{
		std::ostringstream oss;
		oss << std::setprecision(digits) << std::setiosflags(std::ios::fixed) << f;
		return oss.str();
	};
	
	inline void fpsOnWindow()
	{
		std::string str = "framerate is ";
		str += ofToString(ofGetFrameRate(), 2)+"fps"; 
		ofSetWindowTitle(str);
	};

	inline void drawFPS(float x = 0, float y = 0)
	{
		ofDrawBitmapString(ofToString(ofGetFrameRate()),x,y);
	};
	
	inline void prepareSetting(int fps=60,ofColor bgColor=ofColor(0x333333),std::string windowTitle=" ",ofPoint windowPosition=ofPoint(0,0),bool isVerticalSync=true)
	{
		ofSetVerticalSync(isVerticalSync);
		ofSetFrameRate(fps);
		ofBackground(bgColor);
		ofSetWindowTitle(windowTitle);
		ofSetWindowPosition(windowPosition.x,windowPosition.y);
	};
    
#pragma mark -
#pragma mark Utilities - date & time
    
    inline std::string getTodayAsString()
	{
        return ofToString(ofGetDay()) + "." + ofToString(ofGetMonth()) + "." + ofToString(ofGetYear());
	};
    inline std::string getTimeAsString()
	{
        return ofToString(ofGetHours()) + ":" + ofToString(ofGetMinutes()) + ":" + ofToString(ofGetSeconds());
	};
    inline std::string getTimeAndDate()
	{
        return getTimeAsString() + "/" + getTodayAsString();
	};
    
#pragma mark -
#pragma mark Utilities - image & texture
    
    // make ofTexture from filePath.
    inline ofTexture* get_ofTexture(std::string filePath, bool isRGBA=false)
	{
		int internalGlDataType = (isRGBA)? GL_RGBA : GL_RGB;
		
		ofImage* pngImage = new ofImage();
		pngImage->setUseTexture(false);
		pngImage->loadImage(filePath);
		
		ofTexture* texture = new ofTexture();
		texture->allocate(pngImage->width, pngImage->height, internalGlDataType);
		texture->loadData(pngImage->getPixels(), pngImage->width, pngImage->height, internalGlDataType);
		
		delete pngImage;
		
		return texture;
	}
    // make ofTexture from ofImage.
	inline ofTexture* get_ofTexture(ofImage* image)
	{
		int internalGlDataType = image->type;
		
		ofTexture* texture = new ofTexture();
		texture->allocate(image->width, image->height, internalGlDataType);
		texture->loadData(image->getPixels(), image->width, image->height, internalGlDataType);
		
		return texture;
	}
    
#ifdef TARGET_OSX
	
#pragma mark -
#pragma mark Utilities - CF main bundle
    
	inline std::string getMainBundle()
	{
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourcesURL  = CFBundleCopyResourcesDirectoryURL(mainBundle);
		
		CFStringRef executableURLString = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
		
		int  P_MAX = 1024;
		char path[P_MAX];
		
		// get C-String from CFStringRef
		if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, P_MAX))
		{
			errorLog("WDUtil","MainBundle is not found.");
		}
		
		CFRelease(resourcesURL);
		CFRelease(executableURLString);
		CFRelease(mainBundle);
		
		std::string res = (std::string)path + "/";
		
		return res;
	};

#endif
}