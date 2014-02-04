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
    _player->setTag(2);
//    _player->retain();


    // スプライトに座標セット
    _player->setPosition(ccp(x,y));
    _player->setScale(1);
    _player->setAnchorPoint(ccp(0.5,0));

    _tileMap->addChild(_player, 1, kTagPlayer);
//    _tileMap->addChild(_player, 1, kTagPlayer);


    // メタレイヤー
    _meta = _tileMap->layerNamed("Meta");
    // プレイヤーの目からは見えなくする
//    _meta->setVisible(false);

    // 画面の表示座標をセット
    this->setViewPlayerCenter();

    // マルチタッチ
    this->setTouchMode(kCCTouchesAllAtOnce);
    // タッチを有効化
    this->setTouchEnabled(true);



    std::cout << "_player->getPosition x：" << _player->getPosition().x << std::endl;
    std::cout << "_player->getPosition y：" << _player->getPosition().y << std::endl;

    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

    std::cout << "playerPos x：" << playerPos.x << std::endl;
    std::cout << "playerPos y：" << playerPos.y << std::endl;
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

    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    int x = MAX(playerPos.x, winSize.width/2);
    int y = MAX(playerPos.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);

    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);

    std::cout << "LayerPos x：" << this->getPosition().x << std::endl;
    std::cout << "LayerPos y：" << this->getPosition().y << std::endl;

    std::cout << "_tileMap x：" << _tileMap->getPosition().x << std::endl;
    std::cout << "_tileMap y：" << _tileMap->getPosition().y << std::endl;

    std::cout << "viewPoint x：" << viewPoint.x << std::endl;
    std::cout << "viewPoint y：" << viewPoint.y << std::endl;

    _tileMap->setPosition(viewPoint);
//    this->setPosition(viewPoint);
    std::cout << "LayerPos x：" << this->getPosition().x << std::endl;
    std::cout << "LayerPos y：" << this->getPosition().y << std::endl;

    std::cout << "_tileMap x：" << _tileMap->getPosition().x << std::endl;
    std::cout << "_tileMap y：" << _tileMap->getPosition().y << std::endl;
}

//void HelloWorld::setPlayerPosition(CCPoint position)
void HelloWorld::setPlayerPosition()
{
    int way = -1,d_x = 0,d_y = 0;
    if(_virtualPad->getDrawFlag()){
        // 方向取得
        way = _virtualPad->get4Way();
        CCLOG("VirtualPad get4Way is : %d", way);

        switch (way) {
            // 真ん中
            case padDirection::kCenter : break;
            // 上
            case padDirection::kUp : d_y = 1; break;
            // 下
            case padDirection::kDown : d_y = -1; break;
            // 左
            case padDirection::kLeft : d_x = -1; break;
            // 右
            case padDirection::kRight : d_x = 1; break;
        }
    }

    CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

    std::cout << "playerPos x：" << playerPos.x << std::endl;
    std::cout << "playerPos y：" << playerPos.y << std::endl;

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

    std::cout << "playerPos2 x：" << playerPos.x << std::endl;
    std::cout << "playerPos2 y：" << playerPos.y << std::endl;

    CCPoint tileCoord = this->tileCoordForPosition(playerPos);

    std::cout << "tileCoord x：" << tileCoord.x << std::endl;
    std::cout << "tileCoord y：" << tileCoord.y << std::endl;
    CCPoint frompos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();


    std::cout << "_meta->getLayerSize().width：" << _meta->getLayerSize().width << std::endl;
    std::cout << "_meta->getLayerSize().height：" << _meta->getLayerSize().height << std::endl;

    int tileGid = _meta->tileGIDAt(tileCoord);

    // 衝突判定
    if (tileGid) {
        CCDictionary *properties = _tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *collision = new CCString();
            *collision = *properties->valueForKey("Collidable");
            if (collision && (collision->compare("true") == 0)) {
                this->playHeroMoveAnimationFromPosition(frompos, playerPos);
                return;
            }
        }
    }

    this->playHeroMoveAnimationFromPosition(frompos, playerPos);
    CCMoveTo* moveTo = CCMoveTo::create(0.2f, playerPos);
    _player->runAction(moveTo);
    this->setViewPlayerCenter();

    std::cout << "10D10===========================" << std::endl;
    dice.roll(10, 10);
    int result = dice.getRollResult();
    dice.reset();
    std::cout << "合計値 : " << result << std::endl;

    if (this->isEncountered(result)) {
        std::cout << "敵が現れた！！" << std::endl;
//        CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create(3.0f,BattleScene::createScene()) );
//        CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create(3.0f,HelloWorld::scene(2)) );
    }

}

CCPoint HelloWorld::tileCoordForPosition(CCPoint position)
{
    CCLOG("### tileCoordForPosition ###");
    std::cout << "_tileMap->getPosition() x：" << _tileMap->getPosition().x << std::endl;
    std::cout << "_tileMap->getPosition() y：" << _tileMap->getPosition().y << std::endl;

    std::cout << "position x：" << position.x << std::endl;
    std::cout << "position y：" << position.y << std::endl;
    // タップ座標を取得
    CCPoint tilePoint = position;
//    CCPoint tilePoint = ccpSub(position, _tileMap->getPosition());
    std::cout << "tilePoint x：" << tilePoint.x << std::endl;
    std::cout << "tilePoint y：" << tilePoint.y << std::endl;


    // タイルの幅
    float fTileWidth = _tileMap->getTileSize().width;
    // タイルの高さ
    float fTileHeight = _tileMap->getTileSize().height;
    // タイルの行数
    float fTileRows = _tileMap->getMapSize().height;


    // タップ座標をタイル座標に変換
    tilePoint.x = (int)(tilePoint.x / fTileWidth);
    tilePoint.y = (int)((fTileRows * fTileHeight - tilePoint.y) / fTileHeight);


    std::cout << "tilePoint x：" << tilePoint.x << std::endl;
    std::cout << "tilePoint y：" << tilePoint.y << std::endl;

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


        CCPoint playerPos = ((CCSprite *)_tileMap->getChildByTag(kTagPlayer))->getPosition();

        CCPoint tileCoord = this->tileCoordForPosition(playerPos);
        std::cout << "playerPos x：" << playerPos.x << std::endl;
        std::cout << "playerPos y：" << playerPos.y << std::endl;

        std::cout << "tileCoord x：" << tileCoord.x << std::endl;
        std::cout << "tileCoord y：" << tileCoord.y << std::endl;

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
    this->schedule(schedule_selector(HelloWorld::setPlayerPosition), 0.1f);
    this->schedule(schedule_selector(HelloWorld::setViewPlayerCenter));
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
    this->unschedule(schedule_selector(HelloWorld::setPlayerPosition));
    this->unschedule(schedule_selector(HelloWorld::setViewPlayerCenter));
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
    this->unschedule(schedule_selector(HelloWorld::setPlayerPosition));
    this->unschedule(schedule_selector(HelloWorld::setViewPlayerCenter));
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
//    std::cout <<  "This walkCycle" << std::endl;
    CCArray* frames = CCArray::createWithCapacity(9);
    for (int i = 0; i < 9; ++i) {

        CCString *walkCycle = CCString::createWithFormat("male_walkcycle_%s_%02d.png",direction, (i+1));

        frames->addObject(_frameCache->spriteFrameByName(walkCycle->getCString()));

//        std::cout <<  walkCycle->getCString() << std::endl;
    }

    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.05f);
    CCAnimate* animate = CCAnimate::create(animation);
    _player->runAction(animate);
}
