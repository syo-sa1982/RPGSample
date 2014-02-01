#include "HelloWorldScene.h"
#include "BattleScene.h"

USING_NS_CC;

enum {
    kTagTileMap = 1,
    kTagPlayer  = 2,
};

CCScene* HelloWorld::scene(int hoge)
{
    std::cout << "引数：" << hoge << std::endl;

    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    // バーチャルパッド
    _virtualPad = new VirtualPad(this);

    // タイルマップ呼び出し
    _tileMap = CCTMXTiledMap::create("th_cobit_rouka_1f.tmx");
    this->addChild(_tileMap, 0, kTagTileMap);

    CCSize CC_UNUSED tileSize = _tileMap->getContentSize();
    CCLOG("ContentSize: %f, %f", tileSize.width,tileSize.height);

    CCLOG("ContentSize: %f, %f", this->getContentSize().width,this->getContentSize().height);

    std::cout << "width: " << CCDirector::sharedDirector()->getWinSize().width << std::endl;
    std::cout << "height: " << CCDirector::sharedDirector()->getWinSize().height << std::endl;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();



    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Objects");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }

    // tmx上のSpawnPointの座標を取得
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");

    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();

    // テクスチャアトラスからのキャラ生成
    _frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    _frameCache->addSpriteFramesWithFile("character.plist");


    _player = CCSprite::createWithSpriteFrameName("male_walkcycle_e_01.png");

    // プレイヤーをタグ識別
//    _player->setTag(kTagPlayer);
    _player->retain();


    CCLog("playerPos x：%d", x);
    CCLog("playerPos y：%d", y);

    // スプライトに座標セット
    _player->setPosition(ccp(x,y));

//    _player->setPosition(ccp(0,0));
    _player->setScale(1);
    _player->setAnchorPoint(ccp(0.5,0));

    _tileMap->addChild(_player, 1, kTagPlayer);


    // メタレイヤー
    _meta = _tileMap->layerNamed("Meta");
    // プレイヤーの目からは見えなくする
    _meta->setVisible(false);

    // 画面の表示座標をセット
    this->setViewPlayerCenter();
    this->schedule(schedule_selector(HelloWorld::setViewPlayerCenter));


    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();



    int zOrderplay = _player->getZOrder();

    CCLog("playerPos重なり：%d", zOrderplay);
    CCLog("playerPos x：%d", playerPos.x);
    CCLog("playerPos y：%d", playerPos.y);


    // マルチタッチ
    this->setTouchMode(kCCTouchesAllAtOnce);
    // タッチを有効化
    this->setTouchEnabled(true);

    return true;
}

HelloWorld::~HelloWorld()
{
    _player->release();
    delete _virtualPad;
}

// プレイヤーの位置をセンターに
void HelloWorld::setViewPlayerCenter()
{
    //    CCLog("setViewPlayerCenter");
    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    int x = MAX(playerPos.x, winSize.width/2);
    int y = MAX(playerPos.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);

    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);

//    CCLog("_tileMap x：%d", _tileMap->getPositionX());
//    CCLog("_tileMap y：%d", _tileMap->getPositionY());
//    CCLog("viewPoint x：%d", viewPoint.x);
//    CCLog("viewPoint y：%d", viewPoint.y);
    _tileMap->setPosition(viewPoint);
}

//void HelloWorld::setPlayerPosition(CCPoint position)
void HelloWorld::setPlayerPosition()
{
    int way = -1,d_x = 0,d_y = 0;
    if(_virtualPad->getDrawFlag() == true){
        // 方向取得
        way = _virtualPad->get4Way();
        CCLOG("VirtualPad get4Way is : %d", way);

        switch (way) {
            // 真ん中
            case padDirection::kCenter :
                break;
                // 上
            case padDirection::kUp :
                d_y = 1;
                break;
                // 下
            case padDirection::kDown :
                d_y = -1;
                break;
                // 左
            case padDirection::kLeft :
                d_x = -1;
                break;
                // 右
            case padDirection::kRight :
                d_x = 1;
                break;
        }

        _player->setPosition(ccp(_player->getPositionX() + d_x, _player->getPositionY() + d_y));
    }

    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

    CCLog("playerPos x：%d", playerPos.x);
    CCLog("playerPos y：%d", playerPos.y);
    CCLog("_player x：%d", (int)_player->getPosition().x);
    CCLog("_player y：%d", (int)_player->getPosition().y);

    if ( abs(d_x) > abs(d_y) ) {
        if (d_x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (d_y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }

    CCLog("playerPos2 x：%d", playerPos.x);
    CCLog("playerPos2 y：%d", playerPos.y);


//    CCPoint tileCoord = this->tileCoordForPosition(position);
//    CCPoint frompos = _player->getPosition();

//    int tileGid = _meta->tileGIDAt(tileCoord);
//
//    // 衝突判定
//    if (tileGid) {
//        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
//        if (properties) {
//            CCString *collision = new CCString();
//            *collision = *properties->valueForKey("Collidable");
//            if (collision && (collision->compare("true") == 0)) {
//                this->playHeroMoveAnimationFromPosition(frompos, position);
//                return;
//            }
//        }
//    }
//    this->playHeroMoveAnimationFromPosition(frompos, position);
//    CCMoveTo* moveTo = CCMoveTo::create(0.1f, position);
//    _player->runAction(moveTo);
//
//    std::cout << "10D10===========================" << std::endl;
//    dice.roll(10, 10);
//    int result = dice.getRollResult();
//    dice.reset();
//    std::cout << "合計値 : " << result << std::endl;
//
//    if (this->isEncountered(result)) {
//        std::cout << "敵が現れた！！" << std::endl;
////        CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create(3.0f,BattleScene::createScene()) );
////        CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create(3.0f,HelloWorld::scene(2)) );
//    }

}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    // タップ座標を取得
    CCPoint tilePoint = ccpSub(position, _tileMap->getPosition());

    // タイルの幅
    float fTileWidth = _tileMap->getTileSize().width;
    // タイルの高さ
    float fTileHeight = _tileMap->getTileSize().height;
    // タイルの行数
    float fTileRows = _tileMap->getMapSize().height;
    // タップ座標をタイル座標に変換
    tilePoint.x = (int)(tilePoint.x / fTileWidth);
    tilePoint.y = (int)((fTileRows * fTileHeight - tilePoint.y) / fTileHeight);

    return tilePoint;
}

//void HelloWorld::registerWithTouchDispatcher()
//{
//    // シングルタッチ
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}

