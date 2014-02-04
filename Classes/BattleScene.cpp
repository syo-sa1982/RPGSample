//
// Created by 藤井 陽介 on 2014/01/17.
//

#include "BattleScene.h"

USING_NS_CC;
using namespace std;

CCScene *BattleScene::createScene()
{
    CCScene *scene = CCScene::create();
    BattleScene *layer = BattleScene::create();
    scene->addChild(layer);
    return scene;

}

bool BattleScene::init()
{
    if (! CCLayer::init()) {
        return false;
    }
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    this->addChild(pSprite);

    return true;
}