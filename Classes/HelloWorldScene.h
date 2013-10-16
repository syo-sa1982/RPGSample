#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    /**
     @brief 画面の視野をプレイヤーの周辺に設定
     */
    void setViewPlayerCenter();
    
    /**
     @brief キャラクタの位置をセット
     */
//    void setPlayerPosition(CCPoint position);
//    void registerWithTouchDispatcher();
    
private:
    CCTMXTiledMap *_tileMap;
    CCTMXLayer    *_wallFloor;
    CCTMXLayer    *_meta;
    CCSprite      *_player;
    
};

#endif // __HELLOWORLD_SCENE_H__
