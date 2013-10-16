#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
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
    
    
    // タイルマップ呼び出し
    _tileMap = new CCTMXTiledMap();
    _tileMap->initWithTMXFile("th_cobit_rouka_1f.tmx");
    
    //
    _wallFloor = _tileMap->layerNamed("WallFloor");
    // メタレイヤー
    _meta = _tileMap->layerNamed("Meta");
    // プレイヤーの目からは見えなくする
    _meta->setVisible(false);

    // タイルマップ表示
    this->addChild(_tileMap);
    // tmx上のObjectsをobjectGroupに定義
    CCTMXObjectGroup *objectGroup = _tileMap->objectGroupNamed("Object");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    // tmx上のSpawnPointの座標を取得
    CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    int x = ((CCString)*spawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*spawnPoint->valueForKey("y")).intValue();
    
    _player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40));
    
    
    // プレイヤーをタグ識別
    _player->setTag(1);
    
    // スプライトに座標セット
    _player->setPosition(ccp(x,y));
    
    // スプライトをレイヤに追加
    this->addChild(_player);
    
    // 画面の表示座標をセット
    this->setViewPlayerCenter();
    this->schedule(schedule_selector(HelloWorld::setViewPlayerCenter));
    
    
    // タッチを有効化
    this->setTouchEnabled(true);
    
    return true;
}


void HelloWorld::setViewPlayerCenter()
{
    //    CCLog("setViewPlayerCenter");
    CCPoint playerPos = ((CCSprite *)this->getChildByTag(1))->getPosition();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(playerPos.x, winSize.width/2);
    int y = MAX(playerPos.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
    
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

