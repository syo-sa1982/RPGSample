#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
//using namespace std;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual ~HelloWorld();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    void repositionSprite(float dt);
    
    /**
     @brief 画面の視野をプレイヤーの周辺に設定
     */
    void setViewPlayerCenter();
    
    /**
     @brief キャラクタの位置をセット
     */
    void setPlayerPosition(CCPoint position);
    void registerWithTouchDispatcher();
    /**
     @brief タップ開始
     */
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    
    /**
     @brief タップ終了
     */
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
private:
    CCTMXTiledMap *_tileMap;
    CCTMXTiledMap *_tileMapTop;
    CCTMXLayer    *_wallFloor;
    CCTMXLayer    *_wallThrough;
    CCTMXLayer    *_meta;
    CCSprite      *_player;
    
    CCPoint getTilePosition(CCPoint point);
    
    
    /**
     @brief タップした箇所のタイル座標を取得
     */
    CCPoint tileCoordForPosition(CCPoint position);
    
    CCPoint getDestinationPos(CCPoint touchLocation);
    
};

#endif // __HELLOWORLD_SCENE_H__
