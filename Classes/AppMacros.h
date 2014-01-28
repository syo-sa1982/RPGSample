//
//  AppMacros.h
//  EvilHouse
//
//  Created by 藤井 陽介 on 2014/01/18.
//
//

#ifndef EvilHouse_AppMacros____FILEEXTENSION___
#define EvilHouse_AppMacros____FILEEXTENSION___

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directry[100];
} Resource;

static Resource smallResource = {};

#endif
