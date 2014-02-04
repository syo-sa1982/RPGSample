//
// Created by 藤井 陽介 on 2014/01/17.
//


#ifndef __BattleScene_H_
#define __BattleScene_H_

#include "cocos2d.h"

class BattleScene : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene *createScene();
    virtual bool init();
    CREATE_FUNC(BattleScene);
};


#endif //__BattleScene_H_
