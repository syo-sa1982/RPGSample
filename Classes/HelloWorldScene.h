#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Dice.h"
#include "VirtualPad.h"

class HelloWorld : public cocos2d::CCLayer
{
    enum
    {
        kEncounterRate = 50, // エンカウント率
    };

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual ~HelloWorld();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int hoge = 1);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

//    void repositionSprite(float dt);

    // 画面の視野をプレイヤーの周辺に設定
    void setViewPlayerCenter();

    // キャラクタの位置をセット
    void setPlayerPosition();
//    void setPlayerPosition(cocos2d::CCPoint position);
//    void registerWithTouchDispatcher();

    // キャラクタアニメーション
    void playHeroMoveAnimationFromPosition(cocos2d::CCPoint fromPosition, cocos2d::CCPoint toPosition);

    virtual void ccTouchesBegan(cocos2d::CCSet *ptouches, cocos2d::CCEvent *pEvent);

    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

//    virtual void ccTouchesMoved(cocos2d::CCSet );

    // タップ開始
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

    // タップ終了
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

    // タップした箇所のタイル座標を取得
    cocos2d::CCPoint tileCoordForPosition(cocos2d::CCPoint position);

    cocos2d::CCPoint getDestinationPos(cocos2d::CCPoint touchLocation);

    inline bool isEncountered(int rollRsulte)
    {
        return kEncounterRate > rollRsulte ? true :false;
    }

private:
    cocos2d::CCTMXTiledMap      *_tileMap;
    cocos2d::CCTMXLayer         *_meta;
    cocos2d::CCSprite           *_player;
    cocos2d::CCSpriteFrameCache *_frameCache;

    Dice dice;
    VirtualPad *_virtualPad;
};

#endif // __HELLOWORLD_SCENE_H__
