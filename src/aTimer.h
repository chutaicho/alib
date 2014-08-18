//
//  Copyright (c) 2014 Takashi Aoki
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

#include "ofUtils.h"
#include "ofEvents.h"
#include "aTemp.h"

namespace at
{
    class Timer
    {
    public:
        Timer(){ _startTime = 0.f; _isStarted = false; id = 0; };
        ~Timer(){};

        ofEvent<ofEventArgs> TIMER_REACHED;
        ofxEventManager::Event completeEvent;
        
        void setInterval(float interval=1.f)
        {
            _interval = interval;
        };
        void update()
        {
            if(_isStarted == true)
            {
                float time = ofGetElapsedTimef() - _startTime;
                
                if(_interval > 0.0)
                {
                    if(time > _interval)
                    {
                        static ofEventArgs timerEventArgs;
                        ofNotifyEvent(TIMER_REACHED, timerEventArgs, this);
                        
                        //ofDispatchEvent(this->completeEvent); // test
                        start();
                    }
                }
            }
        };
        void start()
        {
            _startTime = ofGetElapsedTimef();
            _isStarted  = true;
        };
        void stop()
        {
            this->_isStarted = false;
        };
        
        //float getTime(){ return _time; };

        int id;
    private:
        bool  _isStarted;
        //bool  _isFinished;
        float _interval;
        float _startTime;
        //float _time;
    };
};