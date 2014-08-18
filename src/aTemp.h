//
//  Copyright (c) 2014 Takashi Aoki
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <stddef.h>

namespace at
{
	// classs templates....
};

/*

EventManager

*/

namespace ofxEventManager
{

#pragma mark -
#pragma mark Event Dispatcher

	class EventObj
	{
	public:
		EventObj():param(0){};
		~EventObj(){};
		int param;
	};
    
	// ToDo
	template<class T>
	class CustomEvent : public EventObj
	{
	public:
		CustomEvent(){};
		~CustomEvent(){};
		T value;
	};

	class ListenerInterface
	{
	public:
	    virtual void send(ofxEventManager::EventObj& e) = 0;
	};

	template <class T>
	class ListenerObject : public ListenerInterface
	{
	public:
	    ListenerObject(T* obj, void (T::*cbf)(ofxEventManager::EventObj&))
	    {
	        _obj = obj;
	        _cbf = cbf;
	    };
	   	virtual void send(ofxEventManager::EventObj& e)
	    {
	        (_obj->*_cbf)(e);
	    };

	private:
	    T* _obj;
	    void (T::*_cbf)(ofxEventManager::EventObj&);
	};

	class Manager
	{
	    static Manager* instance;

	private:
		typedef std::map<ofxEventManager::EventObj*, ListenerInterface*> Keymap;
		Manager(){};
        ~Manager(){};

	public:
		static Manager* getInstance()
		{
			if(instance == NULL)
			{
				instance = new Manager();
			}
			return instance;
		};
		static void deleteInstance()
		{
			if(instance != NULL)
			{
				delete instance;
			}
		};

	    template <class T>
	    void addListener(ofxEventManager::EventObj& event, T* obj, void (T::*cbf)(ofxEventManager::EventObj&))
	    {
	        ListenerInterface* lo = new ListenerObject<T>(obj, cbf);
	    	_map.insert(std::pair<ofxEventManager::EventObj*, ListenerInterface*>(&event,lo));
	    };

	    template <class T>
	    void removeListener(ofxEventManager::EventObj& event, T* obj, void (T::*cbf)(ofxEventManager::EventObj&))
	    {
	        Keymap::iterator it = _map.find(&event);

  			if (it != _map.end()) 
	    	{
	    		_map.erase(it);
	    	}
	    	else std::cout << "removeListener @ ofxEventManager: Object not found." << std::endl;
	    };

	    void dispatchEvent(ofxEventManager::EventObj& event)
	    {
	    	Keymap::iterator it = _map.find(&event);

	    	if (it != _map.end()) 
	    	{
	    		ListenerInterface* target = it->second;
	    		target->send(event);
	    	}
	    	else std::cout << "notifyEvent @ ofxEventManager: Event not found." << std::endl;
	    }
	private:
	    Keymap _map;
	};
};

typedef ofxEventManager::EventObj eEventObj; // this might be useful.

template <class T>
void ofAddEventManager(ofxEventManager::EventObj& event, T* obj, void (T::*cbf)(ofxEventManager::EventObj&))
{
	ofxEventManager::Manager::getInstance()->addListener(event, obj, cbf);
};
template <class T>
void ofRemoveEventManager(ofxEventManager::EventObj& event, T* obj, void (T::*cbf)(ofxEventManager::EventObj&))
{
	ofxEventManager::Manager::getInstance()->removeListener(event, obj, cbf);
};
void ofDispatchEvent(ofxEventManager::EventObj& event);