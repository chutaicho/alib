//
//  Created by Takashi Aoki on 04/06/2014.
//  (c) takashiaoki.com
//
#pragma once

#include <iostream>
#include "ofMain.h"
#include "aType.h"

class ofVec2fEventArgs : public ofEventArgs 
{
public:
	float param1;
	float param2;
};

class TrackPad
{
public:
    static const int POINTER_RADIUS = 64;
    
    TrackPad(int w=100, int h=100) : x(0),y(0),width(w),height(h),_mx(0),_my(0),_onmouse(false),font(NULL)
    {
        
    };
    ~TrackPad(){};
    
    void setup()
    {
        _mx = x + (width >> 1); _my = y + (height >> 1);
        _param1 = _param2 = 0.f;
    };
    void enable()
    {
        ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
    };
    void disable()
    {
        ofUnregisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
    };
    void draw()
    {
        ofSetLineWidth(1);
        
        ofFill();
        ofSetColor(ofColor::black);
        ofRect(x,y,width,height);
        
        ofSetColor(ofColor::white);
        ofNoFill();
        ofLine(_mx, y, _mx, y + height);
        ofLine(x, _my, x + width, _my);
        
        if(_onmouse)
        {
            ofFill();
            
            // info
           if(font != NULL)
           {
               font->drawString("PARAM1: "+ofToString(_param1), x + 10, y + 40);
               font->drawString("PARAM2: "+ofToString(_param2), x + 10, y + 80);
           }
        }
        else ofNoFill();
        
        ofCircle(_mx, _my, TrackPad::POINTER_RADIUS);
        
        ofNoFill();
        ofSetColor(ofColor::white);
        ofRect(x,y,width,height);
        
        ofFill();
    };
	virtual void mouseMoved(ofMouseEventArgs & args)
    {

    };
    virtual void mouseDragged(ofMouseEventArgs & args)
    {
        if(_onmouse)
        {
            if(x < args.x && (x + width) > args.x && y < args.y && (y + height) > args.y)
            {
                _mx = args.x; _my = args.y;
                
                at::Vec2<int> min;
                at::Vec2<int> max;
                min.x = x + TrackPad::POINTER_RADIUS;
                min.y = y + TrackPad::POINTER_RADIUS;
                max.x = x + width - TrackPad::POINTER_RADIUS;
                max.y = y + height - TrackPad::POINTER_RADIUS;
                
                _mx = (_mx < min.x)? min.x : (_mx > max.x)? max.x : _mx;
                _my = (_my < min.y)? min.y : (_my > max.y)? max.y : _my;
                
                _param1 = ofMap(_mx, x, x + width,  0.f, 1.f);
                _param2 = ofMap(_my, y, y + height, 0.f, 1.f);
                
                ofVec2fEventArgs e;
                e.param1 = _param1; e.param2 = _param2;
                ofNotifyEvent(controllEvent, e);
            }
        }        
    };
    virtual void mousePressed(ofMouseEventArgs & args)
    {
        if(x < args.x && (x + width) > args.x && y < args.y && (y + height) > args.y)
        {
            _mx = args.x; _my = args.y;
            _onmouse = true;
        }
    };
    virtual void mouseReleased(ofMouseEventArgs & args)
    {
        _onmouse = false;
    };
    
    int x, y, width, height;
    
    ofEvent<ofVec2fEventArgs> controllEvent;
    ofTrueTypeFont* font;
    
private:
    at::Vec2<int> _range;
    float _param1, _param2;
    int  _mx,_my;
    bool _onmouse;
};

class AppButton
{
public:
	AppButton(int _id=0, float rad=50.f):id(_id),x(0.f),y(0.f),radius(rad),
    _image(NULL),w(0),h(0),cx(0),cy(0),_hover(false),active(false),_col(ofColor::white),_parent(NULL)
    {
        //std::cout << "AppButton created." << std::endl;
    };
	~AppButton(){};
    
    void setImage(ofTexture* image)
    {
        _image = image;
        
        w = (int)_image->getWidth();
        h = (int)_image->getHeight();
        cx = w >> 1;
        cy = h >> 1;
    };
	void setup()
    {
        //ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
    };
    void enable()
    {
        ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
    };
    void disable()
    {
        ofUnregisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
    };
    void setParent(at::ViewObject* parent)
    {
        _parent = parent;
    };
	void update()
    {
        
    };
	void draw()
    {
        glPushMatrix();
        {
            ofTranslate(x, y);
            ofSetColor(_col);
            
            if(_image != NULL) _image->draw(cx*-1, cy*-1);
            else ofCircle(0, 0, radius);
        }
        glPopMatrix();
    };
    