// マルチタッチ：開始
void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event)
{
    CCLog("ccTouchesBegan!");
    for (CCSetIterator it = touches->begin(); it != touches->end(); ++it)
    {
        CCTouch *pTouch  = (CCTouch *)(*it);



        CCLog("pTouch x：%d", (int)pTouch->getLocation().x);
        CCLog("pTouch y：%d", (int)pTouch->getLocation().y);
        CCLog("_player x：%d", (int)_player->getPosition().x);
        CCLog("_player y：%d", (int)_player->getPosition().y);

        //_virtualPad
        _virtualPad->startPad((int)pTouch->getLocation().x,(int)pTouch->getLocation().y,pTouch->getID());
    }

}

// マルチタッチ：移動
void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLog("ccTouchesMoved!");
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *pTouch  = (CCTouch *)(*it);
        //_virtualPad
        _virtualPad->update((int)pTouch->getLocation().x,(int)pTouch->getLocation().y,pTouch->getID());
    }
    // プレイヤーをこの関数で移動
    this->schedule(schedule_selector(HelloWorld::setPlayerPosition));
}

// マルチタッチ：キャンセル
void HelloWorld::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLog("ccTouchesCancelled!");
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *pTouch  = (CCTouch *)(*it);
        //_virtualPad
        _virtualPad->endPad(pTouch->getID());
    }
}

// マルチタッチ：終了
void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCLog("ccTouchesEnded!");
    for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); ++it)
    {
        CCTouch *pTouch  = (CCTouch *)(*it);
        //_virtualPad
        _virtualPad->endPad(pTouch->getID());
    }
}

// シングルタッチ開始
bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCLog("ccTouchBegan!");
    // タップした座標
    CCPoint touchLocation = touch->getLocationInView();

    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);

    // 目的地
    CCPoint destinationPos = this->getDestinationPos(touchLocation);

    // safety check on the bounds of the map
    if (destinationPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        destinationPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        destinationPos.y >= 0 &&
        destinationPos.x >= 0)
    {
//        this->setPlayerPosition(destinationPos);
    }

    CCLog("ccTouchBegan end!");
    return true;
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    CCLog("TouchEnd!");
}

CCPoint HelloWorld::getDestinationPos(CCPoint touchLocation)
{
    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();
    CCPoint diff = ccpSub(touchLocation, playerPos);

    if ( abs(diff.x) > abs(diff.y) ) {
        if (diff.x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
        } else {
            playerPos.x -= _tileMap->getTileSize().width;
        }
    } else {
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        } else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }

    return playerPos;
}

void HelloWorld::playHeroMoveAnimationFromPosition(CCPoint fromPosition, CCPoint toPosition)
{
    /*
    TODO:ここでキャラの向きを指定しているが、この向きを何処かで取得できないか？もしくはメンバ変数に持たせる？
     */
    char* direction = "n";
    if(toPosition.x > fromPosition.x){
        direction = "e";
    }else if(toPosition.x < fromPosition.x){
        direction = "w";
    }else if(toPosition.y < fromPosition.y){
        direction = "s";
    }
    std::cout <<  "This walkCycle" << std::endl;
    CCArray* frames = CCArray::createWithCapacity(9);
    for (int i = 0; i < 9; ++i) {

        CCString *walkCycle = CCString::createWithFormat("male_walkcycle_%s_%02d.png",direction, (i+1));

        frames->addObject(_frameCache->spriteFrameByName(walkCycle->getCString()));

        std::cout <<  walkCycle->getCString() << std::endl;
    }

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.05f);
    CCAnimate* animate = CCAnimate::create(animation);
    _player->runAction(animate);
}
