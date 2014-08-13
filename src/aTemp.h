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

	class Event
	{
	public:
		Event():param(0){};
		~Event(){};
		int param;
	};
    
	// ToDo
	template<class T>
	class CustomEvent : public Event
	{
	public:
		CustomEvent(){};
		~CustomEvent(){};
		T value;
	};

	class ListenerInterface
	{
	public:
	    virtual void send(ofxEventManager::Event& e) = 0;
	};

	template <class T>
	class ListenerObject : public ListenerInterface
	{
	public:
	    ListenerObject(T* obj, void (T::*cbf)(ofxEventManager::Event&))
	    {
	        _obj = obj;
	        _cbf = cbf;
	    };
	   	virtual void send(ofxEventManager::Event& e)
	    {
	        (_obj->*_cbf)(e);
	    };

	private:
	    T* _obj;
	    void (T::*_cbf)(ofxEventManager::Event&);
	};

	class Manager
	{
	private:
		typedef std::map<ofxEventManager::Event*, ListenerInterface*> Keymap;
	    static Manager* instance;
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
	    void addListener(ofxEventManager::Event& event, T* obj, void (T::*cbf)(ofxEventManager::Event&))
	    {
	        ListenerInterface* lo = new ListenerObject<T>(obj, cbf);
	    	_map.insert(std::pair<ofxEventManager::Event*, ListenerInterface*>(&event,lo));
	    };

	    template <class T>
	    void removeListener(ofxEventManager::Event& event, T* obj, void (T::*cbf)(ofxEventManager::Event&))
	    {
	        Keymap::iterator it = _map.find(&event);

  			if (it != _map.end()) 
	    	{
	    		_map.erase(it);
	    	}
	    	else std::cout << "removeListener @ ofxEventManager: Object not found." << std::endl;
	    };

	    void dispatchEvent(ofxEventManager::Event& event)
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
}

typedef ofxEventManager::Event EventObj; // this might be useful.

template <class T>
void ofAddEventManager(ofxEventManager::Event& event, T* obj, void (T::*cbf)(ofxEventManager::Event&))
{
	ofxEventManager::Manager::getInstance()->addListener(event, obj, cbf);
};
template <class T>
void ofRemoveEventManager(ofxEventManager::Event& event, T* obj, void (T::*cbf)(ofxEventManager::Event&))
{
	ofxEventManager::Manager::getInstance()->removeListener(event, obj, cbf);
};
void ofDispatchEvent(ofxEventManager::Event& event);