	virtual void mouseMoved(ofMouseEventArgs & args){};
	virtual void mouseDragged(ofMouseEventArgs & args){};
    virtual void mousePressed(ofMouseEventArgs & args)
    {
        bool isHover = false;
        
        if(_image != NULL)
        {
            if((x - cx) < args.x && (x + cx) > args.x && (y - cy) < args.y && (y + cy) > args.y) isHover = true;
        }
        else
        {
            if(ofDist(x, y, args.x, args.y) < radius) isHover = true;
        }
        
        if(isHover)
        {
            _col = ofColor::gray;
            _hover = true;
        }
    };
    virtual void mouseReleased(ofMouseEventArgs & args)
    {
        bool isHover = false;
        
        if(_hover)
        {
            if(_image != NULL)
            {
                if((x - cx) < args.x && (x + cx) > args.x && (y - cy) < args.y && (y + cy) > args.y) isHover = true;
            }
            else
            {
                if(ofDist(x, y, args.x, args.y) < radius) isHover = true;
            }
            
            if(isHover)
            {
                ofEntryEventArgs e;
                e.state = id;
                ofNotifyEvent(triggerEvent,e);
            }
        }
        
        _col = ofColor::white;
        _hover = false;
    };
    
    ofEvent<ofEntryEventArgs> triggerEvent;
    
    int id;
    float x, y, radius;
    int w, h, cx, cy;
    bool active;
    
protected:
    at::ViewObject* _parent;
    ofTexture* _image;
    bool _hover;
    ofColor _col;
};

class RectButton : public AppButton
{
public:
    static const int ROUND_RADIUS = 8;
    
	RectButton(int width=400, int height=240, ofColor bgOFF=ofColor::blue, ofColor bgON=ofColor::cyan, ofColor fontOFF=ofColor::black, ofColor fontON=ofColor::black):_active(false),font(NULL),label("")
    {
        w = width; h = height; cx = w >> 1; cy = h >> 1;
        
        _bgOFF = bgOFF; _bgON = bgON; _col = _bgOFF;
        _fontOFF = fontOFF; _fontON = fontON; _fontRGB = _fontOFF;
        
        roundRadius = ROUND_RADIUS;
    };
	~RectButton(){};
    
	void draw()
    {
        glPushMatrix();
        {
            ofTranslate((int)x, (int)y);
            
            ofSetColor(_col);
            
            if(roundRadius > 0) ofRectRounded(cx*-1, cy*-1, w, h, RectButton::ROUND_RADIUS);
            else ofRect(cx*-1, cy*-1, w, h);
            
            if(font != NULL)
            {
                ofSetColor(_fontRGB);
                ofRectangle fontrect = font->getStringBoundingBox(label, 0, 0);
                font->drawString(label, (int)(fontrect.width*-.5), (int)(fontrect.height*.5));
            }
            
            ofSetColor(255, 255, 255, 255);
            
        }
        glPopMatrix();
    };
    void on()
    {
        _hover = true;
        _active = true;
        _col = _bgON;
        _fontRGB = _fontON;
    };
    void off()
    {
        _hover = false;
        _active = false;
        _col = _bgOFF;
        _fontRGB = _fontOFF;
    };
    
	virtual void mouseMoved(ofMouseEventArgs & args){};
	virtual void mouseDragged(ofMouseEventArgs & args){};
    virtual void mousePressed(ofMouseEventArgs & args)
    {
        float fx = (_parent == NULL)? (x - cx) : (_parent->x + x) - cx;
        float tx = (_parent == NULL)? (x + cx) : (_parent->x + x) + cx;
        float fy = (_parent == NULL)? (y - cy) : (_parent->y + y) - cy;
        float ty = (_parent == NULL)? (y + cy) : (_parent->y + y) + cy;
        
        //if(!_active && (x - cx) < args.x && (x + cx) > args.x && (y - cy) < args.y && (y + cy) > args.y)
        if(!_active && fx < args.x && tx > args.x && fy < args.y && ty > args.y)
        {
            _hover = true;
            _col = _bgON;
            _fontRGB = _fontON;
            //std::cout << "press." << std::endl;
        }
    };
    virtual void mouseReleased(ofMouseEventArgs & args)
    {
        float fx = (_parent == NULL)? (x - cx) : (_parent->x + x) - cx;
        float tx = (_parent == NULL)? (x + cx) : (_parent->x + x) + cx;
        float fy = (_parent == NULL)? (y - cy) : (_parent->y + y) - cy;
        float ty = (_parent == NULL)? (y + cy) : (_parent->y + y) + cy;
        
        if(!_active && _hover)
        {
            //if((x - cx) < args.x && (x + cx) > args.x && (y - cy) < args.y && (y + cy) > args.y)
            if(fx < args.x && tx > args.x && fy < args.y && ty > args.y)
            {
                _active = true;
                
                ofEntryEventArgs e;
                e.state = id;
                ofNotifyEvent(triggerEvent,e);
            }
            else
            {
                _active = false;
                _col = _bgOFF;
                _fontRGB = _fontOFF;
                //std::cout << "lost." << std::endl;
            }
        }
        
        _hover = false;
    };
    
    ofTrueTypeFont* font;
    std::string label;
    int  roundRadius;
    
protected:
    bool _active;
    
    ofColor _bgOFF;
    ofColor _bgON;
    ofColor _fontRGB;
    ofColor _fontON;
    ofColor _fontOFF;
};