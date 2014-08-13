//
//  Created by Takashi Aoki (c) takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#include "aTemp.h"

/*

EventManager

*/

ofxEventManager::Manager* ofxEventManager::Manager::instance = NULL;

void ofDispatchEvent(ofxEventManager::Event& event)
{	
	ofxEventManager::Manager::getInstance()->dispatchEvent(event);
};